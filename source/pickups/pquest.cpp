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
	{	4,4,	FRM__C4_QUEST_ITEM_1	},
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
	m_collected=false;
	m_collectedFrameCount=0;

	m_starSinRotate=0;
	m_starSinRadius=0;
	m_starFrame=0;
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
	if(!m_hiddenUntilBossDead&!m_collected)
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
//				collect((CPlayer*)_thisThing);
				CSoundMediator::playSfx(sfxToPlayWhenCollected());
				m_collected=true;
				((CPlayer*)_thisThing)->collectedQuestItem();
				break;

			case TYPE_NPC:
				break;

			default:
				ASSERT(0);
		}
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
	if(m_collected)
	{
		m_collectedFrameCount+=_frames;
		if(m_collectedFrameCount>100)
		{
			collect(NULL);
		}
	}
	if(!m_hiddenUntilBossDead)
	{
		m_starSinRotate=(m_starSinRotate+(16*_frames))&4095;
		m_starSinRadius=(m_starSinRadius+(32*_frames))&4095;
		m_starFrame+=_frames;
		m_starSinR=(m_starSinR+(10*_frames))&4095;
		m_starSinG=(m_starSinG+(20*_frames))&4095;
		m_starSinB=(m_starSinB+(30*_frames))&4095;
	}
	else if(GameScene.getBossHasBeenKilled())
	{
		m_hiddenUntilBossDead=false;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static const int s_questStarFrames[8]={FRM__QUESTITEMSTAR1,FRM__QUESTITEMSTAR2,FRM__QUESTITEMSTAR3,FRM__QUESTITEMSTAR2};
void	CQuestItemPickup::renderPickup(DVECTOR *_pos)
{
	if(!m_hiddenUntilBossDead)
	{
		DVECTOR		pos;
		SpriteBank	*sprites;
		sFrameHdr	*fh;
		int			i;
		int			x,y;
		int			angle,radius;
		int			r,g,b;

		sprites=CGameScene::getSpriteBank();

		// Quest item graphic
		if(!m_collected)
		{
			fh=sprites->getFrameHeader(m_gfxFrame);
			x=_pos->vx-(fh->W/2);
			y=_pos->vy-(fh->H/2);
			sprites->printFT4(fh,x,y,0,0,OTPOS__PICKUP_POS);
		}

		// Stars
		angle=m_starSinRotate;
		radius=((msin(m_starSinRadius)*20)>>12)+40+(m_collectedFrameCount*2);
		r=((msin(m_starSinR)*64)>>12)+128;
		g=((msin(m_starSinG)*64)>>12)+128;
		b=((msin(m_starSinB)*64)>>12)+128;
		fh=sprites->getFrameHeader(s_questStarFrames[(m_starFrame>>3)&3]);
		for(i=0;i<4;i++)
		{
			POLY_FT4	*ft4;
			x=_pos->vx+((msin(angle)*radius)>>12);
			y=_pos->vy+((mcos(angle)*radius)>>12);
			ft4=sprites->printFT4(fh,x,y,0,0,0);
			setRGB0(ft4,r,g,b);
			angle=(angle+1024)&4095;
		}

		angle=(m_starSinRotate+512)&4095;
		radius=((msin((m_starSinRadius+2048)&4095)*20)>>12)+40+(m_collectedFrameCount*2);
		r=((msin((m_starSinR+2048)&4095)*64)>>12)+128;
		g=((msin((m_starSinG+2048)&4095)*64)>>12)+128;
		b=((msin((m_starSinB+2048)&4095)*64)>>12)+128;
		fh=sprites->getFrameHeader(s_questStarFrames[((m_starFrame>>3)+2)&3]);
		for(i=0;i<4;i++)
		{
			POLY_FT4	*ft4;
			x=_pos->vx+((msin(angle)*radius)>>12);
			y=_pos->vy+((mcos(angle)*radius)>>12);
			ft4=sprites->printFT4(fh,x,y,0,0,0);
			setRGB0(ft4,r,g,b);
			angle=(angle+1024)&4095;
		}
	}
}


/*===========================================================================
end */
