unsigned char	*xmPtr;
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
		s_songPtr[i]=(u8*)MemAlloc(size,"XMSong");
		XM_SetSongAddress(s_songPtr[i]);
	}
	size=XM_GetFileHeaderSize();
	for(i=0;i<MAX_XM_HEADERS;i++)
	{
		s_fhPtr[i]=(u8*)MemAlloc(size,"XMHeader");
		XM_SetFileHeaderAddress(s_fhPtr[i]);
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
xmSampleId CXMPlaySound::loadSamples(FileEquate _vhFe,FileEquate _vbFe)
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
xmSongId CXMPlaySound::loadSongData(FileEquate _songFe)
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
			return(xmSongId)songId;
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

	return (xmSongId)songId;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::dumpSamples(xmSampleId _sampleId)
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
void CXMPlaySound::dumpSongData(xmSongId _songId)
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
void CXMPlaySound::setVolume(xmPlayingSongId _songId,unsigned char _volume)
{
	XM_SetMasterVol(_songId,_volume>>1);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::setPanning(xmPlayingSongId _songId,char _pan)
{
	XM_SetMasterPan(_songId,_pan);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
//int sid=0;
//int chan=0;
xmPlayingSongId CXMPlaySound::playSong(xmSampleId _sampleId,xmSongId _songId,int _baseChannel)
{
	XMVab	*vab;
	int		id;

	ASSERT(s_xmVabs[_sampleId].m_refCount!=0);
	ASSERT(s_xmSongs[_songId].m_refCount!=0);
	
	vab=&s_xmVabs[_sampleId];
	id=XM_Init(vab->m_vabId,	// id from XM_VABInit
			   _songId,			// XM id ( as passed to InitXMData )
			   0,				// Song id
			   _baseChannel,	// First channel
			   XM_Loop,			// Loop 
			   -1,				// Play mask
			   XM_Music,		// Music
			   0);				// Pattern to start at

	return (xmPlayingSongId)id;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::stopSong(xmPlayingSongId _songId)
{
	XM_PlayStop(_songId);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
//int sid=0;
//int chan=0;
int SONGNUM=1;
xmPlayingSongId	CXMPlaySound::playSfx(xmSampleId _sampleId,xmSongId _songId,int _baseChannel,int _sfxPattern,int _playMask=-1)
{
//	XM_PlaySample(XM_GetSampleAddress(_sampleId,_sfxPattern),23,0x3fff,0x3fff,0x800);
//	return (xmPlayingSongId)0;
	
	int		i, maskCopy,channelCount=0;
	XMVab	*vab;
	int		id;

	ASSERT(s_xmVabs[_sampleId].m_refCount!=0);
	ASSERT(s_xmSongs[_songId].m_refCount!=0);


	maskCopy=_playMask;
	for(i=0;i<24&&maskCopy;i++)
	{
		if(maskCopy&1)
			channelCount++;
		maskCopy>>=1;
	}

	vab=&s_xmVabs[_sampleId];
	XM_SetSFXRange(_baseChannel,channelCount);
	id=XM_Init(vab->m_vabId,	// id from XM_VABInit
				 _songId,		// XM id ( as passed to InitXMData )
				 SONGNUM,		// Song id
				 -1,			// First channel
				 XM_NoLoop,		// Loop 
				 _playMask,		// Play mask
				 XM_SFX,		// Music
				 _sfxPattern);	// Pattern to start at

SONGNUM++;	
if(SONGNUM>=24)SONGNUM=1;
//PAUL_DBGMSG("sfx - ret:%d",id);	
	return (xmPlayingSongId)id;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CXMPlaySound::isSfxActive(xmPlayingSongId _id)
{
	XM_Feedback	fb;

	return XM_GetFeedback(_id,&fb)==0;
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