/* ===========================================================================
	File:		GSYS.C

	Notes:		MSVS PC implemtatiom of GSYS.H api

	Author:		G Robert Liddon @ 73b

	Created:	Wednesday 27th March 1996

	Copyright (C) 1996 DCI Ltd All rights reserved. 
  ============================================================================ */


 /* ---------------------------------------------------------------------------
	Includes
	-------- */

/*	Standard Lib
	------------ */
#include "setjmp.h"

/*	Glib
	---- */
#include "gtypes.h"
#include "gsys.h"


/* ---------------------------------------------------------------------------
	Defines
	------- */

/* ---------------------------------------------------------------------------
	Function Prototypes
	------------------- */
static void MyFunc(void);

/* ---------------------------------------------------------------------------
	Defines
	------- */

/* ---------------------------------------------------------------------------
	Exterenal Variables
	------------------- */

/* ---------------------------------------------------------------------------
	Variables
	--------- */

/* ---------------------------------------------------------------------------
	Define the PSX stack
	-------------------- */
extern int		_stacksize;

/* ---------------------------------------------------------------------------
	Tables
	------ */
MEM_INFO WorkMemInfo=
{
	NULL,
	0,
};

/*	---------------------------------------------------------------------------
	Function:	const MEM_INFO *GSYS_GetMemInfo(MEM_ID Id)

	Purpose:	Get description of a free mem type for this system

	Returns:	-> struct containg info

	--------------------------------------------------------------------------- */
const MEM_INFO *GSYS_GetWorkMemInfo(void)
{
	return(&WorkMemInfo);
}	

/*	---------------------------------------------------------------------------
	Function:	void GSYS_SetStackAndJump(void *Stack,void(*Func)(void *),void *Param)

	Purpose:	Set the stack pointer and jump to a routine on PSX

	Params:		Stack ->	New Stack Ptr
				Func ->		Routine to jump to
				Param		Parameter to pass to function

	--------------------------------------------------------------------------- */
static void *	SaveParam;
void			(*SaveFunc)(void *);

void GSYS_SetStackAndJump(void *Stack,void(*Func)(void *),void *Param)
{
	jmp_buf			GazBuff;
	_JUMP_BUFFER *	PcBuff;

	SaveParam=Param;
	SaveFunc=Func;
	PcBuff=(_JUMP_BUFFER *) GazBuff;
	setjmp(GazBuff);
	PcBuff->Eip=(unsigned long)MyFunc;
	PcBuff->Esp=(unsigned long)Stack;
	longjmp(GazBuff,0);

	Func(Param);
}	

void MyFunc(void)
{
	SaveFunc(SaveParam);
}

/*	---------------------------------------------------------------------------
	Function:	void GSYS_MarkStack(void * Stack, U32 StackSize)

	Purpose:	Marks a stack so that it can be checked to see if it's
				been corrupted. In GSYS to account for stack direction
				on different platforms

	Params:		Stack ->	Stack
				StackSize	Stack Size

	--------------------------------------------------------------------------- */

#define STACK_MARK_CODE 0xabcd0123

void GSYS_MarkStack(void * Stack, U32 StackSize)
{
	U32 *	StackStart;

	StackStart=Stack;
	(*StackStart)=STACK_MARK_CODE;
}


/*	---------------------------------------------------------------------------
	Function:	BOOL GSYS_IsStackCorrupted(void * Stack, U32 StackSize)

	Purpose:	Check to see if a previously marked stack has been corrupted

	Params:		Stack ->	Stack
				StackSize	Stack Size

	Returns:	TRUE if it has

	--------------------------------------------------------------------------- */
BOOL GSYS_IsStackCorrupted(void * Stack, U32 StackSize)
{
	U32 *	StackStart;
	StackStart=Stack;

	return (*StackStart!=STACK_MARK_CODE);
}	

/*	---------------------------------------------------------------------------
	Function:	BOOL	GSYS_InitMachine(void)

	Purpose:	Initialise the machine for work
	
	Returns:	Succesful of not

	--------------------------------------------------------------------------- */
BOOL GSYS_InitMachine(void)
{
	return(TRUE);
}	


/*	---------------------------------------------------------------------------
	Function:	BOOL GSYS_CheckPtr(void *Ptr)


	Purpose:	See if this ptr -> to an address within the machines
				memor
	
	Returns:	Succesful of not

	--------------------------------------------------------------------------- */
BOOL GSYS_CheckPtr(void *Ptr)
{
	return(TRUE);
}	

/*	---------------------------------------------------------------------------
	Function:	BOOL GSYS_IsStackOutOfBounds(void* Stack, U32 StackSize)

	Purpose:	Is the current sp outside the range of the stack
	
	Returns:	TRUE if so

	--------------------------------------------------------------------------- */
BOOL GSYS_IsStackOutOfBounds(void* Stack, U32 StackSize)
{
	return(FALSE);
}	

/* ---------------------------------------------------------------------------
	ends */
