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
static void				levelFinished()		{s_levelFinished=true;}
static void				restartlevel()		{s_restartLevel=true;}
		CLevel			&GetLevel()			{return(Level);}

//		static	MATRIX	&GetCamMtx()		{return(CamMtx);}
static	ACTOR_TYPE	getActorType( int actorNum )			{return actorType[actorNum];}

static void				setBossHasBeenKilled()				{s_bossHasBeenKilled=true;}
static int				getBossHasBeenKilled()				{return s_bossHasBeenKilled;}

static	void			dropHealth(DVECTOR &Pos,int Amount,int Vel);
protected:

		void			initLevel();
		void			shutdownLevel();

		void			AspectCorrectCamera();

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


		int				m_levelHasTimer;
		int				m_timer;

};



/*****************************************************************************/
extern CGameScene	GameScene;

/*****************************************************************************/

#endif