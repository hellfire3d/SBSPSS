/*=========================================================================

	pointer.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gui\pointer.h"

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
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
void	CPointerIcon::think(int _frames)
{
	int	i;

	for(i=0;i<_frames;i++)
	{
		int delta;

		// X
		delta=m_pointerTarget.vx-m_pointerPos.vx;
		if(delta<0)
		{
			delta/=3;
			if(!delta)delta=-1;
		}
		else if(delta>0)
		{
			delta/=3;
			if(!delta)delta=+1;
		}
		m_pointerPos.vx+=delta;

		// Y
		delta=m_pointerTarget.vy+((msin(m_pointerSin)*4)>>12)-m_pointerPos.vy;
		if(delta<0)
		{
			delta/=3;
			if(!delta)delta=-1;
		}
		else if(delta>0)
		{
			delta/=3;
			if(!delta)delta=+1;
		}
		m_pointerPos.vy+=delta;

		// Reached target?
		if(!m_pointerArrivedAtTarget&&
		   m_pointerPos.vx==m_pointerTarget.vx&&m_pointerPos.vy==m_pointerTarget.vy)
		{
			m_pointerArrivedAtTarget=true;
		}

		// Make it bob a bit..
		if(m_pointerArrivedAtTarget)
		{
			m_pointerSin=(m_pointerSin+70)&4095;
		}
		else
		{
			m_pointerSin=0;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPointerIcon::render()
{
	SpriteBank		*sb;
	sFrameHdr		*fh;
	POLY_FT4		*ft4;
	int				x,y;

	sb=CGameScene::getSpriteBank();
	fh=sb->getFrameHeader(FRM__MAPPOINTER);
	x=m_pointerPos.vx-(fh->W/2);
	y=m_pointerPos.vy-(fh->H/2);
	ft4=sb->printFT4(fh,x,y,0,0,0);
	if(!m_pointerArrivedAtTarget)
	{
		setSemiTrans(ft4,true);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPointerIcon::setTarget(DVECTOR _target)
{
	if(m_pointerTarget.vx!=_target.vx||m_pointerTarget.vy!=_target.vy)
	{
		m_pointerTarget=_target;
		m_pointerArrivedAtTarget=false;
		m_pointerSin=0;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPointerIcon::snapToTarget(DVECTOR _target)
{
	m_pointerPos=_target;
	m_pointerTarget=_target;
	m_pointerArrivedAtTarget=true;
	m_pointerSin=0;
}

/*===========================================================================
 end */
