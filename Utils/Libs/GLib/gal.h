/* ==========================================================================
	File:		GAL.H

	Notes:		Memory allocation \ deallocation module header file

	Author:		Gary Liddon

	Copyright (C) 1995 - 1997 Gary Liddon
	All rights reserved. 
  =========================================================================== */

#ifndef __GAL_H__
#define __GAL_H__

/* ---------------------------------------------------------------------------
	Includes
	-------- */
#include "gtypes.h"

/* ---------------------------------------------------------------------------
	Defines
	------- */
#define NULL_HANDLE			-1
#define PHANTOM_MEM			-1

/* ---------------------------------------------------------------------------
	Typedefs
	-------- */
typedef S32 MHANDLE;
typedef int MTYPE;
typedef void (*GAL_FILTER)(U32 MemType,U32 Size,const char *Name);

/* ---------------------------------------------------------------------------
	Enums
	----- */
enum
{
	GAL_PHANTOM_MEM=0,
	GAL_FIRST_FREE_MEM_TYPE,
	GAL_HIGH	=	1<<15,
	GAL_FLAGS =		GAL_HIGH
};

typedef enum GAL_ERROR_CODE
{
	ERR_GAL_NO_ERROR = 0,
	ERR_RUN_OUT_OF_MEM_HDRS,
	ERR_GAL_MEM_TYPE_EXISTS,
	ERR_GAL_MEM_TYPE_OVERLAP,
	ERR_GAL_INVALID_MEM_TYPE,
	ERR_GAL_INVALID_MEM_HANDLE,
	ERR_GAL_MEM_ALREADY_UNLOCKED,
	ERR_GAL_MEM_BLOCK_COLLISION,
	ERR_GAL_MEM_AREA_NOT_COVERED,
	ERR_GAL_NO_MEM_MOVE,
	ERR_GAL_NOT_ENOUGH_MEM,
	NUM_OF_ERROR_MESSAGES
	
}GAL_ERROR_CODE;

typedef enum GAL_VERB_LEV
{
	GAL_SILENT=0,
	GAL_AVERAGE,
	GAL_NOISY

} GAL_VERB_LEV;

/* ---------------------------------------------------------------------------
	Structures
	---------- */

/*	Pre declaration of Header each block of free / used memory has
	-------------------------------------------------------------- */
struct MEM_HDR;

/*	Memory Initialisation module Structure
	Must be placed in RAM
	-------------------------------------- */
typedef struct MEM_INIT_INFO
{
	void *	Mem;																							/* Start of memory region	*/
	U32		Size;																							/* Size of memory region	*/
	U32		Type;																							/* Type of memory region	*/
	char *	TypeString;											/* Debugging string			*/
	U16		Alignment;											/* Alignment required		*/
	void	(*MemMove)(void *Dest,void *Source,U32 size);		/* Memory move vector		*/

	/* Used by System, Leave uninitialised */

	struct MEM_INIT_INFO * NextInitBlock;

	U16					Flags;

	struct MEM_HDR *	Empty;			/* Pointer to free MEM_HDRs of this mem type	*/
	struct MEM_HDR *	Used;			/* Pointer to used MEM_HDRs of this mem type	*/

} MEM_INIT_INFO;


/*	Structure used by GAL_AllocMultiStruct
	-------------------------------------- */
typedef struct GAL_STRUCT
{
	int		OriginalSize;
	UINT	Offset;

} GAL_STRUCT;

/* ---------------------------------------------------------------------------
	Supported Functions
	------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/*	Initialisation Functions
	------------------------ */
void		GAL_InitModule(void);	


/*	Mem allocing / deallocing functions
	----------------------------------- */
GLIB_API MHANDLE	GAL_Alloc(U32 Size,U32 Type,const char *Name);
GLIB_API MHANDLE	GAL_AllocAt(U32 Size,void *Addr,U32 Type,const char *Name);
GLIB_API BOOL		GAL_Free(MHANDLE Handle);
GLIB_API void *		GAL_Lock(MHANDLE Handle);
GLIB_API BOOL		GAL_Unlock(MHANDLE Handle);



/*
	Purpose:	Split a Block into two. If Split point outside
				block to be split then this will return an error.
				Split is rounded rounded up to be on mem type alignment
				boundary
	Returns:	Handle of second block else
				NULL_HANDLE if unsucessful
*/

GLIB_API MHANDLE	GAL_SplitBlock(MHANDLE CurBlock,U32 Split);




GLIB_API BOOL		GAL_SetMemName(MHANDLE Hnd,const char *Text);
GLIB_API MHANDLE	GAL_AllocMultiStruct(GAL_STRUCT * G,U32 Type,const char *Name);
GLIB_API UINT		GAL_ProcessMultiStruct(GAL_STRUCT * G,U32 Type);
GLIB_API UINT		GAL_GetMemSize(MHANDLE hnd);

/*	Mem type Functions
	------------------ */
GLIB_API BOOL		GAL_DefragMem(U32 type);
GLIB_API BOOL		GAL_AddMemType(MEM_INIT_INFO *M);
GLIB_API U32		GAL_TotalMem(U32 Type);
GLIB_API void *		GAL_MemBase(U32 Type);
GLIB_API U32		GAL_GetFreeMem(U32 Type);
GLIB_API U32		GAL_GetUsedMem(U32 Type);
GLIB_API U32		GAL_LargestFreeBlock(U32 Type);
GLIB_API S32		GAL_AlignSizeToType(U32 Size,U32 MemType);
GLIB_API u32		GAL_GetAlignment(u32 MemType);


/*	Error Functions
	--------------- */
GLIB_API GAL_ERROR_CODE	GAL_GetLastErrorCode(void);
GLIB_API char *			GAL_GetLastErrorText(void);
GLIB_API char *			GAL_GetErrorText(GAL_ERROR_CODE Err);
GLIB_API void			GAL_MemDump(U32 Type);
GLIB_API void			GAL_HaltOnError(void);
GLIB_API void			GAL_ReturnOnError(void);

/*	Debug amd diagnostic functions
	------------------------------ */
GLIB_API s32			GAL_GetSize(MHANDLE hnd);		

/*	Debug amd diagnostic functions
	------------------------------ */
GLIB_API BOOL		GAL_CheckMem(U32 Type);
GLIB_API GAL_FILTER	GAL_SetAllocFilter(GAL_FILTER NewFilter);
GLIB_API void		GAL_IterateUsedMem(U32 MemType,void (*Func)(MHANDLE hnd,void *Addr,U32 Size,const char *Name,int Users,int TimeStamp));
GLIB_API void		GAL_IterateEmptyMem(U32 MemType,void (*Func)(void *Addr,U32 Size,const char *Name));
GLIB_API int		GAL_HowManyUsedRegions(U32 Type);
GLIB_API int		GAL_HowManyEmptyRegions(U32 Type);

GLIB_API int		GAL_GetTimeStamp(void);
GLIB_API void		GAL_IncTimeStamp(void);
GLIB_API void		GAL_SetTimeStamp(int Time);
GLIB_API void		GAL_SetErrorChecking(BOOL OnOff);
GLIB_API void		GAL_SetVerbosity(GAL_VERB_LEV G);
GLIB_API int		GAL_GetNumFreeHeaders(void);
GLIB_API u32		GAL_GetLastTypeAlloced(void);

GLIB_API BOOL		GAL_SortUsedRegionsBySize(U32 Type);
GLIB_API BOOL		GAL_SortUsedRegionsByAddress(U32 MemType);

/* ---------------------------------------------------------------------------
	API'd but As Yet Unsupported
	---------------------------- */
GLIB_API BOOL		GAL_Resize(MHANDLE Handle);


#ifdef __cplusplus
};
#endif

/* --------------------------------------------------------------------------- */
#endif
/* ---------------------------------------------------------------------------
	ends */
