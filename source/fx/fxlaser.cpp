/*************/
/*** Laser ***/
/*************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"
#include	"gfx\otpos.h"

#include	"FX\FXLaser.h"

int		LaserWidth=1;

/*****************************************************************************/
void	CFXLaser::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		Life=-1;
		R=G=B=255;
		Offset.vx=Offset.vy=0;
}

/*****************************************************************************/
void	CFXLaser::setOffset(DVECTOR &Pos)
{
		Offset=Pos;
}

/*****************************************************************************/
void	CFXLaser::setTarget(DVECTOR &Pos)
{
		Target=Pos;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXLaser::think(int _frames)
{
CThing	*Parent=getParent();
		ASSERT(Parent);

		Pos=Parent->getPos();
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXLaser::render()
{
DVECTOR		renderPos0,renderPos1;
sOT			*ThisOT=OtPtr+OtPos;

		getFXRenderPos(renderPos0);
		if (!canRender() || !IsVisible) return;

		calcRenderPos(Target,renderPos1);
		renderPos0.vx+=Offset.vx;
		renderPos0.vy+=Offset.vy;
// Main Beam
LINE_F2		*L=GetPrimLF2();
			L->x0=renderPos0.vx;	L->y0=renderPos0.vy;
			L->x1=renderPos1.vx;	L->y1=renderPos1.vy;
			setRGB0(L,R,G,B);		addPrim(ThisOT,L);

// Surround
POLY_F4		*P=GetPrimF4();
			P->x0=renderPos0.vx-LaserWidth;	P->y0=renderPos0.vy-LaserWidth;
			P->x1=renderPos0.vx+LaserWidth;	P->y1=renderPos0.vy+LaserWidth;
			P->x2=renderPos1.vx-LaserWidth;	P->y2=renderPos1.vy-LaserWidth;
			P->x3=renderPos1.vx+LaserWidth;	P->y3=renderPos1.vy+LaserWidth;
			setRGB0(P,R>>1,G>>1,B>>1);	addPrim(ThisOT,P);

//		
int	W=renderPos1.vx-renderPos0.vx;
int	H=renderPos1.vy-renderPos0.vy;
		
		if (W<0) W=-W;
		if (H<0) H=-H;
		setCollisionCentreOffset(W>>1,H>>1);
		setCollisionSize(W,H);

}

