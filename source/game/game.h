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
	CGameScene() {};
	virtual ~CGameScene() {};


	void Init();
	void Shutdown();
	void Render();
	bool Control();
	char *GetSceneName()	{return "Game";}

	
private:

	static	FontBank			*s_genericFont;
};



/*****************************************************************************/
extern CGameScene	GameScene;

/*****************************************************************************/

#endif