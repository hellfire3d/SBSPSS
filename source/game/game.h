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

#ifndef __GAME_THING_H__
#include "game\thing.h"
#endif

/*****************************************************************************/
class	FontBank;

class CGameScene : public CScene
{
public:
				CGameScene()	{;}
virtual			~CGameScene()	{;}


		void	init();
virtual	void	createPlayer();
		void	shutdown();
		void	render();
		void	think(int _frames);
		int		readyToShutdown();
		char	*getSceneName()	{return "Game";}

		static	MATRIX	*GetCamMtx()		{return(&CamMtx);}
		class CPlayer	*getPlayer();
		void	sendEvent( GAME_EVENT evt, CThing *sourceThing );

		static void		setReadyToExit()	{s_readyToExit=true;}
	
protected:

		CLevel				Level;
		class CPauseMenu	*m_pauseMenu;
		class CPlayer		*m_player;
		static	FontBank	*s_genericFont;
		static	MATRIX		CamMtx;

		static int			s_readyToExit;

};



/*****************************************************************************/
extern CGameScene	GameScene;

/*****************************************************************************/

#endif