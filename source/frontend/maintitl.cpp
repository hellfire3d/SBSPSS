/*=========================================================================

	maintitl.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\maintitl.h"

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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::init()
{
	m_sprites=new ("FE Sprites") SpriteBank();
	m_sprites->load(FRONTEND_FRONTEND_SPR);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::shutdown()
{
	m_sprites->dump();		delete m_sprites;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::render()
{
	sFrameHdr	*fh;

	fh=m_sprites->getFrameHeader(FRM__SBLOGO);
	m_sprites->printFT4(fh,(512-fh->W)/2,(256-fh->H)/2,0,0,10);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndMainTitles::think(int _frames)
{
}


/*===========================================================================
 end */