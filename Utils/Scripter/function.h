/*=========================================================================

	function.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FUNCTION_H__
#define	__FUNCTION_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __CODEGEN_H__
#include "codegen.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

extern int lookupFunctionName(char *_name);

extern int getFunctionArgCount(int _functionNumber);

extern CTreeNode *getFunctionArgs(int _functionNumber);


/*---------------------------------------------------------------------- */

#endif	/* __FUNCTION_H__ */

/*===========================================================================
 end */
