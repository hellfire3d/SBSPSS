/******************/
/*** Misc Utils ***/
/******************/

#include "system\global.h"
#include "utils\utils.h"
#include "gfx\Prim.h"

#ifndef __VID_HEADER_
#include "system/vid.h"
#endif

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif


/*****************************************************************************/

const s16 c_circle[ NO_SIN ] =
{
	0,25,49,74,97,120,142,162,181,197,212,225,236,244,251,254,
	255,254,251,244,236,225,212,197,181,162,142,120,97,74,49,25,
	0,-25,-49,-74,-97,-120,-142,-162,-181,-197,-212,-225,-236,-244,-251,-254,
	-255,-254,-251,-244,-236,-225,-212,-197,-181,-162,-142,-120,-97,-74,-49,-25
};

/*****************************************************************************/
const MATRIX IdentityMtx =
{
	{{ONE,   0,  0},
	{   0,ONE,   0},
	{   0,   0,ONE}},
	{   0,   0,  0},
};

/*****************************************************************************/

const u32 NOTNEG = ((u32)1<<(u32)31) | ((u32)1<<(u32)15);
long	s_randomSeed;

/*****************************************************************************/
u8		*MakePtr(void *BasePtr,int Offset)	
{
u8		*Ret;
		Ret=(u8*)BasePtr+(Offset);
		return(Ret);
}

/*****************************************************************************/
void	RotateBox(sBox *B,int W,int H,int _xScale,int _yScale,int _rot)
{
int		halfW,halfH;
int		ca,sa;
int		cw,ch,sw,sh;
int		width,height;
int		aspect;

	halfW=(W*_xScale)>>(12+1);
	halfH=(H*_yScale)>>(12+1);
	ca=mcos(_rot);
	sa=msin(_rot);
	cw=(ca*halfW)>>12;
	ch=(ca*halfH)>>12;
	sw=(sa*halfW)>>12;
	sh=(sa*halfH)>>12;

//	aspect=(512<<12)/384;//((_fh->H<<12)/_fh->W)/2;
//	ch=(ch*aspect)>>12;
//	sh=(sh*aspect)>>12;

	B->x0=-cw+sh;	B->y0=-sw-ch;
	B->x1=+cw+sh;	B->y1=+sw-ch;
	B->x2=-cw-sh;	B->y2=-sw+ch;
	B->x3=+cw-sh;	B->y3=+sw+ch;

}
