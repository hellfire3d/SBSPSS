/******************************/
/*** Health throw out stuff ***/
/******************************/

#include "system\global.h"
#include "mem\memory.h"
#include "gfx\sprbank.h"
#include "utils\utils.h"
#include "gfx\prim.h"
//#include "gfx\actor.h"
#include "game\game.h"
#include "player\player.h"
#include "gfx\otpos.h"
#include "game\healthman.h"

#ifndef __SAVE_SAVE_H__
#include "save\save.h"
#endif

#include <sprites.h>

CHealthManager::sItemTable	CHealthManager::ItemTable[]=
{
	{5,256,	255,255,0},
	{1,256,	127,127,127},
};

const int	CHealthManager::ItemTableSize=sizeof(CHealthManager::ItemTable)/sizeof(CHealthManager::sItemTable);

int	HealthManGrav=128;
int	HealthManShift=9;
int	HealthManPickUpDelay=32;

/*****************************************************************************/
void	CHealthManager::init()
{
sItem	*list=ItemList;

		FrameHdr=CGameScene::getSpriteBank()->getFrameHeader(FRM__SPATULA);
		for (int i=0; i<ITEM_MAX; i++)
		{
			list->Life=0;
			setTSprt(&list->Sprite);
			setTSprtTPage(&list->Sprite,FrameHdr->TPage);
			list->Sprite.clut=FrameHdr->Clut;
			list->Sprite.u0=FrameHdr->U;
			list->Sprite.v0=FrameHdr->V;
			list->Sprite.w=FrameHdr->W;
			list->Sprite.h=FrameHdr->H;
			list++;
		}

}

/*****************************************************************************/
void	CHealthManager::shutdown()
{
}

/*****************************************************************************/
const int	AngleS=2048+1024+512;
void	CHealthManager::drop(DVECTOR const &Pos,int Amount,int Vel)
{
int		Count=0;
int		Am=Amount;
// Count em for Arc
		for (int i=0; i<ItemTableSize; i++)
		{
			sItemTable	&T=ItemTable[i];

			while (Amount>=T.Count)
			{
				Amount-=T.Count;
				Count++;
			}
		}
int		Angle=AngleS;
int		AngleInc=1024/Count;
// Split Em
		Amount=Am;
		for (int i=0; i<ItemTableSize; i++)
		{
			sItemTable	&T=ItemTable[i];

			while (Amount>=T.Count)
			{
				Amount-=T.Count;
				addItem(Pos,i,Angle,Vel);
				Angle+=AngleInc;
				Angle&=4095;
			}
		}
		think(1);
}

/*****************************************************************************/
void	CHealthManager::addItem(DVECTOR const &Pos,int TableIdx,int Angle,int Vel)
{
int		Idx=0;
sItem	*item;

		while (ItemList[Idx].Life) Idx++;
		ASSERT(Idx<ITEM_MAX);

		item=&ItemList[Idx];
		item->Life=ItemTable[TableIdx].Life;
		setRGB0(&item->Sprite,ItemTable[TableIdx].R,ItemTable[TableIdx].G,ItemTable[TableIdx].B);
		item->Pos.vx=Pos.vx<<HealthManShift;
		item->Pos.vy=Pos.vy<<HealthManShift;

		item->Vel.vx=-(msin(Angle)*Vel);//>>1;
		item->Vel.vy=-(mcos(Angle)*Vel);

//		item->Pos.vx+=item->Vel.vx;
//		item->Pos.vy+=item->Vel.vy;

		item->Count=ItemTable[TableIdx].Count;
}

/*****************************************************************************/
void	CHealthManager::checkPlayerCol(CPlayer *Player)
{
CRECT	PRect=Player->getCollisionArea();
sItem	*item=ItemList;


		PRect.x1+=8;
		PRect.x2+=8;
		PRect.y1+=16;
		PRect.y2+=16;

		for (int i=0; i<ITEM_MAX; i++)
		{
			if (item->Life  && item->Life<256-HealthManPickUpDelay)
			{
				if (PRect.x2<item->ScrPos.vx || PRect.x1>item->ScrPos.vx+16 ||
					PRect.y2<item->ScrPos.vy || PRect.y1>item->ScrPos.vy+32)
				{
				}
				else
				{
					item->Life=0;
					Player->addSpatula(item->Count);
					CSoundMediator::playSfx(CSoundMediator::SFX_ITEM__ANY_OTHER_ITEM);
				}

			}

			item++;
		}
}

/*****************************************************************************/
/*** think *******************************************************************/
/*****************************************************************************/
void	CHealthManager::think(int frames)
{
sItem	*item=ItemList;
int		mapHeight;

	CLayerCollision	*ColLayer=CGameScene::getCollision();
	mapHeight=GameScene.GetLevel().getMapSize().vy*16;

		for (int i=0; i<ITEM_MAX; i++)
		{
			for (int f=0; f<frames; f++)
			{
				if (item->Life>1)
				{
					int	OldY=ItemList[i].Pos.vy;
					item->Life--;
					item->Pos.vx+=item->Vel.vx;
					item->Pos.vy+=item->Vel.vy;
					item->ScrPos.vx=item->Pos.vx>>HealthManShift;
					item->ScrPos.vy=item->Pos.vy>>HealthManShift;
					item->Vel.vy+=HealthManGrav;

//					if (item->Vel.vy>0)
					{ 
						int	DistY = ColLayer->getHeightFromGround( item->ScrPos.vx, item->ScrPos.vy, 16 );
						int newBlock=ColLayer->getCollisionBlock(item->ScrPos.vx,item->ScrPos.vy)&COLLISION_TYPE_MASK;
						if (DistY<=0&&newBlock!=COLLISION_TYPE_FLAG_DEATH_FALL )
						{
							if (item->Vel.vy<0)
							{
								item->Pos.vy=OldY;
							}
							item->Vel.vy=-item->Vel.vy>>1;
							item->Vel.vx>>=1;
	//						item->Pos.vy-=DistY<<(HealthManShift-1);
						}

						if(item->ScrPos.vy>mapHeight)
						{
							item->Life=0;
						}
					}

					int	XOfs;
					if (item->Vel.vx>0)
					{ 
						XOfs=+16;
					}
					else
					{
						XOfs=-16;
					}
					// Check X collision
					int	DistX = ColLayer->getHeightFromGround( item->ScrPos.vx+XOfs, item->ScrPos.vy, 32 );
					if (DistX<=0)
					{
						item->Vel.vx=-item->Vel.vx>>1;
	//					item->Pos.vy-=DistY<<(HealthManShift-1);
					}
					
				}
				else
				{
					item->Life=0;
				}
			}

			item++;
		}

}

/*****************************************************************************/
/*** render ******************************************************************/
/*****************************************************************************/
void	CHealthManager::render()
{
sItem	*list=ItemList;
sOT		*ThisOT=OtPtr;//+OTPOS__PICKUP_POS;
DVECTOR	const	&CamPos=CLevel::getCameraPos();
	for (int i=0; i<ITEM_MAX; i++)
	{
		int	life=list->Life;
		if (life&&(life<256-HealthManPickUpDelay||life&3))
		{
			// Calc render pos (dont worry about clipping yet)
			list->Sprite.x0 = list->ScrPos.vx - CamPos.vx;
			list->Sprite.y0 = (list->ScrPos.vy - CamPos.vy)-32;

			addPrim(ThisOT,&list->Sprite);
		}
		list++;
	}

}
