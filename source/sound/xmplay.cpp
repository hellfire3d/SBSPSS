/*=========================================================================

	xmplay.cpp

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

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

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
		XMMod	*mod=&m_xmMods[i];
		mod->m_refCount=0;
	}
	for(i=0;i<MAX_XM_VABS;i++)
	{
		XMVab	*vab=&m_xmVabs[i];
		vab->m_refCount=0;
	}

	for(i=0;i<NUM_SPU_CHANNELS;i++)
	{
		m_spuChannelUse[i].m_useType=FREE;
		m_spuChannelUse[i].m_playingId=NOT_PLAYING;
		m_spuChannelUse[i].m_internalId=0;
		m_spuChannelUse[i].m_priority=0;
		m_spuChannelUse[i].m_locked=false;
		m_spuChannelUse[i].m_vol=MIN_VOLUME;
		m_spuChannelUse[i].m_pan=PAN_CENTRE;
	}

	m_masterSongVolume=0;
	m_masterSfxVolume=0;

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
#ifdef __USER_paul__
int dump=false;
#endif
void CXMPlaySound::think()
{
#ifdef __USER_paul__
	if(dump)
	{
		static char		*text[]={"SONG","SFX","LOOPINGSFX","SILENT","FREE","CONTINUE"};
		spuChannelUse	*ch=m_spuChannelUse;
		PAUL_DBGMSG("=======");
		for(int i=0;i<24;i++,ch++)
		{
			PAUL_DBGMSG("%02d] u:%s  l:%d  pid:%04x",i,text[ch->m_useType],ch->m_locked,ch->m_playingId);
		}
		PAUL_DBGMSG("=======");
		dump=false;
	}
#endif

	int				i;
	int				id;
	spuChannelUse	*ch;
	XM_Feedback		fb;

	// Check to see if any of the sounds have finished
	id=-1;
	for(i=0;i<NUM_SPU_CHANNELS;i++)
	{
		ch=&m_spuChannelUse[i];		// I know!
		switch(ch->m_useType)
		{
			// Silent and unlocked sounds can be freed
			case SILENT:
				if(!ch->m_locked)
				{
					do
					{
PAUL_DBGMSG("freed channel %d  ( %d )",i,ch->m_playingId);
						ch->m_useType=FREE;
#ifdef __VERSION_DEBUG__
						ch->m_internalId=0;
						ch->m_playingId=NOT_PLAYING;
						ch->m_priority=0;
						ch->m_locked=false;
						ch->m_vol=MIN_VOLUME;
						ch->m_pan=PAN_CENTRE;
#endif
						ch++;
					}
					while(ch->m_useType==CONTINUE);
				}
				break;


			// See if these have finished playing					
			case SONG:
			case SFX:
				if(XM_GetFeedback(ch->m_internalId,&fb))
				{
					do
					{
						// Just mark it as silent, if it's unlocked then it'll die next frame
						ch->m_useType=SILENT;
						ch++;
					}
					while(ch->m_useType==CONTINUE);
				}
				break;
				
			case LOOPINGSFX:
			case FREE:
			case CONTINUE:
				break;
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
---------------------------------------------------------------------- */
void CXMPlaySound::setMasterSongVolume(unsigned char _vol)
{
	int				i;
	spuChannelUse	*ch;
	xmPlayingId		lastId;
	
	// New volume
	if(m_masterSongVolume==_vol)	return;
	m_masterSongVolume=_vol;
	
	// Now update any active songs
	ch=m_spuChannelUse;
	lastId=NOT_PLAYING;
	for(i=0;i<NUM_SPU_CHANNELS;i++,ch++)
	{
		if(ch->m_playingId!=lastId&&ch->m_useType==SONG)
		{
			int	oldLock=ch->m_locked;		// hmm..
			ch->m_locked=true;				// not too..
			setVolume(ch->m_playingId,ch->m_vol);
			ch->m_locked=oldLock;			// ..ugly I suppose
			lastId=ch->m_playingId;
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
---------------------------------------------------------------------- */
void CXMPlaySound::setMasterSfxVolume(unsigned char _vol)
{
	int				i;
	spuChannelUse	*ch;
	xmPlayingId		lastId;

	// New volume
	if(m_masterSfxVolume==_vol)		return;
	m_masterSfxVolume=_vol;

	// Now update any active sfx
	ch=m_spuChannelUse;
	lastId=NOT_PLAYING;
	for(i=0;i<NUM_SPU_CHANNELS;i++,ch++)
	{
		if(ch->m_playingId!=lastId&&ch->m_useType==SFX||ch->m_useType==LOOPINGSFX)
		{
			int	oldLock=ch->m_locked;		// hmm..
			ch->m_locked=true;				// not too..
			setVolume(ch->m_playingId,ch->m_vol);
			ch->m_locked=oldLock;			// ..ugly I suppose
			lastId=ch->m_playingId;
		}
	}
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
	vab=m_xmVabs;
	for(vabId=0;vabId<MAX_XM_VABS;vabId++)
	{
		if(vab->m_refCount&&vab->m_vhFile==_vhFe&&vab->m_vbFile==_vbFe)
		{
			// Yup..
			vab->m_refCount++;
			return(xmSampleId)vabId;
		}
	}
	
	// Find next free vab slot
	vabId=0;
	vab=m_xmVabs;
	while(1)
	{
		ASSERT(vabId<MAX_XM_VABS);
		if(vab->m_refCount==0)
		{
			VhPtr=(u8*)CFileIO::loadFile(_vhFe);
			VbPtr=(u8*)CFileIO::loadFile(_vbFe);
			vab->m_vabId=XM_VABInit(VhPtr,VbPtr);
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
xmModId CXMPlaySound::loadModData(FileEquate _modFe)
{
	int		modId;	
	XMMod	*mod;

	// Is the mod already loaded?
	mod=m_xmMods;
	for(modId=0;modId<MAX_XM_SONGS;modId++)
	{
		if(mod->m_refCount&&mod->m_file==_modFe)
		{
			// Yup..
			mod->m_refCount++;
			return(xmModId)modId;
		}
	}

	// Find next free song slot
	mod=m_xmMods;
	modId=0;
	while(1)
	{
		ASSERT(modId<MAX_XM_SONGS);
		if(mod->m_refCount==0)
		{
			mod->m_xmData=(u8*)CFileIO::loadFile(_modFe);
			InitXMData(mod->m_xmData,modId,XM_UseXMPanning);
			mod->m_file=_modFe;
			mod->m_refCount=1;
			break;
		}
		modId++;mod++;
	}

	return (xmModId)modId;
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

	vab=&m_xmVabs[_sampleId];
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
void CXMPlaySound::dumpModData(xmModId _modId)
{
	XMMod	*mod;

	mod=&m_xmMods[_modId];
	mod->m_refCount--;
	if(mod->m_refCount==0)
	{
		MemFree(mod->m_xmData);
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
void CXMPlaySound::setVolume(xmPlayingId _playingId,unsigned char _volume)
{
//	int				i;
	spuChannelUse	*ch;
	int				vol;
	
	ch=&m_spuChannelUse[_playingId&0xff];
//	for(i=0;i<NUM_SPU_CHANNELS;i++,ch++)
	{
		if(ch->m_playingId==_playingId)
		{
			ASSERT(ch->m_locked!=false);			// Cant alter unlocked channels!
			ch->m_vol=_volume;						// Update volume
			switch(ch->m_useType)
			{
				case SILENT:
					break;
					
				case SONG:
					vol=(_volume*m_masterSongVolume)>>8;
					XM_SetMasterVol(ch->m_internalId,vol>>1);
					break;
					
				case SFX:
					vol=(_volume*m_masterSfxVolume)>>8;
					XM_SetMasterVol(ch->m_internalId,vol>>1);
					break;
					
				case LOOPINGSFX:
					updateLoopingSfx(ch);
					break;

				case FREE:
				case CONTINUE:
					ASSERT(0);
					break;
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
void CXMPlaySound::setPanning(xmPlayingId _playingId,char _pan)
{
//	int				i;
	spuChannelUse	*ch;
	
	ch=&m_spuChannelUse[_playingId&0xff];
//	for(i=0;i<NUM_SPU_CHANNELS;i++,ch++)
	{
		if(ch->m_playingId==_playingId)
		{
			ASSERT(ch->m_locked!=false);			// Cant alter unlocked channels!
			ch->m_pan=_pan;							// Update pan
			switch(ch->m_useType)
			{
				case SILENT:
					break;
					
				case SONG:
					XM_SetMasterPan(ch->m_internalId,_pan-128);
					break;
					
				case SFX:
					XM_SetMasterPan(ch->m_internalId,_pan-128);
					break;
					
				case LOOPINGSFX:
					updateLoopingSfx(ch);
					break;

				case FREE:
				case CONTINUE:
					ASSERT(0);
					break;
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
void CXMPlaySound::stopAndUnlockAllSound()
{
	int				i;
	spuChannelUse 	*ch;
	
	ch=m_spuChannelUse;
	for(i=0;i<NUM_SPU_CHANNELS;i++,ch++)
	{
		if(ch->m_useType==SONG||
		   ch->m_useType==SFX||
		   ch->m_useType==LOOPINGSFX)
		{
			ch->m_locked=true;				// hmm.. not too ugly I suppose
			setVolume(ch->m_playingId,0);
			stopPlayingId(ch->m_playingId);

			// Need to unlock too
			unlockPlayingId(ch->m_playingId);
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
xmPlayingId CXMPlaySound::playSong(xmSampleId _sampleId,xmModId _modId,int _startPattern)
{
	int				channelCount,baseChannel;
	XMVab			*vab;
	int				id;
	xmPlayingId		retId;

	ASSERT(m_xmVabs[_sampleId].m_refCount!=0);
	ASSERT(m_xmMods[_modId].m_refCount!=0);

	// Let's grab the channel count from the XM data.. :)
	channelCount=(short int)*(m_xmMods[_modId].m_xmData+68);
	SOUND_DBGMSG("Playing song with %d channels reserved",channelCount);

	baseChannel=findSpareChannels(channelCount,255);
	if(baseChannel!=-1)
	{
		retId=getNextSparePlayingId(baseChannel);
		vab=&m_xmVabs[_sampleId];
		id=XM_Init(vab->m_vabId,		// id from XM_VABInit
				   _modId,				// XM id ( as passed to InitXMData )
				   -1,					// Let xmplay give us a song id
				   baseChannel,			// First channel
				   XM_Loop,				// Play song continuously
				   -1,					// Play all channels
				   XM_Music,			// Music
				   _startPattern);		// Where to start from
		markChannelsAsActive(baseChannel,channelCount,SONG,retId,id,255);
		setVolume(retId,MAX_VOLUME);
		//PAUL_DBGMSG("playSong        %d/%d ( on %d-%d )",retId,id,baseChannel,baseChannel+channelCount-1);
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
			//PAUL_DBGMSG("unlocking %d",_playingId);
			ASSERT(ch->m_locked!=false);
			do
			{
				ch->m_locked=false;
				ch++;
			}
			while(ch->m_useType==CONTINUE);
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
//	int				i;
	spuChannelUse	*ch;
	
	ch=&m_spuChannelUse[_playingId&0xff];
//	for(i=0;i<NUM_SPU_CHANNELS;i++,ch++)
	{
		if(ch->m_playingId==_playingId)
		{
			ASSERT(ch->m_locked!=false);			// Cant stop unlocked channels!
			switch(ch->m_useType)
			{
				case SONG:
				case SFX:
					{
					XM_Feedback		fb;
					do
					{
						XM_PlayStop(ch->m_internalId);
						XM_GetFeedback(ch->m_internalId,&fb);
					}
					while(fb.Status!=XM_STOPPED);
					}
					break;

				case LOOPINGSFX:
					XM_StopSample(ch->m_internalId);
					break;
					
				case SILENT:
				case FREE:
				case CONTINUE:
					ASSERT(0);
					break;
			}

			do
			{
				ch->m_useType=SILENT;
				ch++;
			}
			while(ch->m_useType==CONTINUE);
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
xmPlayingId	CXMPlaySound::playSfx(xmSampleId _sampleId,xmModId _modId,int _sfxPattern,int _playMask,u8 _priority)
{
	int				i,j;
	int				maskCopy,channelCount;
	int				valid;
	int				baseChannel;
	XMVab			*vab;
	int				songId;
	int				id;
	xmPlayingId		retId;

	ASSERT(m_xmVabs[_sampleId].m_refCount!=0);
	ASSERT(m_xmMods[_modId].m_refCount!=0);

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
		retId=getNextSparePlayingId(baseChannel);
		vab=&m_xmVabs[_sampleId];
		XM_SetSFXRange(baseChannel,channelCount);
		id=XM_Init(vab->m_vabId,		// id from XM_VABInit
				   _modId,				// XM id ( as passed to InitXMData )
				   -1,					// Let xmplay give us a song id
				   -1,					// Use SFX range to get first channel
			       XM_NoLoop,			// One-shot
				   _playMask,			// Play mask
				   XM_SFX,				// SFX
				   _sfxPattern);		// SFX pattern to play
		XM_ClearSFXRange();
		//PAUL_DBGMSG("playSfx         %d/%d ( on %d-%d )",retId,id,baseChannel,baseChannel+channelCount-1);
		markChannelsAsActive(baseChannel,channelCount,SFX,retId,id,_priority);
		setVolume(retId,MAX_VOLUME);
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
xmPlayingId	CXMPlaySound::playLoopingSfx(xmSampleId _sampleId,xmModId _modId,int _soundId,u8 _priority,int _pitch)
{
	int				baseChannel;
	xmPlayingId		retId;

	baseChannel=findSpareChannels(1,_priority);
	if(baseChannel!=-1)
	{
		retId=getNextSparePlayingId(baseChannel);
		//PAUL_DBGMSG("playLoopingSfx  %d/- ( on %d-%d )",retId,baseChannel,baseChannel);
		XM_PlaySample(XM_GetSampleAddress(_sampleId,_soundId),baseChannel,0x3fff,0x3fff,_pitch);
		markChannelsAsActive(baseChannel,1,LOOPINGSFX,retId,baseChannel,_priority);
		setVolume(retId,MAX_VOLUME);
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
xmPlayingId	CXMPlaySound::getNextSparePlayingId(int _baseChannel)
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
		validId=(xmPlayingId)((nextId<<8)+_baseChannel);
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
			if(m_spuChannelUse[j].m_useType!=FREE) valid=false;
		}
		if(valid) return i;
		i=j;
	}

	// PKG - Add priority stuff here!!

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
	spuChannelUse	*ch;


	ch=&m_spuChannelUse[_baseChannel];
	ch->m_useType=_useType;
	ch->m_internalId=_internalId;
	ch->m_playingId=_playingId;
	ch->m_priority=_priority;
	ch->m_locked=true;
	ch->m_vol=MAX_VOLUME;
	ch->m_pan=PAN_CENTRE;

	ch++;
	for(i=_baseChannel+1;i<_baseChannel+_channelCount;i++,ch++)
	{
		ch->m_useType=CONTINUE;
#ifdef __VERSION_DEBUG__
		ch->m_internalId=0;
		ch->m_playingId=NOT_PLAYING;
		ch->m_priority=0;
		ch->m_locked=false;
		ch->m_vol=MIN_VOLUME;
		ch->m_pan=PAN_CENTRE;
#endif
	}
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
	vab=m_xmVabs;
	for(vabId=0;vabId<MAX_XM_VABS;vabId++,vab++)
	{
		if(vab->m_refCount)
		{
			XM_CloseVAB(vab->m_vabId);
		}
	}

	// Now reload them
	vab=m_xmVabs;
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


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::updateLoopingSfx(spuChannelUse *ch)
{
	int				actualVol,actualPan;
	int				leftVol,rightVol;
	SpuVoiceAttr	attr;

	actualVol=(ch->m_vol*m_masterSfxVolume)>>8;		// 0=silent, 255=full vol
	actualPan=ch->m_pan;							// 0=hard left, 255=hard right

	leftVol=(actualVol*actualPan)>>2;
	rightVol=(actualVol*(255-actualPan))>>2;
	ASSERT(leftVol<=0x3fff);	ASSERT(leftVol>=0);
	ASSERT(rightVol<=0x3fff);	ASSERT(rightVol>=0);

	attr.voice=(1<<ch->m_internalId);
	attr.mask=(SPU_VOICE_VOLL|SPU_VOICE_VOLR);
	attr.volume.left=leftVol;
	attr.volume.right=rightVol;

	SpuSetVoiceAttr(&attr);
}


/*===========================================================================
 end */