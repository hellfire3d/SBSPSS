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

#ifndef __GUI_GREADOUT_H__
#include "gui\greadout.h"
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

int testValue=1;
CGUITextReadout::TextReadoutData testData[]=
{
	{	1,	STR__PAULS_TEST_STRING_1,	},
	{	2,	STR__PAULS_TEST_STRING_2,	},
	{	3,	STR__PAULS_TEST_STRING_3,	},
	{	0,	0,							},
};



void CPaulScene::init()
{
	s_fontBank.initialise(&standardFont);
	guiOpen();

	CGUITextBox		*tb;
	tb=new ("textbox") CGUITextBox();
	tb->init(1);
	tb->setObjectXY(200,130);

	CGUITextReadout	*tr;
	tr=new ("textreadout") CGUITextReadout();
	tr->init(2);
	tr->setObjectXYWH(40,130,120,80);
	tr->setReadoutTarget(&testValue);
	tr->setReadoutData(testData);
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
	guiClose();
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
	
	guiRender();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::think(int _frames)
{
	CGUITextBox		*tb;
	tb=(CGUITextBox *)guiGetItem(1);
	tb->setObjectWH(w,h);
	tb->setText(str);

	guiThink(GameState::getFramesSinceLast());
}


/*===========================================================================
 end */