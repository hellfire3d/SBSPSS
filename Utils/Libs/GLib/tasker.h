/* ===========================================================================
	File:		TASKER.H

	Notes:		Cooperative multitasking Header file

	Author:		Gary Liddon @ 73b

	Created:	Wednesday 27th March 1996

	Copyright (C) 1996 - 1997 Gary Liddon
	All rights reserved. 
  ============================================================================ */

#ifndef	__TASKER_HPP_
#define	__TASKER_HPP_

/* ---------------------------------------------------------------------------
	Standard Lib Includes
	--------------------- */
#include <setjmp.h>

/* ---------------------------------------------------------------------------
	Glib Includes
	------------- */
#include "gtypes.h"
#include "gal.h"


/* ---------------------------------------------------------------------------
	Structure Definitions
	--------------------- */
typedef struct TASK
{
	struct TASK *Next;
	struct TASK *Prev;

	U32		Id;
	U32		SleepTime;

	U32		fToInit:1,		/* Has this task been inited		*/
			fToDie:1,		/* Does this task deserve to die!	*/
			fKillable:1,	/* Can this task be killed */
			fActive:1,		/* Is this task active or what */
			fXtraStack:1;

	void *	Stack;
	U32		StackSize;

	void *	Data;

	jmp_buf	TskEnv;

	void (*Main)(struct TASK *T);

	MHANDLE		hndTask;
	u16			XtraLongs;
	u16			MaxStackSizeBytes;

} TASK;


/* ---------------------------------------------------------------------------
	Function Prototypes
	------------------- */

#ifdef __cplusplus
extern "C" {
#endif

GLIB_API BOOL	TSK_OpenModule(U32 MemType);
GLIB_API void	TSK_DoTasks(void);

/*	Task management
	--------------- */
GLIB_API TASK *	TSK_AddTask(U32 Id,void (*Main)(TASK *T),int StackSize,int DataSize);
GLIB_API void	TSK_RepointProc(TASK *T,void (*Func)(TASK *T));
GLIB_API void	TSK_Kill(TASK *T);
GLIB_API int		TSK_KillTasks(TASK * CallingT,U32 Id,U32 Mask);

GLIB_API void	TSK_Sleep(int Frames);
GLIB_API void	TSK_Die(void);

GLIB_API void	TSK_JumpAndResetStack(void (*RunFunc)(TASK *));

GLIB_API void	TSK_ClearExecFilter(void);
GLIB_API void	TSK_SetExecFilter(U32 Id,U32 Mask);

GLIB_API void	TSK_IterateTasks(U32 Id,U32 Mask,void (*CallBack)(TASK *T));

GLIB_API void	TSK_MakeTaskInactive(TASK *T);
GLIB_API void	TSK_MakeTaskActive(TASK *T);
GLIB_API void	TSK_MakeTaskImmortal(TASK *T);
GLIB_API void	TSK_MakeTaskMortal(TASK *T);

/*	Info and reporting
	------------------ */
GLIB_API TASK *	TSK_Exist(TASK *T,U32 Id,U32 Mask);
GLIB_API TASK *	TSK_GetCurrentTask(void);
GLIB_API BOOL	TSK_IsCurrentTask(TASK *T);
GLIB_API TASK *	TSK_GetFirstActive(void);
GLIB_API BOOL	TSK_IsStackCorrupted(TASK *T);

GLIB_API BOOL	TSK_IsTaskActive(TASK *T);
GLIB_API BOOL	TSK_IsTaskMortal(TASK *T);

/*	Debug Stuff
	----------- */
typedef void (*TSK_CBACK)(TASK *T);
typedef void (*DOTSK_CBACK)(void);

GLIB_API DOTSK_CBACK TSK_SetDoTasksEpilogue(DOTSK_CBACK Func);
GLIB_API DOTSK_CBACK TSK_SetDoTasksPrologue(DOTSK_CBACK Func);

GLIB_API TSK_CBACK 	TSK_SetTaskPrologue(TSK_CBACK Pro);
GLIB_API TSK_CBACK 	TSK_SetTaskEpilogue(TSK_CBACK Epi);
GLIB_API void		TSK_SetEpiProFilter(U32 Id,U32 Mask);
GLIB_API void		TSK_ClearEpiProFilter(void);

GLIB_API void		TSK_SetExtraStackProtection(BOOL OnOff);
GLIB_API TSK_CBACK	TSK_SetStackFloodCallback(TSK_CBACK Func);
GLIB_API int			TSK_SetExtraStackSize(int Size);

#ifdef __cplusplus
};
#endif


/* --------------------------------------------------------------------------- */
#endif

/* ---------------------------------------------------------------------------
	ends */
