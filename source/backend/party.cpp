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

#ifndef	__BACKEND_CREDITS_H__
#include "backend\credits.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
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

static int	s_leftBobSin,s_leftBob,s_rightBobSin,s_rightBob;

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
	CFader::setFadingIn();

	CActorPool::Reset();
	m_actorSpongebob=CActorPool::GetActor(ACTORS_SPONGEBOB_SBK);
	m_actorSpongebob->SetOtPos(5);
	m_actorPatrick=CActorPool::GetActor(ACTORS_PATRICK_SBK);
	m_actorPatrick->SetOtPos(5);
	CActorPool::SetUpCache();
	CActorPool::CleanUpCache();

	s_leftBobSin=s_leftBob=s_rightBobSin=s_rightBob=0;

	CSoundMediator::setSong(CSoundMediator::SONG_PARTY);
	CSoundMediator::playSong();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPartyScene::shutdown()
{
	CSoundMediator::dumpSong();

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
	int						*m_extraYOffset;
	u8						m_flipX;
	u8						m_ot;
	u8						m_elementId;
}PARTY_IMAGE;
static PARTY_IMAGE images[]=
{
	{	CShopScene::SHOPITEM_BLOWER,	FRM_BALLOONS1,			0,0,	&s_leftBob,		false,	2,	0	},
	{	CShopScene::SHOPITEM_BLOWER,	FRM_BALLOONS2,			2,0,	&s_rightBob,	false,	2,	0	},
	{	CShopScene::SHOPITEM_BLOWER,	FRM_BALLOONS3,			0,0,	&s_leftBob,		false,	2,	0	},
	{	CShopScene::SHOPITEM_BLOWER,	FRM_BALLOONS4,			0,0,	&s_leftBob,		false,	2,	0	},
	{	CShopScene::SHOPITEM_BLOWER,	FRM_BALLOONS5,			0,0,	&s_leftBob,		false,	2,	0	},
	{	CShopScene::SHOPITEM_BLOWER,	FRM_BALLOONS6,			1,0,	&s_rightBob,	false,	2,	0	},
	{	CShopScene::SHOPITEM_BLOWER,	FRM_BALLOONS7,			1,0,	&s_rightBob,	false,	2,	0	},
	{	CShopScene::SHOPITEM_BLOWER,	FRM_BALLOONS8,			2,0,	&s_rightBob,	false,	2,	0	},
																
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER1,			0,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER2,			0,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER3,			1,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER4,			1,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER5,			1,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER6,			2,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_BANNER7,			2,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS1,			1,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS2,			1,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS3,			0,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS4,			0,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS5,			0,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS6,			0,0,	NULL,			false,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS1,			1,0,	NULL,			true,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS2,			1,0,	NULL,			true,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS3,			2,0,	NULL,			true,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS4,			2,0,	NULL,			true,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS5,			2,0,	NULL,			true,	1,	0	},
	{	CShopScene::SHOPITEM_PARTYHAT,	FRM_STREAMERS6,			2,0,	NULL,			true,	1,	0	},
																
	{	CShopScene::SHOPITEM_CAKE,		FRM_BIGCAKE,			1,1,	NULL,			false,	4,	0	},
	{	CShopScene::SHOPITEM_CAKE,		FRM_SLICEOFCAKE,		1,1,	NULL,			false,	4,	0	},
	{	CShopScene::SHOPITEM_CAKE,		FRM_TABLEFRONT1,		1,1,	NULL,			false,	5,	1	},
	{	CShopScene::SHOPITEM_CAKE,		FRM_TABLEFRONT2,		1,1,	NULL,			false,	5,	2	},
	{	CShopScene::SHOPITEM_CAKE,		FRM_TABLEFRONT3,		1,1,	NULL,			false,	5,	3	},
	{	CShopScene::SHOPITEM_CAKE,		FRM_TABLEFRONT4,		1,1,	NULL,			false,	5,	4	},
	{	CShopScene::SHOPITEM_CAKE,		FRM_TABLEFRONT5,		1,1,	NULL,			false,	5,	5	},
	{	CShopScene::SHOPITEM_CAKE,		FRM_TABLEFRONT6,		1,1,	NULL,			false,	5,	6	},
	{	CShopScene::SHOPITEM_CAKE,		FRM_TABLEFRONT7,		1,1,	NULL,			false,	5,	7	},
																
	{	CShopScene::SHOPITEM_JELLY2,	FRM_JELLY,				1,1,	NULL,			false,	3,	0	},
	{	CShopScene::SHOPITEM_JELLY2,	FRM_TABLEFRONT1,		1,1,	NULL,			false,	5,	1	},
	{	CShopScene::SHOPITEM_JELLY2,	FRM_TABLEFRONT2,		1,1,	NULL,			false,	5,	2	},
	{	CShopScene::SHOPITEM_JELLY2,	FRM_TABLEFRONT3,		1,1,	NULL,			false,	5,	3	},
	{	CShopScene::SHOPITEM_JELLY2,	FRM_TABLEFRONT4,		1,1,	NULL,			false,	5,	4	},
	{	CShopScene::SHOPITEM_JELLY2,	FRM_TABLEFRONT5,		1,1,	NULL,			false,	5,	5	},
	{	CShopScene::SHOPITEM_JELLY2,	FRM_TABLEFRONT6,		1,1,	NULL,			false,	5,	6	},
	{	CShopScene::SHOPITEM_JELLY2,	FRM_TABLEFRONT7,		1,1,	NULL,			false,	5,	7	},
																
	{	CShopScene::SHOPITEM_PREZZIE,	FRM_PRESSIES1,			0,1,	NULL,			false,	4,	0	},
	{	CShopScene::SHOPITEM_PREZZIE,	FRM_PRESSIES2,			0,1,	NULL,			false,	4,	0	},
	{	CShopScene::SHOPITEM_PREZZIE,	FRM_PRESSIES3,			2,1,	NULL,			false,	4,	0	},
	{	CShopScene::SHOPITEM_PREZZIE,	FRM_PRESSIES4,			2,1,	NULL,			false,	4,	0	},
	
	{	CShopScene::SHOPITEM_SARNIE,	FRM_CUCUMBERSARNIES,	0,1,	NULL,			false,	5,	0	},
	{	CShopScene::SHOPITEM_SARNIE,	FRM_TABLESIDE1,			0,1,	NULL,			false,	5,	0	},
	{	CShopScene::SHOPITEM_SARNIE,	FRM_TABLESIDE2,			0,1,	NULL,			false,	5,	0	},
	{	CShopScene::SHOPITEM_SARNIE,	FRM_TABLESIDE3,			0,1,	NULL,			false,	5,	0	},
																
	{	CShopScene::SHOPITEM_CUPCAKE,	FRM_CHERRYCAKES,		1,1,	NULL,			false,	4,	0	},
	{	CShopScene::SHOPITEM_CUPCAKE,	FRM_TABLEFRONT1,		1,1,	NULL,			false,	5,	1	},
	{	CShopScene::SHOPITEM_CUPCAKE,	FRM_TABLEFRONT2,		1,1,	NULL,			false,	5,	2	},
	{	CShopScene::SHOPITEM_CUPCAKE,	FRM_TABLEFRONT3,		1,1,	NULL,			false,	5,	3	},
	{	CShopScene::SHOPITEM_CUPCAKE,	FRM_TABLEFRONT4,		1,1,	NULL,			false,	5,	4	},
	{	CShopScene::SHOPITEM_CUPCAKE,	FRM_TABLEFRONT5,		1,1,	NULL,			false,	5,	5	},
	{	CShopScene::SHOPITEM_CUPCAKE,	FRM_TABLEFRONT6,		1,1,	NULL,			false,	5,	6	},
	{	CShopScene::SHOPITEM_CUPCAKE,	FRM_TABLEFRONT7,		1,1,	NULL,			false,	5,	7	},
																
	{	CShopScene::SHOPITEM_TEDDY,		FRM_SOFTTOYS1,			1,1,	NULL,			false,	6,	0	},
	{	CShopScene::SHOPITEM_TEDDY,		FRM_SOFTTOYS2,			1,0,	NULL,			false,	6,	0	},
	{	CShopScene::SHOPITEM_TEDDY,		FRM_SOFTTOYS3,			2,1,	NULL,			false,	6,	0	},
};																
static const int numimages=sizeof(images)/sizeof(PARTY_IMAGE);

DVECTOR	sbpos={220,195};
int sbanim=5,sbfrm=1;
DVECTOR	patpos={300,200};
int patanim=0,patfrm=0;
void CPartyScene::render()
{
	int							i;
	int							drawn[8];
	PARTY_IMAGE					*pimage;
	CGameSlotManager::GameSlot	*gameSlot;
	
	for(i=0;i<8;i++)
	{
		drawn[i]=false;
	}

	gameSlot=CGameSlotManager::getSlotData();
	pimage=images;
	for(i=0;i<numimages;i++)
	{
		ASSERT(pimage->m_fh<=FRM_TABLESIDE3);
		if(gameSlot->isPartyItemHeld(pimage->m_itemId)&&!drawn[pimage->m_elementId])
		{
			int		x,y;
			x=pimage->m_xOffsetBroken*256;
			y=pimage->m_yOffsetBroken*256;
			if(pimage->m_extraYOffset)
			{
				y+=*pimage->m_extraYOffset;
			}
			m_sprites->printFT4(pimage->m_fh,x,y,pimage->m_flipX,0,pimage->m_ot);
			if(pimage->m_elementId)
			{
				drawn[pimage->m_elementId]=true;
			}
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
			GameState::setNextScene(&CreditsScene);
		}
	}

	s_leftBobSin=(s_leftBobSin+(_frames*20))&4095;
	s_rightBobSin=(s_rightBobSin+(_frames*16))&4095;
	s_leftBob=(msin(s_leftBobSin)*2)>>12;
	s_rightBob=(msin(s_rightBobSin)*2)>>12;
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