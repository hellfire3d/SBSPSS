/**********************/
/*** ZZZZ (for FMA) ***/
/**********************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"
#include	"gfx\otpos.h"

#include	"FX\FXzzzz.h"

static const int	FRM_Z=FRM__90;
static const int	ZLifeInc=-8;
static const int	ZDelay=32;

static const s8		XT[16]={ 0,+1,+2,+2,+3,+2,+2,+1, 
							 0,-1,-2,-2,-3,-2,-2,-1};

/*****************************************************************************/
void	CFXZZZZ::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);

		for (int i=0;i<Z_COUNT; i++)
		{
			Z[i].Life=0;
		}
		Count=0;
		DieFlag=0;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXZZZZ::think(int _frames)
{
//		CFX::think(_frames);

		Count-=_frames;
		
		if (Count<=0 && !DieFlag)
		{ // Make new

			Count=ZDelay;

			int	Idx;
			for (Idx=0; Idx<Z_COUNT; Idx++)
			{
				if (Z[Idx].Life==0) break;
			}
			if (Idx<Z_COUNT)
			{ // Got free
				sZ	&ThisZ=Z[Idx];

				ThisZ.Ofs.vx=0;
				ThisZ.Ofs.vy=0;
				ThisZ.Life=256;
				ThisZ.TablePos=getRnd() & 15;
			}
		}

// Move Em
		for (int i=0;i<Z_COUNT; i++)
		{
			if (Z[i].Life>0)
			{
				Z[i].Ofs.vx+=XT[Z[i].TablePos]-1;
				Z[i].Ofs.vy--;
				Z[i].Life+=ZLifeInc;
				if (Z[i].Life<0) Z[i].Life=0;

				Z[i].TablePos++;
				Z[i].TablePos&=15;

			}
			else
			{
				Z[i].Life=0;
			}

		}
		if (DieFlag==1)
		{
			DieFlag++;
		}
		else
		if (DieFlag==2)
		{
			CFX::killFX();
		}
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXZZZZ::render()
{
		CFX::render();
DVECTOR		renderPos;
		getFXRenderPos(renderPos);
		if (!canRender() || Flags & FX_FLAG_HIDDEN) return;
int		Frame=FRM_Z;
		if (DieFlag)
			Frame=FRM__BUBBLEPOP;

POLY_FT4	*Ft4;
			for (int i=0; i<Z_COUNT; i++)
			{
				sZ	&ThisZ=Z[i];

				if (ThisZ.Life)
				{	
					DVECTOR	ThisPos;
					int		HLife=ThisZ.Life/2;
					ThisPos.vx=renderPos.vx+ThisZ.Ofs.vx;
					ThisPos.vy=renderPos.vy+ThisZ.Ofs.vy;
					Ft4=CGameScene::getSpriteBank()->printFT4Scaled(Frame,ThisPos.vx,ThisPos.vy,0,0,OtPos,128+HLife);
					setShadeTex(Ft4,0);
					setSemiTrans(Ft4,1);
					Ft4->tpage|=1<<5;
					
					setRGB0(Ft4,HLife,HLife,HLife);
				}
			}
}

