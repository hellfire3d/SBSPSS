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

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
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
	{	1,1,	FRM__C1_L1_QUEST_ITEM	},
	{	1,2,	FRM__C1_L2_QUEST_ITEM	},
	{	1,3,	FRM__C1_L3_QUEST_ITEM	},
	{	1,4,	FRM__C1_L4_QUEST_ITEM	},
	{	2,1,	FRM__C2_L1_QUEST_ITEM	},
	{	2,2,	FRM__C2_L2_QUEST_ITEM	},
	{	2,3,	FRM__C2_L3_QUEST_ITEM	},
	{	2,4,	FRM__C2_L4_QUEST_ITEM	},
	{	3,1,	FRM__C3_L1_QUEST_ITEM	},
	{	3,2,	FRM__C3_L2_QUEST_ITEM	},
	{	3,3,	FRM__C3_L3_QUEST_ITEM	},
	{	3,4,	FRM__C3_L4_QUEST_ITEM	},
	{	4,1,	FRM__C4_QUEST_ITEM_1	},
	{	4,2,	FRM__C4_QUEST_ITEM_1	},
	{	4,3,	FRM__C4_QUEST_ITEM_1	},
	{	4,4,	FRM__C4_QUEST_ITEM_2	},
	{	5,1,	FRM__C5_L1_QUEST_ITEM	},
	{	5,2,	FRM__C5_L2_QUEST_ITEM	},
	{	5,3,	FRM__C5_L3_QUEST_ITEM	},
	{	5,4,	FRM__C5_L4_QUEST_ITEM	},
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

	if(level==5-1)
	{
		m_hiddenUntilBossDead=true;
	}
	else
	{
		m_hiddenUntilBossDead=false;
	}

	m_starSin=0;
	m_starSinR=m_starSinG=m_starSinB=0;
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
	_player->setCanExitLevelNow();
	CBasePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CQuestItemPickup::collidedWith(CThing *_thisThing)
{
	if(!m_hiddenUntilBossDead)
	{
		CBasePickup::collidedWith(_thisThing);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CQuestItemPickup::getVisibilityRadius()
{
	return 100;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CQuestItemPickup::thinkPickup(int _frames)
{
	if(!m_hiddenUntilBossDead)
	{
		m_starSin=(m_starSin+(65*_frames))&4095;
		m_starSinR=(m_starSinR+(20*_frames))&4095;
		m_starSinG=(m_starSinG+(40*_frames))&4095;
		m_starSinB=(m_starSinB+(70*_frames))&4095;
	}
	else if(GameScene.getBossHasBeenKilled())
	{
		m_hiddenUntilBossDead=false;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:	LOTS to optimise here.. (pkg)
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static int s_questStarFrames[4]={FRM__GLINT1,FRM__GLINT2,FRM__GLINT3,FRM__GLINT4};
void	CQuestItemPickup::renderPickup(DVECTOR *_pos)
{
	if(!m_hiddenUntilBossDead)
	{
		DVECTOR		pos;
		SpriteBank	*sprites;
		sFrameHdr	*fh;
		int			i;
		int			x,y;
		int			angle,colourOfs;

		sprites=CGameScene::getSpriteBank();

		// Quest item graphic
		fh=sprites->getFrameHeader(m_gfxFrame);
		x=_pos->vx-(fh->W/2);
		y=_pos->vy-(fh->H/2);
		sprites->printFT4(fh,x,y,0,0,OTPOS__PICKUP_POS);

		// Stars
		angle=m_starSin;
		colourOfs=0;
		for(i=0;i<4;i++)
		{
			POLY_FT4	*ft4;
			int			r,g,b;
			fh=sprites->getFrameHeader(s_questStarFrames[i]);
			x=_pos->vx+((msin(angle)*40)>>12)-(fh->W/2);
			y=_pos->vy+((mcos(angle)*40)>>12)-(fh->H/2);
			ft4=sprites->printFT4(fh,x,y,0,0,0);
			r=((msin((m_starSinR+colourOfs)&4095)*64)>>12)+128;
			g=((msin((m_starSinG+colourOfs)&4095)*64)>>12)+128;
			b=((msin((m_starSinB+colourOfs)&4095)*64)>>12)+128;
			setRGB0(ft4,r,g,b);
			angle=(angle+300)&4095;
			colourOfs+=200;
		}
	}
}


/*===========================================================================
end */
