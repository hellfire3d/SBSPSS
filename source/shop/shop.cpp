/*=========================================================================

	gameover.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	
				
	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "shop\shop.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef	__GUI_GFACTORY_H__
#include "gui\gfactory.h"
#endif

#ifndef	__GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif

#ifndef	__GUI_POINTER_H__
#include "gui\pointer.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SHOP_H__
#include <shop.h>
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef __STRING_ENUMS__
#include <trans.h>
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

// 125 tokens in total
CShopScene::SHOPITEM	CShopScene::s_shopItems[NUM_SHOP_ITEM_IDS]=
{
	{	 5,	FRM_TEDDY		},			// SHOPITEM_TEDDY
	{	 8,	FRM_SARNIE		},			// SHOPITEM_SARNIE
	{	10,	FRM_CUPCAKE		},			// SHOPITEM_CUPCAKE
	{	12,	FRM_PREZZIE		},			// SHOPITEM_PREZZIE
	{	16,	FRM_JELLY2		},			// SHOPITEM_JELLY2
	{	20,	FRM_CAKE		},			// SHOPITEM_CAKE
	{	24,	FRM_BLOWER		},			// SHOPITEM_BLOWER
	{	30,	FRM_PARTYHAT	},			// SHOPITEM_PARTYHAT
};

xmPlayingId	s_playId;

CShopScene	ShopScene;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int fx=76;
int fy=80;
int fw=200;
int fh=20;
void CShopScene::init()
{
	m_image=LoadPakScreen(BACKDROP_SHOP_GFX);
	ASSERT(m_image);
	SetScreenImage((u8*)m_image);

	m_font=new ("shop font") ScalableFontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(ScalableFontBank::JUST_CENTRE);

	m_spriteBank=new ("shop sprites") SpriteBank();
	m_spriteBank->load(SHOP_SHOP_SPR);


	// GUI Frame
	m_guiFrame=new ("Token count frame") CGUIGroupFrame();
	m_guiFrame->init(0);
	m_guiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);
	m_guiFrame->setObjectXYWH(SHOP_MAIN_UI_X,SHOP_MAIN_UI_Y,SHOP_MAIN_UI_W,SHOP_MAIN_UI_H);

	m_guiConfirmPurchaseFrame=new ("Confirm purchase frame") CGUIControlFrame();
	m_guiConfirmPurchaseFrame->init(0);
	m_guiConfirmPurchaseFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);
	m_guiConfirmPurchaseFrame->setObjectXYWH(SHOP_QUERY_UI_X,SHOP_QUERY_UI_Y,SHOP_QUERY_UI_W,SHOP_QUERY_UI_H);
	CGUIFactory::createValueButtonFrame(m_guiConfirmPurchaseFrame,
										fx,fy+fh,fw,fh,
										STR__NO,
										&m_queryAnswer,
										ANSWER_NO);
	CGUIFactory::createValueButtonFrame(m_guiConfirmPurchaseFrame,
										fx,fy,fw,fh,
										STR__YES,
										&m_queryAnswer,
										ANSWER_YES);

	m_guiCannotAffordFrame=new ("Cannot afford frame") CGUIControlFrame();
	m_guiCannotAffordFrame->init(0);
	m_guiCannotAffordFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);
	m_guiCannotAffordFrame->setObjectXYWH(SHOP_QUERY_UI_X,SHOP_QUERY_UI_Y,SHOP_QUERY_UI_W,SHOP_QUERY_UI_H);
	CGUIFactory::createValueButtonFrame(m_guiCannotAffordFrame,
										fx,fy,fw,fh,
										STR__OK,
										&m_queryAnswer,
										ANSWER_OK);


	m_readyToExit=false;
	CFader::setFadingIn();

	m_currentlySelectedItem=0;
	m_flashSin=0;

	s_playId=NOT_PLAYING;

	m_state=SHOP_STATE__FADING_IN;

	m_mainUiYOffset=SHOP_STATE__SELECT_ITEM;

	m_pointerIcon=new ("MapPointer") CPointerIcon();
	m_pointerIcon->snapToTarget(getPointerTarget());

	CSoundMediator::setSong(CSoundMediator::SONG_SHOPSCREEN);
	CSoundMediator::playSong();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CShopScene::shutdown()
{
	CSoundMediator::dumpSong();

	delete m_pointerIcon;

	m_guiCannotAffordFrame->shutdown();
	m_guiConfirmPurchaseFrame->shutdown();
	m_guiFrame->shutdown();

	m_spriteBank->dump();		delete m_spriteBank;
	m_font->dump();				delete m_font;

	MemFree(m_image);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CShopScene::render()
{
	SHOPITEM	*shopItem;
	int			i,x,y,gap;
	POLY_FT4	*ft4;


	renderUi();

	shopItem=s_shopItems;
	x=SHOP_ICON_XBASE;
	y=SHOP_ICON_YBASE;
	gap=SHOP_ICON_WIDTH/(SHOP_ICON_ITEMS_PER_ROW-1);
	for(i=0;i<NUM_SHOP_ITEM_IDS;i++)
	{
		// Icon
		sFrameHdr	*fh=m_spriteBank->getFrameHeader(shopItem->m_frame);
		ft4=m_spriteBank->printFT4(fh,x-fh->W/2,y,0,0,5);

		// Cursor?
		if(i==m_currentlySelectedItem)
		{
			if(!CFader::isFading())
			{
				if(m_state==SHOP_STATE__SELECT_ITEM)
				{
					m_pointerIcon->render();
				}

				// Flash selected item
				int	rgb=((msin(m_flashSin)*SHOP_ICON_FLASH_COLOUR_RANGE)>>12)+SHOP_ICON_FLASH_COLOUR_BASE;
				setRGB0(ft4,rgb,rgb,rgb);
			}
		}

		// Darken item if already bought
		if(!isItemAvailableToBuy(i))
		{
			setRGB0(ft4,SHOP_ICON_PURCHASED_RGB,SHOP_ICON_PURCHASED_RGB,SHOP_ICON_PURCHASED_RGB);
		}

		shopItem++;
		if(i%SHOP_ICON_ITEMS_PER_ROW==SHOP_ICON_ITEMS_PER_ROW-1)
		{
			x=SHOP_ICON_XBASE;
			y+=SHOP_ICON_Y_GAP;
		}
		else
		{
			x+=gap;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR CShopScene::getPointerTarget()
{
	int			i,x,y,gap;

	x=SHOP_ICON_XBASE;
	y=SHOP_ICON_YBASE;
	gap=SHOP_ICON_WIDTH/(SHOP_ICON_ITEMS_PER_ROW-1);

	for(i=0;i<NUM_SHOP_ITEM_IDS;i++)
	{
		if(i==m_currentlySelectedItem)
		{
			break;
		}

		if(i%SHOP_ICON_ITEMS_PER_ROW==SHOP_ICON_ITEMS_PER_ROW-1)
		{
			x=SHOP_ICON_XBASE;
			y+=SHOP_ICON_Y_GAP;
		}
		else
		{
			x+=gap;
		}
	}

	DVECTOR	ret={x-15,y+35};
	return ret;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CShopScene::think(int _frames)
{
	if(m_state==SHOP_STATE__FADING_IN)
	{
		if(!CFader::isFading())
		{
			m_state=SHOP_STATE__SELECT_ITEM;
		}
	}
	else if(m_state==SHOP_STATE__SELECT_ITEM&&!CFader::isFading())
	{
		int	pad;

		m_pointerIcon->think(_frames);

		pad=PadGetDown(0);
		if(pad&PAD_LEFT)
		{
			m_currentlySelectedItem--;
			if(m_currentlySelectedItem<0)
			{
				m_currentlySelectedItem=NUM_SHOP_ITEM_IDS-1;
			}
			m_flashSin=0;
			playSound(CSoundMediator::SFX_FRONT_END__MOVE_CURSOR);
			m_pointerIcon->setTarget(getPointerTarget());
		}
		else if(pad&PAD_RIGHT)
		{
			m_currentlySelectedItem++;
			if(m_currentlySelectedItem>=NUM_SHOP_ITEM_IDS)
			{
				m_currentlySelectedItem=0;
			}
			m_flashSin=0;
			playSound(CSoundMediator::SFX_FRONT_END__MOVE_CURSOR);
			m_pointerIcon->setTarget(getPointerTarget());
		}
		else if(m_pointerIcon->canPointerSelect()&&pad&PAD_CROSS)
		{
			if(isItemAvailableToBuy(m_currentlySelectedItem))
			{
				int	cost,available;
				cost=s_shopItems[m_currentlySelectedItem].m_cost;
				available=CGameSlotManager::getSlotData()->getNumberOfKelpTokensHeld();
				if(available>=cost)
				{
					playSound(CSoundMediator::SFX_FRONT_END__SELECT);
					m_state=SHOP_STATE__CONFIRM_PURCHASE;
					m_queryAnswer=ANSWER_NONE;
					m_guiConfirmPurchaseFrame->select();
				}
				else
				{
					playSound(CSoundMediator::SFX_FRONT_END__ERROR);
					m_state=SHOP_STATE__CANNOT_AFFORD_ITEM;
					m_queryAnswer=ANSWER_NONE;
					m_guiCannotAffordFrame->select();
				}
			}
			else
			{
				playSound(CSoundMediator::SFX_FRONT_END__ERROR);
			}
		}
		else if(pad&PAD_TRIANGLE)
		{
			m_readyToExit=true;
			CFader::setFadingOut();
			GameState::setNextScene(&MapScene);
		}
	
		m_guiFrame->think(_frames);
	}
	else if(m_state==SHOP_STATE__CONFIRM_PURCHASE)
	{
		m_guiConfirmPurchaseFrame->think(_frames);
		if(m_queryAnswer==ANSWER_YES)
		{
			CGameSlotManager::getSlotData()->buyPartyItem(m_currentlySelectedItem);
			CGameSlotManager::getSlotData()->useKelpTokens(s_shopItems[m_currentlySelectedItem].m_cost);
			m_state=SHOP_STATE__SELECT_ITEM;
			m_guiConfirmPurchaseFrame->unselect();
		}
		else if(m_queryAnswer==ANSWER_NO)
		{
			m_state=SHOP_STATE__SELECT_ITEM;
			m_guiConfirmPurchaseFrame->unselect();
		}
	}
	else if(m_state==SHOP_STATE__CANNOT_AFFORD_ITEM)
	{
		m_guiCannotAffordFrame->think(_frames);
	
		if(m_queryAnswer==ANSWER_OK)
		{
			m_state=SHOP_STATE__SELECT_ITEM;
			m_guiCannotAffordFrame->unselect();
		}
	}


	m_flashSin=(m_flashSin+(_frames*SHOP_ICON_FLASH_SPEED))&4095;


	// Slide the main UI menu about..
	if(m_state!=SHOP_STATE__SELECT_ITEM||m_readyToExit)
	{
		m_mainUiYOffset+=30*_frames;
		if(m_mainUiYOffset>MAIN_UI_MAX_Y_OFFSET)
		{
			m_mainUiYOffset=MAIN_UI_MAX_Y_OFFSET;
		}
	}
	else
	{
		m_mainUiYOffset-=30*_frames;
		if(m_mainUiYOffset<0)
		{
			m_mainUiYOffset=0;
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CShopScene::readyToShutdown()
{
	return m_readyToExit&&!CFader::isFading();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Ugh... I *really* hope this works ok with all languages, otherwise
				it might just be easier to start again :/
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CShopScene::renderUi()
{
	int			xbase,ybase;
	SpriteBank	*sb;
	int			w1,w2,w3,instructionsWidth,instructionsXBase;
	int			maxInstructionsWidth;
	sFrameHdr	*fh1,*fh2;
	int			x,y;
	char		buf[100];


	m_font->setPrintArea(SHOP_QUERY_UI_X+10,SHOP_QUERY_UI_Y+5,SHOP_QUERY_UI_W-20,SHOP_QUERY_UI_H-10);
	m_font->setJustification(FontBank::JUST_CENTRE);
	if(m_state==SHOP_STATE__CONFIRM_PURCHASE)
	{
		int		cost,available;
		char	buf[100];
		cost=s_shopItems[m_currentlySelectedItem].m_cost;
		available=CGameSlotManager::getSlotData()->getNumberOfKelpTokensHeld();
		sprintf(buf,TranslationDatabase::getString(STR__SHOP_SCREEN__CONFIRM_PURCHASE),cost,available);
		m_font->print((SHOP_QUERY_UI_W-20)/2,0,buf);
		m_guiConfirmPurchaseFrame->render();
	}
	else if(m_state==SHOP_STATE__CANNOT_AFFORD_ITEM)
	{
		int		cost,available;
		char	buf[100];
		cost=s_shopItems[m_currentlySelectedItem].m_cost;
		available=CGameSlotManager::getSlotData()->getNumberOfKelpTokensHeld();
		sprintf(buf,TranslationDatabase::getString(available!=1?STR__SHOP_SCREEN__CANNOT_AFFORD_PURCHASE:STR__SHOP_SCREEN__CANNOT_AFFORD_PURCHASE_SINGULAR),cost,available);
		m_font->print((SHOP_QUERY_UI_W-20)/2,0,buf);
		m_guiCannotAffordFrame->render();
	}
	m_font->setPrintArea(0,0,512,256);


	m_guiFrame->setObjectXYWH(SHOP_MAIN_UI_X,SHOP_MAIN_UI_Y+m_mainUiYOffset,SHOP_MAIN_UI_W,SHOP_MAIN_UI_H);
	xbase=SHOP_MAIN_UI_X;
	ybase=SHOP_MAIN_UI_Y+m_mainUiYOffset;
	sb=CGameScene::getSpriteBank();

	// Instructions
	m_font->setJustification(FontBank::JUST_LEFT);

	w1=m_font->getStringWidth(STR__SHOP_SCREEN__LEFT_RIGHT_TO_SELECT_ITEM);
	w2=m_font->getStringWidth(STR__SHOP_SCREEN__CROSS_TO_PURCHASE);
	w3=m_font->getStringWidth(STR__SHOP_SCREEN__TRIANGLE_TO_EXIT);
	instructionsWidth=w1;
	if(w2>instructionsWidth)instructionsWidth=w2;
	if(w3>instructionsWidth)instructionsWidth=w3;
	instructionsXBase=SHOP_MAIN_UI_W-instructionsWidth-SHOP_XBORDER;

	x=xbase+instructionsXBase;
	y=ybase+SHOP_INSTRUCTIONS_Y_BASE;
	m_font->print(x,y,STR__SHOP_SCREEN__LEFT_RIGHT_TO_SELECT_ITEM);
	fh1=sb->getFrameHeader(FRM__BUTL);
	fh2=sb->getFrameHeader(FRM__BUTR);
	x-=SHOP_SPACE_BETWEEN_INSTRUCTIONS_AND_BUTTONS+fh2->W;
	y+=SHOP_Y_BUTTON_OFFSET_FORM_TOP_OF_TEXT;
	sb->printFT4(fh2,x,y,0,0,0);
	x-=SHOP_GAP_BETWEEN_INSTRUCTION_BUTTONS+fh1->W;
	sb->printFT4(fh1,x,y,0,0,0);
	maxInstructionsWidth=xbase+SHOP_MAIN_UI_Y-x;

	x=xbase+instructionsXBase;
	y=ybase+SHOP_INSTRUCTIONS_Y_BASE+SHOP_Y_GAP_BETWEEN_INSTRUCTION_LINES;
	m_font->print(x,y,STR__SHOP_SCREEN__CROSS_TO_PURCHASE);
	fh1=sb->getFrameHeader(FRM__BUTX);
	x-=SHOP_SPACE_BETWEEN_INSTRUCTIONS_AND_BUTTONS+fh2->W;
	y+=SHOP_Y_BUTTON_OFFSET_FORM_TOP_OF_TEXT;
	sb->printFT4(fh1,x-1,y,0,0,0);

	x=xbase+instructionsXBase;
	y=ybase+SHOP_INSTRUCTIONS_Y_BASE+(SHOP_Y_GAP_BETWEEN_INSTRUCTION_LINES*2);
	m_font->print(x,y,STR__SHOP_SCREEN__TRIANGLE_TO_EXIT);
	fh1=sb->getFrameHeader(FRM__BUTT);
	x-=SHOP_SPACE_BETWEEN_INSTRUCTIONS_AND_BUTTONS+fh2->W;
	y+=SHOP_Y_BUTTON_OFFSET_FORM_TOP_OF_TEXT;
	sb->printFT4(fh1,x-1,y,0,0,0);

	// Item price
	int		x1,x2;
	x1=SHOP_XBORDER;
	x2=SHOP_MAIN_UI_Y-maxInstructionsWidth-(SHOP_XBORDER*1);
	m_font->setJustification(FontBank::JUST_CENTRE);
	if(isItemAvailableToBuy(m_currentlySelectedItem))
	{
		SHOPITEM	*shopItem;
		shopItem=&s_shopItems[m_currentlySelectedItem];
		fh1=m_spriteBank->getFrameHeader(shopItem->m_frame);
		x=xbase+SHOP_ITEM_X_POS;
		y=ybase+SHOP_ITEM_Y_POS;
		sb->printFT4(fh1,x,y,0,0,0);

		x1+=32;

		m_font->setPrintArea(xbase+x1,ybase,x2-x1,100);
		m_font->print((x2-x1)/2,SHOP_PRICE_Y_GAP_FROM_TOP,STR__SHOP_SCREEN__PRICE);

		sprintf(buf,"%d",shopItem->m_cost);
		m_font->print((x2-x1)/2,SHOP_PRICE_Y_GAP_FROM_TOP+m_font->getStringHeight(STR__SHOP_SCREEN__PRICE),buf);
	}
	else
	{
		m_font->setPrintArea(xbase+x1,ybase,x2-x1,100);
		m_font->print((x2-x1)/2,SHOP_PRICE_Y_GAP_FROM_TOP,STR__SHOP_SCREEN__ITEM_ALREADY_PURCHASED);
	}
	m_font->setPrintArea(0,0,512,256);

	// Available token count
	x=xbase+SHOP_TOKEN_COUNT_X_POS;
	y=ybase+SHOP_TOKEN_COUNT_Y_POS;
	fh1=m_spriteBank->getFrameHeader(FRM_SMALLTOKEN);
	m_spriteBank->printFT4(fh1,x,y,0,0,0);
	x+=fh1->W;
	sprintf(buf,"x%d",CGameSlotManager::getSlotData()->getNumberOfKelpTokensHeld());
	m_font->setJustification(FontBank::JUST_LEFT);
	m_font->print(x,y,buf);

	m_guiFrame->render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CShopScene::isItemAvailableToBuy(int _itemNumber)
{
	return !CGameSlotManager::getSlotData()->isPartyItemHeld(_itemNumber);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CShopScene::playSound(int _sfxId)
{
	if(s_playId!=NOT_PLAYING)
	{
		CSoundMediator::stopAndUnlockSfx(s_playId);
	}
	s_playId=CSoundMediator::playSfx((CSoundMediator::SFXID)_sfxId,true);
/*
	if(s_playId)
	{
		PAUL_DBGMSG("Didn't play..!?");
	}
*/
}


/*===========================================================================
 end */