/**********************/
/*** Main Game File ***/
/**********************/

#ifndef __GAME_GAME_H__
#define __GAME_GAME_H__

#include "system\gstate.h"


/*****************************************************************************/
class FontBank;

class CGameScene : public CScene
{
public:
			CGameScene()	{;}
	virtual ~CGameScene()	{;}


	void init();
	void shutdown();
	void render();
	void think();
	char *getSceneName()	{return "Game";}

	
private:

	static	FontBank			*s_genericFont;
};



/*****************************************************************************/
extern CGameScene	GameScene;

/*****************************************************************************/

#endif