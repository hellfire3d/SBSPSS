/*=========================================================================

	pause.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include 	"system\vid.h"
#include "game\pause.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef	__GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef	__GUI_GFACTORY_H__
#include "gui\gfactory.h"
#endif

#ifndef __GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
#endif

#ifndef __GUI_GBUTTON_H__
#include "gui\gbutton.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad/pads.h"
#endif

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __STRING_ENUMS__
#include <trans.h>
#endif

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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static const int	FRAME_WIDTH		=352;
static const int	FRAME_HEIGHT	=160-64;
static const int	TEXT_BOX_WIDTH	=200;
static const int	TEXT_BOX_HEIGHT	=20;
static const int	TEXT_SPACING	=16;
extern int newmode;

extern int invincibleSponge;
int	inv_data[]={0,1,-1};
static CGUITextReadout::TextReadoutData s_yesNoReadoutData[2]={{0,STR__OFF},{1,STR__ON}};
static CGUIGroupFrame	*s_vibFrame;


void CPauseMenu::init()
{
	int			ypos;
	CGUITextBox	*tb;


	// MAIN MENU
	m_pauseGuiFrame=new ("Conversation GUI") CGUIControlFrame();
	m_pauseGuiFrame->init(0);
	m_pauseGuiFrame->setObjectXYWH((INGAME_SCREENW-FRAME_WIDTH)/2,(INGAME_SCREENH-FRAME_HEIGHT)/2,FRAME_WIDTH,FRAME_HEIGHT);
	m_pauseGuiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);

	ypos=TEXT_SPACING/3;
	tb=new ("textbox") CGUITextBox();
	tb->init(m_pauseGuiFrame);
	tb->setObjectXYWH((FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT);
	tb->setText(STR__PAUSE_MENU__GAME_PAUSED);
	ypos+=TEXT_SPACING*2;
	CGUIFactory::createValueButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__PAUSE_MENU__CONTINUE,
										&m_responseFlag,RESPONSE__CONTINUE);
	ypos+=TEXT_SPACING;
	{
		// Vibration toggle
		CGUIGroupFrame		*fr;
		CGUITextBox			*tb;
		CGUIValueButton		*vb;
		CGUITextReadout		*tr;

		fr=new ("frame") CGUIGroupFrame();
		m_vibGUIOption=fr;
		fr->init(m_pauseGuiFrame);
		fr->setObjectXYWH((FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT);
			tb=new ("textbox") CGUITextBox();
			tb->init(fr);
			tb->setObjectXYWH(0,0,(TEXT_BOX_WIDTH/3)*2,TEXT_BOX_HEIGHT);
			tb->setText(STR__PAUSE_MENU__VIBRATION);
			vb=new ("valuebutton") CGUIValueButton();
			vb->init(fr);
			vb->setButtonTarget(&m_vibrationChangeFlag);
			vb->setButtonValue(true);
			tr=new ("textreadout") CGUITextReadout();
			tr->init(fr);
			tr->setObjectXYWH((TEXT_BOX_WIDTH/3)*2,0,(TEXT_BOX_WIDTH/3)*1,TEXT_BOX_HEIGHT);
			tr->setReadoutTarget(&m_vibrationState);
			tr->setReadoutData(s_yesNoReadoutData);
		s_vibFrame=fr;
	}
	ypos+=TEXT_SPACING;
	CGUIFactory::createValueButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__PAUSE_MENU__QUIT,
										&m_responseFlag,RESPONSE__QUIT);

#ifdef __VERSION_DEBUG__
	ypos+=8;
	CGUIFactory::createCycleButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__INVINCIBILE_SPONGEBOB,
										&invincibleSponge,inv_data,s_yesNoReadoutData);
	ypos+=TEXT_SPACING;
#if defined (__USER_paul__) || defined (__USER_charles__)
	CGUIFactory::createValueButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__BASICUNARMED_MODE,
										&newmode,PLAYER_MODE_BASICUNARMED);
	ypos+=8;
	CGUIFactory::createValueButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__FULLUNARMED_MODE,
										&newmode,PLAYER_MODE_FULLUNARMED);
	ypos+=8;
	CGUIFactory::createValueButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__BALLOON_MODE,
										&newmode,PLAYER_MODE_BALLOON);
	ypos+=8;
	CGUIFactory::createValueButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__BUBBLEMIXTURE_MODE,
										&newmode,PLAYER_MODE_BUBBLE_MIXTURE);
	ypos+=8;
	CGUIFactory::createValueButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__NET_MODE,
										&newmode,PLAYER_MODE_NET);
	ypos+=8;
	CGUIFactory::createValueButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__CORALBLOWER_MODE,
										&newmode,PLAYER_MODE_CORALBLOWER);
	ypos+=8;
	CGUIFactory::createValueButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__JELLYLAUNCHER_MODE,
										&newmode,PLAYER_MODE_JELLY_LAUNCHER);
	ypos+=8;
	CGUIFactory::createValueButtonFrame(m_pauseGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__DEAD_MODE,
										&newmode,PLAYER_MODE_DEAD);
#endif
#endif


	// CONFIRM QUIT MENU
	m_confirmQuitGuiFrame=new ("Conversation GUI") CGUIControlFrame();
	m_confirmQuitGuiFrame->init(0);
	m_confirmQuitGuiFrame->setObjectXYWH((INGAME_SCREENW-FRAME_WIDTH)/2,(INGAME_SCREENH-FRAME_HEIGHT)/2,FRAME_WIDTH,FRAME_HEIGHT);
	m_confirmQuitGuiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);

	ypos=TEXT_SPACING/3;
	tb=new ("textbox") CGUITextBox();
	tb->init(m_confirmQuitGuiFrame);
	tb->setObjectXYWH((FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT);
	tb->setText(STR__PAUSE_MENU__CONFIRM_QUIT);
	ypos+=TEXT_SPACING*2;
	CGUIFactory::createValueButtonFrame(m_confirmQuitGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__NO,
										&m_responseFlag,RESPONSE__CONFIRM_QUIT_NO);
	ypos+=TEXT_SPACING;
	CGUIFactory::createValueButtonFrame(m_confirmQuitGuiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,ypos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__YES,
										&m_responseFlag,RESPONSE__CONFIRM_QUIT_YES);


	m_fontBank=new ("PauseMenuFont") FontBank();
	m_fontBank->initialise(&standardFont);

	m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::shutdown()
{
	m_fontBank->dump();		delete m_fontBank;
	m_confirmQuitGuiFrame->shutdown();
	m_pauseGuiFrame->shutdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::select()
{
	int		chapter,level;
	m_active=true;
	m_padDebounce=true;
	m_currentState=STATE__MAIN_MENU;
	m_responseFlag=RESPONSE__WAITING;
	m_vibrationChangeFlag=false;
	m_vibrationState=PadGetVibrationIsTurnedOn(0);
	m_pauseGuiFrame->select();
	s_vibFrame->think(1);	// This just makes sure that the readout reads correctly on the first frame


	chapter=GameScene.getChapterNumber()-1;
	level=GameScene.getLevelNumber()-1;
	m_SpeechPlaying=false;
	if (CXAStream::IsPlaying())
	{
		m_SpeechPlaying=true;
		CXAStream::Pause();
	}

	CPadVibrationManager::stopAllVibration();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::unselect()
{
	m_active=false;
	if (m_SpeechPlaying)
	{
		CXAStream::Resume();
		m_SpeechPlaying=false;
	}

}
/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::think(int _frames)
{
	if(m_active)
	{
		if(m_pauseGuiFrame->isSelected())
		{
			// Hide the VIBRATION option if vibration not available
			if(PadCanVibrate(0))
			{
				m_vibGUIOption->unhide();
			}
			else
			{
				if(m_vibGUIOption->isSelected())
				{
					m_pauseGuiFrame->selectPreviousItem();
				}
				m_vibGUIOption->hide();
			}
		}


		if(PadGetDown(0)&PAD_START)
		{
			if(!m_padDebounce)
			{
				if(m_pauseGuiFrame->isSelected())m_pauseGuiFrame->unselect();
				if(m_confirmQuitGuiFrame->isSelected())m_confirmQuitGuiFrame->unselect();
				unselect();
			}
		}
		else
		{
			switch(m_currentState)
			{
				// Main menu
				case STATE__MAIN_MENU:
					m_pauseGuiFrame->think(_frames);
					switch(m_responseFlag)
					{
						case RESPONSE__WAITING:
							if(m_vibrationChangeFlag)
							{
								m_vibrationState=PadGetVibrationIsTurnedOn(0)==true?false:true;
								PadSetVibrationIsTurnedOn(0,m_vibrationState);
								if(m_vibrationState)
								{
									CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_MEDIUM);
								}
								else
								{
									CPadVibrationManager::stopAllVibration();
								}
								m_vibrationChangeFlag=false;
							}
							break;

						case RESPONSE__CONTINUE:
							m_pauseGuiFrame->unselect();
							unselect();
							break;

						case RESPONSE__QUIT:
							m_pauseGuiFrame->unselect();
							m_confirmQuitGuiFrame->select();
							m_currentState=STATE__CONFIRM_QUIT;
							m_responseFlag=RESPONSE__WAITING;
							break;

						case RESPONSE__CONFIRM_QUIT_YES:
						case RESPONSE__CONFIRM_QUIT_NO:
							ASSERT(0);
							break;
					}
					break;

				// Confirm quit menu
				case STATE__CONFIRM_QUIT:
					m_confirmQuitGuiFrame->think(_frames);
					switch(m_responseFlag)
					{
						case RESPONSE__WAITING:
							break;

						case RESPONSE__CONTINUE:
						case RESPONSE__QUIT:
							ASSERT(0);
							break;

						case RESPONSE__CONFIRM_QUIT_YES:
							m_confirmQuitGuiFrame->unselect();
							CGameScene::setReadyToExit();
							CXAStream::Reset();
							unselect();
							break;

						case RESPONSE__CONFIRM_QUIT_NO:
							m_pauseGuiFrame->select();
							m_confirmQuitGuiFrame->unselect();
							m_currentState=STATE__MAIN_MENU;
							m_responseFlag=RESPONSE__WAITING;
							break;
					}
					break;
			}
		}

		if(m_padDebounce)
		{
			m_padDebounce=false;
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::render()
{
	if(m_active)
	{
		switch(m_currentState)
		{
			case STATE__MAIN_MENU:
				renderLives();
				m_pauseGuiFrame->render();
				break;
			case STATE__CONFIRM_QUIT:
				m_confirmQuitGuiFrame->render();
				break;
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::renderLives()
{
	CGameSlotManager::GameSlot	*gameSlot;
	int			x,y,textYOff;
	SpriteBank	*sb;
	sFrameHdr	*fh;
	char		buf[10];

	gameSlot=CGameSlotManager::getSlotData();

/*
	// Spat count
	x=100;
	y=137;
	sb=GameScene.getSpriteBank();
	fh=sb->getFrameHeader(FRM__SPATULA);
	sb->printFT4(fh,x,y,0,0,0);
	x+=fh->W;
	sprintf(buf,"%d/%d",GameScene.getPlayer()->getSpatulasHeld(),GameScene.GetLevel().getTotalSpatCount());
	textYOff=fh->H-m_fontBank->getStringHeight(buf);
	y+=textYOff;
	m_fontBank->print(x,y,buf);
*/

	// Lives
	x=350;
	y=137;
	sb=GameScene.getSpriteBank();
	fh=sb->getFrameHeader(FRM__PANTS);
	sb->printFT4(fh,x,y,0,0,0);
	x+=fh->W;
	sprintf(buf,"x%d",gameSlot->m_lives);
	textYOff=fh->H-m_fontBank->getStringHeight(buf);
	y+=textYOff;
	m_fontBank->print(x,y,buf);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPauseMenu::isActive()
{
	return m_active;
}


/*===========================================================================
 end */
