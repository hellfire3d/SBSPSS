/*=========================================================================

	function.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "script\function.h"

#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

typedef struct
{
	signed short	(*m_func)(unsigned short *_args);
	int				m_argCount;
} FunctionDef;


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

static signed short func_setCharacterExpression(unsigned short *_args);
static signed short func_setText(unsigned short *_args);


/*----------------------------------------------------------------------
	Vars
	---- */

static FunctionDef	s_functionDefs[]=
{
	{	func_setCharacterExpression,	2	},		// character, expression
	{	func_setText,					1	},		// textId
};
static const int	s_numFunctionDefs=sizeof(s_functionDefs)/sizeof(FunctionDef);


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
signed short callFunction(int _functionNumber,int _argCount,unsigned short *_args)
{
	FunctionDef	*fd;

	ASSERT(_functionNumber<s_numFunctionDefs);
	fd=&s_functionDefs[_functionNumber];
	ASSERT(_argCount==fd->m_argCount);
	return fd->m_func(_args);
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static signed short func_setCharacterExpression(unsigned short *_args)
{
	return _args[0];
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static signed short func_setText(unsigned short *_args)
{
	return _args[0];
}


/*===========================================================================
 end */
