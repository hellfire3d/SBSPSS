/**********************/
/*** Main Game File ***/
/**********************/

#ifndef __GAME_GAME_H__
#define __GAME_GAME_H__

#include	"system\gstate.h"
#include	"level\level.h"

#ifndef __GAME_EVENT_H__
#include "game\event.h"
#endif

/*****************************************************************************/
class	FontBank;
class	SpriteBank;
class	CPlayer;
class	CLayerCollision;
class	CHealthManager;
class CGameScene : public CScene
{
public:
				CGameScene()	{;}
virtual			~CGameScene()	{;}
		enum ACTOR_TYPE
		{
			ACTOR_PLAYER = 0,
			ACTOR_FRIEND_NPC = 1,
			ACTOR_ENEMY_NPC,
			ACTOR_UNKNOWN,
		};


		void			init();
virtual	void			createPlayer();
virtual	int				canPause();
		void			shutdown();
		void			render();
		void			think(int _frames);
		int				readyToShutdown();
		char			*getSceneName()	{return "Game";}

		int				getChapterNumber();
		int				getLevelNumber();
		int				getTotalSpatCountForThisLevel();

		CPlayer			*getPlayer();
		void			respawnLevel();
		void			sendEvent( GAME_EVENT evt, class CThing *sourceThing );

static void				setReadyToExit();
static void				hitBossArenaTrigger();

static void				levelFinished()		{s_levelFinished=true;}
static void				restartlevel()		{s_restartLevel=true;}
		CLevel			&GetLevel()			{return(Level);}



//		static	MATRIX	&GetCamMtx()		{return(CamMtx);}
static	ACTOR_TYPE	getActorType( int actorNum )			{return actorType[actorNum];}

static void				setBossHasBeenKilled()				{s_bossHasBeenKilled=true;}
static int				getBossHasBeenKilled()				{return s_bossHasBeenKilled;}

static	void			dropHealth(DVECTOR &Pos,int Amount,int Vel);


static	void			setCameraShake(s16 X,s16 Y);
static	void			shakeCamera(DVECTOR &CamPos);
		void			thinkCameraShake(int _frames);
private:
	typedef enum
	{
		GAMESTATE_SHOWING_LIVES,
		GAMESTATE_PLAYING,
		GAMESTATE_FADING_INTO_BOSS_INTRO,
		GAMESTATE_BOSS_INTRO,
		GAMESTATE_FADING_OUT_OF_BOSS_INTRO,
	}GAMESTATE;

	enum
	{
		TIME_TO_DISPLAY_LIVES_COUNT=3*60,
		SPEED_OF_FADE=10,
	};

	GAMESTATE			m_gamestate;
	int					m_showingLivesTimer;
	static int			s_justHitBossArenaTrigger;

	void				think_showing_lives(int _frames);
	void				think_playing(int _frames);
	void				think_boss_intro(int _frames);
	void				render_showing_lives();
	void				render_playing();
	void				render_boss_intro();


protected:

		void			initLevel();
		void			shutdownLevel();

		CLevel			Level;
class	CPauseMenu		*m_pauseMenu;
class	CPlayer			*m_player;
static	CHealthManager	*m_HealthManager;
static	FontBank		*s_genericFont;
		class ScalableFontBank	*m_scalableFont;
static	MATRIX			CamMtx;

static int				s_restartLevel;
static int				s_readyToExit;
static int				s_levelFinished;
#ifdef __VERSION_DEBUG__
static int				s_skipToNextLevel;
#endif

static	ACTOR_TYPE		actorType[];

// global vars
public:
static	void			setSpriteBank(SpriteBank *SprBank)	{s_GlobalSpritebank=SprBank;}
static	SpriteBank		*getSpriteBank()					{return(s_GlobalSpritebank);}

static	void			setCollision(CLayerCollision *Col)	{s_GlobalCollision=Col;}
static	CLayerCollision	*getCollision()						{return(s_GlobalCollision);}

static	SpriteBank		*s_GlobalSpritebank;
static	CLayerCollision	*s_GlobalCollision;
static	int				s_bossHasBeenKilled;
static	DVECTOR			s_CamShake;


		int				m_levelHasTimer;
		int				m_timer;

};



/*****************************************************************************/
extern CGameScene	GameScene;

/*****************************************************************************/

#endif