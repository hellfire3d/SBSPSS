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
int shopygap=50;
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
			sb->printFT4(fh,x-fh->W,y+25,0,0,0);

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
int CShopScene::isItemAvailableToBut(int _itemNumber)
{
	return _itemNumber!=3;
}


/*===========================================================================
 end */