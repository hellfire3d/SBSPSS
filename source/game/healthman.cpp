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
#include "system\vid.h"
#include "game\healthman.h"

#ifndef __SAVE_SAVE_H__
#include "save\save.h"
#endif

#include <sprites.h>

CHealthManager::sItemTable	CHealthManager::ItemTable[]=
{
	{CHealthManager::SPAT_CLUMP,	255,255,0},
	{1,								127,127,127},
};

const int	CHealthManager::ItemTableSize=sizeof(CHealthManager::ItemTable)/sizeof(CHealthManager::sItemTable);

const int	HealthManGrav=128;
const int	HealthManShift=9;
const int	HealthManPickUpDelay=32;

/*****************************************************************************/
void	CHealthManager::init()
{
sItem	*list=ItemList;

		FrameHdr=CGameScene::getSpriteBank()->getFrameHeader(FRM__SPATULA);
		for (int i=0; i<SPAT_MAX; i++)
		{
			list->Life=0;
			for (int s=0; s<2; s++)
			{
				TSPRT	*Spr=&list->Sprite[s];
				setTSprt(Spr);
				setTSprtTPage(Spr,FrameHdr->TPage);
				Spr->clut=FrameHdr->Clut;
				Spr->u0=FrameHdr->U-1;
				Spr->v0=FrameHdr->V-1;
				Spr->w=FrameHdr->W;
				Spr->h=FrameHdr->H;
			}
			list++;
		}
	Frame=0;
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
		ASSERT(Idx<SPAT_MAX);
		item=&ItemList[Idx];
		item->Life=SPAT_LIFE;
		setRGB0(&item->Sprite[0],ItemTable[TableIdx].R,ItemTable[TableIdx].G,ItemTable[TableIdx].B);
		setRGB0(&item->Sprite[1],ItemTable[TableIdx].R,ItemTable[TableIdx].G,ItemTable[TableIdx].B);
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

		for (int i=0; i<SPAT_MAX; i++)
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
const int	YO=32;
const int	CO=1;
void	CHealthManager::think(int frames)
{
sItem	*item=ItemList;
int		Dist;
	
		CLayerCollision	*ColLayer=CGameScene::getCollision();

		for (int i=0; i<SPAT_MAX; i++)
		{
			for (int f=0; f<frames; f++)
			{
				if (item->Life>1)
				{
					int		CheckOfs=16;
					int		YOfs=0;
					int		ColEx=COLLISION_TYPE_FLAG_DEATH_FALL;
					int		XS=1;
					int		XYO=0;

					item->Life--;
					item->Pos.vx+=item->Vel.vx;
					item->Pos.vy+=item->Vel.vy;
					item->ScrPos.vx=item->Pos.vx>>HealthManShift;
					item->ScrPos.vy=item->Pos.vy>>HealthManShift;
					item->Vel.vy+=HealthManGrav;

					if (item->Vel.vy)
					{
						if (item->Vel.vy<0)
						{ // Going Up
							YOfs=-YO;//+(item->Vel.vy>>HealthManShift);
							CheckOfs=CO;
							ColEx=0;	// Dont exclude any types when going up
							XS=0;		// dont reduce x on head col
						}
					
						Dist = ColLayer->getHeightFromGround( item->ScrPos.vx, item->ScrPos.vy+YOfs, CheckOfs );
						if ((item->Vel.vy>0 && Dist<=0) || (item->Vel.vy<0 && Dist!=CheckOfs))
						{
							int newBlock=ColLayer->getCollisionBlock(item->ScrPos.vx,item->ScrPos.vy) & COLLISION_TYPE_MASK;
							if (!(ColEx && newBlock==ColEx))
							{
								item->Pos.vy+=Dist<<HealthManShift;	// align to ground
								item->ScrPos.vy=item->Pos.vy>>HealthManShift;
								item->Vel.vy-=HealthManGrav;
								item->Vel.vy=-item->Vel.vy>>1;
								item->Vel.vx>>=XS;
								XYO=-4;			// to stop spat coming back on hitting ground
							}
						}
					}

					if(item->ScrPos.vy>GameScene.GetLevel().getMapHeight16()) item->Life=0;

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
					Dist = ColLayer->getHeightFromGround( item->ScrPos.vx+XOfs, item->ScrPos.vy+XYO, 16);
					if (Dist<=0)
					{
						item->Vel.vx=-item->Vel.vx>>1;
						item->Vel.vy>>=1;
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
sOT		*ThisOT=OtPtr+OTPOS__PICKUP_POS;
DVECTOR	const	&CamPos=CLevel::getCameraPos();

	for (int i=0; i<SPAT_MAX; i++)
	{
		int	Life=list->Life;
		if (Life<64 && Frame) Life=0;
		if (Life)
		{
			TSPRT	*Spr=&list->Sprite[Frame];
			// Calc render pos (dont worry about clipping yet)
			Spr->x0 =  list->ScrPos.vx - CamPos.vx;
			Spr->y0 = (list->ScrPos.vy - CamPos.vy)-32;

			addPrim(ThisOT,Spr);
		}
		list++;
	}
	Frame^=1;
}
