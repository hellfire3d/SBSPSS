/*=========================================================================

	options.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\options.h"

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef	__FRONTEND_SCROLLBG_H__
#include "frontend\scrollbg.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __GUI_GUI_H__
#include "gui\gui.h"
#endif

#ifndef __GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
#endif

#ifndef __GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef __GUI_GBUTTON_H__
#include "gui\gbutton.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_FRONTEND_H__
#include <frontend.h>
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndOptions::init()
{
	CGUIGroupFrame		*fr;
	CGUITextBox			*tb;
	CGUIToggleButton	*tg;

	m_background=new ("Options Background") CScrollyBackground();
	m_background->init();

	m_optionsMenu=new ("Options GUI") CGUIControlFrame();
	m_optionsMenu->init(NULL);
	m_optionsMenu->setObjectXYWH(50,40,412,176);

	fr=new ("frame") CGUIGroupFrame();
	fr->init(m_optionsMenu);
	fr->setObjectXYWH(50,0,200,20);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(0,0,200,20);
		tb->setText(STR__FRONTEND__EXIT);
		tg=new ("togglebutton") CGUIToggleButton();
		tg->init(fr);
		tg->setButtonTarget(&m_exitFlag);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndOptions::shutdown()
{
	m_optionsMenu->shutdown();
	m_background->shutdown();		delete m_background;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndOptions::select()
{
	m_optionsMenu->select();

	m_exitFlag=false;
	m_closingDown=false;

	CFader::setFadingIn();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndOptions::unselect()
{
	m_optionsMenu->unselect();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndOptions::render()
{
	POLY_G4	*g4;

	g4=GetPrimG4();
	setXYWH(g4,0,0,512,256);
	setRGB0(g4,50, 0, 0);
	setRGB1(g4, 0, 0,50);
	setRGB2(g4, 0,50, 0);
	setRGB3(g4,50, 0,50);
	AddPrimToList(g4,1001);

	m_background->render();

	m_optionsMenu->render();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int ox=-1;
int oy=-1;
int os=0;
void CFrontEndOptions::think(int _frames)
{
	m_background->setSpeed(ox,oy);
	m_background->setSpeedScale(os);
	m_background->think(_frames);

	if(!CFader::isFading())
	{
		m_optionsMenu->think(_frames);
	}

	if(!m_closingDown&&m_exitFlag)
	{
		CFader::setFadingOut();
		m_closingDown=true;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndOptions::isReadyToExit()
{
	return m_closingDown&&!CFader::isFading();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CFrontEndScene::FrontEndMode CFrontEndOptions::getNextMode()
{
	return CFrontEndScene::MODE__MAIN_TITLES;
}

/*===========================================================================
 end */