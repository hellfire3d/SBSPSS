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

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
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

static signed short func_setCharacterAnimation(unsigned short *_args);
static signed short func_setText(unsigned short *_args);
static signed short func_drawSprite(unsigned short *_args);
static signed short func_getFrameTime(unsigned short *_args);


/*----------------------------------------------------------------------
	Vars
	---- */

static FunctionDef	s_functionDefs[]=
{
	{	func_setCharacterAnimation,		2	},		// character,animation
	{	func_setText,					1	},		// textId
	{	func_drawSprite,				4	},		// frame,x,y,ot
	{	func_getFrameTime,				0	},		//
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
	Params:		character,animation
	Returns:
  ---------------------------------------------------------------------- */
static signed short func_setCharacterAnimation(unsigned short *_args)
{
	return _args[0];
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		textid
	Returns:
  ---------------------------------------------------------------------- */
static signed short func_setText(unsigned short *_args)
{
	return _args[0];
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		frame,x,y,ot
	Returns:
  ---------------------------------------------------------------------- */
#include "gfx\sprbank.h"
SpriteBank *sb=NULL;
static signed short func_drawSprite(unsigned short *_args)
{
	sFrameHdr	*fh;
	if(!sb)
	{
		sb=new ("sb") SpriteBank;
		sb->load(UI_UIGFX_SPR);
	}
	fh=sb->getFrameHeader(_args[0]);
	sb->printFT4(_args[0],_args[1]-(fh->W/2),_args[2]-(fh->H/2),0,0,_args[3]);
	return 0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		textid
	Returns:
  ---------------------------------------------------------------------- */
static signed short func_getFrameTime(unsigned short *_args)
{
	return GameState::getFramesSinceLast();
}


/*===========================================================================
 end */
