/*=========================================================================

	saveload.cpp

	Author:		PKG
	Created: 
	Project:	SBSP
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "memcard\saveload.h"

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef __MEMCARD_MEMCARD_H__
#include "memcard\memcard.h"
#endif

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif


/*	Std Lib
	------- */

/*	Gfx
	--- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

static void callback(int _status);


/*----------------------------------------------------------------------
	Vars
	---- */

static int	s_callbackEnded=false;
static int	s_callbackStatus=0;




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
SaveLoadDatabase::SaveLoadDatabase()
{
	m_saving=false;
	m_loading=false;
	m_formatting=false;
	m_loading=false;
	m_autoloading=false;
	m_tempBuffer=NULL;
	m_memcardHeader=CFileIO::loadFile(MEMCARD_MEMHEAD_BIN);

	gatherData();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
SaveLoadDatabase::~SaveLoadDatabase()
{
	MemFree(m_memcardHeader);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SaveLoadDatabase::think()
{
	MemCard::Handler();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SaveLoadDatabase::gatherData()
{
	ASSERT(!m_saving);
	ASSERT(!m_loading);
	ASSERT(!m_formatting);
	ASSERT(!m_autoloading);

	createData();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool SaveLoadDatabase::startSave(char *_filename,int _fileNum=-1)
{
	char	nameBuf[9]="\0";

	ASSERT(!m_saving);
	ASSERT(!m_loading);
	ASSERT(!m_formatting);
	ASSERT(!m_autoloading);

	if(_fileNum!=-1)
	{
		strncat(nameBuf,MemCard::GetFileName(0,_fileNum),8);
		_filename=nameBuf;
	}

	// Alloc a buffer and copy everything to it
	allocateBuffer();
	memcpy((char*)&m_tempBuffer[0],m_memcardHeader,256);								// Memcard header
	MemCard::FillHeaderDetails(m_tempBuffer,m_bufferSize,_filename);					// Setup header
	memcpy((char*)&m_tempBuffer[256],(char*)&m_dataBuffer,sizeof(m_dataBuffer));		// Data
	MemCard::GiveCheckSum(&m_tempBuffer[m_bufferSize-MD5_CHECKSUM_SIZE],				// Chksum
						  m_tempBuffer,m_bufferSize-MD5_CHECKSUM_SIZE);


	// Start write to memcard
	if(!MemCard::WriteFile(0,_fileNum,(u8*)_filename,(void*)m_tempBuffer,m_bufferSize,&callback))
	{
		MEMCARD_DBGMSG("write error");
		freeBuffer();
		return false;
	}

	s_callbackEnded=false;
	m_saving=true;
	return true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int SaveLoadDatabase::getSaveStatus()
{
	int ret=INACTIVE;

	if(m_saving)
	{
		if(MemCard::GetCardStatus(0)==MemCard::CS_NoCard)
		{
MEMCARD_DBGMSG("======= weird card removal - bonus 50 points");
			// Card removed at weird time..
			ret=FAILED;
			s_callbackEnded=false;
			m_saving=false;
			freeBuffer();
		}
		else if(s_callbackEnded)
		{
			ASSERT(m_tempBuffer);

			if(s_callbackStatus==MemCard::CCS_WriteFileOK) ret=FINISHED_OK;
			else ret=FAILED;
			freeBuffer();

			s_callbackEnded=false;
			m_saving=false;
		}
		else
		{
			ret=IN_PROGRESS;
		}
	}
	
	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool SaveLoadDatabase::startLoad(int _file)
{
	ASSERT(!m_saving);
	ASSERT(!m_loading);
	ASSERT(!m_formatting);

	// Alloc a buffer to load to
	allocateBuffer();
	
	// Start read from memcard
	if(!MemCard::ReadFile(0,_file,(void*)m_tempBuffer,&callback))
	{
		MEMCARD_DBGMSG("read error");
		freeBuffer();
		return false;
	}

	s_callbackEnded=false;
	m_loading=true;
	return true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int SaveLoadDatabase::getLoadStatus()
{
	int ret=INACTIVE;

	if(m_loading)
	{
		if(MemCard::GetCardStatus(0)==MemCard::CS_NoCard)
		{
			MEMCARD_DBGMSG("======= weird card removal - bonus 50 points");
			// Card removed at weird time..
			freeBuffer();

			m_loading=false;
			m_autoloading=false;
			s_callbackEnded=false;
			ret=FAILED;
		}
		else if(s_callbackEnded)
		{
			ASSERT(m_tempBuffer);

			// Load ok?
			if(s_callbackStatus==MemCard::CCS_ReadFileOK&&
			   MemCard::TestCheckSum(&m_tempBuffer[m_bufferSize-MD5_CHECKSUM_SIZE],m_tempBuffer,m_bufferSize-MD5_CHECKSUM_SIZE))
			{
				memcpy(&m_dataBuffer,&m_tempBuffer[256],sizeof(m_dataBuffer));
				if(m_dataBuffer.m_headerId!=SAVELOAD_HEADERID)
				{
					ASSERT(!"YOUR MEMCARD SAVE IS OUT OF DATE! PLEASE DELETE IT!");
				}

				// Loaded ok - Copy the data in
				restoreData();
				ret=FINISHED_OK;
				MEMCARD_DBGMSG("load ok :)");
			}
			else
			{
				MEMCARD_DBGMSG("load failed!!!");
				ret=FAILED;
			}
			freeBuffer();

			m_loading=false;
			m_autoloading=false;
			s_callbackEnded=false;
		}
		else
		{
			ret=IN_PROGRESS;
		}
	}
	
	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool SaveLoadDatabase::startFormat()
{
	ASSERT(!m_saving);
	ASSERT(!m_loading);
	ASSERT(!m_formatting);
	ASSERT(!m_autoloading);

	if(!MemCard::FormatCard(0,&callback))
	{
		MEMCARD_DBGMSG("format error");
		return false;
	}

MEMCARD_DBGMSG("======= m_formatting=true");
	s_callbackEnded=false;
	m_formatting=true;
	return true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int SaveLoadDatabase::getFormatStatus()
{
	int ret=INACTIVE;

	if(m_formatting)
	{
		if(MemCard::GetCardStatus(0)==MemCard::CS_NoCard)
		{
MEMCARD_DBGMSG("======= weird card removal - bonus 50 points");
			// Card removed at weird time..
			ret=FAILED;
			s_callbackEnded=false;
			m_formatting=false;
		}
		else if(s_callbackEnded)
		{
			if(s_callbackStatus==MemCard::CCS_FormatCardOK) ret=FINISHED_OK;
			else ret=FAILED;

MEMCARD_DBGMSG("======= m_formatting=false");
			s_callbackEnded=false;
			m_formatting=false;
		}
		else
		{
MEMCARD_DBGMSG("======= still formatting...");
			ret=IN_PROGRESS;
		}
	}
	return ret;
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SaveLoadDatabase::startAutoload()
{
	MEMCARD_DBGMSG("Trying autoload..");
	m_autoloadFrameCounter=0;
	m_autoloading=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool SaveLoadDatabase::monitorAutoload()
{
	if(m_autoloading)
	{
		if(m_loading)
		{
			getLoadStatus();
		}
		else if(m_autoloading&&
				m_autoloadFrameCounter>(60*2)&&
				MemCard::GetCardStatus(0)!=MemCard::CS_CardInserted)
		{
			if(MemCard::GetFileCountOnCard(0))
			{
				startLoad(0);
			}
			else
			{
				m_autoloading=false;
			}
		}
		m_autoloadFrameCounter++;
	}
	else
	{
		return false;
	}

	return true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SaveLoadDatabase::createData()
{
	int i;

	// Header
	m_dataBuffer.m_headerId=SAVELOAD_HEADERID;

	// System details
	m_dataBuffer.m_systemDetails.m_screenOffX=VidGetXOfs();
	m_dataBuffer.m_systemDetails.m_screenOffY=VidGetYOfs();
	for(i=0;i<CSoundMediator::NUM_VOLUMETYPES;i++)
	{
		m_dataBuffer.m_systemDetails.m_volumes[i]=CSoundMediator::getVolume((CSoundMediator::VOLUMETYPE)i);
	}
	m_dataBuffer.m_systemDetails.m_language=(char)TranslationDatabase::getLanguage();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SaveLoadDatabase::restoreData()
{
	int i;

	// System details
	VidSetXYOfs(m_dataBuffer.m_systemDetails.m_screenOffX,m_dataBuffer.m_systemDetails.m_screenOffY);
	for(i=0;i<CSoundMediator::NUM_VOLUMETYPES;i++)
	{
		CSoundMediator::setVolume((CSoundMediator::VOLUMETYPE)i,m_dataBuffer.m_systemDetails.m_volumes[i]);
	}
	if(!TranslationDatabase::isLoaded())
	{
		TranslationDatabase::loadLanguage(m_dataBuffer.m_systemDetails.m_language);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SaveLoadDatabase::allocateBuffer()
{
	ASSERT(!m_tempBuffer);

	m_bufferSize=MC_HEADER_SIZE+sizeof(m_dataBuffer)+MD5_CHECKSUM_SIZE;
	m_bufferSize=((m_bufferSize/MemCard::BLOCKSIZE)+1)*MemCard::BLOCKSIZE;
	m_tempBuffer=(unsigned char*)MemAlloc(m_bufferSize,"MEMCARD");
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SaveLoadDatabase::freeBuffer()
{
	ASSERT(m_tempBuffer);
	
	MemFree(m_tempBuffer);
	m_tempBuffer=NULL;
}








/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void callback(int _status)
{
	ASSERT(!s_callbackEnded);
	s_callbackEnded=true;
	s_callbackStatus=_status;
}


/*===========================================================================
 end */