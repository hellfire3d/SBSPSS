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

#include <sprites.h>

CHealthManager::sItemTable	CHealthManager::ItemTable[]=
{
	{5,256,	255,255,0},
	{1,256,	127,127,127},
};

const int	CHealthManager::ItemTableSize=sizeof(CHealthManager::ItemTable)/sizeof(CHealthManager::sItemTable);

int	HealthManGrav=128;
int	HealthManShift=9;
int	HealthManPickUpDelay=16;

/*****************************************************************************/
void	CHealthManager::init()
{
		FrameHdr=CGameScene::getSpriteBank()->getFrameHeader(FRM__SPATULA);
		for (int i=0; i<ITEM_MAX; i++)
		{
			ItemList[i].Life=0;
			setTSprt(&ItemList[i].Sprite);
			setTSprtTPage(&ItemList[i].Sprite,FrameHdr->TPage);
			ItemList[i].Sprite.clut=FrameHdr->Clut;
			ItemList[i].Sprite.u0=FrameHdr->U;
			ItemList[i].Sprite.v0=FrameHdr->V;
			ItemList[i].Sprite.w=FrameHdr->W;
			ItemList[i].Sprite.h=FrameHdr->H;
		}

}

/*****************************************************************************/
void	CHealthManager::shutdown()
{
}

/*****************************************************************************/
const int	AngleS=2048+1024+512;
void	CHealthManager::drop(DVECTOR &Pos,int Amount,int Vel)
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
}

/*****************************************************************************/
void	CHealthManager::addItem(DVECTOR &Pos,int TableIdx,int Angle,int Vel)
{
int		Idx=0;

		while (ItemList[Idx].Life) Idx++;
		ASSERT(Idx<ITEM_MAX);

		ItemList[Idx].Life=ItemTable[TableIdx].Life;
		setRGB0(&ItemList[Idx].Sprite,ItemTable[TableIdx].R,ItemTable[TableIdx].G,ItemTable[TableIdx].B);
		ItemList[Idx].Pos.vx=Pos.vx<<HealthManShift;
		ItemList[Idx].Pos.vy=Pos.vy<<HealthManShift;

		ItemList[Idx].Vel.vx=-(msin(Angle)*Vel)>>1;
		ItemList[Idx].Vel.vy=-(mcos(Angle)*Vel);
		ItemList[Idx].Count=ItemTable[TableIdx].Count;
}

/*****************************************************************************/
void	CHealthManager::checkPlayerCol(CPlayer *Player)
{
CRECT	const &PRect=Player->getCollisionArea();

		for (int i=0; i<ITEM_MAX; i++)
		{
			if (ItemList[i].Life  && ItemList[i].Life<256-HealthManPickUpDelay)
			{
				if (PRect.x2<ItemList[i].ScrPos.vx || PRect.x1>ItemList[i].ScrPos.vx+16 ||
					PRect.y2<ItemList[i].ScrPos.vy || PRect.y1>ItemList[i].ScrPos.vy+32)
				{
				}
				else
				{
					ItemList[i].Life=0;
					Player->addSpatula(ItemList[i].Count);
				}

			}

		}
}

/*****************************************************************************/
/*** think *******************************************************************/
/*****************************************************************************/
void	CHealthManager::think(int frames)
{
	CLayerCollision	*ColLayer=CGameScene::getCollision();

		for (int i=0; i<ITEM_MAX; i++)
		{
			for (int f=0; f<frames; f++)
			{
				if (ItemList[i].Life)
				{
					ItemList[i].Life--;
					ItemList[i].Pos.vx+=ItemList[i].Vel.vx;
					ItemList[i].Pos.vy+=ItemList[i].Vel.vy;
					ItemList[i].ScrPos.vx=ItemList[i].Pos.vx>>HealthManShift;
					ItemList[i].ScrPos.vy=ItemList[i].Pos.vy>>HealthManShift;
					ItemList[i].Vel.vy+=HealthManGrav;

					if (ItemList[i].Vel.vy>0)
					{ // Check ground collision
						int	DistY = ColLayer->getHeightFromGround( ItemList[i].ScrPos.vx, ItemList[i].ScrPos.vy, 16 );
						if (DistY<=0)
						{
							ItemList[i].Vel.vy=-ItemList[i].Vel.vy>>1;
							ItemList[i].Vel.vx>>=1;
	//						ItemList[i].Pos.vy-=DistY<<(HealthManShift-1);
						}
					}

					int	XOfs;
					if (ItemList[i].Vel.vx>0)
					{ 
						XOfs=+16;
					}
					else
					{
						XOfs=-16;
					}
					// Check X collision
					int	DistX = ColLayer->getHeightFromGround( ItemList[i].ScrPos.vx+XOfs, ItemList[i].ScrPos.vy, 32 );
					if (DistX<=0)
					{
						ItemList[i].Vel.vx=-ItemList[i].Vel.vx>>1;
	//					ItemList[i].Pos.vy-=DistY<<(HealthManShift-1);
					}
					
				}
			}
		}

}

/*****************************************************************************/
/*** render ******************************************************************/
/*****************************************************************************/
void	CHealthManager::render()
{
sOT		*ThisOT=OtPtr;//+OTPOS__PICKUP_POS;
DVECTOR	const	&CamPos=CLevel::getCameraPos();

	for (int i=0; i<ITEM_MAX; i++)
	{
		if (ItemList[i].Life)
		{
			// Calc render pos (dont worry about clipping yet)
			ItemList[i].Sprite.x0 = ItemList[i].ScrPos.vx - CamPos.vx;
			ItemList[i].Sprite.y0 = (ItemList[i].ScrPos.vy - CamPos.vy)-32;

			addPrim(ThisOT,&ItemList[i].Sprite);
		}
	}

}
