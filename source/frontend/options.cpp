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

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
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

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif

#ifndef __MEMCARD_MEMCARD_H__
#include "memcard\memcard.h"
#endif

#ifndef __MEMCARD_SAVELOAD_H__
#include "memcard\saveload.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
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
	0,1,2,
	-1
};
CGUITextReadout::TextReadoutData CFrontEndOptions::s_controlStyleReadoutText[CPadConfig::NUM_PAD_CONFIGS]=
{
	{	0,STR__FRONTEND__A	},
	{	1,STR__FRONTEND__B	},
	{	2,STR__FRONTEND__C	},
};


int	CFrontEndOptions::s_vibrationValues[2+1]=
{
	false,true,
	-1
};
CGUITextReadout::TextReadoutData CFrontEndOptions::s_vibrationReadoutText[2]=
{
	{	false,STR__OFF	},
	{	true,STR__ON	},
};


int CFrontEndOptions::s_buttonOrder[]=
{
	CPadConfig::PAD_CFG_UP,
	CPadConfig::PAD_CFG_DOWN,
	CPadConfig::PAD_CFG_LEFT,
	CPadConfig::PAD_CFG_RIGHT,
	CPadConfig::PAD_CFG_JUMP,
	CPadConfig::PAD_CFG_FIRE,
	CPadConfig::PAD_CFG_CATCH,
	CPadConfig::PAD_CFG_WEAPONCHANGE,
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

// Frame dimensions
static const int OPTIONS_FRAME_W=416;
static const int OPTIONS_FRAME_H=160;

// Backgrounds
int				CFrontEndOptions::s_modeBackground[MODE__COUNT]=
{
	FRM__BG3,
	FRM__BG5,
	FRM__BG7,
	FRM__BG8,
	FRM__BG9,
};


CGUITextBox			*m_loadTextBox;
int				CFrontEndOptions::s_loadModeTextIds[LOADMODE__COUNT]=
{
	STR__MEMCARD__CHECKING_MEM_CARD,		// LOADMODE__INIT
	STR__MEMCARD__CHECKING_MEM_CARD,		// LOADMODE__CHECKING
	STR__MEMCARD__UNFORMATTEDCARD,			// LOADMODE__UNFORMATTED
	STR__MEMCARD__NOFILESONCARD,			// LOADMODE__NODATA
	STR__MEMCARD__NOCARDINSLOT,				// LOADMODE__NOCARD
	STR__FRONTEND__CONFIRM_LOAD,			// LOADMODE__CONFIRMLOAD
	STR__MEMCARD__LOADINGPLEASEWIAT,		// LOADMODE__LOADING
	STR__MEMCARD__LOADOK,					// LOADMODE__LOADOK
	STR__MEMCARD__LOADERROR,				// LOADMODE__LOADERROR
};



enum
{
	ID_SCREEN_X_SLIDER=10,
	ID_SCREEN_Y_SLIDER,
	ID_SOUND_BGM_SLIDER,
	ID_SOUND_SFX_SLIDER,
	ID_SOUND_SPEECH_SLIDER,
};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		_hue = 0..4095, _brightness = 0..255, _rgb -> int[3]
	Returns:
  ---------------------------------------------------------------------- */
static void paulColourSpaceToRGB(int _hue,int _brightness,int *_rgb)
{
	static const int s_colourTab[16][3]=
	{
		{ 255,  0,  0 },
		{ 255, 97,  0 },
		{ 255,192,  0 },
		{ 222,255,  0 },
		{ 127,255,  0 },
		{  31,255,  0 },
		{   0,255, 67 },
		{   0,255,162 },
		{   0,252,255 },
		{   0,157,255 },
		{   0, 61,255 },
		{  37,  0,255 },
		{ 132,  0,255 },
		{ 228,  0,255 },
		{ 255,  0,186 },
		{ 255,  0, 91 },
	};
	const int	*c1,*c2;
	int			*cr;
	int			i,component;

	c1=&s_colourTab[(_hue>>8)&15][0];
	c2=&s_colourTab[((_hue>>8)+1)&15][0];
	cr=_rgb;
	for(i=0;i<3;i++)
	{
		component=*c1+(((*c2-*c1)*(_hue&0xff))>>8);
		component=(component*_brightness)>>8;
		*cr=component;
		c1++;
		c2++;
		cr++;
	}
}

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
	m_background->setTheDrawMode(CScrollyBackground::DRAWMODE_ADDITIVE);
	m_background->setColour(80,80,80);
	m_background->setSpeed(1,-1);
	m_background->setSpeedScale(0);
	m_background->setFrame(s_modeBackground[MODE__OPTIONS]);


	// Create the menu frames
	for(i=0;i<MODE__COUNT;i++)
	{
		CGUIControlFrame	**mm=&m_modeMenus[i];
		*mm=new ("optionsframe") CGUIControlFrame();
		(*mm)->init(NULL);
		(*mm)->setObjectXYWH((512-OPTIONS_FRAME_W)/2,(256-OPTIONS_FRAME_H)/2,OPTIONS_FRAME_W,OPTIONS_FRAME_H);
	}

	// Populate OPTIONS menu
	CGUIFactory::createValueButtonFrame(m_modeMenus[MODE__OPTIONS],
										X_BORDER,Y_BORDER,OPTIONS_FRAME_W-(X_BORDER*2),20,
										STR__FRONTEND__CONTROLS,
										&m_nextMode,MODE__CONTROL);
	CGUIFactory::createValueButtonFrame(m_modeMenus[MODE__OPTIONS],
										X_BORDER,Y_BORDER+20,OPTIONS_FRAME_W-(X_BORDER*2),20,
										STR__FRONTEND__SCREEN,
										&m_nextMode,MODE__SCREEN);
	CGUIFactory::createValueButtonFrame(m_modeMenus[MODE__OPTIONS],
										X_BORDER,Y_BORDER+40,OPTIONS_FRAME_W-(X_BORDER*2),20,
										STR__FRONTEND__SOUND,
										&m_nextMode,MODE__SOUND);
	CGUIFactory::createValueButtonFrame(m_modeMenus[MODE__OPTIONS],
										X_BORDER,Y_BORDER+60,OPTIONS_FRAME_W-(X_BORDER*2),20,
										STR__FRONTEND__LOAD,
										&m_nextMode,MODE__LOAD);
	CGUIFactory::createValueButtonFrame(m_modeMenus[MODE__OPTIONS],
										X_BORDER,OPTIONS_FRAME_H-Y_BORDER-20,OPTIONS_FRAME_W-(X_BORDER*2),20,
										STR__FRONTEND__EXIT,
										&m_exitFlag,true);

	// Populate CONTROLS menu
	CGUIFactory::createCycleButtonFrame(m_modeMenus[MODE__CONTROL],
										X_BORDER,Y_BORDER,OPTIONS_FRAME_W-(X_BORDER*2),25,
										STR__FRONTEND__CONTROL_STYLE,
										&m_controlStyle,s_controlStyleValues,s_controlStyleReadoutText);
	fr=new ("frame") CGUIGroupFrame();
	fr->init(m_modeMenus[MODE__CONTROL]);
	fr->setObjectXYWH(X_BORDER,40,OPTIONS_FRAME_W-(X_BORDER*2),60);	//292
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(0,0,26,15);	//176
		sr->setReadoutTarget(&m_controlIcons[CONTROL_UP]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(26,0,150,15);
		tb->setText(STR__FRONTEND__LOOK_UP);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(0,15,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_DOWN]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(26,15,150,15);
		tb->setText(STR__FRONTEND__LOOK_DOWN);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(0,30,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_LEFT]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(26,30,150,15);
		tb->setText(STR__FRONTEND__LEFT);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(0,45,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_RIGHT]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(26,45,150,15);
		tb->setText(STR__FRONTEND__RIGHT);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(176,0,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_JUMP]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(176+26,0,150,15);
		tb->setText(STR__FRONTEND__JUMP);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(176,15,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_FIRE]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(176+26,15,150,15);
		tb->setText(STR__FRONTEND__FIRE);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(176,30,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_CATCH]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(176+26,30,150,15);
		tb->setText(STR__FRONTEND__CATCH);
		sr=new ("spritereadout") CGUISpriteReadout();
		sr->init(fr);
		sr->setObjectXYWH(176,45,26,15);
		sr->setReadoutTarget(&m_controlIcons[CONTROL_WEAPONCHANGE]);
		sr->setReadoutData(s_controlReadoutSprites);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(176+26,45,150,15);
		tb->setText(STR__FRONTEND__WEAPONCHANGE);
	m_vibGUIOption=CGUIFactory::createCycleButtonFrame(m_modeMenus[MODE__CONTROL],
													   X_BORDER,Y_BORDER+90,OPTIONS_FRAME_W-(X_BORDER*2),25,
													   STR__FRONTEND__VIBRATION,
													   &m_vibrationStatus,s_vibrationValues,s_vibrationReadoutText);


	// Populate SCREEN menu
	CGUIFactory::createSliderButtonFrame(m_modeMenus[MODE__SCREEN],
										 X_BORDER,Y_BORDER,OPTIONS_FRAME_W-(X_BORDER*2),35,
										 STR__FRONTEND__HORIZONTAL_POSITION,
										 &m_screenXOff,0,64,
										 ID_SCREEN_X_SLIDER);
	CGUIFactory::createSliderButtonFrame(m_modeMenus[MODE__SCREEN],
										 X_BORDER,Y_BORDER+40,OPTIONS_FRAME_W-(X_BORDER*2),35,
										 STR__FRONTEND__VERTICAL_POSITION,
										 &m_screenYOff,0,32,
										 ID_SCREEN_Y_SLIDER);

	// Populate SOUND menu
	CGUIFactory::createSliderButtonFrame(m_modeMenus[MODE__SOUND],
										 X_BORDER,Y_BORDER,OPTIONS_FRAME_W-(X_BORDER*2),35,
										 STR__FRONTEND__BGM,
										 &m_bgmVolume,CSoundMediator::MIN_VOLUME,CSoundMediator::MAX_VOLUME,
										 ID_SOUND_BGM_SLIDER);
	CGUIFactory::createSliderButtonFrame(m_modeMenus[MODE__SOUND],
										 X_BORDER,Y_BORDER+35,OPTIONS_FRAME_W-(X_BORDER*2),35,
										 STR__FRONTEND__SFX,
										 &m_sfxVolume,CSoundMediator::MIN_VOLUME,CSoundMediator::MAX_VOLUME,
										 ID_SOUND_SFX_SLIDER);
	CGUIFactory::createSliderButtonFrame(m_modeMenus[MODE__SOUND],
										 X_BORDER,Y_BORDER+70,OPTIONS_FRAME_W-(X_BORDER*2),35,
										 STR__FRONTEND__SPEECH,
										 &m_speechVolume,CSoundMediator::MIN_VOLUME,CSoundMediator::MAX_VOLUME,
										 ID_SOUND_SPEECH_SLIDER);

	// Memcard menu
	m_loadTextBox=new ("textbox") CGUITextBox();
	m_loadTextBox->init(m_modeMenus[MODE__LOAD]);
	m_loadTextBox->setObjectXYWH(X_BORDER,Y_BORDER,OPTIONS_FRAME_W-(X_BORDER*2),80);
	m_loadTextBox->setText(STR__MEMCARD__CHECKING_MEM_CARD);

	// Extra memcard frames..
	m_loadModeConfirmFrame=new ("loadmodeconfirmframe") CGUIControlFrame();
	m_loadModeConfirmFrame->init(NULL);
	m_loadModeConfirmFrame->setObjectXYWH(((512-OPTIONS_FRAME_W)/2)+X_BORDER,((256-OPTIONS_FRAME_H)/2)+OPTIONS_FRAME_H-Y_BORDER-40,OPTIONS_FRAME_W-(X_BORDER*2),40);
	m_loadModeConfirmFrame->clearFlags(CGUIObject::FLAG_DRAWBORDER);
		CGUIFactory::createValueButtonFrame(m_loadModeConfirmFrame,
											0,20,OPTIONS_FRAME_W-(X_BORDER*2),20,
											STR__NO,
											&m_loadUserResponse,USERRESPONSE__NO);
		CGUIFactory::createValueButtonFrame(m_loadModeConfirmFrame,
											0,0,OPTIONS_FRAME_W-(X_BORDER*2),20,
											STR__YES,
											&m_loadUserResponse,USERRESPONSE__YES);

	m_loadModeOKFrame=new ("loadmodeokframe") CGUIControlFrame();
	m_loadModeOKFrame->init(NULL);
	m_loadModeOKFrame->setObjectXYWH(((512-OPTIONS_FRAME_W)/2)+X_BORDER,((256-OPTIONS_FRAME_H)/2)+OPTIONS_FRAME_H-Y_BORDER-40,OPTIONS_FRAME_W-(X_BORDER*2),40);
	m_loadModeOKFrame->clearFlags(CGUIObject::FLAG_DRAWBORDER);
		CGUIFactory::createValueButtonFrame(m_loadModeOKFrame,
											0,0,OPTIONS_FRAME_W-(X_BORDER*2),20,
											STR__OK,
											&m_loadUserResponse,USERRESPONSE__OK);


	// Add BACK to all of the sub menus
	for(i=1;i<MODE__COUNT;i++)
	{
		if(i!=MODE__LOAD)
		{
			CGUIFactory::createValueButtonFrame(m_modeMenus[i],
												X_BORDER,OPTIONS_FRAME_H-Y_BORDER-20,OPTIONS_FRAME_W-(X_BORDER*2),20,
												STR__FRONTEND__BACK,
												&m_nextMode,MODE__OPTIONS);
		}
	}

	getValues();

	m_saveLoadDatabase=NULL;

	m_fontBank=new ("CGameScene::Init") FontBank();
	m_fontBank->initialise( &standardFont );
	m_fontBank->setColour(80,80,80);

	m_spriteBank=new ("options sprites") SpriteBank();
	m_spriteBank->load(SPRITES_SPRITES_SPR);
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

	m_spriteBank->dump();		delete m_spriteBank;
	m_fontBank->dump();				delete m_fontBank;

	m_loadModeConfirmFrame->shutdown();
	m_loadModeOKFrame->shutdown();

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
	m_musicStarted=false;
	
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
int rh=0;
int rb=80;
int rspeed=5;
int rstep=1024;
void CFrontEndOptions::render()
{
	POLY_G4	*g4;
	int		rgb[3];

rh+=rspeed;
rh&=4095;
//paulColourSpaceToRGB(rh,rb,rgb);

	g4=GetPrimG4();
	setXYWH(g4,0,0,512,256);
	paulColourSpaceToRGB(rh,rb,rgb);
	setRGB0(g4,rgb[0],rgb[1],rgb[2]);
	paulColourSpaceToRGB(rh+(rstep*1),rb,rgb);
	setRGB1(g4,rgb[0],rgb[1],rgb[2]);
	paulColourSpaceToRGB(rh+(rstep*3),rb,rgb);
	setRGB2(g4,rgb[0],rgb[1],rgb[2]);
	paulColourSpaceToRGB(rh+(rstep*2),rb,rgb);
	setRGB3(g4,rgb[0],rgb[1],rgb[2]);
	AddPrimToList(g4,MAX_OT-1);

	m_background->render();

	// Button prompts
	renderButtonPrompts();

	// UI
	if(m_mode==MODE__LOAD)
	{
		switch(m_loadMode)
		{
			case LOADMODE__INIT:
			case LOADMODE__CHECKING:
			case LOADMODE__LOADING:
				break;

			case LOADMODE__UNFORMATTED:
			case LOADMODE__NODATA:
			case LOADMODE__NOCARD:
			case LOADMODE__LOADOK:
			case LOADMODE__LOADERROR:
				m_loadModeOKFrame->render();
				break;

			case LOADMODE__CONFIRMLOAD:
				m_loadModeConfirmFrame->render();
				break;
		}
	}
	m_modeMenus[m_mode]->render();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndOptions::think(int _frames)
{
	int		i,j,button;


	// Hide the VIBRATION option if vibration not available
	if(PadCanVibrate(0))
	{
		m_vibGUIOption->unhide();
	}
	else
	{
		if(m_vibGUIOption->isSelected())
		{
			m_modeMenus[MODE__CONTROL]->selectPreviousItem();
		}
		m_vibGUIOption->hide();
	}

	m_background->think(_frames);

	if(!CFader::isFading())
	{
		if(!m_musicStarted)
		{
			CSoundMediator::playSong();
			m_musicStarted=true;
		}

		if(m_nextMode!=m_mode)
		{
			if(m_nextMode==MODE__LOAD)
			{
				// Entering LOAD option
				setLoadMode(LOADMODE__INIT);
			}
			else if(m_mode==MODE__LOAD)
			{
				// Leaving LOAD option
				MemCard::Stop();
				delete m_saveLoadDatabase;	
				m_saveLoadDatabase=NULL;
			}

			if(m_mode!=MODE__LOAD)m_modeMenus[m_mode]->unselect();
			m_mode=m_nextMode;
			if(m_mode!=MODE__LOAD)m_modeMenus[m_mode]->select();
			m_background->setFrame(s_modeBackground[m_mode]);
		}
		if(m_mode!=MODE__LOAD)m_modeMenus[m_mode]->think(_frames);

		
		if(PadGetDown(0)&PAD_TRIANGLE)
		{
			if(m_mode==MODE__OPTIONS)
			{
				CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
				m_exitFlag=true;
			}
			else if(m_mode==MODE__LOAD)
			{
				switch(m_loadMode)
				{
					case LOADMODE__INIT:
					case LOADMODE__CHECKING:
					case LOADMODE__LOADING:
					default:
						break;

					case LOADMODE__UNFORMATTED:
					case LOADMODE__NODATA:
					case LOADMODE__NOCARD:
					case LOADMODE__CONFIRMLOAD:
					case LOADMODE__LOADOK:
					case LOADMODE__LOADERROR:
						CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
						m_nextMode=MODE__OPTIONS;
						break;
				}
			}
			else
			{
				CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
				m_nextMode=MODE__OPTIONS;
			}
		}


		if(m_mode==MODE__CONTROL)
		{
			if(m_vibrationStatus!=m_lastVibrationStatus)
			{
				m_lastVibrationStatus=m_vibrationStatus;
				PadSetVibrationIsTurnedOn(0,m_vibrationStatus);
				if(m_vibrationStatus==true)
				{
					CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_MEDIUM);
				}
			}
		}
		else if(m_mode==MODE__SCREEN)
		{
			VidSetXYOfs(m_screenXOff,m_screenYOff);
		}
		else if(m_mode==MODE__SOUND)
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
		else if(m_mode==MODE__LOAD)
		{
			if(m_loadModeConfirmFrame->isSelected())m_loadModeConfirmFrame->think(_frames);
			if(m_loadModeOKFrame->isSelected())m_loadModeOKFrame->think(_frames);

			switch(m_loadMode)
			{
				case LOADMODE__INIT:
					if(m_loadTimeInMode>10)
					{
						// If we init the memcard as the load page is selected then there's a small but noticable pause
						// This hides it a bit..
						MemCard::Start();
						m_saveLoadDatabase=new ("saveloaddb") CSaveLoadDatabase();
						setLoadMode(LOADMODE__CHECKING);
					}
					break;

				case LOADMODE__CHECKING:
					if(MemCard::GetCardStatus(0)==MemCard::CS_CardInserted)
					{
						// Scanning a new card..
						m_loadTimeInMode=0;
					}
					else if(m_loadTimeInMode>60)
					{
						// Wait for card status to settle for one second before trusting it
						if(MemCard::GetCardStatus(0)==MemCard::CS_NoCard)
						{
							setLoadMode(LOADMODE__NOCARD);
						}
						else if(MemCard::GetCardStatus(0)==MemCard::CS_UnformattedCard)
						{
							setLoadMode(LOADMODE__UNFORMATTED);
						}
						else if(MemCard::GetFileCountOnCard(0))
						{
							setLoadMode(LOADMODE__CONFIRMLOAD);
						}
						else
						{
							setLoadMode(LOADMODE__NODATA);
						}
					}
					break;

				case LOADMODE__UNFORMATTED:
					if(MemCard::GetCardStatus(0)!=MemCard::CS_UnformattedCard)
					{
						setLoadMode(LOADMODE__CHECKING);
					}
					else if(m_loadUserResponse==USERRESPONSE__OK)
					{
						m_nextMode=MODE__OPTIONS;
					}
					break;

				case LOADMODE__NODATA:
					if(MemCard::GetCardStatus(0)!=MemCard::CS_ValidCard)
					{
						setLoadMode(LOADMODE__CHECKING);
					}
					else if(m_loadUserResponse==USERRESPONSE__OK)
					{
						m_nextMode=MODE__OPTIONS;
					}
					break;

				case LOADMODE__NOCARD:
					if(MemCard::GetCardStatus(0)!=MemCard::CS_NoCard)
					{
						setLoadMode(LOADMODE__CHECKING);
					}
					else if(m_loadUserResponse==USERRESPONSE__OK)
					{
						m_nextMode=MODE__OPTIONS;
					}
					break;

				case LOADMODE__CONFIRMLOAD:
					if(MemCard::GetCardStatus(0)!=MemCard::CS_ValidCard)
					{
						setLoadMode(LOADMODE__CHECKING);
					}
					else if(m_loadUserResponse==USERRESPONSE__YES)
					{
						setLoadMode(LOADMODE__LOADING);
						if(!m_saveLoadDatabase->startLoad(0))
						{
							setLoadMode(LOADMODE__LOADERROR);
						}
					}
					else if(m_loadUserResponse==USERRESPONSE__NO)
					{
						m_nextMode=MODE__OPTIONS;
					}
					break;

				case LOADMODE__LOADING:
					{
					int stat=m_saveLoadDatabase->getLoadStatus();
					if(stat==CSaveLoadDatabase::FINISHED_OK)
					{
						getValues();
						setLoadMode(LOADMODE__LOADOK);
					}
					else if(stat==CSaveLoadDatabase::FAILED)
					{
						setLoadMode(LOADMODE__LOADERROR);
					}
					}
					break;

				case LOADMODE__LOADOK:
					if(m_loadUserResponse==USERRESPONSE__OK)
					{
						m_nextMode=MODE__OPTIONS;
					}
					break;

				case LOADMODE__LOADERROR:
					if(m_loadUserResponse==USERRESPONSE__OK)
					{
						setLoadMode(LOADMODE__CHECKING);
					}
					break;
			}
			if(m_saveLoadDatabase)
			{
				m_saveLoadDatabase->think();
			}
			m_loadTimeInMode+=_frames;
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFrontEndOptions::getValues()
{
	m_bgmVolume=CSoundMediator::getVolume(CSoundMediator::VOL_SONG);
	m_sfxVolume=CSoundMediator::getVolume(CSoundMediator::VOL_SFX);
	m_speechVolume=CSoundMediator::getVolume(CSoundMediator::VOL_SPEECH);
	m_controlStyle=CPadConfig::getConfig();
	m_vibrationStatus=m_lastVibrationStatus=PadGetVibrationIsTurnedOn(0);
	m_screenXOff=VidGetXOfs();
	m_screenYOff=VidGetYOfs();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFrontEndOptions::setLoadMode(int _newMode)
{
	if(m_loadModeConfirmFrame->isSelected())m_loadModeConfirmFrame->unselect();
	if(m_loadModeOKFrame->isSelected())m_loadModeOKFrame->unselect();

	m_loadTextBox->setText(s_loadModeTextIds[_newMode]);
	m_loadMode=_newMode;
	m_loadTimeInMode=0;
	m_loadUserResponse=USERRESPONSE__NONE;

	switch(m_loadMode)
	{
		case LOADMODE__INIT:
		case LOADMODE__CHECKING:
		case LOADMODE__LOADING:
			break;

		case LOADMODE__UNFORMATTED:
		case LOADMODE__NODATA:
		case LOADMODE__NOCARD:
		case LOADMODE__LOADOK:
		case LOADMODE__LOADERROR:
			m_loadModeOKFrame->select();
			break;

		case LOADMODE__CONFIRMLOAD:
			m_loadModeConfirmFrame->select();
			break;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFrontEndOptions::renderButtonPrompts()
{
	CGUIObject::GUIId	id;
	int					renderCross=false;
	int					renderArrows=false;
	int					renderTriangle=false;
	sFrameHdr			*fh1,*fh2;
	int					x,y,width;


	// What shall we draw today?
	switch(m_mode)
	{
		case MODE__OPTIONS:
		case MODE__CONTROL:
			renderCross=true;
			renderTriangle=true;
			break;

		case MODE__SCREEN:
			id=m_modeMenus[MODE__SCREEN]->getIdOfSelectedItem();
			if(id==ID_SCREEN_X_SLIDER||id==ID_SCREEN_Y_SLIDER)
			{
				renderArrows=true;
			}
			else
			{
				renderCross=true;
			}
			renderTriangle=true;
			break;

		case MODE__SOUND:
			id=m_modeMenus[MODE__SOUND]->getIdOfSelectedItem();
			if(id==ID_SOUND_BGM_SLIDER||id==ID_SOUND_SFX_SLIDER||id==ID_SOUND_SPEECH_SLIDER)
			{
				renderArrows=true;
			}
			else
			{
				renderCross=true;
			}
			renderTriangle=true;
			break;

		case MODE__LOAD:
			switch(m_loadMode)
			{
				case LOADMODE__INIT:
				case LOADMODE__CHECKING:
				case LOADMODE__LOADING:
					break;

				case LOADMODE__UNFORMATTED:
				case LOADMODE__NODATA:
				case LOADMODE__NOCARD:
				case LOADMODE__CONFIRMLOAD:
				case LOADMODE__LOADOK:
				case LOADMODE__LOADERROR:
					renderCross=true;
					renderTriangle=true;
					break;
			}
			break;
	}


	if(renderCross)
	{
		fh1=m_spriteBank->getFrameHeader(FRM__BUTX);
		width=fh1->W+OPTIONS_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_fontBank->getStringWidth(STR__FRONTEND__CROSS_TO_SELECT);
		x=128-(width/2);
		m_spriteBank->printFT4(fh1,x,OPTIONS_INSTRUCTIONS_Y_POS+OPTIONS_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
		x+=fh1->W+OPTIONS_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
		m_fontBank->print(x,OPTIONS_INSTRUCTIONS_Y_POS,STR__FRONTEND__CROSS_TO_SELECT);
	}

	if(renderArrows)
	{
		fh1=m_spriteBank->getFrameHeader(FRM__BUTL);
		fh2=m_spriteBank->getFrameHeader(FRM__BUTR);
		width=fh1->W+OPTIONS_INSTRUCTIONS_GAP_BETWEEN_BUTTONS+fh2->W+OPTIONS_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_fontBank->getStringWidth(STR__FRONTEND__ARROWS_TO_ADJUST);
		x=128-(width/2);
		m_spriteBank->printFT4(fh1,x,OPTIONS_INSTRUCTIONS_Y_POS+OPTIONS_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
		x+=fh1->W+OPTIONS_INSTRUCTIONS_GAP_BETWEEN_BUTTONS;
		m_spriteBank->printFT4(fh2,x,OPTIONS_INSTRUCTIONS_Y_POS+OPTIONS_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
		x+=fh2->W+OPTIONS_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
		m_fontBank->print(x,OPTIONS_INSTRUCTIONS_Y_POS,STR__FRONTEND__ARROWS_TO_ADJUST);
	}

	if(renderTriangle)
	{
		fh1=m_spriteBank->getFrameHeader(FRM__BUTT);
		width=fh1->W+OPTIONS_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_fontBank->getStringWidth(STR__FRONTEND__TRIANGLE_TO_GO_BACK);
		x=256+128-(width/2);
		m_spriteBank->printFT4(fh1,x,OPTIONS_INSTRUCTIONS_Y_POS+OPTIONS_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
		x+=fh1->W+OPTIONS_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
		m_fontBank->print(x,OPTIONS_INSTRUCTIONS_Y_POS,STR__FRONTEND__TRIANGLE_TO_GO_BACK);
	}
}

/*===========================================================================
 end */