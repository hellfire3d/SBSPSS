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
	{	0,	STR__ON,	},
	{	1,	STR__OFF,	},
	{	0,	0,			},
};
CGUISpriteReadout::SpriteReadoutData onOffSpriteReadouts[]=
{
	{	0,	FRM__CROSS,	},
	{	1,	FRM__TICK,	},
	{	0,	0,			},
};
int onOffValues[]=
{
	0,1,
	0,
};
int musicStatus=0;
int sfxStatus=0;



CGUIControlFrame	*baseGUIObject;


void CPaulScene::init()
{
	CGUIGroupFrame		*fr;
	CGUITextBox			*tb;
	CGUIButton			*bu;
	CGUITextReadout		*tr;
	CGUISpriteReadout	*sr;

	s_fontBank.initialise(&standardFont);

	baseGUIObject=new ("Uber GUI object") CGUIControlFrame();
	baseGUIObject->init(NULL,1);
	baseGUIObject->setObjectXYWH(32,32,512-64,256-64);

	fr=new ("frame") CGUIGroupFrame();
	fr->init(baseGUIObject,2);
	fr->setObjectXYWH(10,10,448-20,30);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr,20);
		tb->setObjectXYWH(0,0,300,30);
		tb->setText(STR__PAULS_TEST__MUSIC);
		bu=new ("button") CGUIButton();
		bu->init(fr,21);
//		bu->setObjectXYWH(0,0,0,0);
		bu->setButtonTarget(&musicStatus);
		bu->setButtonData(onOffValues);
		tr=new ("textreadout") CGUITextReadout();
		tr->init(fr,22);
		tr->setObjectXYWH(300,0,128,30);
		tr->setReadoutTarget(&musicStatus);
		tr->setReadoutData(onOffTextReadouts);

	fr=new ("frame") CGUIGroupFrame();
	fr->init(baseGUIObject,3);
	fr->setObjectXYWH(10,50,448-20,30);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr,30);
		tb->setObjectXYWH(0,0,300,30);
		tb->setText(STR__PAULS_TEST__SFX);
		bu=new ("button") CGUIButton();
		bu->init(fr,31);
//		bu->setObjectXYWH(0,0,0,0);
		bu->setButtonTarget(&sfxStatus);
		bu->setButtonData(onOffValues);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr,32);
		sr->setObjectXYWH(300,0,128,30);
		sr->setReadoutTarget(&sfxStatus);
		sr->setReadoutData(onOffSpriteReadouts);



	fr=new ("frame") CGUIGroupFrame();
	fr->init(baseGUIObject,3);
	fr->setObjectXYWH(10,90,448-20,30);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr,30);
		tb->setObjectXYWH(0,0,300,30);
		tb->setText(STR__PAULS_TEST__SFX);
		bu=new ("button") CGUIButton();
		bu->init(fr,31);
//		bu->setObjectXYWH(0,0,0,0);
		bu->setButtonTarget(&sfxStatus);
		bu->setButtonData(onOffValues);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr,32);
		sr->setObjectXYWH(300,0,128,30);
		sr->setReadoutTarget(&sfxStatus);
		sr->setReadoutData(onOffSpriteReadouts);
		


/*
	tb=new ("textbox") CGUITextBox();
	tb->init(baseGUIObject,1);
	tb->setObjectXYWH(10,10,400,25);
	tb->setText(STR__PAULS_TEST_STRING_1);

	fr=new ("frame") CGUIGroupFrame();
	fr->init(baseGUIObject,2);
	fr->setObjectXYWH(10,40,400,25);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr,20);
		tb->setObjectXYWH(50,1,300,22);
		tb->setText(STR__PAULS_TEST_STRING_1);
		bu=new ("button") CGUIButton();
		bu->init(fr,21);
		bu->setObjectXYWH(50,1,10,10);
		bu->setButtonTarget(&testValue);
		bu->setButtonData(testButtonData);

	tr=new ("textreadout") CGUITextReadout();
	tr->init(baseGUIObject,3);
	tr->setObjectXYWH(10,70,400,25);
	tr->setReadoutTarget(&testValue);
	tr->setReadoutData(testReadoutData);
*/
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
	/*
	CGUITextBox		*tb;
	tb=(CGUITextBox *)guiGetItem(1);
	tb->setObjectWH(w,h);
	tb->setText(str);
	*/

	baseGUIObject->think(_frames);
}


/*===========================================================================
 end */