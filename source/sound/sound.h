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
		HYPERMMX,
		DROPPOP,
		MUSIC,
		NUM_SONGIDS,
	};
	typedef enum SFXBANKID
	{
		INGAME,
		NUM_SFXBANKIDS,
	};

	typedef enum VOLUMETYPE
	{
		SONG,
		SFX,
		SPEECH,

		SONGFADE,

		NUM_VOLUMETYPES,
	};

	typedef enum
	{
		MIN_VOLUME=0,
		MAX_VOLUME=255,
	};


	// General
	static void		initialise();
	static void		shutdown();
	static void		think(int _frames);


	// Song interface
	static void		setSong(SONGID _songId);
	static void		playSong();
	static void		dumpSong();
	static void		setSongToFadedOut()				{setVolume(SONGFADE,0);}
	static void		setSongToFadedIn()				{setVolume(SONGFADE,255);}

	// SFX interface
	static void		setSfxBank(SFXBANKID _bankId);
	static int		playSfx(int _sfxId);
//	static void		stopSfx(int _id);

	// Speech interface
	static void		playSpeech(SpeechEquate _speech);

	// Control
	static void		setVolume(VOLUMETYPE _type,int _val);
	static int		getVolume(VOLUMETYPE _type);
//	static void		stopAllSound();


private:
	enum
	{
		NUM_SPU_CHANNELS=24,

		SONG_BASE_CHANNEL	=0,
		SONG_CHANNELS		=10,
		SONG_MAX_CHANNEL	=SONG_BASE_CHANNEL+SONG_CHANNELS-1,

		SFX_BASE_CHANNEL	=SONG_MAX_CHANNEL+1,
		SFX_CHANNELS		=NUM_SPU_CHANNELS-SFX_BASE_CHANNEL,
		SFX_MAX_CHANNEL		=SFX_BASE_CHANNEL+SFX_CHANNELS-1,

		VOLUME_CHANGE_SPEED=2,
		INITIAL_VOLUME=192,
	};
	
					CSoundMediator()			{;}

	static int		s_initialised;

//	static int		s_currentVolume[NUM_VOLUMETYPES];
//	static int		s_targetVolume[NUM_VOLUMETYPES];

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
