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
		SONG_TITLE,			SONG_OPTIONS,		SONG_MEMORYCARD,	SONG_GAMECOMPLETE,	SONG_GAMEOVER,
		SONG_CHAPTER1_LEVEL1,	SONG_CHAPTER1_LEVEL2,	SONG_CHAPTER1_LEVEL3,	SONG_CHAPTER1_LEVEL4,	SONG_CHAPTER1_BOSS,
		SONG_CHAPTER2_LEVEL1,	SONG_CHAPTER2_LEVEL2,	SONG_CHAPTER2_LEVEL3,	SONG_CHAPTER2_LEVEL4,	SONG_CHAPTER2_BOSS,
		SONG_CHAPTER3_LEVEL1,	SONG_CHAPTER3_LEVEL2,	SONG_CHAPTER3_LEVEL3,	SONG_CHAPTER3_LEVEL4,	SONG_CHAPTER3_BOSS_ALSEEP,	SONG_CHAPTER3_BOSS_AWAKE,
		SONG_CHAPTER4_LEVEL1,	SONG_CHAPTER4_LEVEL2,	SONG_CHAPTER4_LEVEL3,	SONG_CHAPTER4_LEVEL4,	SONG_CHAPTER4_BOSS,
		SONG_CHAPTER5_LEVEL1,	SONG_CHAPTER5_LEVEL2,	SONG_CHAPTER5_LEVEL3,	SONG_CHAPTER5_LEVEL4,	SONG_CHAPTER5_BOSS,
		SONG_CHAPTER6_ROLLERCOASTER,	SONG_CHAPTER6_SNAILRACE,	SONG_CHAPTER6_BIGWHEEL,		SONG_CHAPTER6_TUNNELOFLOVE,		SONG_CHAPTER6_GHOSTTRAIN,	SONG_CHAPTER6_BIGTOP,
		NUM_SONGIDS,
	};

	typedef enum SFXBANKID
	{
		SFX_INGAME,
		NUM_SFXBANKIDS,
	};
	typedef enum SFXID
	{
		SFX_TEST0,		SFX_TEST1,		SFX_TEST2,		SFX_TEST3,
		SFX_TEST4,		SFX_TEST5,		SFX_TEST6,		SFX_TEST7,
		NUM_SFXIDS,
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
	static xmPlayingId	playSfx(SFXID _sfxId,int _lock=false);
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
	static int				s_songStartPattern;

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
