/* ===========================================================================
	File:		GTIMER.C

	Notes:		Timing Stuff

	Author:		G Robert Liddon @ 73b

	Copyright (C) 1996 DCI Ltd All rights reserved. 
  ============================================================================ */

/* ---------------------------------------------------------------------------
	Standard Lib Includes
	--------------------- */

/*	Standard Lib
	------------ */

/*	Glib
	---- */
#include "gal.h"

/*	Headers
	------- */
#include "gtimer.h"
#include "gtimsys.h"

/* ---------------------------------------------------------------------------
	Defines and Enums
	----------------- */

/* ---------------------------------------------------------------------------
	Structs
	------- */

/* ---------------------------------------------------------------------------
	Vars
	---- */
MHANDLE		hndClocks;
CLOCK *		Clocks;
int			ClocksOut;
int			ClocksInAll;
U32			TimeForAFrame;
U32			ReadTime;

/* ---------------------------------------------------------------------------
	Function Prototypes
	------------------- */
static void		StartClock(CLOCK * C,U32 Id);
static void		StopClock(CLOCK * C);
static void		ReadTestFunc(void);

static U32 GetTimer(void)
{
	return((GTIMSYS_GetTimer()*0x10000)/TimeForAFrame);
}

/*	---------------------------------------------------------------------------
	Function:	BOOL GTIM_Open(int MaxClocks,U32 RamId);

	Purpose:	Creates the timer moduler

	Params:		MaxClocks	=	Max amount of clocks you can have

	Returns:	FALSE if an error

	--------------------------------------------------------------------------- */
BOOL GTIM_Open(int MaxClocks,U32 RamId)
{
	hndClocks=GAL_Alloc(sizeof(CLOCK)*MaxClocks,RamId,NULL);

	if (hndClocks != NULL_HANDLE)
		{
		Clocks=GAL_Lock(hndClocks);

		if (Clocks)
			{
			ClocksInAll=MaxClocks;
			TimeForAFrame=GTIMSYS_InitTimer();
			GTIM_ClearClocks();

			ReadTime=GTIM_TimeFunction(ReadTestFunc,5000);

			return(TRUE);
			}
		}

	return(FALSE);
}

/*	---------------------------------------------------------------------------
	Function:	void GTIM_ClearClocks(void)

	Purpose:	Reset all clocks

	--------------------------------------------------------------------------- */
void GTIM_ClearClocks(void)
{
	ClocksOut=0;
	GTIMSYS_ResetTimer();
}

/*	---------------------------------------------------------------------------
	Function:	GTHANDLE GTIM_StartClock(U32 Id)

	Purpose:	Start a clock

	Params:		Id for a clock

	Returns:	Handle used to stop the clock

	--------------------------------------------------------------------------- */
GTHANDLE GTIM_StartClock(U32 Id)
{
	S32	RetHandle;

	if (ClocksOut==ClocksInAll)
		RetHandle=NULL_GTHANDLE;
	else
		{
		RetHandle=ClocksOut;

		ClocksOut++;

		StartClock(&Clocks[RetHandle],Id);
		}

	return(RetHandle);
}

/*	---------------------------------------------------------------------------
	Function:	void GTIM_StopClock(GTHANDLE Hnd)

	Purpose:	Stop a clock

	Params:		Id for a clock to stop

	--------------------------------------------------------------------------- */
U32 GTIM_StopClock(GTHANDLE Hnd)
{
	CLOCK *	C;

	C=&Clocks[Hnd];
	StopClock(C);
	return(C->StopTime-C->StartTime);
}

/*	---------------------------------------------------------------------------
	Function:	void GTIM_IterateClocks(void (*Func)(U32 Start,U32 End,U32 Id)

	Purpose:	Callback through all the current clocks

	Params:		Id for a clock to stop

	--------------------------------------------------------------------------- */
void GTIM_IterateClocks(void (*Func)(U32 Start,U32 End,U32 Id))
{
	int		f;
	CLOCK *	C;

	for (f=0,C=Clocks;f<ClocksOut;f++,C++)
		Func(C->StartTime,C->StopTime,C->Id);
}


/*	---------------------------------------------------------------------------
	Function:	U32 GTIM_TimeFunction(void (*Func)(void),int Tries)

	Purpose:	Test this functions speed

	Params:		Func ->		Function to test
				Tries =		Number of tries to average it out against

	Returns:	Time taken

	--------------------------------------------------------------------------- */
U32 GTIM_TimeFunction(void (*Func)(void),int Tries)
{
	U32		Time0;
	U32		Time1;
	int		f;

	Time0=GetTimer();

	for (f=0;f<Tries;f++)
		Func();

	Time1=GetTimer();

	return ((Time1-Time0)/Tries);
}


/*	---------------------------------------------------------------------------
	Helpy routines
	--------------------------------------------------------------------------- */
static void StartClock(CLOCK * C,U32 Id)
{
	int		RealTime;

	RealTime=GetTimer();

	C->StartTime=RealTime;
	C->Id=Id;
	C->Running=TRUE;
}

static void StopClock(CLOCK * C)
{
	int		RealTime;

	RealTime=GetTimer();
	C->StopTime=RealTime;
	C->Running=FALSE;
}

static void ReadTestFunc(void)
{
	GTHANDLE	GtHnd0;

	GtHnd0=GTIM_StartClock(0);
	GTIM_StopClock(GtHnd0);
}

/* ---------------------------------------------------------------------------
	ends */
