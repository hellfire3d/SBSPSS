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
#define	RENDER_Z (378)
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
		void			hitBossArenaTrigger();

static void				levelFinished()		{s_levelFinished=true;}
static void				restartlevel()		{s_restartLevel=true;}
		CLevel			&GetLevel()			{return(Level);}

		int				triggerTeleportEffect();


static	ACTOR_TYPE	getActorType( int actorNum )			{return actorType[actorNum];}

static void				setBossHasBeenKilled();
static int				getBossHasBeenKilled()				{return s_bossHasBeenKilled;}

static	void			initHealth();
static	void			dropHealth(DVECTOR const &Pos,int Amount,int Vel);


static	void			setCameraShake(s16 X,s16 Y);
static	void			shakeCamera(DVECTOR &CamPos);
		void			thinkCameraShake(int _frames);

static	void			setCameraMtx();
private:
	typedef enum
	{
		GAMESTATE_SHOWING_LIVES,
		GAMESTATE_SHOWING_LIVES_BUT_GOING_TO_BOSS_TEXT,
		GAMESTATE_PLAYING,
		GAMESTATE_FADING_INTO_BOSS_INTRO,
		GAMESTATE_BOSS_INTRO,
		GAMESTATE_TELEPORT_START,
		GAMESTATE_TELEPORT_END,
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
	void				render_showing_lives();
	void				render_playing();

	class CBossText		*m_bossText;


protected:

		void			initLevel();
		void			shutdownLevel();

		CLevel			Level;
class	CPauseMenu		*m_pauseMenu;
class	CPlayer			*m_player;
static	CHealthManager	*m_HealthManager;
static	FontBank		*s_genericFont;
		class ScalableFontBank	*m_scalableFont;

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
static	int				s_showBossTextOnRespawn;
		int				m_playingBossMusic;


		int				m_levelHasTimer;
		int				m_timer;

		int				m_musicStarted;

public:
		int				getIsPaused()						{return(m_isPaused);}
protected:		
		int				m_isPaused;
};



/*****************************************************************************/
extern CGameScene	GameScene;

/*****************************************************************************/

#endif