/*=========================================================================

	frontend.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\frontend.h"

#ifndef	__FRONTEND_SCROLLBG_H__
#include "frontend\scrollbg.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_FRONTEND_H__
#include <frontend.h>
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



static CScrollyBackground		*s_bg1;


CFrontEndScene	FrontEndScene;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::init()
{
	s_bg1=new ("scrolly background") CScrollyBackground();
	s_bg1->init();
	s_bg1->setSpeed(+2,+2);


	m_sprites=new ("FE Sprites") SpriteBank();
	m_sprites->load(FRONTEND_FRONTEND_SPR);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::shutdown()
{
	m_sprites->dump();		delete m_sprites;
	s_bg1->shutdown();	delete s_bg1;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::render()
{
	sFrameHdr	*fh;

	fh=m_sprites->getFrameHeader(FRM__SBLOGO);
	m_sprites->printFT4(fh,(512-fh->W)/2,(256-fh->H)/2,0,0,10);

	s_bg1->render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndScene::think(int _frames)
{
	s_bg1->think(_frames);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndScene::readyToShutdown()
{
	return false;
}


/*===========================================================================
 end */