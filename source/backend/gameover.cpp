/*=========================================================================

	gameover.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "backend\gameover.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __STRING_ENUMS__
#include <trans.h>
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

CGameOverScene	GameOverScene;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::init()
{
	m_font=new ("game over font") FontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(FontBank::JUST_CENTRE);
	m_font->setOt(10);

	m_readyToExit=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::shutdown()
{
	m_font->dump();				delete m_font;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::render()
{
	POLY_F4	*f4;

	f4=GetPrimF4();
	setXYWH(f4,0,0,512,256);
	setRGB0(f4,0,0,0);
	AddPrimToList(f4,20);

	m_font->setColour(255,255,255);
	m_font->print(256,100,STR__BACKEND__GAME_OVER);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::think(int _frames)
{
	if(!m_readyToExit)
	{
		if(PadGetDown(0)&(PAD_CROSS|PAD_START))
		{
			m_readyToExit=true;
			CFader::setFadingOut();
			GameState::setNextScene(&FrontEndScene);
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CGameOverScene::readyToShutdown()
{
	return m_readyToExit&&!CFader::isFading();
}


/*===========================================================================
 end */