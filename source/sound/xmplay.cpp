/*=========================================================================

	spu.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "xmplay.h"

#ifndef __SYSTEM_DBG_H__
#include "system\dbg.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef XMPlayer
#include "system\global.h"
#include <xmplay.h>
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

#define MAX_XM_SONGS	5
#define MAX_XM_VABS		5


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

typedef struct XMSong
{
	unsigned char	*m_xmData;
	FileEquate		m_file;
	int				m_refCount;
	// refcount these!
};
static XMSong		s_xmSongs[MAX_XM_SONGS];

typedef struct XMVab
{
	int				m_vabId;
	FileEquate		m_vhFile,m_vbFile;
	int				m_refCount;
};
static XMVab		s_xmVabs[MAX_XM_VABS];

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::initialise()
{
	int		size,i;

	
#if defined(__TERRITORY_USA__) || defined(__TERRITORY_JAP__)
	XM_OnceOffInit(XM_NTSC);
#else
	XM_OnceOffInit(XM_PAL);
#endif
	setStereo(true);
	size=XM_GetSongSize();
	for(i=0;i<MAX_SONG_HEADERS;i++)
	{
		m_songPtr[i]=(u8*)MemAlloc(size,"XMSong");
		XM_SetSongAddress(m_songPtr[i]);
	}
	size=XM_GetFileHeaderSize();
	for(i=0;i<MAX_XM_HEADERS;i++)
	{
		m_fhPtr[i]=(u8*)MemAlloc(size,"XMHeader");
		XM_SetFileHeaderAddress(m_fhPtr[i]);
	}
	VidAddVSyncFunc(XM_Update);


	// Clear internal data
	for(i=0;i<MAX_XM_SONGS;i++)
	{
		XMSong	*song=&s_xmSongs[i];
		song->m_refCount=0;
	}
	for(i=0;i<MAX_XM_VABS;i++)
	{
		XMVab	*vab=&s_xmVabs[i];
		vab->m_refCount=0;
	}

	for(i=0;i<NUM_SPU_CHANNELS;i++)
	{
		m_spuChannelUse[i].m_useType=SILENT;
		m_spuChannelUse[i].m_locked=false;
	}


	SOUND_DBGMSG("XMPlay sound initialised");
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::shutdown()
{
	SOUND_DBGMSG("XMPlay sound shutdown");
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int spuflags[24];
void CXMPlaySound::think()
{
	int				i;
	int				id;
	spuChannelUse	*ch;
	XM_Feedback		fb;

	// Check to see if any of the sounds have finished
	id=-1;
	ch=m_spuChannelUse;
	for(i=0;i<NUM_SPU_CHANNELS;i++)
	{
//pkg tidy		
		// Only unlocked SFX need to be checked
		if(ch->m_locked==false&&ch->m_useType==SFX)
		{
			id=ch->m_playingId;
			if(id!=-1)
			{
				if(XM_GetFeedback(ch->m_internalId,&fb))
				{
PAUL_DBGMSG("%d finished.. ( was on chnl %d )",id,i);
					while(ch->m_playingId==id&&i<NUM_SPU_CHANNELS)
					{
						ch->m_useType=SILENT;
						ch++;
						i++;
					}
					ch--;
				}
			}
		}
		ch++;
	}

for(i=0;i<24;i++)
	spuflags[i]=m_spuChannelUse[i].m_useType;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
---------------------------------------------------------------------- */
xmSampleId CXMPlaySound::loadSampleData(FileEquate _vhFe,FileEquate _vbFe)
{
	int				vabId;
	XMVab			*vab;
	unsigned char	*VbPtr,*VhPtr;


	// Is the bank already loaded?
	vab=s_xmVabs;
	for(vabId=0;vabId<MAX_XM_VABS;vabId++)
	{
		if(vab->m_refCount&&vab->m_vhFile==_vhFe&&vab->m_vbFile==_vbFe)
		{
			// Yup..
			vab->m_refCount++;
			return(xmSampleId)vabId;
		}
	}
	
// PKG - Can be neatened up a bit..

	// Find next free vab slot
	vabId=0;
	vab=s_xmVabs;
	while(1)
	{
		ASSERT(vabId<MAX_XM_VABS);
		if(vab->m_refCount==0)
		{
			VhPtr=(u8*)CFileIO::loadFile(_vhFe);
			VbPtr=(u8*)CFileIO::loadFile(_vbFe);
			vab->m_vabId=XM_VABInit(VhPtr,VbPtr);
			//defragSpuMemory();		somewhere around here..
			MemFree(VhPtr);
			MemFree(VbPtr);
			vab->m_vhFile=_vhFe;
			vab->m_vbFile=_vbFe;
			vab->m_refCount=1;
			break;
		}
		vabId++;vab++;
	}

	return (xmSampleId)vabId;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
---------------------------------------------------------------------- */
xmModId CXMPlaySound::loadModData(FileEquate _songFe)
{
	int		songId;	
	XMSong	*song;

	// Is the song already loaded?
	song=s_xmSongs;
	for(songId=0;songId<MAX_XM_SONGS;songId++)
	{
		if(song->m_refCount&&song->m_file==_songFe)
		{
			// Yup..
			song->m_refCount++;
			return(xmModId)songId;
		}
	}

// PKG - Can be neatened up a bit..

	// Find next free song slot
	song=s_xmSongs;
	songId=0;
	while(1)
	{
		ASSERT(songId<MAX_XM_SONGS);
		if(song->m_refCount==0)
		{
			song->m_xmData=(u8*)CFileIO::loadFile(_songFe);
			InitXMData(song->m_xmData,songId,XM_UseS3MPanning);
			song->m_file=_songFe;
			song->m_refCount=1;
			break;
		}
		songId++;song++;
	}

	return (xmModId)songId;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::dumpSampleData(xmSampleId _sampleId)
{
	XMVab	*vab;

	vab=&s_xmVabs[_sampleId];
	vab->m_refCount--;
	if(vab->m_refCount==0)
	{
		XM_CloseVAB(vab->m_vabId);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::dumpModData(xmModId _songId)
{
	XMSong	*song;

	song=&s_xmSongs[_songId];
	song->m_refCount--;
	if(song->m_refCount==0)
	{
		MemFree(song->m_xmData);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::setStereo(int _stereo)
{
	if(_stereo)
		XM_SetStereo();
	else
		XM_SetMono();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::setVolume(xmPlayingId _songId,unsigned char _volume)
{
	XM_SetMasterVol(_songId,_volume>>1);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::setPanning(xmPlayingId _songId,char _pan)
{
	XM_SetMasterPan(_songId,_pan);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
xmPlayingId CXMPlaySound::playSong(xmSampleId _sampleId,xmModId _songId,int _channelCount)
{
	int				baseChannel;
	XMVab			*vab;
	int				id;
	xmPlayingId		retId;

	ASSERT(s_xmVabs[_sampleId].m_refCount!=0);
	ASSERT(s_xmSongs[_songId].m_refCount!=0);

	baseChannel=findSpareChannels(_channelCount,255);
	if(baseChannel!=-1)
	{
		retId=getNextSparePlayingId();
		vab=&s_xmVabs[_sampleId];
		id=XM_Init(vab->m_vabId,	// id from XM_VABInit
				_songId,			// XM id ( as passed to InitXMData )
				   -1,//baseChannel,//0,				// Song id
				baseChannel,		// First channel
				XM_Loop,			// Loop 
				-1,				// Play mask
				XM_Music,		// Music
				0);				// Pattern to start at
		markChannelsAsActive(baseChannel,_channelCount,SONG,retId,id,255);
PAUL_DBGMSG("playSong        %d/%d ( on %d-%d )",retId,id,baseChannel,baseChannel+_channelCount-1);
	}
	else
	{
		retId=NOT_PLAYING;
	}

	return retId;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::unlockPlayingId(xmPlayingId _playingId)
{
	int				i;
	spuChannelUse	*ch;

	ch=m_spuChannelUse;
	for(i=0;i<NUM_SPU_CHANNELS;i++,ch++)
	{
		if(ch->m_playingId==_playingId)
		{
PAUL_DBGMSG("unlocking %d",_playingId);
			ASSERT(ch->m_locked!=false);
			while(ch->m_playingId==_playingId)
			{
				ch->m_locked=false;
				ch++;
			}
			return;
		}
	}
	ASSERT(0);			// Couldn't find the sound to unlock it!
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::stopPlayingId(xmPlayingId _playingId)
{
//	ASSERT(m_spuChannelUse[_playingId].m_locked!=true);		// Unlock channel first!


	int				i;
	spuChannelUse	*ch;
	
	ch=m_spuChannelUse;
	for(i=0;i<NUM_SPU_CHANNELS;i++,ch++)
	{
		if(ch->m_playingId==_playingId)
		{
			switch(ch->m_useType)
			{
				case SILENT:
					PAUL_DBGMSG("stopping.. woah! %d is already stopped",_playingId);
					break;

				case SONG:
					PAUL_DBGMSG("stopping song on %d",_playingId);
					XM_PlayStop(ch->m_internalId);
					break;

				case SFX:
					PAUL_DBGMSG("stopping sfx on %d",_playingId);
					XM_PlayStop(ch->m_internalId);
					break;

				case LOOPINGSFX:
					PAUL_DBGMSG("stopping loopingsfx on %d",_playingId);
					XM_StopSample(ch->m_internalId);
					break;
			}
			
			while(ch->m_playingId==_playingId)
			{
				ch->m_useType=SILENT;
				ch++;
			}
			return;
		}
	}
	ASSERT(0);			// Couldn't find the sound to stop it!
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
xmPlayingId	CXMPlaySound::playSfx(xmSampleId _sampleId,xmModId _songId,int _sfxPattern,int _playMask,u8 _priority)
{
	int				i,j;
	int				maskCopy,channelCount;
	int				valid;
	int				baseChannel;
	XMVab			*vab;
	int				songId;
	int				id;
	xmPlayingId		retId;

	ASSERT(s_xmVabs[_sampleId].m_refCount!=0);
	ASSERT(s_xmSongs[_songId].m_refCount!=0);

	// Count channels
	maskCopy=_playMask;
	channelCount=0;
	for(i=0;i<NUM_SPU_CHANNELS&&maskCopy;i++)
	{
		if(maskCopy&1)
			channelCount++;
		maskCopy>>=1;
	}

	// Play the sound
	baseChannel=findSpareChannels(channelCount,_priority);
	if(baseChannel!=-1)
	{
		retId=getNextSparePlayingId();
		vab=&s_xmVabs[_sampleId];
		XM_SetSFXRange(baseChannel,channelCount);
		id=XM_Init(vab->m_vabId,	// id from XM_VABInit
					 _songId,		// XM id ( as passed to InitXMData )
					 -1,//baseChannel,//SONGNUM,		// Song id
					 -1,			// First channel
					 XM_NoLoop,		// Loop 
					 _playMask,		// Play mask
					 XM_SFX,		// Music
					 _sfxPattern);	// Pattern to start at
		XM_ClearSFXRange();
PAUL_DBGMSG("playSfx         %d/%d ( on %d-%d )",retId,id,baseChannel,baseChannel+channelCount-1);
		markChannelsAsActive(baseChannel,channelCount,SFX,retId,id,_priority);
	}
	else
	{
		retId=NOT_PLAYING;
	}
	
	return retId;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
xmPlayingId	CXMPlaySound::playLoopingSfx(xmSampleId _sampleId,xmModId _songId,int _soundId,u8 _priority,int _pitch)
{
	int				baseChannel;
	xmPlayingId		retId;

	baseChannel=findSpareChannels(1,_priority);
	if(baseChannel!=-1)
	{
		retId=getNextSparePlayingId();
PAUL_DBGMSG("playLoopingSfx  %d/- ( on %d-%d )",retId,baseChannel,baseChannel);
		XM_PlaySample(XM_GetSampleAddress(_sampleId,_soundId),baseChannel,0x3fff,0x3fff,_pitch);
		markChannelsAsActive(baseChannel,1,LOOPINGSFX,retId,baseChannel,_priority);
	}
	else
	{
		retId=NOT_PLAYING;
	}

	return retId;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
xmPlayingId	CXMPlaySound::getNextSparePlayingId()
{
	xmPlayingId		validId;
	int				i;
	spuChannelUse	*ch;
	static xmPlayingId	nextId=(xmPlayingId)(0-1);		// fudge to make first id=0


	validId=NOT_PLAYING;
	while(validId==NOT_PLAYING)
	{
		nextId=(xmPlayingId)((nextId+1)&0xff);			// gives 256 unique id numbers

		// Is this id still in use?
		validId=nextId;
		ch=m_spuChannelUse;
		for(i=0;i<NUM_SPU_CHANNELS&&validId!=NOT_PLAYING;i++)
		{
			if(ch->m_playingId==validId&&!(ch->m_locked==false&&ch->m_useType==SILENT))
				validId=NOT_PLAYING;
			ch++;
		}
	}

	return validId;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Returns the base channel of a slot big enuf for the
				requested number of channels
	Params:
	Returns:	baseChannel or -1 if no space
  ---------------------------------------------------------------------- */
int CXMPlaySound::findSpareChannels(int _channelCount,int _priority)
{
	int			i,j;
	int			valid;
	
	// First we search for channels that are marked as unused
	valid=false;
	for(i=0;i<NUM_SPU_CHANNELS-_channelCount+1&&valid==false;)
	{
		valid=true;
		for(j=i;j<i+_channelCount&&valid;j++)
		{
			if(m_spuChannelUse[j].m_useType!=SILENT) valid=false;			// pkg - tidy up
		}
		if(valid) return i;
		i=j;
	}

/*
	// Couldn't find one.. can we kill off a lower priority sound?
	if(valid==false)
	{
		int		lowestPrioity=_priority;
		int		possibleBase=-1;
		int		id;
		
		// Find the lowest priority sound with enuf spare channels
		i=0;
		while(i<24)
		{
			if(m_spuChannelUse[i].m_priority<=lowestPriority&&m_spuChannelUse[j].m_useType!=SILENT)
			{
				valid=true;
				id=m_spuChannelUse[i].m_id;
				for(j=i;j<i+_channelCount&&valid;j++)
				{
					if(m_spuChannelUse[j].m_id!=id) valid=false;
				}

			}
			i++;
		}
	}
*/
	// Can't play it :(
	return -1;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::markChannelsAsActive(int _baseChannel,int _channelCount,CHANNELUSETYPE _useType,xmPlayingId _playingId,int _internalId,u8 _priority)
{
	int				i;
	spuChannelUse	*ch,details;


	details.m_useType=_useType;
	details.m_internalId=_internalId;
	details.m_playingId=_playingId;
	details.m_priority=_priority;
	details.m_locked=true;

	ch=&m_spuChannelUse[_baseChannel];
	for(i=_baseChannel;i<_baseChannel+_channelCount;i++,ch++)
		*ch=details;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Defrag SPU memory by reloading sample banks. Should mean that
				bank load order isn't so vital.
				Really needs to check for and pause playing sounds first.. hmm..
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::defragSpuMemory()
{
	int				vabId;
	XMVab			*vab;
	unsigned char	*VbPtr,*VhPtr;


	SOUND_DBGMSG("CXMPlaySound is defragging..");
	
	// Dump banks
	vab=s_xmVabs;
	for(vabId=0;vabId<MAX_XM_VABS;vabId++,vab++)
	{
		if(vab->m_refCount)
		{
			XM_CloseVAB(vab->m_vabId);
		}
	}

	// Now reload them
	vab=s_xmVabs;
	for(vabId=0;vabId<MAX_XM_VABS;vabId++,vab++)
	{
		if(vab->m_refCount)
		{
			VhPtr=(u8*)CFileIO::loadFile(vab->m_vhFile);
			VbPtr=(u8*)CFileIO::loadFile(vab->m_vbFile);
			vab->m_vabId=XM_VABInit(VhPtr,VbPtr);
			MemFree(VhPtr);
			MemFree(VbPtr);
		}
	}

	SOUND_DBGMSG("..done!");
}


/*===========================================================================
 end */