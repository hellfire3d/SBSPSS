/*=========================================================================

	paul.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "paul\paul.h"

#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
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

#ifndef __GUI_GREADOUT_H__
#include "gui\greadout.h"
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

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
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
static FontBank		s_fontBank;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
unsigned int str=STR__PAULS_TEST_STRING_1;
int w=150;
int h=40;

CGUITextReadout::TextReadoutData onOffTextReadouts[]=
{
	{	false,	STR__ON,	},
	{	true,	STR__OFF,	},
	{	0,		0,			},
};
CGUISpriteReadout::SpriteReadoutData onOffSpriteReadouts[]=
{
	{	false,	FRM__CROSS,	},
	{	true,	FRM__TICK,	},
	{	0,		0,			},
};
int musicStatus=false;
int sfxStatus=false;
int readyToExit=false;
int	musicVol=0;



CGUIControlFrame	*baseGUIObject;


void CPaulScene::init()
{
	CGUIGroupFrame		*fr;
	CGUITextBox			*tb;
	CGUIToggleButton	*tg;
	CGUITextReadout		*tr;
	CGUISpriteReadout	*sr;
	CGUIBarReadout		*br;
	CGUISliderButton	*sl;

	s_fontBank.initialise(&standardFont);

	baseGUIObject=new ("Uber GUI object") CGUIControlFrame();
	baseGUIObject->init(NULL);
	baseGUIObject->setObjectXYWH(32,32,512-64,256-64);

	fr=new ("frame") CGUIGroupFrame();
	fr->init(baseGUIObject);
	fr->setObjectXYWH(10,10,448-20,30);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(0,0,300,30);
		tb->setText(STR__PAULS_TEST__MUSIC);
		tg=new ("togglebutton") CGUIToggleButton();
		tg->init(fr);
		tg->setButtonTarget(&musicStatus);
		tr=new ("textreadout") CGUITextReadout();
		tr->init(fr);
		tr->setObjectXYWH(300,0,128,30);
		tr->setReadoutTarget(&musicStatus);
		tr->setReadoutData(onOffTextReadouts);

	fr=new ("frame") CGUIGroupFrame();
	fr->init(baseGUIObject);
	fr->setObjectXYWH(10,50,448-20,30);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(0,0,300,30);
		tb->setText(STR__PAULS_TEST__SFX);
		tg=new ("togglebutton") CGUIToggleButton();
		tg->init(fr);
		tg->setButtonTarget(&sfxStatus);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(300,0,128,30);
		sr->setReadoutTarget(&sfxStatus);
		sr->setReadoutData(onOffSpriteReadouts);

		fr=new ("frame") CGUIGroupFrame();
		fr->init(baseGUIObject);
		fr->setObjectXYWH(10,90,448-20,30);
			sl=new("sliderbutton") CGUISliderButton();
			sl->init(fr);
			sl->setButtonTarget(&musicVol);
			sl->setButtonRange(0,255);
			br=new ("spritereadout") CGUIBarReadout();
			br->init(fr);
			br->setObjectXYWH(0,0,448-20,30);
			br->setReadoutTarget(&musicVol);
			br->setReadoutRange(0,255);

	fr=new ("frame") CGUIGroupFrame();
	fr->init(baseGUIObject);
	fr->setObjectXYWH(10,155,448-20,30);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(0,0,428,30);
		tb->setText(STR__PAULS_TEST__EXIT);
		tg=new ("togglebutton") CGUIToggleButton();
		tg->init(fr);
		tg->setButtonTarget(&readyToExit);

	// Heh.. this'll actually work =)
//	baseGUIObject->shutdown();


	baseGUIObject->select();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::shutdown()
{
	s_fontBank.dump();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int showDebugLog=false;
void CPaulScene::render()
{

	if(showDebugLog)
	{
		int		logCount;
		int		i,y,charHeight;
		logCount=getNumberOfDbgLinesInLog();
		y=20;
		charHeight=s_fontBank.getCharHeight();
		for(i=0;i<logCount;i++)
		{
			s_fontBank.print(20,y,getDbgLineFromLog(i));
			y+=charHeight;
		}
	}

	if(baseGUIObject)	
		baseGUIObject->render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::think(int _frames)
{
	if(readyToExit)
	{
		baseGUIObject->shutdown();
		delete baseGUIObject;
		baseGUIObject=NULL;
		readyToExit=0;
	}
		
	if(baseGUIObject)
		baseGUIObject->think(_frames);
}


/*===========================================================================
 end */