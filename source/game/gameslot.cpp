/*=========================================================================

	gameslot.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "game\gameslot.h"

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

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

CGameSlotManager::GameSlot	CGameSlotManager::s_gameSlots[4];
CGameSlotManager::GameSlot	*CGameSlotManager::s_currentGameSlot=0;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameSlotManager::init()
{
	int			i;

	for(i=0;i<NUM_GAME_SLOTS;i++)
	{
		eraseGameSlot(i);
	}

	setActiveSlot(0);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameSlotManager::setActiveSlot(unsigned int _slot)
{
	ASSERT(_slot<=NUM_GAME_SLOTS);
	s_currentGameSlot=&s_gameSlots[_slot];
	s_currentGameSlot->m_isInUse=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CGameSlotManager::GameSlot *CGameSlotManager::getSlotData()
{
	ASSERT(s_currentGameSlot!=0);
	return s_currentGameSlot;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameSlotManager::setSlotData(GameSlot *_data)
{
	ASSERT(s_currentGameSlot!=0);
	*s_currentGameSlot=*_data;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameSlotManager::eraseGameSlot(unsigned int _slot)
{
	ASSERT(_slot<=NUM_GAME_SLOTS);

	GameSlot	*slot;
	int			i,j;

	slot=&s_gameSlots[_slot];

	slot->m_isInUse=false;
	slot->m_lives=INITIAL_LIVES;
	slot->m_continues=INITIAL_CONTINUES;
	slot->m_maxLevelCompleted=0;

	for(i=0;i<NUM_CHAPTERS*NUM_LEVELS_WITH_SPATULAS;i++)
	{
		for(j=0;j<16;j++)
		{
			slot->m_spatulaCollectedFlags[i][j]=0;
		}
	}

	for(i=0;i<NUM_CHAPTERS;i++)
	{
		for(j=0;j<16;j++)
		{
			slot->m_kelpTokenCollectedFlags[i][j]=0;
		}
	}

#ifdef __USER_paul__
	slot->m_kelpTokensHeld=82;
#else
	slot->m_kelpTokensHeld=0;
#endif
	for(i=0;i<CShopScene::NUM_SHOP_ITEM_IDS;i++)
	{
		slot->m_partyItemsHeld[i]=false;
	}

}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameSlotManager::copyGameSlot(unsigned int _src,unsigned int _dest)
{
	ASSERT(_src<=NUM_GAME_SLOTS);
	ASSERT(_dest<=NUM_GAME_SLOTS);

	s_gameSlots[_dest]=s_gameSlots[_src];
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameSlotManager::setSlotData(int _slot,GameSlot *_data)
{
	ASSERT(_slot<=NUM_GAME_SLOTS);
	s_gameSlots[_slot]=*_data;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CGameSlotManager::GameSlot CGameSlotManager::getSlotData(int _slot)
{
	ASSERT(_slot<=NUM_GAME_SLOTS);
	return s_gameSlots[_slot];
}


/*===========================================================================
 end */
