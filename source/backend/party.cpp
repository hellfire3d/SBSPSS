/*=========================================================================

	party.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	
				
	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "backend\party.h"

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
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

#ifndef	__SHOP_SHOP_H__
#include "shop\shop.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __ACTOR_HEADER__
#include "gfx\actor.h"
#endif

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __STRING_ENUMS__
#include <trans.h>
#endif

#ifndef __SPR_PARTY_H__
#include <party.h>
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

CPartyScene	PartyScene;

CActorGfx	*m_actorSpongebob;
CActorGfx	*m_actorPatrick;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPartyScene::init()
{
	m_image=CFileIO::loadFile(BACKDROP_PARTYBACKDROP_GFX);
	ASSERT(m_image);
	SetScreenImage((u8*)m_image);

//	m_font=new ("game over font") ScalableFontBank();
//	m_font->initialise(&standardFont);
//	m_font->setJustification(ScalableFontBank::JUST_CENTRE);
//	m_font->setOt(5);

	m_sprites=new ("Party sprites") SpriteBank();
	m_sprites->load(PARTY_PARTY_SPR);

	m_readyToExit=false;
	CFader::setFadingIn(CFader::BLACK_FADE);

	CActorPool::Reset();
	m_actorSpongebob=CActorPool::GetActor(ACTORS_SPONGEBOB_SBK);
	m_actorSpongebob->SetOtPos(5);
	m_actorPatrick=CActorPool::GetActor(ACTORS_PATRICK_SBK);
	m_actorPatrick->SetOtPos(5);
	CActorPool::SetUpCache();
	CActorPool::CleanUpCache();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPartyScene::shutdown()
{
	delete m_actorPatrick;
	delete m_actorSpongebob;
	CActorPool::Reset();

	m_sprites->dump();			delete m_sprites;
//	m_font->dump();				delete m_font;

	MemFree(m_image);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
typedef struct
{
	CShopScene::SHOPITEM_ID	m_itemId;
	int						m_fh;
	u8						m_xOffsetBroken,m_yOffsetBroken;
	u8						m_ot;
}PARTY_IMAGE;
static PARTY_IMAGE images[]=
{
	{	CShopScene::SHOPITEM_BLOWER,	FRM_BALLOON1,		0,0,	2	},
	{	CShopScene::SHOPITEM_BLOWER,	FRM_BALLOON2,		1,0,	2	},

	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER1,		0,0,	1	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER2,		0,0,	1	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER3,		1,0,	1	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER4,		1,0,	1	},
	
	{	CShopScene::SHOPITEM_CAKE,		FRM_BIGCAKE,		1,1,	4	},
	{	CShopScene::SHOPITEM_CAKE,		FRM_TABLE2,			1,1,	5	},
	
	{	CShopScene::SHOPITEM_JELLY2,	FRM_JELLY,			1,1,	3	},
	{	CShopScene::SHOPITEM_JELLY2,	FRM_TABLE2,			1,1,	5	},
	
	{	CShopScene::SHOPITEM_PREZZIE,	FRM_PRESENTS1,		0,1,	4	},
	{	CShopScene::SHOPITEM_PREZZIE,	FRM_PRESENTS2,		2,1,	5	},
	
	{	CShopScene::SHOPITEM_SARNIE,	FRM_SANDWICHES,		0,1,	5	},
	{	CShopScene::SHOPITEM_SARNIE,	FRM_LEFTTABLE,		0,1,	5	},
	
	{	CShopScene::SHOPITEM_CUPCAKE,	FRM_SMALLCAKES,		1,1,	4	},
	{	CShopScene::SHOPITEM_CUPCAKE,	FRM_TABLE2,			1,1,	5	},

	{	CShopScene::SHOPITEM_TEDDY,		FRM_TOYCHEST,		1,0,	6	},
};
static const int numimages=sizeof(images)/sizeof(PARTY_IMAGE);

DVECTOR	sbpos={220,195};
int sbanim=5,sbfrm=1;
DVECTOR	patpos={300,200};
int patanim=0,patfrm=0;
void CPartyScene::render()
{
	int							i;
	int							drawn[FRM_TOYCHEST+1];
	PARTY_IMAGE					*pimage;
	CGameSlotManager::GameSlot	*gameSlot;
	
	for(i=0;i<FRM_TOYCHEST+1;i++)
	{
		drawn[i]=false;
	}

	gameSlot=CGameSlotManager::getSlotData();
	pimage=images;
	for(i=0;i<numimages;i++)
	{
		ASSERT(pimage->m_fh<=FRM_TOYCHEST);
		if(gameSlot->isPartyItemHeld(pimage->m_itemId)&&!drawn[pimage->m_fh])
		{
			m_sprites->printFT4(pimage->m_fh,pimage->m_xOffsetBroken*256,pimage->m_yOffsetBroken*256,0,0,pimage->m_ot);
			drawn[pimage->m_fh]=true;
		}
		pimage++;
	}

	// Actors
	m_actorSpongebob->Render(sbpos,sbanim,sbfrm,0,0);
	sbfrm=0;
	m_actorPatrick->Render(patpos,patanim,patfrm,0,0);


	CActorPool::CleanUpCache();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPartyScene::think(int _frames)
{
	if(!CFader::isFading())
	{
		if(PadGetDown(0)&(PAD_CROSS|PAD_START))
		{
			m_readyToExit=true;
			CFader::setFadingOut();
			GameState::setNextScene(&FrontEndScene);
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPartyScene::readyToShutdown()
{
	return m_readyToExit&&!CFader::isFading();
}


/*===========================================================================
 end */