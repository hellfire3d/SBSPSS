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

struct FunctionDef
{
	int		m_functionNumber;
	char	*m_name;
	int		m_argCount;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

extern FunctionDef *lookupFunctionName(char *_name);

extern int getFunctionArgCount(int _functionNumber);

extern CTreeNode *getFunctionArgs(int _argCount);


/*---------------------------------------------------------------------- */

#endif	/* __FUNCTION_H__ */

/*===========================================================================
 end */
