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

int testValue=0;
CGUITextReadout::TextReadoutData testReadoutData[]=
{
	{	1,	STR__PAULS_TEST_STRING_1,	},
	{	2,	STR__PAULS_TEST_STRING_2,	},
	{	3,	STR__PAULS_TEST_STRING_3,	},
	{	0,	0,							},
};
int testButtonData[]=
{
	1,2,3,
	0,
};

CGUIControlFrame	*baseGUIObject;


void CPaulScene::init()
{
	CGUITextBox		*tb;
	CGUITextReadout	*tr;
	CGUIGroupFrame	*fr;
	CGUIButton		*bu;

	s_fontBank.initialise(&standardFont);

	baseGUIObject=new ("Uber GUI object") CGUIControlFrame();
	baseGUIObject->init(NULL,0);
	baseGUIObject->setObjectXYWH(32,100,512-64,120);

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
void CPaulScene::render()
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