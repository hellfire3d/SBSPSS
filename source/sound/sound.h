/*=========================================================================

	sound.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__SOUND_SOUND_H__
#define	__SOUND_SOUND_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef	__SOUND_XMPLAY_H__
#include "sound\xmplay.h"
#endif

#ifndef	__SOUND_SPU_H__
#include "sound\spu.h"
#endif
#ifndef	__SOUND_XMPLAY_H__
#include "sound\xmplay.h"
#endif
#ifndef __CDXAHeader__
#include "sound\cdxa.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CSoundMediator
{
public:
	typedef enum SONGID
	{
		SONG_HYPERMMX,
		SONG_DROPPOP,
		SONG_MUSIC,
		SONG_INGAME,
		SONG_TITLE,
		NUM_SONGIDS,
	};
	typedef enum SFXBANKID
	{
		SFX_INGAME,
		NUM_SFXBANKIDS,
	};

	typedef enum VOLUMETYPE
	{
		VOL_SONG,
		VOL_SFX,
		VOL_SPEECH,

		VOL_FADE,

		NUM_VOLUMETYPES,
	};

	typedef enum
	{
		MIN_VOLUME=0,
		MAX_VOLUME=255,
	};

	typedef enum REVERBTYPE
	{
		REV_NONE,
		REV_ECHOTEST,

		NUM_REVERBTYPES,
	};

	// General
	static void			initialise();
	static void			shutdown();
	static void			think(int _frames);


	// Reverb
	static void			setReverbType(REVERBTYPE _type);

	// Song interface
	static void			setSong(SONGID _songId);
	static void			playSong();
	static void			dumpSong();

	// SFX interface
	static void			setSfxBank(SFXBANKID _bankId);
	static xmPlayingId	playSfx(int _sfxId,int _lock=false);
	static void			setposition(xmPlayingId _playingId,VECTOR *pos);
	static void			stopSfx(xmPlayingId _playingId);

	// Speech interface
	static void			playSpeech(SpeechEquate _speech);
	static void			stopSpeech();

	// Control
	static void			setVolume(VOLUMETYPE _type,int _val);
	static int			getVolume(VOLUMETYPE _type);
	static void			stopAllSound();


private:
	enum
	{
		VOLUME_CHANGE_SPEED=2,
		INITIAL_VOLUME=192,
	};

					CSoundMediator()			{;}

	static int				s_initialised;

	static int				s_currentVolume[NUM_VOLUMETYPES];
	static int				s_targetVolume[NUM_VOLUMETYPES];
	static int				s_volumeDirty[NUM_VOLUMETYPES];

	static xmSampleId		s_songSampleId;
	static xmModId			s_songModId;
	static xmPlayingId		s_songPlayingId;
	static xmSampleId		s_sfxSampleId;
	static xmModId			s_sfxModId;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __SOUND_SOUND_H__ */

/*===========================================================================
 end */
