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

CShopScene::SHOPITEM	CShopScene::s_shopItems[NUM_SHOP_ITEM_IDS]=
{
	{	1,	FRM_BLOWER		},
	{	2,	FRM_CAKE		},
	{	3,	FRM_CUPCAKE		},
	{	4,	FRM_JELLY2		},
	{	5,	FRM_PARTYHAT	},
	{	6,	FRM_PREZZIE		},
	{	7,	FRM_SARNIE		},
	{	8,	FRM_TEDDY		},
};





CShopScene	ShopScene;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int fx=48;
int fy=155;
int fw=416;
int fh=64;
void CShopScene::init()
{
	m_image=CFileIO::loadFile(BACKDROP_SHOP_GFX);
	ASSERT(m_image);
	SetScreenImage((u8*)m_image);

	m_font=new ("shop font") ScalableFontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(ScalableFontBank::JUST_CENTRE);
	m_font->setOt(5);

	m_spriteBank=new ("shop sprites") SpriteBank();
	m_spriteBank->load(SHOP_SHOP_SPR);

	// GUI Frame
	CGUITextBox			*tb;
	m_guiFrame=new ("Token Count Frame") CGUIGroupFrame();
	m_guiFrame->init(0);
	m_guiFrame->setOt(0);
	m_guiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);
	m_guiFrame->setObjectXYWH(fx,fy,fw,fh);

	m_readyToExit=false;
	CFader::setFadingIn(CFader::BLACK_FADE);

	m_currentlySelectedItem=0;
	m_flashSin=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CShopScene::shutdown()
{
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
int shopx=180;
int shopy=38;
int shopw=512-(180*2);
int shopygap=51;
int shopitemsperrow=4;
int shopflashspeed=200;
int shopflashbase=128;
int shopflashrange=50;
int shopboughtrgb=45;
void CShopScene::render()
{
	SHOPITEM	*shopItem;
	int			i,x,y,gap;
	POLY_FT4	*ft4;

	shopItem=s_shopItems;
	x=shopx;
	y=shopy;
	gap=shopw/(shopitemsperrow-1);
	for(i=0;i<NUM_SHOP_ITEM_IDS;i++)
	{
		// Icon
		sFrameHdr	*fh=m_spriteBank->getFrameHeader(shopItem->m_frame);
		ft4=m_spriteBank->printFT4(fh,x-fh->W/2,y,0,0,0);

		// Cursor?
		if(i==m_currentlySelectedItem)
		{
			SpriteBank	*sb=CGameScene::getSpriteBank();
			sFrameHdr	*fh=sb->getFrameHeader(FRM__MAPPOINTER);
			sb->printFT4(fh,x-fh->W,y+30,0,0,0);

			// Flash selected item
			int	rgb=((msin(m_flashSin)*shopflashrange)>>12)+shopflashbase;
			setRGB0(ft4,rgb,rgb,rgb);
		}

		// Darken item if already bought
		if(!isItemAvailableToBut(i))
		{
			setRGB0(ft4,shopboughtrgb,shopboughtrgb,shopboughtrgb);
		}

		shopItem++;
		if(i%shopitemsperrow==shopitemsperrow-1)
		{
			x=shopx;
			y+=shopygap;
		}
		else
		{
			x+=gap;
		}
	}

	renderUi();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CShopScene::think(int _frames)
{
	int	pad;

	pad=PadGetDown(0);
	if(pad&PAD_LEFT)
	{
		m_currentlySelectedItem--;
		if(m_currentlySelectedItem<0)
		{
			m_currentlySelectedItem=NUM_SHOP_ITEM_IDS-1;
		}
		m_flashSin=0;
	}
	else if(pad&PAD_RIGHT)
	{
		m_currentlySelectedItem++;
		if(m_currentlySelectedItem>=NUM_SHOP_ITEM_IDS)
		{
			m_currentlySelectedItem=0;
		}
		m_flashSin=0;
	}

	m_flashSin=(m_flashSin+(_frames*shopflashspeed))&4095;

	m_guiFrame->think(_frames);
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
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
typedef struct
{
	int
	tokenx,tokeny,
	pricex,pricey,
	gaptoreadout,
	instructionsx,instructionsy,
	instructionsygap,
	instructionsbuttonyoffset,
	instructionsbuttonspace,
	instructionsbuttongap,

	
	end;
} shopdata;

shopdata shop=
{
	10,10,	//	tokenx,tokeny,
	40,10,	//	pricex,pricey,
	5,		//	gaptoreadout
	250,7,	//	instructionsx,instructionsy,
	16,		//	instructionsygap,
	3,		//	instructionsbuttonyoffset,
	10,		//	instructionsbuttonspace,
	5,		//	instructionsbuttongap,
};

void CShopScene::renderUi()
{
	int			xbase,ybase;
	SpriteBank	*sb;
	sFrameHdr	*fh1,*fh2;
	int			x,y;
	char		buf[10];

	xbase=fx;
	ybase=fy;
	sb=CGameScene::getSpriteBank();

	m_font->setJustification(FontBank::JUST_LEFT);

	// Token count
	fh1=sb->getFrameHeader(FRM__TOKEN);
	x=xbase+shop.tokenx;
	y=ybase+shop.tokeny;
	sb->printFT4(fh1,x,y,0,0,0);
	sprintf(buf,"x%d",99);
//	x+=fh1->W+shop.gaptoreadout;
//	y+=fh1->H+m_font->getStringHeight(buf);
PAUL_DBGMSG("%d,%d",fh1->H,m_font->getStringHeight(buf));
	m_font->print(x,y,buf);


	// Item price
	m_font->print(xbase+shop.pricex,ybase+shop.pricey,STR__SHOP_SCREEN__PRICE);


	// Instructions
	x=xbase+shop.instructionsx;
	y=ybase+shop.instructionsy;
	m_font->print(x,y,STR__SHOP_SCREEN__LEFT_RIGHT_TO_SELECT_ITEM);
	fh1=sb->getFrameHeader(FRM__BUTL);
	fh2=sb->getFrameHeader(FRM__BUTR);
	x-=shop.instructionsbuttonspace+fh2->W;
	y+=shop.instructionsbuttonyoffset;
	sb->printFT4(fh2,x,y,0,0,0);
	x-=shop.instructionsbuttongap+fh1->W;
	sb->printFT4(fh1,x,y,0,0,0);

	x=xbase+shop.instructionsx;
	y=ybase+shop.instructionsy+shop.instructionsygap;
	m_font->print(x,y,STR__SHOP_SCREEN__CROSS_TO_PURCHASE);
	fh1=sb->getFrameHeader(FRM__BUTX);
	x-=shop.instructionsbuttonspace+fh2->W;
	y+=shop.instructionsbuttonyoffset;
	sb->printFT4(fh1,x,y,0,0,0);

	x=xbase+shop.instructionsx;
	y=ybase+shop.instructionsy+(shop.instructionsygap*2);
	m_font->print(x,y,STR__SHOP_SCREEN__TRIANGLE_TO_EXIT);
	fh1=sb->getFrameHeader(FRM__BUTT);
	x-=shop.instructionsbuttonspace+fh2->W;
	y+=shop.instructionsbuttonyoffset;
	sb->printFT4(fh1,x,y,0,0,0);

	m_guiFrame->render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CShopScene::isItemAvailableToBut(int _itemNumber)
{
	return _itemNumber!=3;
}


/*===========================================================================
 end */