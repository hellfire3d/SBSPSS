/******************/
/*** TV Explode ***/
/******************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"
#include	"gfx\otpos.h"

#include	"FX\FXTVExplode.h"
#include	"FX\FXBaseAnim.h"

/*****************************************************************************/
CFXBaseAnim::sFXBaseData	FXCogBaseData=
{
		FRM__COG01,FRM__COG01,3,
		FX_FLAG_LOOP | FX_FLAG_COLLIDE_BOUNCE | FX_FLAG_HAS_GRAVITY, 
};

CFXBaseAnim::sFXBaseData	FXValveBaseData=
{
		FRM__ELECVALVE,FRM__ELECVALVE,1,
		FX_FLAG_LOOP | FX_FLAG_COLLIDE_BOUNCE | FX_FLAG_HAS_GRAVITY, 
};


/*****************************************************************************/
static const int		TVExplodeVel=8;
static const int		TVExplodeCogs=4;
static const int		TVExplodeValves=4;

/*****************************************************************************/
void	CFXTVExplode::init(DVECTOR const &_Pos)
{
	CFX::init(_Pos);
}

/*****************************************************************************/
void	CFXTVExplode::think(int _frames)
{
CFX			*NewFX;

int			VelX,VelY;

			NewFX=CFX::Create(CFX::FX_TYPE_SMOKE_PUFF,Pos);
			NewFX->SetOtPos(1);

			NewFX=CFX::Create(CFX::FX_TYPE_EXPLODE,Pos);
			((CFXBaseAnim*)NewFX)->SetScale(ONE*2);
			NewFX->SetOtPos(1);


			for (int i=0;i<TVExplodeCogs; i++)
			{
				NewFX=CFX::Create(CFX::FX_TYPE_BASE_ANIM,Pos);
				NewFX->setBaseData(&FXCogBaseData);
				VelX=getRndRange(TVExplodeVel*2)-TVExplodeVel;
				VelY=-getRndRange(TVExplodeVel);
				NewFX->setVelocity(VelX,VelY);
				NewFX->SetOtPos(1);
			}

			for (int i=0;i<TVExplodeValves; i++)
			{
				NewFX=CFX::Create(CFX::FX_TYPE_BASE_ANIM,Pos);
				NewFX->setBaseData(&FXValveBaseData);
				VelX=getRndRange(TVExplodeVel*2)-TVExplodeVel;
				VelY=-getRndRange(TVExplodeVel);
				NewFX->setVelocity(VelX,VelY);
				NewFX->SetOtPos(1);
			}

			killFX();
}

