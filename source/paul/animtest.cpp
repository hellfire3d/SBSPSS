/*=========================================================================

	animtest.cpp

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

CAnimTestScene	AnimTestScene;

CActorGfx	*m_actorSpongebob;
CActorGfx	*m_actorPatrick;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CAnimTestScene::init()
{
	m_font=new ("game over font") ScalableFontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(ScalableFontBank::JUST_CENTRE);
	m_font->setOt(5);

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
void CAnimTestScene::render()
{
	DVECTOR	sbpos={100,100};

	// Actors
	m_actorSpongebob->Render(sbpos,0,0,0,0);


	CActorPool::CleanUpCache();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CAnimTestScene::think(int _frames)
{
}


/*===========================================================================
 end */