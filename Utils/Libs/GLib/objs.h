/* ===========================================================================
	File:			OBJS.H

	Notes:			Genric Object Server

	Author:			G Robert Liddon @ The Park

	Created:		Wednesday 5th April, 1995

	Copyright (C) 1995 - 1997 Gary Liddon
	All rights reserved. 
  ============================================================================ */

#ifndef __GAZ_OBJS_H
#define __GAZ_OBJS_H

/* ---------------------------------------------------------------------------
	Includes
	-------- */

/*	Glib
	---- */
#include "gtypes.h"
#include "gal.h"

/* ---------------------------------------------------------------------------
	Typedefs, Defines & Structs
	--------------------------- */

struct OBJ_LIST;
struct OBJ_STRUCT;

/* Information about a type of object
	---------------------------------- */
typedef struct OBJ_TYPE_INFO
{
	void	(*Constructor)(struct OBJ_STRUCT *O,void *Ptr);				/*	Object constructor routine	*/
	void	(*Destructor)(struct OBJ_STRUCT *O);						/*	Object destructor routine	*/
	void	(*Printer)(struct OBJ_STRUCT *O,struct OBJ_LIST *OL);		/*	Individual object printer	*/
	int		(*GetWidth)(struct OBJ_STRUCT *O);							/*	Get this obj width			*/
	int		(*GetHeight)(struct OBJ_STRUCT *O);							/*	Get this obj height			*/
	int		(*GetXOff)(struct OBJ_STRUCT *O);							/*	Get this obj x				*/
	int		(*GetYOff)(struct OBJ_STRUCT *O);							/*	Get this obj y				*/
	int		(*GetPal)(struct OBJ_STRUCT *O);							/*	Get this obj pal			*/

}	OBJ_TYPE_INFO;

/*	A List of objects
	----------------- */
typedef struct OBJ_LIST
{
	U32		PrintDepth;
	BOOL	Visible;
	BOOL	Killable;

	char *	ListName;

	struct OBJ_LIST *Prev;
	struct OBJ_LIST *Next;

	S32 X,Y,Z;
	struct OBJ_STRUCT *Head;

	BOOL (*SortCompare)(struct OBJ_STRUCT *O1,struct OBJ_STRUCT *O2);

} OBJ_LIST;


/*	An Object itself
	---------------- */
typedef struct OBJ_STRUCT
{
	struct OBJ_STRUCT *Next;
	struct OBJ_STRUCT *Prev;

	U32 ID;

	S32 XPos,YPos,ZPos;
	S32 XVel,YVel,ZVel;

	const OBJ_TYPE_INFO *	OTI;			/* Object type info */
	OBJ_LIST *				OL;				/* Obj list we're currently in */

	void	*				Data;

	MHANDLE					MemHandle;

} OBJ_STRUCT;


/* ---------------------------------------------------------------------------
	Globals Functions
	----------------- */
#ifdef __cplusplus
extern "C" {
#endif

/*	Module Management
	----------------- */
GLIB_API BOOL	OBJS_OpenModule(U32 MemType);
GLIB_API BOOL	OBJS_CloseModule(void);

GLIB_API void	OBJS_ProcessObjs(void);

GLIB_API void	OBJS_ProcessObjsPrologue(void (*Func)(void));
GLIB_API void	OBJS_ProcessObjsEpilogue(void (*Func)(void));

GLIB_API void	OBJS_AddDisplayList(OBJ_LIST *OL);

GLIB_API BOOL	OBJS_AddVels(void);
GLIB_API BOOL	OBJS_NoAddVels(void);
GLIB_API BOOL	OBJS_GetVelState(void);
GLIB_API BOOL	OBJS_SetVelState(BOOL NewState);

/*	Object Management
	----------------- */
GLIB_API OBJ_STRUCT *	OBJS_CreateObj(const OBJ_TYPE_INFO *OTI,OBJ_LIST *OL,int DataSize,void *Ptr);
GLIB_API OBJ_STRUCT *	OBJS_CreateObjMulti(const OBJ_TYPE_INFO *OTI,OBJ_LIST *OL,GAL_STRUCT * G,void *Ptr);
GLIB_API void			OBJS_ChangeDisplayList(OBJ_STRUCT *O,OBJ_LIST *NewList);
GLIB_API OBJ_STRUCT *	OBJS_DestroyObj(OBJ_STRUCT *O);

GLIB_API void	OBJS_DestroyAllObjs(void);
GLIB_API void	OBJS_DestroyListObjs(OBJ_LIST *OL);
GLIB_API void	OBJS_DestroyAllObjsOfAType(U32 Type,U32 AndMask);
GLIB_API void	OBJS_DestroyListObjsOfAType(OBJ_LIST *OL,U32 Type,U32 AndMask);

GLIB_API void	OBJS_IterateAllObjs(U32 Type,U32 AndMask,void (*CallBack)(OBJ_STRUCT *O));
GLIB_API void	OBJS_IterateListObjs(OBJ_LIST *OL,U32 Type,U32 AndMask,void (*CallBack)(OBJ_STRUCT *O));

/*	List Management
	--------------- */
GLIB_API void	OBJS_SetListXY(OBJ_LIST *OL,S32 X,S32 Y);
GLIB_API void	OBJS_SetListX(OBJ_LIST *OL,S32 X);
GLIB_API void	OBJS_SetListY(OBJ_LIST *OL,S32 Y);
GLIB_API void	OBJS_SetListZ(OBJ_LIST *OL,S32 Z);
GLIB_API void	OBJS_MakeListImmortal(OBJ_LIST *OL);
GLIB_API void	OBJS_MakeListMortal(OBJ_LIST *OL);
GLIB_API BOOL	OBJS_MakeListInvisible(OBJ_LIST *OL);
GLIB_API BOOL	OBJS_MakeListVisible(OBJ_LIST *OL);
GLIB_API void	OBJS_SetSortCompare(OBJ_LIST *OL,BOOL (*CompRout)(OBJ_STRUCT *O1,OBJ_STRUCT *O2));

GLIB_API int		OBJS_GetHeight(OBJ_STRUCT *O);
GLIB_API int		OBJS_GetWidth(OBJ_STRUCT *O);
GLIB_API int		OBJS_GetPal(OBJ_STRUCT *O);
GLIB_API int		OBJS_GetYOff(OBJ_STRUCT *O);
GLIB_API int		OBJS_GetXOff(OBJ_STRUCT *O);

GLIB_API int		OBJS_GetObjsAlloced(void);
GLIB_API void	OBJS_ObjsVelAddOnly(void);

#ifdef __cplusplus
};
#endif


/* --------------------------------------------------------------------------- */
#endif
