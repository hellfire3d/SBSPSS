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

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

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
		if (!canRender() || Flags & FX_FLAG_HIDDEN) return;

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
		
		setCollisionCentreOffset( (W>>1) + Offset.vx, (H>>1) + Offset.vy );
		if (W<0) W=-W;
		if (H<0) H=-H;
		setCollisionSize(W,H);

}

/*****************************************************************************/
/*** checkCollisionAgainst ***************************************************/
/*****************************************************************************/
int		CFXLaser::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
//	DVECTOR	pos,thisThingPos;
	int		radius;
	int		collided;

DVECTOR const	&pos=getCollisionCentre();
DVECTOR const	&thisThingPos=_thisThing->getCollisionCentre();

	radius=getCollisionRadius()+_thisThing->getCollisionRadius();
	collided=false;
	if(abs(pos.vx-thisThingPos.vx)<radius&&
	   abs(pos.vy-thisThingPos.vy)<radius)
	{
//		CRECT	thisRect,thatRect;

CRECT const	&thisRect=getCollisionArea();
CRECT const	&thatRect=_thisThing->getCollisionArea();

		if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
		   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
		{
			// bounding boxes touch, now check for line vs box collision

			s32 w = thatRect.x2 - thatRect.x1;
			s32 h = thatRect.y2 - thatRect.y1;

			s32 a = ( thatRect.x1 + thatRect.x2 ) >> 1;
			s32 b = ( thatRect.y1 + thatRect.y2 ) >> 1;

			s32 x1 = Pos.vx + Offset.vx;
			s32 y1 = Pos.vy + Offset.vy;

			s32 x2 = Target.vx;
			s32 y2 = Target.vy;

			s32 t1, t2, t3, t4;

			if ( x1 == x2 )
			{
				// vertical line
				// bounding boxes already colliding

				collided = true;
			}
			else if ( y1 == y2 )
			{
				// horizontal line
				// bounding boxes already colliding

				collided = true;
			}
			else
			{
				// see http://www.flipcode.com/tpractice/issue01.shtml

				t1 = ( ( ( w >> 1 ) - x1 + a ) << 8 ) / ( x2 - x1 );
				t2 = ( ( -( ( w >> 1 ) + x1 - a ) ) << 8 ) / ( x2 - x1 );

				t3 = ( ( ( h >> 1 ) - y1 + b ) << 8 ) / ( y2 - y1 );
				t4 = ( ( -( ( h >> 1 ) + y1 - b ) ) << 8 ) / ( y2 - y1 );

				if ( t1 > t2 )
				{
					s32 temp = t2;
					t2 = t1;
					t1 = temp;
				}

				if ( t3 > t4 )
				{
					s32 temp = t4;
					t4 = t3;
					t3 = temp;
				}

				if ( t1 < t4 && t3 < t2 )
				{
					collided = true;
				}
			}
		}
	}

	return collided;
}

/*****************************************************************************/
/*** CollidedWith ************************************************************/
/*****************************************************************************/
void	CFXLaser::collidedWith(CThing *_thisThing)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			CPlayer *player = (CPlayer *) _thisThing;

			if ( !player->isRecoveringFromHit() )
			{
				player->takeDamage( DAMAGE__HIT_ENEMY );
			}

			break;
		}

		default:
			break;
	}
}

