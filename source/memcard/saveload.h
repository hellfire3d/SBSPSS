/*=========================================================================

	saveload.h

	Author:		PKG
	Created:
	Project:	SBSP
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __MEMCARD_SAVELOAD_H__
#define __MEMCARD_SAVELOAD_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

struct SaveLoad_SystemDetails
{
	char						m_screenOffX,m_screenOffY;
	unsigned char				m_volumes[CSoundMediator::NUM_VOLUMETYPESTOSAVE+1];
	unsigned char				m_controlStyle,m_vibrationIsTurnedOn;
	unsigned char				m_language;
};

struct SaveLoad_DataBuffer
{
	unsigned char				m_headerId;
	SaveLoad_SystemDetails		m_systemDetails;
	CGameSlotManager::GameSlot	m_gameSlots[CGameSlotManager::NUM_GAME_SLOTS];
};


class CSaveLoadDatabase
{
public:
	enum
	{
		INACTIVE,
		IN_PROGRESS,
		FINISHED_OK,
		FAILED,
	} STATUS;


			CSaveLoadDatabase();
			~CSaveLoadDatabase();

	void	think();

	void	gatherData();

	bool	startSave(char *_filename,int _fileNum=-1);
	int		getSaveStatus();
	bool	startLoad(int _file);
	int		getLoadStatus();
	bool	startFormat();
	int		getFormatStatus();

	void	startAutoload();
	bool	monitorAutoload();


private:
	enum
	{
		MC_HEADER_SIZE=256,
	};

	enum
	{
		SAVELOAD_HEADERID=5,
	};


	void	createData();
	void	restoreData(int _loadSettingsOnly);

	void	allocateBuffer();
	void	freeBuffer();


	SaveLoad_DataBuffer	m_dataBuffer;

	int					m_saving,m_loading,m_formatting,m_autoloading;
	unsigned char		*m_tempBuffer;
	int					m_bufferSize;
	unsigned char		*m_memcardHeader;

	int					m_autoloadFrameCounter;

};



/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __MEMCARD_SAVELOAD_H__ */

/*===========================================================================
 end */
