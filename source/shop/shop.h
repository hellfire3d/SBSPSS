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

#ifndef _GLOBAL_HEADER_
#include "system\global.h"		// Doh.. just for DVECTOR :/
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

	typedef enum
	{
		SHOPITEM_TEDDY,
		SHOPITEM_SARNIE,
		SHOPITEM_CUPCAKE,
		SHOPITEM_PREZZIE,
		SHOPITEM_JELLY2,
		SHOPITEM_CAKE,
		SHOPITEM_BLOWER,
		SHOPITEM_PARTYHAT,

		NUM_SHOP_ITEM_IDS
	} SHOPITEM_ID;


private:
	void	renderUi();
	DVECTOR	getPointerTarget();

	int		isItemAvailableToBuy(int _itemNumber);
	void	playSound(int _sfxId);


	enum
	{
		// Icon stuff
		SHOP_ICON_XBASE=180,
		SHOP_ICON_YBASE=38,
		SHOP_ICON_WIDTH=512-(SHOP_ICON_XBASE*2),
		SHOP_ICON_Y_GAP=51,
		SHOP_ICON_ITEMS_PER_ROW=4,

		SHOP_ICON_FLASH_SPEED=150,
		SHOP_ICON_FLASH_COLOUR_BASE=128,
		SHOP_ICON_FLASH_COLOUR_RANGE=50,

		SHOP_ICON_PURCHASED_RGB=40,


		// UI stuff
		SHOP_MAIN_UI_X=48,
		SHOP_MAIN_UI_Y=155,
		SHOP_MAIN_UI_W=416,
		SHOP_MAIN_UI_H=64,

		SHOP_XBORDER=10,

		SHOP_GAP_BETWEEN_INSTRUCTION_BUTTONS=5,
		SHOP_SPACE_BETWEEN_INSTRUCTIONS_AND_BUTTONS=10,
		SHOP_Y_BUTTON_OFFSET_FORM_TOP_OF_TEXT=3,
		SHOP_INSTRUCTIONS_Y_BASE=7,
		SHOP_Y_GAP_BETWEEN_INSTRUCTION_LINES=16,

		SHOP_PRICE_Y_GAP_FROM_TOP=5,

		SHOP_ITEM_X_POS=10,
		SHOP_ITEM_Y_POS=6,

		SHOP_TOKEN_COUNT_X_POS=10,
		SHOP_TOKEN_COUNT_Y_POS=42,


		// Overlay UI for confirmation
		SHOP_QUERY_UI_X=80,
		SHOP_QUERY_UI_Y=50,
		SHOP_QUERY_UI_W=352,
		SHOP_QUERY_UI_H=128,
	};


	typedef struct
	{
		int		m_cost;
		int		m_frame;
	} SHOPITEM;
	static SHOPITEM			s_shopItems[NUM_SHOP_ITEM_IDS];


	typedef enum
	{
		SHOP_STATE__FADING_IN,
		SHOP_STATE__SELECT_ITEM,
		SHOP_STATE__CONFIRM_PURCHASE,
		SHOP_STATE__CANNOT_AFFORD_ITEM,
	} SHOP_STATE;
	SHOP_STATE				m_state;

	class ScalableFontBank	*m_font;
	class SpriteBank		*m_spriteBank;
	int						m_readyToExit;
	unsigned char			*m_image;

	class CGUIGroupFrame	*m_guiFrame;
	class CGUIControlFrame	*m_guiConfirmPurchaseFrame;
	class CGUIControlFrame	*m_guiCannotAffordFrame;

	enum
	{
		ANSWER_NONE=0,
		ANSWER_OK=1,
		ANSWER_YES=1,
		ANSWER_NO,
	};
	int						m_queryAnswer;

	int						m_currentlySelectedItem;
	int						m_flashSin;

	enum
	{
		MAIN_UI_MAX_Y_OFFSET=120,
	};
	int						m_mainUiYOffset;


	class CPointerIcon		*m_pointerIcon;

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
