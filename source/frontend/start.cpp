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

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

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

#ifndef __GUI_GFACTORY_H__
#include "gui\gfactory.h"
#endif

#ifndef __GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef __GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef __SPR_SHOP_H__
#include <shop.h>
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

const int CFrontEndStart::s_itemFrames[]=
{
	FRM_TEDDY,		// SHOPITEM_TEDDY
	FRM_SARNIE,		// SHOPITEM_SARNIE
	FRM_CUPCAKE,	// SHOPITEM_CUPCAKE
	FRM_PREZZIE,	// SHOPITEM_PREZZIE
	FRM_JELLY2,		// SHOPITEM_JELLY2
	FRM_CAKE,		// SHOPITEM_CAKE
	FRM_BLOWER,		// SHOPITEM_BLOWER
	FRM_PARTYHAT,	// SHOPITEM_PARTYHAT
};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::init()
{
	CGUITextBox			*tb;

	
	m_emptyGuiFrame=new ("Frame") CGUIGroupFrame();
	m_emptyGuiFrame->init(NULL);
	m_emptyGuiFrame->setObjectXYWH((512-416)/2,130,416,96);
	m_emptyGuiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);

	m_confirmEraseGuiFrame=new ("Frame") CGUIControlFrame();
	m_confirmEraseGuiFrame->init(NULL);
	m_confirmEraseGuiFrame->setObjectXYWH((512-416)/2,130,416,96);
		CGUIFactory::createValueButtonFrame(m_confirmEraseGuiFrame,
											8,55,400,20,
											STR__NO,&m_confirmFlag,CONFIRM_NO);
		CGUIFactory::createValueButtonFrame(m_confirmEraseGuiFrame,
											8,70,400,20,
											STR__YES,&m_confirmFlag,CONFIRM_YES);
		tb=new ("textbox") CGUITextBox();
		tb->init(m_confirmEraseGuiFrame);
		tb->setObjectXYWH(8,5,400,35);
		tb->setText(STR__SLOT_SELECT_SCREEN__CONFIRM_ERASE_GAME);

	m_createdSlotGuiFrame=new ("Frame") CGUIControlFrame();
	m_createdSlotGuiFrame->init(NULL);
	m_createdSlotGuiFrame->setObjectXYWH((512-416)/2,130,416,96);
		CGUIFactory::createValueButtonFrame(m_createdSlotGuiFrame,
											8,55,400,20,
											STR__OK,&m_confirmFlag,CONFIRM_OK);
		tb=new ("textbox") CGUITextBox();
		tb->init(m_createdSlotGuiFrame);
		tb->setObjectXYWH(8,5,400,35);
		tb->setText(STR__SLOT_SELECT_SCREEN__NEW_SLOT_CREATED);

	m_font=new ("ChooseSlotFont") FontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(FontBank::JUST_LEFT);
	m_font->setOt(0);
	m_font->setPrintArea(0,0,712,256);

	m_spriteBank=new ("PartItemSprites") SpriteBank();
	m_spriteBank->load(SHOP_SHOP_SPR);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::shutdown()
{
	m_spriteBank->dump();		delete m_spriteBank;
	m_font->dump();				delete m_font;

	m_createdSlotGuiFrame->shutdown();
	m_confirmEraseGuiFrame->shutdown();
	m_emptyGuiFrame->shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::select()
{
	CFader::setFadingIn();

	m_state=STATE_SELECT;
	m_selectedSlot=0;
	m_slotDrawOffset=0;

	CSoundMediator::setSong(CSoundMediator::SONG_MEMCARD2);
	m_musicStarted=false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::unselect()
{
	CSoundMediator::dumpSong();
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
	setRGB0(g4,  0,150, 50);
	setRGB1(g4,  0,150, 50);
	setRGB2(g4,150, 50,  0);
	setRGB3(g4,150, 50,  0);
	AddPrimToList(g4,MAX_OT-1);

	drawGameSlot(m_slotDrawOffset,m_selectedSlot);
	if(m_slotDrawOffset)
	{
		int	ofs;
		ofs=m_slotDrawOffset<0?512:-512;
		drawGameSlot(ofs+m_slotDrawOffset,m_lastSelectedSlot);
	}

	switch(m_state)
	{
		case STATE_SELECT:
			if(!m_slotDrawOffset)
			{
				drawInstructions();
			}
			m_emptyGuiFrame->render();
			break;

		case STATE_CONFIRM_ERASE:
			m_confirmEraseGuiFrame->render();
			break;

		case STATE_EXITING_TO_FRONT_END:
		case STATE_EXITING_TO_GAME:
			m_emptyGuiFrame->render();
			break;

		case STATE_SLOT_CREATED:
			m_createdSlotGuiFrame->render();
			break;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::drawGameSlot(int _xOff,int _slotNumber)
{
	int							xbase;
	CGameSlotManager::GameSlot	*gameSlot;
	TPOLY_F4						*f4;
	int							x,y;
	sFrameHdr					*fh;
	int							i;
	char						buf[100];

	xbase=_xOff+SLOT_FRAME_X;
	CGameSlotManager::setActiveSlot(_slotNumber);
	gameSlot=CGameSlotManager::getSlotData();

	drawBambooBorder(xbase,SLOT_FRAME_Y,SLOT_FRAME_W,SLOT_FRAME_H,3);
	f4=GetPrimTF4();
	setXYWH(f4,xbase,SLOT_FRAME_Y,SLOT_FRAME_W,SLOT_FRAME_H);
	setRGB0(f4,  0,  0, 90);
	setTSemiTrans(f4,true);
	AddPrimToList(f4,3);

	x=xbase+SLOT_SLOTNUMBER_X;
	y=SLOT_FRAME_Y+SLOT_SLOTNUMBER_Y;
	sprintf(buf,TranslationDatabase::getString(STR__SLOT_SELECT_SCREEN__SLOT_NUMBER),_slotNumber+1,4);
	m_font->print(x,y,buf);

	if(gameSlot->m_isInUse)
	{
		int		chapter,level;

		gameSlot->getHighestLevelOpen(&chapter,&level);
		sprintf(buf,TranslationDatabase::getString(STR__SLOT_SELECT_SCREEN__LEVEL_REACHED),chapter+1,level+1);
		m_font->print(xbase+SLOT_LEVEL_TEXT_X,SLOT_FRAME_Y+SLOT_LEVEL_TEXT_Y,buf);

		x=xbase+SLOT_TOKENCOUNT_X;
		y=SLOT_FRAME_Y+SLOT_TOKENCOUNT_Y;
		fh=m_spriteBank->getFrameHeader(FRM_SMALLTOKEN);
		m_spriteBank->printFT4(fh,x,y,0,0,2);
		x+=fh->W;
		sprintf(buf,"x%d",CGameSlotManager::getSlotData()->getNumberOfKelpTokensHeld());
		m_font->print(x,y,buf);

		x=xbase+SLOT_ITEM_X;
		y=SLOT_FRAME_Y+SLOT_ITEM_Y;
		for(i=0;i<8;i++)
		{
			POLY_FT4	*ft4;
			ft4=m_spriteBank->printFT4(s_itemFrames[i],x,y,0,0,2);
			if(!gameSlot->isPartyItemHeld(i))
			{
				setRGB0(ft4,35,35,35);
			}
			x+=SLOT_ITEM_YGAP;
		}
	}
	else
	{
		m_font->setJustification(FontBank::JUST_CENTRE);
		m_font->print(xbase+SLOT_EMPTYTEXT_X,SLOT_FRAME_Y+SLOT_EMPTYTEXT_Y,STR__SLOT_SELECT_SCREEN__EMPTY_SLOT);
		m_font->setJustification(FontBank::JUST_LEFT);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::drawInstructions()
{
	int				slotInUse;
	SpriteBank		*sb;
	sFrameHdr		*fh1,*fh2;
	int				width;
	int				x,y;
	int				text;

	CGameSlotManager::setActiveSlot(m_selectedSlot);
	slotInUse=CGameSlotManager::getSlotData()->m_isInUse;

	sb=CGameScene::getSpriteBank();
	m_font->setColour(255,255,255);

	y=INSTRUCTIONS_YSTART;
	fh1=sb->getFrameHeader(FRM__BUTL);
	fh2=sb->getFrameHeader(FRM__BUTR);
	width=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS+fh2->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(STR__SLOT_SELECT_SCREEN__LEFT_RIGHT_TO_SELECT_SLOT);
	x=256-(width/2);
	sb->printFT4(fh1,x,y+INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS;
	sb->printFT4(fh2,x,y+INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh2->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_font->print(x,y,STR__SLOT_SELECT_SCREEN__LEFT_RIGHT_TO_SELECT_SLOT);

	text=slotInUse?STR__SLOT_SELECT_SCREEN__CROSS_TO_CONFIRM:STR__SLOT_SELECT_SCREEN__CROSS_TO_CREATE;
	y+=INSTRUCTIONS_Y_SPACE_BETWEEN_LINES;
	fh1=sb->getFrameHeader(FRM__BUTX);
	width=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(text);
	x=256-(width/2);
	sb->printFT4(fh1,x,y+INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_font->print(x,y,text);

	y+=INSTRUCTIONS_Y_SPACE_BETWEEN_LINES;
	fh1=sb->getFrameHeader(FRM__BUTC);
	width=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(STR__SLOT_SELECT_SCREEN__CIRCLE_TO_ERASE_SLOT);
	x=256-(width/2);
	if(slotInUse)
	{
		sb->printFT4(fh1,x,y+INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	}
	x+=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	if(slotInUse)
	{
		m_font->print(x,y,STR__SLOT_SELECT_SCREEN__CIRCLE_TO_ERASE_SLOT);
	}

	y+=INSTRUCTIONS_Y_SPACE_BETWEEN_LINES;
	fh1=sb->getFrameHeader(FRM__BUTT);
	width=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(STR__SLOT_SELECT_SCREEN__TRIANGLE_TO_EXIT);
	x=256-(width/2);
	sb->printFT4(fh1,x,y+INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_font->print(x,y,STR__SLOT_SELECT_SCREEN__TRIANGLE_TO_EXIT);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndStart::think(int _frames)
{
	if(!CFader::isFading())
	{
		if(!m_musicStarted)
		{
			CSoundMediator::playSong();
			m_musicStarted=true;
		}

		if(m_slotDrawOffset==0)
		{
			if(m_state==STATE_SELECT)
			{
				// Select a slot
				int	pad=PadGetDown(0);
				if(pad&PAD_LEFT)
				{
					m_lastSelectedSlot=m_selectedSlot;
					if(--m_selectedSlot<0)
					{
						m_selectedSlot=CGameSlotManager::NUM_GAME_SLOTS-1;
					}
					m_slotDrawOffset=-500;
					CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__MOVE_CURSOR);
				}
				else if(pad&PAD_RIGHT)
				{
					m_lastSelectedSlot=m_selectedSlot;
					if(++m_selectedSlot>=CGameSlotManager::NUM_GAME_SLOTS)
					{
						m_selectedSlot=0;
					}
					m_slotDrawOffset=+500;
					CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__MOVE_CURSOR);
				}
				else if(pad&PAD_CROSS)
				{
					CGameSlotManager::GameSlot	*gameSlot;
					CGameSlotManager::setActiveSlot(m_selectedSlot);
					gameSlot=CGameSlotManager::getSlotData();
					if(gameSlot->m_isInUse)
					{
						int	chapter,level;
						gameSlot->getHighestLevelOpen(&chapter,&level);
						CMapScene::setLevelToStartOn(chapter,level);
						gameSlot->m_lives=CGameSlotManager::INITIAL_LIVES;
						gameSlot->m_continues=CGameSlotManager::INITIAL_CONTINUES;
						m_state=STATE_EXITING_TO_GAME;
						CFader::setFadingOut();
						CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
					}
					else
					{
						m_state=STATE_SLOT_CREATED;
						m_confirmFlag=CONFIRM_NONE;
						m_createdSlotGuiFrame->select();
						gameSlot->m_isInUse=true;
						CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
					}
				}
				else if(pad&PAD_CIRCLE)
				{
					CGameSlotManager::GameSlot	*gameSlot;
					CGameSlotManager::setActiveSlot(m_selectedSlot);
					gameSlot=CGameSlotManager::getSlotData();
					if(gameSlot->m_isInUse)
					{
						m_state=STATE_CONFIRM_ERASE;
						m_confirmFlag=CONFIRM_NONE;
						m_confirmEraseGuiFrame->select();
						CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
					}
				}
				else if(pad&PAD_TRIANGLE)
				{
					m_state=STATE_EXITING_TO_FRONT_END;
					CFader::setFadingOut();
					CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
				}
				m_emptyGuiFrame->think(_frames);
			}
			else if(m_state==STATE_SLOT_CREATED)
			{
				m_createdSlotGuiFrame->think(_frames);
				if(m_confirmFlag==CONFIRM_OK)
				{
					m_createdSlotGuiFrame->unselect();
					m_state=STATE_SELECT;
				}
			}
			else if(m_state==STATE_CONFIRM_ERASE)
			{
				m_confirmEraseGuiFrame->think(_frames);
				if(m_confirmFlag==CONFIRM_YES)
				{
					CGameSlotManager::eraseGameSlot(m_selectedSlot);
					m_state=STATE_SELECT;
					m_confirmEraseGuiFrame->unselect();
				}
				else if(m_confirmFlag==CONFIRM_NO)
				{
					m_state=STATE_SELECT;
					m_confirmEraseGuiFrame->unselect();
				}
			}
		}
		else
		{
			// Slide the slot boxes about..
			for(int i=0;i<_frames;i++)
			{
				int	delta=m_slotDrawOffset/3;
				if(m_slotDrawOffset<0)
				{
					if(!delta)delta=-1;
				}
				else if(m_slotDrawOffset>0)
				{
					if(!delta)delta=+1;
				}
				else
				{
					break;
				}
				m_slotDrawOffset-=delta;
			}
		}
	}

	/*
	if(!m_shuttingDown)
	{
		if(/m_selectedSlot!=NO_SLOT_SELECTED||
		   m_escapeToTitles||/
		{			
			m_shuttingDown=true;
			CFader::setFadingOut();
		}
	}
	*/
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndStart::isReadyToExit()
{
	return !CFader::isFading()&&(m_state==STATE_EXITING_TO_FRONT_END||m_state==STATE_EXITING_TO_GAME);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CFrontEndScene::FrontEndMode CFrontEndStart::getNextMode()
{
	return m_state==STATE_EXITING_TO_GAME?CFrontEndScene::MODE__EXIT_TO_GAME:CFrontEndScene::MODE__MAIN_TITLES;
}

/*===========================================================================
 end */