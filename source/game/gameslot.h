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
		INITIAL_LIVES=4,
		INITIAL_CONTINUES=3,
		
		NUM_GAME_SLOTS=4,

		NUM_CHAPTERS=5,
		NUM_LEVELS=6,
	};
	

	typedef struct
	{
		unsigned char	m_isInUse;
		unsigned char	m_lives;
		unsigned char	m_continues;
		unsigned char	m_maxLevelCompleted;
		unsigned char	m_spatulaCollectedFlags[NUM_CHAPTERS*NUM_LEVELS][8];		// Enuf space for 64 spats per level

		int				getSpatulaCollectedCount(int _chapter,int _level)
		{
			int		i,j,count;
			count=0;
			for(i=0;i<8;i++)
			{
				unsigned char	flags=m_spatulaCollectedFlags[(_chapter*NUM_LEVELS)+_level][i];
				for(j=0;j<8;j++)
				{
					if(flags&1)count++;
					flags>>=1;
				}
			}
			return count;
		}
		void			collectSpatula(int _chapter,int _level,int _spat)
		{
			m_spatulaCollectedFlags[(_chapter*NUM_LEVELS)+_level][_spat>>3]|=1<<(_spat&7);
		}
		int				isSpatulaUncollected(int _chapter,int _level,int _spat)
		{
			return (m_spatulaCollectedFlags[(_chapter*NUM_LEVELS)+_level][_spat>>3]>>(_spat&7))&1?false:true;
		}
	} GameSlot;

	static void			init();

	static void			setActiveSlot(unsigned int _slot);
	static GameSlot		*getSlotData();
	static void			setSlotData(GameSlot *_data);

	static void			eraseGameSlot(unsigned int _slot);
	static void			copyGameSlot(unsigned int _src,unsigned int _dest);


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
