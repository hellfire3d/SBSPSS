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

#ifdef __VERSION_DEBUG__
#define _SOUNDDEBUG
#endif



#ifdef	_SOUNDDEBUG

#include "pad\pads.h"

#if defined (__USER_paul__) || defined (__USER_charles__)
static int sounddebug=true;
#else
static int sounddebug=false;
#endif

#endif	/* SFX_JELLY_LAUNCHER */


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
#include "gfx\prim.h"

#ifdef __USER_paul__
int dump=false;
#endif
void CXMPlaySound::think()
{
#ifdef __USER_paul__
	if(dump)
	{
		static char		*text[]={"SONG","SFX","LOOPINGSFX","SILENTSONG","SILENTSFX","FREE","CONTINUE"};
		spuChannelUse	*ch=m_spuChannelUse;
		PAUL_DBGMSG("=======");
		for(int i=0;i<24;i++,ch++)
		{
			PAUL_DBGMSG("%02d] u:%s  l:%d  pid:%04x  sPtn:%03d",i,text[ch->m_useType],ch->m_locked,ch->m_playingId,ch->m_startPattern);
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
			case SILENTSONG:
			case SILENTSFX:
				if(!ch->m_locked)
				{
					do
					{
						ch->m_useType=FREE;
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
					// Just mark it as silent, if it's unlocked then it'll die next frame
					ch->m_useType=ch->m_useType==SFX?SILENTSFX:SILENTSONG;

					// And kill it in the player
					XM_Quit(ch->m_internalId);
				}
				break;
				
			case LOOPINGSFX:
			case FREE:
			case CONTINUE:
				break;
		}
	}


//////////////
#ifdef	_SOUNDDEBUG
if(PadGetDown(1)&PAD_L1&&PadGetHeld(1)&(PAD_L2|PAD_R1|PAD_R2))
{
	sounddebug=!sounddebug;
}
if(sounddebug)
{
	static const int	colours[7][3]=
	{
		{	255,255,255		},			// SONG
		{	255,  0,255		},			// SFX
		{	  0,  0,255		},			// LOOPINGSFX
		{	255,255,  0		},			// SILENTSONG
		{	255,255,  0		},			// SILENTSFX
		{	  0,255,  0		},			// FREE
		{	128,128,128		},			// CONTINUE
	};
	static int			frameFlag=0;

	int				i,x,free;
	spuChannelUse	*ch;
	POLY_F4			*f4;

	x=50;
	free=0;
	ch=m_spuChannelUse;

	for(i=0;i<NUM_SPU_CHANNELS;i++)
	{
		const int	*colour=&colours[ch->m_useType][0];
		f4=GetPrimF4();
		setXYWH(f4,x,20,8,8);
		setRGB0(f4,*(colour++),*(colour++),*(colour++));
		AddPrimToList(f4,0);
		if(ch->m_locked)
		{
			f4=GetPrimF4();
			setXYWH(f4,x-1,20-1,8+2,8+2);
			setRGB0(f4,0,0,0);
			AddPrimToList(f4,0);
		}
		if(ch->m_useType==FREE)
		{
			free++;
		}

		x+=10;
		ch++;
	}

	f4=GetPrimF4();
	setXYWH(f4,50,15,(24*10)-2,2);
	if(frameFlag)
	{
		setRGB0(f4,255,255,255);
	}
	else
	{
		setRGB0(f4,0,0,0);
	}
	AddPrimToList(f4,0);
	frameFlag^=1;

	if(!free)
	{
		f4=GetPrimF4();
		setXYWH(f4,50,30,(24*10)-2,4);
		setRGB0(f4,255,0,0);
		AddPrimToList(f4,0);
	}
}
#endif	/* _SOUNDDEBUG */
//////////////
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
			ASSERT(vab->m_vabId!=-1);	// Didn't load!
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
	spuChannelUse	*ch;
	int				vol;
	
	ch=&m_spuChannelUse[_playingId&0xff];
	if(ch->m_playingId==_playingId)
	{
		ASSERT(ch->m_locked!=false);			// Cant alter unlocked channels!
		ch->m_vol=_volume;						// Update volume
		switch(ch->m_useType)
		{
			case SILENTSONG:
			case SILENTSFX:
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
	spuChannelUse	*ch;
	
	ch=&m_spuChannelUse[_playingId&0xff];
	if(ch->m_playingId==_playingId)
	{
		ASSERT(ch->m_locked!=false);			// Cant alter unlocked channels!
		ch->m_pan=_pan;							// Update pan
		switch(ch->m_useType)
		{
			case SILENTSONG:
			case SILENTSFX:
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

	ASSERT(0);			// Couldn't find the sound to unlock it!
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::stopAndUnlockAllSfx()
{
	int				i;
	spuChannelUse 	*ch;
	
	ch=m_spuChannelUse;
	for(i=0;i<NUM_SPU_CHANNELS;i++,ch++)
	{
		if(ch->m_useType==SFX||
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
		m_spuChannelUse[baseChannel].m_startPattern=_startPattern;
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
int	CXMPlaySound::isStillPlaying(xmPlayingId _playingId)
{
	spuChannelUse	*ch;

	ch=&m_spuChannelUse[_playingId&0xff];
	if(ch->m_playingId==_playingId)
	{
		ASSERT(ch->m_locked!=false);	// Can't do this on an unlocked sound!
		return ch->m_useType!=SILENTSFX&&ch->m_useType!=SILENTSONG;
	}

	ASSERT(0);			// Couldn't find the sound to check it!
	return false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CXMPlaySound::isSfxPatternPlaying(int _startPattern)
{
	spuChannelUse	*ch;
	int				i;

	ch=m_spuChannelUse;
	for(i=0;i<NUM_SPU_CHANNELS;i++)
	{
		if((ch->m_useType==SFX||ch->m_useType==SILENTSFX)&&ch->m_startPattern==_startPattern)
		{
			return true;
		}
		ch++;
	}

	return false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CXMPlaySound::unlockPlayingId(xmPlayingId _playingId)
{
	spuChannelUse	*ch;

	ch=&m_spuChannelUse[_playingId&0xff];
	if(ch->m_playingId==_playingId)
	{
		ASSERT(ch->m_locked!=false);	// Can't do this on an unlocked sound!
		ch->m_locked=false;
		return;
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
	spuChannelUse	*ch;
	
	ch=&m_spuChannelUse[_playingId&0xff];
	if(ch->m_playingId==_playingId)
	{
		ASSERT(ch->m_locked!=false);			// Cant stop unlocked channels!
		switch(ch->m_useType)
		{
			case SONG:
			case SFX:
				{
				XM_PlayStop(ch->m_internalId);
				XM_Quit(ch->m_internalId);
				ch->m_useType=ch->m_useType==SFX?SILENTSFX:SILENTSONG;
				}
				break;

			case LOOPINGSFX:
				XM_StopSample(ch->m_internalId);
				ch->m_useType=SILENTSFX;
				break;
				
			case SILENTSONG:
			case SILENTSFX:
				break;

			case FREE:
			case CONTINUE:
				ASSERT(0);
				break;
		}

		return;
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
//		XM_SetSFXRange(baseChannel,channelCount);
		id=XM_Init(vab->m_vabId,		// id from XM_VABInit
				   _modId,				// XM id ( as passed to InitXMData )
				   -1,					// Let xmplay give us a song id
				   baseChannel,			// Base cahnnel to play on
			       XM_NoLoop,			// One-shot
				   _playMask,			// Play mask
				   XM_SFX,				// SFX
				   _sfxPattern);		// SFX pattern to play
		XM_ClearSFXRange();
		markChannelsAsActive(baseChannel,channelCount,SFX,retId,id,_priority);
		m_spuChannelUse[baseChannel].m_startPattern=_sfxPattern;
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
			if(ch->m_playingId==validId&&!(ch->m_locked==false&&(ch->m_useType==SILENTSONG||ch->m_useType==SILENTSFX)))
			{
				validId=NOT_PLAYING;
			}
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
	for(i=_baseChannel;i<_baseChannel+_channelCount-1;i++,ch++)
	{
		ch->m_useType=CONTINUE;
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
			ASSERT(vab->m_vabId!=-1);	// Didn't load!
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