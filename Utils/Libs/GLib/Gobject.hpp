/*=========================================================================

	GOBJECT.HPP

	Author:  Gary Liddon @ Watford
	Created: 4th May 1991
	Purpose: Base object

	Copyright (c) 1991 - 1997 Gary Liddon

===========================================================================*/

#ifndef __PC_GLIB_GOBJECT_HPP__
#define __PC_GLIB_GOBJECT_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <fstream>

/*	Glib
	---- */
#include "gtypes.h"

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
enum
	{
	ERR_FATAL,
	ERR_WARNING,
	ERR_SERIOUS,
	};

enum
	{
	ERM_TOOMANYERRORS,
	};

/*----------------------------------------------------------------------
	Class defintions
	---------------- */
#define ERM_OUTOFMEM ERR_FATAL,"Mem flood line %d,file %s,",__LINE__,__FILE__

/*----------------------------------------------------------------------
	Class defintions
	---------------- */
class GLIB_API GObject
{
public:
	GObject();
	~GObject();


	static void Error(int Etype,char const *e, ...);
	static void Error(int ErrNum);
//	static int GetNumOfGobjs(void){return NumOfGobjs;}
//	static int GetNumOfWarnings(void){return NumOfWarnings;}

protected:	

private:

	static int NumOfWarnings;
	static int NumOfErrors;
	static int MaxErrors;
	static char const * const ErrorText[];
	static int NumOfGobjs;
	static unsigned int BigObjFlags;
};


/*---------------------------------------------------------------------- */

#endif	/* __PC_GLIB_GOBJECT_HPP__ */

/*===========================================================================
 end */


