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

//#ifndef __GUI_GTEXTENT_H__
//#include "gui\gtextent.h"
//#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef	__FRONTEND_SCROLLBG_H__
#include "frontend\scrollbg.h"
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
static CScrollyBackground		*s_bg1;
static CScrollyBackground		*s_bg2;
static CScrollyBackground		*s_bg3;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
/*
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
char textEntry[16+1]="SOME TEXT";



CGUIControlFrame	*baseGUIObject;
*/

void CPaulScene::init()
{
	s_fontBank.initialise(&standardFont);
	s_fontBank.setColour(255,255,255);

	/*	
	CGUIGroupFrame		*fr;
	CGUITextBox			*tb;
	CGUIToggleButton	*tg;
	CGUITextReadout		*tr;
	CGUISpriteReadout	*sr;
	CGUIBarReadout		*br;
	CGUISliderButton	*sl;
	CGUITextEntry		*te;
	
int mem=MainRam.TotalRam-MainRam.RamUsed;
PAUL_DBGMSG("initial mem free=%d",mem);
	
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
	fr->setObjectXYWH(10,155,200,30);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(0,0,200,30);
		tb->setText(STR__PAULS_TEST__EXIT);
		tg=new ("togglebutton") CGUIToggleButton();
		tg->init(fr);
		tg->setButtonTarget(&readyToExit);

	fr=new ("frame") CGUIGroupFrame();
	fr->init(baseGUIObject);
	fr->setObjectXYWH(220,155,200,30);
		te=new ("textentry") CGUITextEntry();
		te->init(fr);
		te->setObjectXYWH(0,0,200,30);
		te->setTextDetails(textEntry,16);

	baseGUIObject->select();

	// Heh.. this'll actually work =)
//ASSERT(0);	
//	baseGUIObject->shutdown();
//	baseGUIObject=0;
	
PAUL_DBGMSG("change=%d",mem-(MainRam.TotalRam-MainRam.RamUsed));


	s_bg1=new ("scrolly background") CScrollyBackground();
	s_bg1->init();
	s_bg1->setSpeed(+2,+2);

	s_bg2=new ("scrolly background") CScrollyBackground();
	s_bg2->init();
	s_bg2->setSpeed(-2,0);
	s_bg2->setOt(1001);

	s_bg3=new ("scrolly background") CScrollyBackground();
	s_bg3->init();
	s_bg3->setSpeed(0,-2);
	s_bg3->setOt(1002);
*/
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::shutdown()
{
/*	
	s_bg3->shutdown();
	s_bg2->shutdown();
	s_bg1->shutdown();
	s_fontBank.dump();
*/
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

	/*
	if(baseGUIObject)	
		baseGUIObject->render();

	s_bg1->render();
	s_bg2->render();
	s_bg3->render();
*/
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::think(int _frames)
{
	if(PadGetHeld(0)&PAD_R1&&PadGetDown(0)&PAD_R2)
	{
		showDebugLog=!showDebugLog;
	}

	/*		
	if(readyToExit)
	{
		baseGUIObject->shutdown();
		delete baseGUIObject;
		baseGUIObject=NULL;
		readyToExit=0;
	}

	if(baseGUIObject)
		baseGUIObject->think(_frames);

	s_bg1->think(_frames);
	s_bg2->think(_frames);
	s_bg3->think(_frames);
*/
}


/*===========================================================================
 end */



/*
BubicleEmitterData bubData=
{
	206,200,60,60,		// m_x,m_y,m_w,m_h
	10,1,				// m_birthRate,m_birthAmount
	-1,					// m_life
	{					// m_bubicleBase
		100,				// m_life
		0,0,0,				// m_vx,m_vdx,m_vxmax
		-40,-15,-100,		// m_vy,m_vdy,m_vymax
		13,10,				// m_w,m_h
		10,					// m_dvSizeChange
		0,100,				// m_theta,m_vtheta
		100,2,0,			// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
		40,					// m_ot
		{ 128,128,128,	}	// m_colour
	},
	{					// m_bubicleRange
		100,				// m_life
		0,0,0,				// m_vx,m_vdx,m_vxmax
		20,10,0,			// m_vy,m_vdy,m_vymax
		5,5,				// m_w,m_h
		10,					// m_dvSizeChange
		4095,50,			// m_theta,m_vtheta
		100,5,0,			// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
		0,					// m_ot
		{   0, 64,127,	}	// m_colour
	}
};
CBubicleEmitter	*testBub;


  CBubicleFactory::init();
  testBub=CBubicleFactory::spawnEmitter(&bubData);

  CBubicleFactory::shutdown();

  testBub->setPos(X,Y);
  
	CBubicleFactory::render();
	CBubicleFactory::think();
  

*/