/**********************/
/*** Main Game File ***/
/**********************/

#ifndef __GAME_GAME_H__
#define __GAME_GAME_H__

#include	"system\gstate.h"
#include	"level\level.h"

/*****************************************************************************/
class	FontBank;

class CGameScene : public CScene
{
public:
			CGameScene()	{;}
	virtual ~CGameScene()	{;}


	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()	{return "Game";}

	
private:

		CLevel		Level;
static	FontBank	*s_genericFont;
//static	class SpriteBank	*s_sprites;	<-- Was causing compile error, sorry

};



/*****************************************************************************/
extern CGameScene	GameScene;

/*****************************************************************************/

#endif