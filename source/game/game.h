/**********************/
/*** Main Game File ***/
/**********************/

#ifndef __GAME_GAME_H__
#define __GAME_GAME_H__

#include	"system\gstate.h"
#include	"level\level.h"

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

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
		void	shutdown();
		void	render();
		void	think(int _frames);
		int		readyToShutdown();
		char	*getSceneName()	{return "Game";}

		static	MATRIX	*GetCamMtx()		{return(&CamMtx);}
		class CPlayer	*getPlayer();
		CConversation	*getConversation();
		void	sendEvent( GAME_EVENT evt, CThing *sourceThing );
	
private:

		CLevel		Level;
		class CPlayer	*m_player;
		CConversation	m_conversation;
static	FontBank	*s_genericFont;
static	MATRIX		CamMtx;
//static	class SpriteBank	*s_sprites;	<-- Was causing compile error, sorry

};



/*****************************************************************************/
extern CGameScene	GameScene;

/*****************************************************************************/

#endif