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

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#include	"game/game.h"

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

typedef struct
{
	u8		m_chapter,m_level;
	u16		m_gfxFrame;
} sQuestItemMap;


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

static const sQuestItemMap	s_questItemMap[]=
{
	{	1,1,	FRM__C1_L1_JELLY		},
	{	1,2,	FRM__C1_L2_SEAWEED		},
	{	1,3,	FRM__C1_L3_SEANUTBUTTER	},
	{	1,4,	FRM__C1_L4_BREADSLICE	},
	{	2,1,	FRM__C2_L1_FALSETEETH	},
	{	2,2,	FRM__C2_L2_KELPKREAM	},
	{	2,3,	FRM__C2_L3_MUDPACK		},
	{	2,4,	FRM__C2_L4_SPONGE		},
	{	3,1,	FRM__C3_L1_SLIPPERS		},
	{	3,2,	FRM__C3_L2_CLAMBRA		},
	{	3,3,	FRM__C3_L3_STARFISHMASK	},
	{	3,4,	FRM__C3_L4_SUPERPANTZ	},
	{	4,1,	FRM__C4_COIN			},
	{	4,2,	FRM__C4_COIN			},
	{	4,3,	FRM__C4_COIN			},
	{	4,4,	FRM__C4_KELPBAR			},
	{	5,1,	FRM__C5_L1_HAMMER		},
	{	5,2,	FRM__C5_L2_ARIEL		},
	{	5,3,	FRM__C5_L3_OILCAN		},
	{	5,4,	FRM__C5_L4_WRENCH		},
};
static const int			s_numQuestItemMap=sizeof(s_questItemMap)/sizeof(sQuestItemMap);


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CQuestItemPickup::init()
{
	int					chapter,level;
	const sQuestItemMap	*qi;
	int					i;
	sFrameHdr			*fh;


	CBasePickup::init();
	m_pingFrame=0;

	chapter=GameScene.getChapterNumber();
	level=GameScene.getLevelNumber();
	m_gfxFrame=-1;
	qi=s_questItemMap;
	for(i=0;i<s_numQuestItemMap;i++)
	{
		if(qi->m_chapter==chapter&&qi->m_level==level)
		{
			m_gfxFrame=qi->m_gfxFrame;
			break;
		}
		qi++;
	}
	ASSERT(m_gfxFrame!=-1);

	fh=CGameScene::getSpriteBank()->getFrameHeader(m_gfxFrame);
	setCollisionSize(fh->W,fh->H);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CQuestItemPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(m_gfxFrame);
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
void	CQuestItemPickup::collect(class CPlayer *_player)
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
int		CQuestItemPickup::getVisibilityRadius()
{
	return quest_pingsize;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CQuestItemPickup::thinkPickup(int _frames)
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
void	CQuestItemPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(m_gfxFrame);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	sprites->printFT4(fh,x,y,0,0,OTPOS__PICKUP_POS);

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
			AddPrimToList(g3,OTPOS__PICKUP_POS+1);
			p1=p2;
		}

		// Trans
		ft3=GetPrimFT3();
		setShadeTex(ft3,1);
		setSemiTrans(ft3,1);
		ft3->tpage=(quest_transmode<<5);
		setXY3(ft3,512,512,512,512,512,512);
		AddPrimToList(ft3,OTPOS__PICKUP_POS+1);
	}
}


/*===========================================================================
end */
