/*******************/
/*** Fixed Stupg ***/
/*******************/

#ifndef	__DAVES_FIXED__
#define	__DAVES_FIXED__

#include	"system/global.h"

/*****************************************************************************/
typedef	s32							Fixed;

/*****************************************************************************/
#define	FixedShift					16
#define	FixedMultFactor				(1<<FixedShift)
#define	FixedIntMask				(0xffffffff<<FixedShift)
#define	FixedFractMask				((0xffffffff-FixedIntMask)/* | 0x80000000*/)	// save sign bit

#define	FixedInt(Fixed)				(Fixed & FixedIntMask)
#define	FixedFract(Fixed)			(Fixed & FixedFractMask)

#define	Fixed2NumQ(Fixed)			(*((s16*)&Fixed+1))			
#define	Num2Fixed(Num)				((s32)Num*FixedMultFactor)
#define	Fixed2Num(Fixed)			((s16)(Fixed>>FixedShift))			

#define	FixedMult(a,b)				((a*b)>>FixedShift)
#define	FixedMult2(a,b)				((a>>(FixedShift/2))*(b>>(FixedShift/2)))

/*****************************************************************************/
#endif