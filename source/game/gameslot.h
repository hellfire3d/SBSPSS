/*=========================================================================

	gameslot.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__GAME_GAMESLOT_H__
#define	__GAME_GAMESLOT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SHOP_SHOP_H__
#include "shop\shop.h"
#endif

// Just for ASSERT()..
#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CGameSlotManager
{
public:
	enum
	{
		INITIAL_LIVES=5,
		INITIAL_CONTINUES=3,
		
		NUM_GAME_SLOTS=4,

		NUM_CHAPTERS=5,
		NUM_LEVELS_WITH_SPATULAS=4,
		NUM_LEVELS_PER_CHAPTER_WITH_QUEST_ITEMS=4,
		NUM_BONUS_LEVELS_PER_CHAPTER=1,
	};

	enum
	{
		LEVELCOMPETESTATE_NOT_OPEN,			// Level is not available to play
		LEVELCOMPETESTATE_OPEN,				// Level is available to play
		LEVELCOMPETESTATE_COMPLETED,		// Level has been completed
	};
	

	typedef struct
	{
		unsigned char	m_isInUse;
		signed char		m_lives;
		signed char		m_continues;
		unsigned char	m_hasSeenOpeningFMA;
		unsigned char	m_spatulaCollectedCounts[NUM_CHAPTERS*NUM_LEVELS_WITH_SPATULAS];
		unsigned char	m_kelpTokenCollectedFlags[NUM_CHAPTERS][16];								// Enuf space for 128 tokens per level
		unsigned char	m_kelpTokensHeld;
		unsigned char	m_partyItemsHeld[CShopScene::NUM_SHOP_ITEM_IDS];
		unsigned char	m_levelCompletionState[NUM_CHAPTERS*(NUM_LEVELS_PER_CHAPTER_WITH_QUEST_ITEMS+NUM_BONUS_LEVELS_PER_CHAPTER)];

		// Spat functions..
		int				getSpatulaCollectedCount(unsigned int _chapter,unsigned int _level)
		{
			ASSERT(_chapter<=NUM_CHAPTERS);
			ASSERT(_level<=NUM_LEVELS_WITH_SPATULAS);
			return m_spatulaCollectedCounts[(_chapter*NUM_LEVELS_WITH_SPATULAS)+_level];
		}
		void			setSpatulaCollectedCount(unsigned int _chapter,unsigned int _level,unsigned char _count,unsigned char _totalCount)
		{
			ASSERT(_chapter<=NUM_CHAPTERS);
			ASSERT(_level<=NUM_LEVELS_WITH_SPATULAS);
			m_spatulaCollectedCounts[(_chapter*NUM_LEVELS_WITH_SPATULAS)+_level]=_count;
			
			// Does this open up a bonus level?
			int	percentRequired,percentAchieved;
			percentRequired=(_chapter*10)+60;			// 60% on chapter 1 ... to ... 100% on chapter 5
			percentAchieved=(_count*100)/_totalCount;
			if(percentAchieved>=percentRequired)
			{
				levelIsNowOpen(_chapter,4);
			}
		}

		// Kelp Token functions..
		int				getKelpTokenCollectedCount(unsigned int _chapter,unsigned int _level)
		{
			ASSERT(_chapter<=NUM_CHAPTERS);
			ASSERT(_level==NUM_LEVELS_WITH_SPATULAS);
			int		i,j,count;
			count=0;
			for(i=0;i<8;i++)
			{
				unsigned char	flags=m_kelpTokenCollectedFlags[_chapter][i];
				for(j=0;j<8;j++)
				{
					if(flags&1)count++;
					flags>>=1;
				}
			}
			return count;
		}
		void			collectKelpToken(unsigned int _chapter,unsigned int _level,unsigned int _token)
		{
			ASSERT(_chapter<=NUM_CHAPTERS);
			ASSERT(_level==NUM_LEVELS_WITH_SPATULAS);
			ASSERT(_token<=128);
			m_kelpTokenCollectedFlags[_chapter][_token>>3]|=1<<(_token&7);
			m_kelpTokensHeld++;
		}
		int				isKelpTokenUncollected(unsigned int _chapter,unsigned int _level,unsigned int _token)
		{
			ASSERT(_chapter<=NUM_CHAPTERS);
			ASSERT(_level==NUM_LEVELS_WITH_SPATULAS);
			ASSERT(_token<=128);
			return (m_kelpTokenCollectedFlags[_chapter][_token>>3]>>(_token&7))&1?false:true;
		}

		// Shop
		int				getNumberOfKelpTokensHeld()
		{
			return m_kelpTokensHeld;
		}
		void			useKelpTokens(int _count)
		{
			ASSERT(((int)m_kelpTokensHeld-_count)>=0);
			m_kelpTokensHeld-=_count;
		}
		int				isPartyItemHeld(int _itemNumber)
		{
			return m_partyItemsHeld[_itemNumber];
		}
		void			buyPartyItem(int _itemNumber)
		{
			ASSERT(!m_partyItemsHeld[_itemNumber]);
			m_partyItemsHeld[_itemNumber]=true;
		}

		// Levels
		int				isLevelOpen(unsigned int _chapter,unsigned int _level)
		{
			int	index;
			index=(_chapter*(NUM_LEVELS_PER_CHAPTER_WITH_QUEST_ITEMS+NUM_BONUS_LEVELS_PER_CHAPTER))+_level;
			return m_levelCompletionState[index]!=(int)LEVELCOMPETESTATE_NOT_OPEN;
		}
		int				hasQustItemBeenCollected(unsigned int _chapter,unsigned int _level)
		{
			int	index;
			index=(_chapter*(NUM_LEVELS_PER_CHAPTER_WITH_QUEST_ITEMS+NUM_BONUS_LEVELS_PER_CHAPTER))+_level;
			return m_levelCompletionState[index]==(int)LEVELCOMPETESTATE_COMPLETED;
		}
		void			levelIsNowOpen(unsigned int _chapter,unsigned int _level)
		{
			int	index;
			index=(_chapter*(NUM_LEVELS_PER_CHAPTER_WITH_QUEST_ITEMS+NUM_BONUS_LEVELS_PER_CHAPTER))+_level;
			if(m_levelCompletionState[index]==LEVELCOMPETESTATE_NOT_OPEN)
			{
				m_levelCompletionState[index]=LEVELCOMPETESTATE_OPEN;
			}
		}
		void			levelHasBeenCompleted(unsigned int _chapter,unsigned int _level)
		{
			int	index;
			index=(_chapter*(NUM_LEVELS_PER_CHAPTER_WITH_QUEST_ITEMS+NUM_BONUS_LEVELS_PER_CHAPTER))+_level;
			m_levelCompletionState[index]=LEVELCOMPETESTATE_COMPLETED;
		}

		void			getHighestLevelOpen(int *_chapter,int *_level)
		{
			int	chapter,level;
			for(chapter=0;chapter<NUM_CHAPTERS;chapter++)
			{
				for(level=0;level<NUM_LEVELS_PER_CHAPTER_WITH_QUEST_ITEMS;level++)
				{
					if(isLevelOpen(chapter,level))
					{
						*_chapter=chapter;
						*_level=level;
					}
				}
			}
		}

#ifdef __VERSION_DEBUG__
		void			debugCheatOpenAllLevels()
		{
///*
			for(int i=1;i<NUM_CHAPTERS*(NUM_LEVELS_PER_CHAPTER_WITH_QUEST_ITEMS+NUM_BONUS_LEVELS_PER_CHAPTER);i++)
			{
				m_levelCompletionState[i]=LEVELCOMPETESTATE_OPEN;
			}
//*/
/*
m_levelCompletionState[ 0]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[ 1]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[ 2]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[ 3]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[ 4]=LEVELCOMPETESTATE_OPEN;

m_levelCompletionState[ 5]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[ 6]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[ 7]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[ 8]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[ 9]=LEVELCOMPETESTATE_OPEN;

m_levelCompletionState[10]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[11]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[12]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[13]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[14]=LEVELCOMPETESTATE_OPEN;

m_levelCompletionState[15]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[16]=LEVELCOMPETESTATE_OPEN;
//m_levelCompletionState[17]=LEVELCOMPETESTATE_OPEN;
//m_levelCompletionState[18]=LEVELCOMPETESTATE_OPEN;
m_levelCompletionState[19]=LEVELCOMPETESTATE_OPEN;

//m_levelCompletionState[20]=LEVELCOMPETESTATE_OPEN;
//m_levelCompletionState[21]=LEVELCOMPETESTATE_OPEN;
//m_levelCompletionState[22]=LEVELCOMPETESTATE_OPEN;
//m_levelCompletionState[23]=LEVELCOMPETESTATE_OPEN;
//m_levelCompletionState[24]=LEVELCOMPETESTATE_OPEN;
*/
		}
#endif
	} GameSlot;

	static void			init();

	static void			setActiveSlot(unsigned int _slot);
	static GameSlot		*getSlotData();
	static void			setSlotData(GameSlot *_data);

	static void			eraseGameSlot(unsigned int _slot);
	static void			copyGameSlot(unsigned int _src,unsigned int _dest);

	static int			getNumberOfFrontendScreenToUse();


private:
	static GameSlot		s_gameSlots[NUM_GAME_SLOTS];
	static GameSlot		*s_currentGameSlot;

	// These allow the CSaveLoadDatabase total access to the game slots
	static void			setSlotData(int _slot,GameSlot *_data);
	static GameSlot		getSlotData(int _slot);
	friend class CSaveLoadDatabase;


};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GAME_GAMESLOT_H__ */

/*===========================================================================
 end */
