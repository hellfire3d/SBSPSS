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
	};
	
	typedef struct
	{
		int		m_lives;
		int		m_continues;
		int		m_maxLevelCompleted;
	} GameSlot;


	static void		init();

	static void		setActiveSlot(unsigned int _slot);
	static GameSlot	getSlotData();
	static void		setSlotData(GameSlot *_data);

	static void		eraseGameSlot(unsigned int _slot);
	static void		copyGameSlot(unsigned int _src,unsigned int _dest);


private:
	static GameSlot	s_gameSlots[NUM_GAME_SLOTS];
	static GameSlot	*s_currentGameSlot;

	// These allow the CSaveLoadDatabase total access to the game slots
	static void		setSlotData(int _slot,GameSlot *_data);
	static GameSlot	getSlotData(int _slot);
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
