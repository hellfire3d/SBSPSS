/*=========================================================================

	pquest.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"	// Damnit.. include order! :( (pkg)
#endif

#include "pickups\pquest.h"

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
#endif



/*
Chapter 1
	1 jelly
	2 sea weed
	3 seanut butter
	4 slice of bread
Chapter 2
	1 false teeth
	2 kelp kream
	3 mud pack
	4 sponge
Chapter 3
	1 clam bra
	2 slippers
	3 starfish mask
	4 super pants
Chapter 4
	1 coin
	2 coin
	3 coin
	4 kelp bar
Chapter 5
	1 ariel
	2 hammer
	3 oilcan
	4 wrench
*/

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
void	CBaseQuestItemPickup::init()
{
	CBasePickup::init();
	m_pingFrame=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CBaseQuestItemPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=getSpriteBank()->getFrameHeader(getFrameNumber());
	size.vx=fh->W;
	size.vy=fh->H;
	return size;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseQuestItemPickup::collect(class CPlayer *_player)
{
	CBasePickup::collect(_player);
}

int quest_pingframes=50;
int quest_pingwaitframes=100;
int quest_pingsize=100;
int quest_pingr=0;
int quest_pingg=255;
int quest_pingb=0;
int	quest_pingsegments=16;
int quest_transmode=1;
/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CBaseQuestItemPickup::getVisibilityRadius()
{
	return quest_pingsize;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseQuestItemPickup::thinkPickup(int _frames)
{
	m_pingFrame+=_frames;
	if(m_pingFrame>(quest_pingframes+quest_pingwaitframes))
	{
		// Do sound too.. (pkg)
		m_pingFrame=0;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:	LOTS to optimise here.. (pkg)
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseQuestItemPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(getFrameNumber());
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	sprites->printFT4(fh,x,y,0,0,PICKUPS_OT_POS);

	if(m_pingFrame<quest_pingframes)
	{
		int			radius;
		int			endr,endg,endb;
		int			angle;
		DVECTOR		p1,p2;
		int			i;
		POLY_G3		*g3;
		POLY_FT3	*ft3;

		x=_pos->vx;
		y=_pos->vy;
		radius=(quest_pingsize*m_pingFrame)/quest_pingframes;
		endr=(quest_pingr*(quest_pingframes-m_pingFrame))/quest_pingframes;
		endg=(quest_pingg*(quest_pingframes-m_pingFrame))/quest_pingframes;
		endb=(quest_pingb*(quest_pingframes-m_pingFrame))/quest_pingframes;

		p1.vx=x;
		p1.vy=y+radius;
		for(i=0;i<quest_pingsegments;i++)
		{
			angle=(4095*(i+1))/quest_pingsegments;
			p2.vx=x+((msin(angle)*radius)>>12);
			p2.vy=y+((mcos(angle)*radius)>>12);
			g3=GetPrimG3();
			setXY3(g3,x,y,p1.vx,p1.vy,p2.vx,p2.vy);
			setRGB0(g3,0,0,0);
			setRGB1(g3,endr,endg,endb);
			setRGB2(g3,endr,endg,endb);
			setShadeTex(g3,0);
			setSemiTrans(g3,1);
			AddPrimToList(g3,PICKUPS_OT_POS+1);
			p1=p2;
		}

		// Trans
		ft3=GetPrimFT3();
		setShadeTex(ft3,1);
		setSemiTrans(ft3,1);
		ft3->tpage=(quest_transmode<<5);
		setXY3(ft3,512,512,512,512,512,512);
		AddPrimToList(ft3,PICKUPS_OT_POS+1);
	}
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CTestQuestItemPickup::getFrameNumber()
{
	return FRM__C2_L1_FALSETEETH;
}

/*===========================================================================
end */
