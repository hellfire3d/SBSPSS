/*=========================================================================

	xmplay.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__SOUND_XMPLAY_H__
#define	__SOUND_XMPLAY_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

// This is a bit messy, but makes sure that you can't easily pass the wrong IDs to the xm functions
// ( ..something scares me about this way of doing it tho :)
typedef enum	{NO_SAMPLE=-1}		xmSampleId;
typedef enum	{NO_SONG=-1}		xmModId;
typedef enum	{NOT_PLAYING=-1}	xmPlayingId;
// Note that a playing id is a 16 bit value. Top 8 bits are an ( effectively ) random number and the bottom
// 8 bits are the base channel of the playing sound.


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CXMPlaySound
{
public:
	enum
	{
		MIN_VOLUME=0,
		MAX_VOLUME=255,
		PAN_LEFT=0,
		PAN_CENTRE=127,
		PAN_RIGHT=255,
	};

	
	void			initialise();
	void			shutdown();
	void			think();

	void			setMasterSongVolume(unsigned char _vol);
	void			setMasterSfxVolume(unsigned char _vol);

	xmSampleId		loadSampleData(FileEquate _vhFe,FileEquate _vbFe);
	xmModId			loadModData(FileEquate _modFe);
	void			dumpSampleData(xmSampleId _sampleId);
	void			dumpModData(xmModId _modId);

	void			setStereo(int _stereo);
	
	void			setVolume(xmPlayingId _playingId,unsigned char _volume);
	void			setPanning(xmPlayingId _playingId,char _pan);

	void			stopAndUnlockAllSfx();
	void			stopAndUnlockAllSound();

	xmPlayingId		playSong(xmSampleId _sampleId,xmModId _modId,int _startPattern=0);
	xmPlayingId		playSfx(xmSampleId _sampleId,xmModId _modId,int _sfxPattern,int _playMask,u8 _priority);
	xmPlayingId		playLoopingSfx(xmSampleId _sampleId,xmModId _modId,int _soundId,u8 _priority,int _pitch=0x400);

	int				isStillPlaying(xmPlayingId _playingId);
	int				isSfxPatternPlaying(int _startPattern);

	void			unlockPlayingId(xmPlayingId _playingId);
	void			stopPlayingId(xmPlayingId _playingId);

	
private:
	enum
	{
		MAX_XM_SONGS=2,					// How many mods our internal loader copes with
		MAX_XM_VABS=2,					// How many vabs our internal loader copes with

		MAX_SONG_HEADERS=24,			// Number of mods that xmplay can play at once
		MAX_XM_HEADERS=MAX_XM_SONGS,	// Number of mods that xmplay can load at once

		NUM_SPU_CHANNELS=24,
	};

	typedef enum
	{
		SONG,
		SFX,
		LOOPINGSFX,

		SILENTSONG,			// Channel is silent
		SILENTSFX,			//	"	"	"	"
		FREE,				// Channel is free for re-allocation
		CONTINUE,			// Channel is a continuation of the previous channel
	} CHANNELUSETYPE;

	// Internal representation of loaded mods
	typedef struct XMMod
	{
		unsigned char	*m_xmData;
		FileEquate		m_file;
		int				m_refCount;
	};

	// Internal representation of loaded vabs
	typedef struct XMVab
	{
		int				m_vabId;
		FileEquate		m_vhFile,m_vbFile;
		int				m_refCount;
	};

	// Structure that records what is going on for each SPU channel
	typedef struct
	{
		CHANNELUSETYPE		m_useType;
		xmPlayingId			m_playingId;
		u8					m_internalId;
		u8					m_priority;
		u8					m_locked;
		u8					m_vol,m_pan;
		u8					m_startPattern;
	} spuChannelUse;

	xmPlayingId		getNextSparePlayingId(int _baseChannel);
	int				findSpareChannels(int _channelCount,int _priority);
	void			markChannelsAsActive(int _baseChannel,int _channelCount,CHANNELUSETYPE _useType,xmPlayingId _playingId,int _internalId,u8 _priority);

	void			defragSpuMemory();

	void			updateLoopingSfx(spuChannelUse *ch);

	
	unsigned char	*m_fhPtr[MAX_XM_HEADERS];
	unsigned char	*m_songPtr[MAX_SONG_HEADERS];
	XMMod			m_xmMods[MAX_XM_SONGS];
	XMVab			m_xmVabs[MAX_XM_VABS];
	spuChannelUse	m_spuChannelUse[NUM_SPU_CHANNELS];

	unsigned char	m_masterSongVolume;
	unsigned char	m_masterSfxVolume;
	
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __SOUND_XMPLAY_H__ */

/*===========================================================================
 end */
