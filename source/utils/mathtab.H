/*************************/
/*** Math Table Header ***/
/*************************/

#ifndef __MATHTABLE_HEADER__
#define __MATHTABLE_HEADER__

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif


/*****************************************************************************/
extern VECTOR upVec;

//extern const s16 ACosTable[4097];
extern const s16 SinTable[1024];
//extern const s32 OneSinTable[1024];

/*****************************************************************************/
/*
inline s32 macos(const s32 &a)
{
	return(ACosTable[a>>1]);
}
*/
/*****************************************************************************/

inline s32 msin(const s32 &a)
{
	if (a>3071)
	{
		return(-SinTable[1023 - (a&1023)]);
	} else
	if (a>2047)
	{
		return(-SinTable[a&1023]);
	} else
	if (a>1023)
	{
		return(SinTable[1023 - (a&1023)]);
	} else
	{
		return(SinTable[a]);
	}
}

/*****************************************************************************/

inline s32 mcos(const s32 &a)
{
	if (a>3071)
	{
		return(SinTable[(a&1023)]);
	} else
	if (a>2047)
	{
		return(-SinTable[1023 - (a&1023)]);
	} else
	if (a>1023)
	{
		return(-SinTable[a&1023]);
	} else
	{
		return(SinTable[1023 - a]);
	}
}

/*****************************************************************************/
/*
inline s32 monesin(const s32 &a)
{
	if (a>3071)
	{
		return(-OneSinTable[1024 - (a&1023)]);
	} else
	if (a>2047)
	{
		return(-OneSinTable[a&1023]);
	} else
	if (a>1023)
	{
		return(OneSinTable[1024 - (a&1023)]);
	} else
	{
		return(OneSinTable[a]);
	}
}
*/
/*****************************************************************************/
/*
#define msin(x) rsin((x))
#define mcos(x) rcos((x))
#define monesin(x) (((s32)4096<<12)/rsin((x)))
*/
#endif
