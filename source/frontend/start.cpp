/*=========================================================================

	start.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\start.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#include "gfx\font.h"


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
FontBank	*s_smallFont;
void CFrontEndStart::init()
{
	s_smallFont=new ("Start SmallFont") FontBank();
	s_smallFont->initialise(&standardFont);
	s_smallFont->setJustification(FontBank::JUST_CENTRE);
	s_smallFont->setOt(5);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::shutdown()
{
	s_smallFont->dump();		delete s_smallFont;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::select()
{
	m_shuttingDown=false;
	m_selectedSlot=NO_SLOT_SELECTED;
	m_escapeToTitles=false;
	CFader::setFadingIn();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::unselect()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::render()
{
	POLY_G4	*g4;
	
	g4=GetPrimG4();
	setXYWH(g4,0,0,512,256);
	setRGB0(g4,50, 0, 0);
	setRGB1(g4, 0, 0,50);
	setRGB2(g4, 0,50, 0);
	setRGB3(g4,50, 0,50);
	AddPrimToList(g4,MAX_OT-1);

	if(!CFader::isFading())
	{
		s_smallFont->print(256,120,"Press START to enter game scene");
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif
void CFrontEndStart::think(int _frames)
{
	if(!m_shuttingDown)
	{
/////////// PKG		
if(PadGetDown(0)&PAD_START)
{
	m_selectedSlot=0;
}
/////////// PKG		
		if(m_selectedSlot!=NO_SLOT_SELECTED||
		   m_escapeToTitles)
		{
			m_shuttingDown=true;
			CFader::setFadingOut();
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndStart::isReadyToExit()
{
	return !CFader::isFading()&&m_shuttingDown;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CFrontEndScene::FrontEndMode CFrontEndStart::getNextMode()
{
	if(m_selectedSlot==NO_SLOT_SELECTED)
	{
		return CFrontEndScene::MODE__MAIN_TITLES;
	}
	else
	{
		return CFrontEndScene::MODE__EXIT_TO_GAME;
	}
}

/*===========================================================================
 end */