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


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int result;
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
	m_font->setOt(3);
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
typedef struct
{
	int	x,y,w,h;
	int	levelx,levely;
	int	itemx,itemy,itemgap;
	int	tokenx,tokeny;
	int	emptyslotx,emptysloty;
}SLOTDATA;
SLOTDATA slotdata=
{
	(512-416)/2,30,416,64,		// x,y,w,h
	15,5,						// levelx,levely
	16,25,50,					// itemx,itemy,itemgap
	250,5,						// tokenx,tokeny
	416/2,15,					// emptyslotx,emptysloty
};


static const int s_itemFrames[]=
{
	FRM_BLOWER,		// SHOPITEM_BLOWER
	FRM_CAKE,		// SHOPITEM_CAKE
	FRM_CUPCAKE,	// SHOPITEM_CUPCAKE
	FRM_JELLY2,		// SHOPITEM_JELLY2
	FRM_PARTYHAT,	// SHOPITEM_PARTYHAT
	FRM_PREZZIE,	// SHOPITEM_PREZZIE
	FRM_SARNIE,		// SHOPITEM_SARNIE
	FRM_TEDDY,		// SHOPITEM_TEDDY
};
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
			m_emptyGuiFrame->render();
			if(!m_slotDrawOffset)
			{
				drawInstructions();
			}
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

#ifdef __USER_paul__
char buf[100];
sprintf(buf,"slot %d",m_selectedSlot);
m_font->print(65,7,buf);
#endif
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
	POLY_F4						*f4;
	int							x,y;
	sFrameHdr					*fh;
	char						buf[100];
	int							i;

	xbase=_xOff+slotdata.x;
	CGameSlotManager::setActiveSlot(_slotNumber);
	gameSlot=CGameSlotManager::getSlotData();

	drawBambooBorder(xbase,slotdata.y,slotdata.w,slotdata.h,3);
	f4=GetPrimF4();
	setXYWH(f4,xbase,slotdata.y,slotdata.w,slotdata.h);
	setRGB0(f4,  0,  0, 90);
	setSemiTrans(f4,true);
	AddPrimToList(f4,3);

	if(gameSlot->m_isInUse)
	{
		m_font->print(xbase+slotdata.levelx,slotdata.y+slotdata.levely,"Level 0-0");

		x=xbase+slotdata.tokenx;
		y=slotdata.y+slotdata.tokeny;
		fh=m_spriteBank->getFrameHeader(FRM_SMALLTOKEN);
		m_spriteBank->printFT4(fh,x,y,0,0,2);
		x+=fh->W;
		sprintf(buf,"x%d",CGameSlotManager::getSlotData()->getNumberOfKelpTokensHeld());
		m_font->print(x,y,buf);

		x=xbase+slotdata.itemx;
		y=slotdata.y+slotdata.itemy;
		for(i=0;i<8;i++)
		{
			POLY_FT4	*ft4;
			ft4=m_spriteBank->printFT4(s_itemFrames[i],x,y,0,0,2);
			if(!gameSlot->isPartyItemHeld(i))
			{
				setRGB0(ft4,35,35,35);
			}
			x+=slotdata.itemgap;
		}
	}
	else
	{
		m_font->setJustification(FontBank::JUST_CENTRE);
		m_font->print(xbase+slotdata.emptyslotx,slotdata.y+slotdata.emptysloty,"EMPTY SLOT");
		m_font->setJustification(FontBank::JUST_LEFT);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int MAP_INSTRUCTIONS_YSTART=147;
int MAP_INSTRUCTIONS_BUTTON_Y_OFFSET=3;
int MAP_INSTRUCTIONS_TEXT_R=128;
int MAP_INSTRUCTIONS_TEXT_G=64;
int MAP_INSTRUCTIONS_TEXT_B=64;
int MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS=5;
int MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT=10;
int MAP_INSTRUCTIONS_Y_SPACE_BETWEEN_LINES=15;
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

	y=MAP_INSTRUCTIONS_YSTART;
	fh1=sb->getFrameHeader(FRM__BUTL);
	fh2=sb->getFrameHeader(FRM__BUTR);
	width=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS+fh2->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(STR__SLOT_SELECT_SCREEN__LEFT_RIGHT_TO_SELECT_SLOT);
	x=256-(width/2);
	sb->printFT4(fh1,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS;
	sb->printFT4(fh2,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh2->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_font->print(x,y,STR__SLOT_SELECT_SCREEN__LEFT_RIGHT_TO_SELECT_SLOT);

	text=slotInUse?STR__SLOT_SELECT_SCREEN__CROSS_TO_CONFIRM:STR__SLOT_SELECT_SCREEN__CROSS_TO_CREATE;
	y+=MAP_INSTRUCTIONS_Y_SPACE_BETWEEN_LINES;
	fh1=sb->getFrameHeader(FRM__BUTX);
	width=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(text);
	x=256-(width/2);
	sb->printFT4(fh1,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_font->print(x,y,text);

	y+=MAP_INSTRUCTIONS_Y_SPACE_BETWEEN_LINES;
	fh1=sb->getFrameHeader(FRM__BUTC);
	width=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(STR__SLOT_SELECT_SCREEN__CIRCLE_TO_ERASE_SLOT);
	x=256-(width/2);
	if(slotInUse)
	{
		sb->printFT4(fh1,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	}
	x+=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	if(slotInUse)
	{
		m_font->print(x,y,STR__SLOT_SELECT_SCREEN__CIRCLE_TO_ERASE_SLOT);
	}

	y+=MAP_INSTRUCTIONS_Y_SPACE_BETWEEN_LINES;
	fh1=sb->getFrameHeader(FRM__BUTT);
	width=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_font->getStringWidth(STR__SLOT_SELECT_SCREEN__TRIANGLE_TO_EXIT);
	x=256-(width/2);
	sb->printFT4(fh1,x,y+MAP_INSTRUCTIONS_BUTTON_Y_OFFSET,0,0,0);
	x+=fh1->W+MAP_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
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
						m_state=STATE_EXITING_TO_GAME;
						CFader::setFadingOut();
						CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__OK);
					}
					else
					{
						m_state=STATE_SLOT_CREATED;
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
	return m_state==STATE_EXITING_TO_GAME?CFrontEndScene::MODE__EXIT_TO_MAP:CFrontEndScene::MODE__MAIN_TITLES;
}

/*===========================================================================
 end */