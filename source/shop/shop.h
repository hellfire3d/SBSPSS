/*=========================================================================

	shop.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__SHOP_SHOP_H__
#define __SHOP_SHOP_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CShopScene : public CScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"Shop";}


private:
	int		isItemAvailableToBut(int _itemNumber);


	typedef enum
	{
		SHOPITEM_BLOWER,
		SHOPITEM_CAKE,
		SHOPITEM_CUPCAKE,
		SHOPITEM_JELLY2,
		SHOPITEM_PARTYHAT,
		SHOPITEM_PREZZIE,
		SHOPITEM_SARNIE,
		SHOPITEM_TEDDY,
		NUM_SHOP_ITEM_IDS
	} SHOPITEM_ID;
	typedef struct
	{
		int		m_cost;
		int		m_frame;
	} SHOPITEM;
	static SHOPITEM			s_shopItems[NUM_SHOP_ITEM_IDS];


	class ScalableFontBank	*m_font;
	class SpriteBank		*m_spriteBank;
	int						m_readyToExit;
	unsigned char			*m_image;

	int						m_currentlySelectedItem;
	int						m_flashSin;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CShopScene	ShopScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __SHOP_SHOP_H__ */

/*===========================================================================
 end */
