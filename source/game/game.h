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

		CPlayer			*getPlayer();
		void			respawnLevel();
		void			sendEvent( GAME_EVENT evt, class CThing *sourceThing );

static void				setReadyToExit()	{s_readyToExit=true;}
static void				levelFinished()		{s_levelFinished=true;}
	
//		static	MATRIX	&GetCamMtx()		{return(CamMtx);}
static	ACTOR_TYPE	getActorType( int actorNum )			{return actorType[actorNum];}

protected:

		void			initLevel();
		void			shutdownLevel();

		void			AspectCorrectCamera();

		CLevel			Level;
class	CPauseMenu		*m_pauseMenu;
class	CPlayer			*m_player;
static	FontBank		*s_genericFont;
static	MATRIX			CamMtx;

static int				s_readyToExit;
static int				s_levelFinished;

static	ACTOR_TYPE		actorType[];

};



/*****************************************************************************/
extern CGameScene	GameScene;

/*****************************************************************************/

#endif