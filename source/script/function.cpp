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

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif



/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

// For getAmmoCount(), setAmmoCount()
enum
{
	AMMO_BUBBLEWAND,
};

// For isHoldingWeapon()
enum
{
	WEAPON_BUBBLEWAND,
	WEAPON_BALLOON,
};


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
static signed short func_setResponseOptions(unsigned short *_args);
static signed short func_getResponse(unsigned short *_args);
static signed short func_getAmmoCount(unsigned short *_args);
static signed short func_setAmmoCount(unsigned short *_args);
static signed short func_isHoldingWeapon(unsigned short *_args);
static signed short func_giveWeapon(unsigned short *_args);


/*----------------------------------------------------------------------
	Vars
	---- */

static FunctionDef	s_functionDefs[]=
{
	{	func_setCharacterAnimation,		2	},		// characterId,animationId
	{	func_setText,					2	},		// characterId,textId
	{	func_setResponseOptions,		1	},		// optionsId
	{	func_getResponse,				0	},		// 
	{	func_getAmmoCount,				1	},		// ammoId
	{	func_setAmmoCount,				2	},		// ammoId,amount
	{	func_isHoldingWeapon,			1	},		// weaponId
	{	func_giveWeapon,				1	},		// weaponId
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
	Params:		ammoId
	Returns:	ammoCount
  ---------------------------------------------------------------------- */
static signed short func_getAmmoCount(unsigned short *_args)
{
	int	ammo=0;

	switch(_args[0])
	{
		case AMMO_BUBBLEWAND:
			ammo=GameScene.getPlayer()->getBubbleAmmo();
			break;

		default:
			ASSERT(!"BAD AMMO TYPE IN func_getAmmoCount()");
			break;
	}

	return ammo;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	
	Params:		ammoId,amount
	Returns:	0
  ---------------------------------------------------------------------- */
static signed short func_setAmmoCount(unsigned short *_args)
{
	switch(_args[0])
	{
		case AMMO_BUBBLEWAND:
			GameScene.getPlayer()->setBubbleAmmo(_args[1]);
			break;

		default:
			ASSERT(!"BAD AMMO TYPE IN func_setAmmoCount()");
			break;
	}
	return 0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	
	Params:		weaponId
	Returns:	true(1) or false(0)
  ---------------------------------------------------------------------- */
static signed short func_isHoldingWeapon(unsigned short *_args)
{
	int	held=0;

	switch(_args[0])
	{
		case WEAPON_BUBBLEWAND:
			if(GameScene.getPlayer()->isHoldingBubbleWand())
			{
				held=1;
			}
			break;

		case WEAPON_BALLOON:
			if(GameScene.getPlayer()->isHoldingBalloon())
			{
				held=1;
			}
			break;

		default:
			ASSERT(!"BAD WEAPON TYPE IN func_isHoldingWeapon()");
			break;
	}

	return held;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	
	Params:		weaponId
	Returns:	true(1) or false(0)
  ---------------------------------------------------------------------- */
static signed short func_giveWeapon(unsigned short *_args)
{
	switch(_args[0])
	{
		case WEAPON_BUBBLEWAND:
			GameScene.getPlayer()->setMode(PLAYER_MODE_BUBBLE_MIXTURE);
			break;

		case WEAPON_BALLOON:
			GameScene.getPlayer()->setMode(PLAYER_MODE_BALLOON);
			break;

		default:
			ASSERT(!"BAD WEAPON TYPE IN func_giveWeapon()");
			break;
	}

	return 0;
}


/*===========================================================================
 end */
