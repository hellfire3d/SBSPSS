/*
//text to speech hooks
question/response
//give item
*/
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

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
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
static signed short func_giveItem(unsigned short *_args);
static signed short func_gotItem(unsigned short *_args);
static signed short func_setResponseOptions(unsigned short *_args);
static signed short func_getResponse(unsigned short *_args);

static signed short func_drawSprite(unsigned short *_args);
static signed short func_getFrameTime(unsigned short *_args);


/*----------------------------------------------------------------------
	Vars
	---- */

static FunctionDef	s_functionDefs[]=
{
	{	func_setCharacterAnimation,		2	},		// characterId,animationId
	{	func_setText,					2	},		// characterId,textId
	{	func_giveItem,					1	},		// itemId
	{	func_gotItem,					1	},		// itemId
	{	func_setResponseOptions,		1	},		// optionsId
	{	func_getResponse,				0	},		// 

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
	Purpose:	Set characters animation state
	Params:		characterId,animationId
	Returns:	0
  ---------------------------------------------------------------------- */
static signed short func_setCharacterAnimation(unsigned short *_args)
{
	return 0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Set text/speech/character for ingame texts
	Params:		characterId, textId
	Returns:	0
  ---------------------------------------------------------------------- */
static signed short func_setText(unsigned short *_args)
{
	CConversation::setCharacterAndText(_args[0],_args[1]);
	return 0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Flag item as collected
	Params:		itemId
	Returns:	0
  ---------------------------------------------------------------------- */
static signed short func_giveItem(unsigned short *_args)
{
	return 0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Test whether an item has been collected or not
	Params:		itemId
	Returns:	true/false
  ---------------------------------------------------------------------- */
static signed short func_gotItem(unsigned short *_args)
{

	return false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Sets the allowable responses for a question
	Params:		optionsId
	Returns:	0
  ---------------------------------------------------------------------- */
static signed short func_setResponseOptions(unsigned short *_args)
{
	CConversation::setResponseOptions(_args[0]);
	return 0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Gets the response from a question
	Params:		optionsId
	Returns:	0
  ---------------------------------------------------------------------- */
static signed short func_getResponse(unsigned short *_args)
{
	return CConversation::getResponse();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		frame,x,y,ot
	Returns:	0
  ---------------------------------------------------------------------- */
#include "gfx\sprbank.h"
SpriteBank *sb=NULL;
static signed short func_drawSprite(unsigned short *_args)
{
	sFrameHdr	*fh;
	if(!sb)
	{
		sb=new ("sb") SpriteBank;
		sb->load(INGAMEFX_INGAMEFX_SPR);
	}
	fh=sb->getFrameHeader(_args[0]);
	sb->printFT4(_args[0],_args[1]-(fh->W/2),_args[2]-(fh->H/2),0,0,_args[3]);
	return 0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		0
	Returns:	frameCount
  ---------------------------------------------------------------------- */
static signed short func_getFrameTime(unsigned short *_args)
{
	return GameState::getFramesSinceLast();
}


/*===========================================================================
 end */
