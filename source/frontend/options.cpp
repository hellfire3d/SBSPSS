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

#ifndef __GUI_GFACTORY_H__
#include "gui\gfactory.h"
#endif

#ifndef __GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef __GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_FRONTEND_H__
#include <frontend.h>
#endif

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

int	CFrontEndOptions::s_controlStyleValues[CPadConfig::NUM_PAD_CONFIGS+1]=
{
	0,1,2,3,
	-1
};
CGUITextReadout::TextReadoutData CFrontEndOptions::s_controlStyleReadoutText[CPadConfig::NUM_PAD_CONFIGS]=
{
	{	0,STR__FRONTEND__A	},
	{	1,STR__FRONTEND__B	},
	{	2,STR__FRONTEND__C	},
	{	3,STR__FRONTEND__D	},
};


int CFrontEndOptions::s_buttonOrder[]=
{
	CPadConfig::PAD_CFG_UP,
	CPadConfig::PAD_CFG_DOWN,
	CPadConfig::PAD_CFG_LEFT,
	CPadConfig::PAD_CFG_RIGHT,
	CPadConfig::PAD_CFG_JUMP,
	CPadConfig::PAD_CFG_ACTION,
};

CGUISpriteReadout::SpriteReadoutData	CFrontEndOptions::s_controlReadoutSprites[]=
{
	{	ICON_UP,FRM__BUTU		},
	{	ICON_DOWN,FRM__BUTD		},
	{	ICON_LEFT,FRM__BUTL		},
	{	ICON_RIGHT,FRM__BUTR	},
	{	ICON_CROSS,FRM__BUTX	},
	{	ICON_CIRCLE,FRM__BUTC	},
	{	ICON_SQUARE,FRM__BUTS	},
	{	ICON_TRIANGLE,FRM__BUTT	},
};

CFrontEndOptions::ButtonToIconMap	CFrontEndOptions::s_controlMap[]=
{
	{	PAD_UP,			ICON_UP			},
	{	PAD_DOWN,		ICON_DOWN		},
	{	PAD_LEFT,		ICON_LEFT		},
	{	PAD_RIGHT,		ICON_RIGHT		},
	{	PAD_CROSS,		ICON_CROSS		},
	{	PAD_SQUARE,		ICON_SQUARE		},
	{	PAD_CIRCLE,		ICON_CIRCLE		},
	{	PAD_TRIANGLE,	ICON_TRIANGLE	},
};

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndOptions::init()
{
	int					i;
	CGUIGroupFrame		*fr;
	CGUITextBox			*tb;
	CGUISpriteReadout	*sr;


	m_background=new ("Options Background") CScrollyBackground();
	m_background->init();
	m_background->setOt(MAX_OT-2);

	// Create the menu frames
	for(i=0;i<MODE__COUNT;i++)
	{
		CGUIControlFrame	**mm=&m_modeMenus[i];
		*mm=new ("optionsframe") CGUIControlFrame();
		(*mm)->init(NULL);
		(*mm)->setObjectXYWH(50,40,412,176);
	}


	// Populate OPTIONS menu
	CGUIFactory::createValueButtonFrame(m_modeMenus[MODE__OPTIONS],
										X_BORDER,Y_BORDER,412-(X_BORDER*2),20,
										STR__FRONTEND__CONTROLS,
										&m_nextMode,MODE__CONTROL);
	CGUIFactory::createValueButtonFrame(m_modeMenus[MODE__OPTIONS],
										X_BORDER,Y_BORDER+30,412-(X_BORDER*2),20,
										STR__FRONTEND__SCREEN,
										&m_nextMode,MODE__SCREEN);
	CGUIFactory::createValueButtonFrame(m_modeMenus[MODE__OPTIONS],
										X_BORDER,Y_BORDER+60,412-(X_BORDER*2),20,
										STR__FRONTEND__SOUND,
										&m_nextMode,MODE__SOUND);
	CGUIFactory::createValueButtonFrame(m_modeMenus[MODE__OPTIONS],
										X_BORDER,176-Y_BORDER-20,412-(X_BORDER*2),20,
										STR__FRONTEND__EXIT,
										&m_exitFlag,true);


	// Populate CONTROLS menu
	CGUIFactory::createCycleButtonFrame(m_modeMenus[MODE__CONTROL],
										X_BORDER,Y_BORDER,412-(X_BORDER*2),40,
										STR__FRONTEND__CONTROL_STYLE,
										&m_controlStyle,s_controlStyleValues,s_controlStyleReadoutText);
	fr=new ("frame") CGUIGroupFrame();
	fr->init(m_modeMenus[MODE__CONTROL]);
	fr->setObjectXYWH(X_BORDER*2,65,412-(X_BORDER*4),60);	//292
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(0,0,26,15);	//146
		sr->setReadoutTarget(&m_controlIcons[CONTROL_UP]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(26,0,120,15);
		tb->setText(STR__FRONTEND__UP);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(0,15,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_DOWN]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(26,15,120,15);
		tb->setText(STR__FRONTEND__DOWN);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(0,30,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_LEFT]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(26,30,120,15);
		tb->setText(STR__FRONTEND__LEFT);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(0,45,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_RIGHT]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(26,45,120,15);
		tb->setText(STR__FRONTEND__RIGHT);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(146,0,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_JUMP]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(146+26,0,120,15);
		tb->setText(STR__FRONTEND__JUMP);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(146,15,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_ACTION]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(146+26,15,120,15);
		tb->setText(STR__FRONTEND__ACTION);


	// Populate SCREEN menu
	CGUIFactory::createSliderButtonFrame(m_modeMenus[MODE__SCREEN],
										 X_BORDER,Y_BORDER,412-(X_BORDER*2),35,
										 STR__FRONTEND__HORIZONTAL_POSITION,
										 &m_screenXOff,0,64);
	CGUIFactory::createSliderButtonFrame(m_modeMenus[MODE__SCREEN],
										 X_BORDER,Y_BORDER+40,412-(X_BORDER*2),35,
										 STR__FRONTEND__VERTICAL_POSITION,
										 &m_screenYOff,0,32);


	// Populate SOUND menu
	CGUIFactory::createSliderButtonFrame(m_modeMenus[MODE__SOUND],
										 X_BORDER,Y_BORDER,412-(X_BORDER*2),35,
										 STR__FRONTEND__BGM,
										 &m_bgmVolume,CSoundMediator::MIN_VOLUME,CSoundMediator::MAX_VOLUME);
	CGUIFactory::createSliderButtonFrame(m_modeMenus[MODE__SOUND],
										 X_BORDER,Y_BORDER+40,412-(X_BORDER*2),35,
										 STR__FRONTEND__SFX,
										 &m_sfxVolume,CSoundMediator::MIN_VOLUME,CSoundMediator::MAX_VOLUME);
	CGUIFactory::createSliderButtonFrame(m_modeMenus[MODE__SOUND],
										 X_BORDER,Y_BORDER+80,412-(X_BORDER*2),35,
										 STR__FRONTEND__SPEECH,
										 &m_speechVolume,CSoundMediator::MIN_VOLUME,CSoundMediator::MAX_VOLUME);

	// Add BACK to all of the sub menus
	for(i=1;i<MODE__COUNT;i++)
	{
		CGUIFactory::createValueButtonFrame(m_modeMenus[i],
											X_BORDER,176-Y_BORDER-20,412-(X_BORDER*2),20,
											STR__FRONTEND__BACK,
											&m_nextMode,MODE__OPTIONS);
	}

	m_bgmVolume=CSoundMediator::getVolume(CSoundMediator::VOL_SONG);
	m_sfxVolume=CSoundMediator::getVolume(CSoundMediator::VOL_SFX);
	m_speechVolume=CSoundMediator::getVolume(CSoundMediator::VOL_SPEECH);
	m_controlStyle=CPadConfig::getConfig();
	m_screenXOff=VidGetXOfs();
	m_screenYOff=VidGetYOfs();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndOptions::shutdown()
{
	int		i;

	for(i=0;i<MODE__COUNT;i++)
	{
		m_modeMenus[i]->shutdown();
	}
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
	m_modeMenus[MODE__OPTIONS]->select();

	m_mode=m_nextMode=MODE__OPTIONS;

	m_exitFlag=false;
	m_closingDown=false;

	CSoundMediator::setSong(CSoundMediator::SONG_OPTIONS);
	CSoundMediator::playSong();
	
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
	CSoundMediator::dumpSong();

	m_modeMenus[m_mode]->unselect();
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
	setRGB0(g4,90, 0, 0);
	setRGB1(g4, 0, 0,90);
	setRGB2(g4, 0,90, 0);
	setRGB3(g4,90, 0,90);
	AddPrimToList(g4,MAX_OT-1);

	m_background->render();

	m_modeMenus[m_mode]->render();
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
	int		i,j,button;


	m_background->setSpeed(ox,oy);
	m_background->setSpeedScale(os);
	m_background->think(_frames);

	if(!CFader::isFading())
	{
		if(m_nextMode!=m_mode)
		{
			m_modeMenus[m_mode]->unselect();
			m_mode=m_nextMode;
			m_modeMenus[m_mode]->select();
		}
		m_modeMenus[m_mode]->think(_frames);

		if(m_mode==MODE__SOUND)
		{
			if(m_bgmVolume!=CSoundMediator::getVolume(CSoundMediator::VOL_SONG))
			{
				CSoundMediator::setVolume(CSoundMediator::VOL_SONG,m_bgmVolume);
			}
			if(m_sfxVolume!=CSoundMediator::getVolume(CSoundMediator::VOL_SFX))
			{
				CSoundMediator::setVolume(CSoundMediator::VOL_SFX,m_sfxVolume);
			}
			if(m_speechVolume!=CSoundMediator::getVolume(CSoundMediator::VOL_SPEECH))
			{
				CSoundMediator::setVolume(CSoundMediator::VOL_SPEECH,m_speechVolume);
			}
		}
		else if(m_mode==MODE__SCREEN)
		{
			VidSetXYOfs(m_screenXOff,m_screenYOff);
		}
	}

	if(!m_closingDown&&m_exitFlag)
	{
		CFader::setFadingOut();
		m_closingDown=true;
	}

	// Change the icons on the control display
	CPadConfig::setConfig(m_controlStyle);
	for(i=0;i<CONTROL_COUNT;i++)
	{
		button=CPadConfig::getButton((CPadConfig::PAD_CFG)s_buttonOrder[i]);
		for(j=0;j<ICON_COUNT;j++)
		{
			if(s_controlMap[j].m_padButton==button)
			{
				m_controlIcons[i]=s_controlMap[j].m_icon;
				break;
			}
		}
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