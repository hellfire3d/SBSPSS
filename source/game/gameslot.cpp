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

// Level data include files:
#include <CHAPTER01_LEVEL01_INF.h>
#include <CHAPTER01_LEVEL02_INF.h>
#include <CHAPTER01_LEVEL03_INF.h>
#include <CHAPTER01_LEVEL04_INF.h>
#include <CHAPTER02_LEVEL01_INF.h>
#include <CHAPTER02_LEVEL02_INF.h>
#include <CHAPTER02_LEVEL03_INF.h>
#include <CHAPTER02_LEVEL04_INF.h>
#include <CHAPTER03_LEVEL01_INF.h>
#include <CHAPTER03_LEVEL02_INF.h>
#include <CHAPTER03_LEVEL03_INF.h>
#include <CHAPTER03_LEVEL04_INF.h>
#include <CHAPTER04_LEVEL01_INF.h>
#include <CHAPTER04_LEVEL02_INF.h>
#include <CHAPTER04_LEVEL03_INF.h>
#include <CHAPTER04_LEVEL04_INF.h>
#include <CHAPTER05_LEVEL01_INF.h>
#include <CHAPTER05_LEVEL02_INF.h>
#include <CHAPTER05_LEVEL03_INF.h>
#include <CHAPTER05_LEVEL04_INF.h>
#include <CHAPTER06_LEVEL01_INF.h>
#include <CHAPTER06_LEVEL02_INF.h>
#include <CHAPTER06_LEVEL03_INF.h>
#include <CHAPTER06_LEVEL04_INF.h>
#include <CHAPTER06_LEVEL05_INF.h>


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
	slot->m_hasSeenOpeningFMA=false;


	// Clear spatula and kelp token flags
	for(i=0;i<NUM_CHAPTERS*NUM_LEVELS_WITH_SPATULAS;i++)
	{
		slot->m_spatulaCollectedCounts[i]=0;
	}
	for(i=0;i<NUM_CHAPTERS;i++)
	{
		for(j=0;j<16;j++)
		{
			slot->m_kelpTokenCollectedFlags[i][j]=0;
		}
	}

#ifdef __USER_paul__
	slot->m_kelpTokensHeld=125;
#else
	slot->m_kelpTokensHeld=0;
#endif

	// No party items held yet
	for(i=0;i<CShopScene::NUM_SHOP_ITEM_IDS;i++)
	{
		slot->m_partyItemsHeld[i]=false;
	}

	// Mark all levels except first as NOT_OPEN
	// On a non-cd build, everything starts as open
	slot->m_levelCompletionState[0]=LEVELCOMPETESTATE_OPEN;
	for(i=1;i<NUM_CHAPTERS*(NUM_LEVELS_PER_CHAPTER_WITH_QUEST_ITEMS+NUM_BONUS_LEVELS_PER_CHAPTER);i++)
	{
		slot->m_levelCompletionState[i]=LEVELCOMPETESTATE_NOT_OPEN;
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


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:	0..3
  ---------------------------------------------------------------------- */
int CGameSlotManager::getNumberOfFrontendScreenToUse()
{
	int			bestSoFar;
	GameSlot	*slot;
	int			i;

	bestSoFar=0;
	slot=s_gameSlots;
	for(i=0;i<NUM_GAME_SLOTS&&bestSoFar<3;i++)
	{
		int bestForThisSlot=0;

		if(slot->m_isInUse)
		{
			// All normal levels completed? ( screen 1 )
			if(slot->hasQustItemBeenCollected(5-1,4-1))
			{
				bestForThisSlot=1;

				// ..and bought all party items? ( screen 2 )
				int j;
				for(j=CShopScene::NUM_SHOP_ITEM_IDS;j>0&&slot->isPartyItemHeld(j);j--);
				if(j==0)
				{
					bestForThisSlot=2;

					// ..and collected all spats? ( screen 3 )
					int	count,c,l;
					count=0;
					for(c=0;c<5;c++)
					{
						for(l=0;l<4;l++)
						{
							count+=slot->getSpatulaCollectedCount(c,l);
						}
					}
					if(count==CHAPTER01_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER01_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER01_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER01_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER02_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER02_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER02_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER02_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER03_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER03_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER03_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER03_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER04_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER04_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER04_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER04_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER05_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER05_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER05_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA+
							  CHAPTER05_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA)
					{
						bestForThisSlot=3;
					}
				}
			}

			// Is this slot better than the others?
			if(bestForThisSlot>bestSoFar)
			{
				bestSoFar=bestForThisSlot;
			}
		}

		slot++;
	}

	return bestSoFar;
}


/*===========================================================================
 end */
