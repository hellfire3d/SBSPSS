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
		SFX_SPONGEBOB_WALK_1,			SFX_SPONGEBOB_WALK_2,
		SFX_SPONGEBOB_SQUEAKY_SHOES_1,	SFX_SPONGEBOB_SQUEAKY_SHOES_2,
		SFX_SPONGEBOB_JUMP,
		SFX_SPONGEBOB_BUTTBOUNCE,
		SFX_SPONGEBOB_BLINK,
		SFX_BUBBLE_WAND,
		SFX_SPONGEBOB_DEFEATED_JINGLE,
		SFX_BALLOON_INFLATE,			SFX_BALLOON_POP,
		SFX_GLASSES_ON,
		SFX_JELLY_LAUNCHER,
		SFX_SPONGEBOB_KARATE_1,			SFX_SPONGEBOB_KARATE_2,
		SFX_SPONGEBOB_ATTACK_IMPACT_WITH_ENEMY_1,		SFX_SPONGEBOB_ATTACK_IMPACT_WITH_ENEMY_2,
		SFX_SPONGEBOB_LAND_AFTER_FALL,
		SFX_SPONGEBOB_NET,
		SFX_ANEMONE_ATTACK_LEVEL1,		SFX_ANEMONE_ATTACK_LEVEL2,		SFX_ANEMONE_ATTACK_LEVEL3,
		SFX_ANEMONE_DEFEATED_LEVEL1,	SFX_ANEMONE_DEFEATED_LEVEL2,	SFX_ANEMONE_DEFEATED_LEVEL3,
		SFX_ANEMONE_MOVE,
		SFX_ANGLER_FISH_MOVE,
		SFX_BALL_BLOB_MOVE,		SFX_BALL_BLOB_ATTACK,
		SFX_BEANIE_MOVE,
		SFX_BOOGER_MOVE,
		SFX_CLAM_ATTACK,	SFX_CLAM_DEFEATED,	SFX_CLAM_MOVE,
		SFX_EYEBALL_SENTRY_ATTACK,	SFX_EYEBALL_SENTRY_MOVE,
		SFX_FISH_HOOK_MOVE,
		SFX_FLAMING_SKULL_MOVE,	SFX_FLAMING_SKULL_DEFEATED,	SFX_FLAMING_SKULL_ATTACK,
		SFX_GARY_DE_SNAIL,
		SFX_GHOST,
		SFX_FISH_FOLK_MOVE_1,	SFX_FISH_FOLK_MOVE_2,
		SFX_CANNING_MACHINE,
		SFX_LAVA,
		SFX_ROLLING_ROCK,
		SFX_SAW,
		SFX_WHIRLPOOL_DRAIN,	SFX_WHIRLPOOL_WHIRL,
		SFX_HERMIT_CRAB_ATTACK,	SFX_HERMIT_CRAB_MOVE,
		SFX_HAZARD__ACID_DROP,
		SFX_HAZARD__ACORN_LAND,
		SFX_HAZARD__BLADE_ATTACK,	SFX_HAZARD__BLADE_MOVE,
		SFX_HAZARD__ROPE_MOVE,		SFX_HAZARD__ROPE_SNAP,
		SFX_ANY_OBJECT_FALLING,
		SFX_HAZARD__FIREBALL_LAND,	SFX_HAZARD__FIREBALL_LAUNCH,
		SFX_HAZARD__FALLING_ROCK_FALL,	SFX_HAZARD__FALLING_ROCK_LAND,
		SFX_HAZARD__MINE,
		SFX_SPONGEBOB_SPLASH_INTO_WATER,
		SFX_SPONGEBOB_DIVING_HELMET,
		SFX_HAZARD__STALACTITE_LAND,	SFX_HAZARD__STALACTITE_RATTLE,
		SFX_HAZARD__STEAM,
		SFX_HAZARD__SWAMP_GAS,
		SFX_ITEM__CORAL_BLOWER,
		SFX_ITEM__DISGUISE,
		SFX_ITEM__JAMJAR_AMMO,
		SFX_ITEM__LEVEL_ITEM,	SFX_ITEM__ANY_OTHER_ITEM,	SFX_ITEM__POWER_UP_ITEM,	SFX_ITEM__SKILL_ITEM,
		SFX_JELLYFISH_MOVE,		SFX_JELLYFISH_DEFEATED,		SFX_JELLYFISH_ATTACK,
		SFX_NAUTILUS_ATTACK,	SFX_NAUTILUS_MOVE,
		SFX_NINJASTARFISH_ATTACK,
		SFX_PRICKLY_BUG_ATTACK,	SFX_PRICKLY_BUG_DEFEATED,	SFX_PRICKLY_BUG_MOVE,
		SFX_PUFFAFISH_MOVE,
		SFX_SKULL_ATTACK,
		SFX_SPIDERCRAB_ATTACK,	SFX_SPIDERCRAB_MOVE,
		SFX_SQUIDDART_ATTACK,	SFX_SQUIDDART_DEFEATED,	SFX_SQUIDDART_MOVE,
		SFX_SEASNAKE_ATTACK,	SFX_SEASNAKE_MOVE,
		SFX_TELEPHONE_BOX,
		SFX_WORLD_OBJECT__BARRIER,
		SFX_WORLD_OBJECT__BIG_WHEEL,
		SFX_WORLD_OBJECT__GEYSER,
		SFX_WORLD_OBJECT__GHOST_TRAIN_DOORS,
		SFX_WORLD_OBJECT__DISAPPEARING_PLATFORM,
		SFX_WORLD_OBJECT__RESPAWNING_PLATFORM,
		SFX_WORLD_OBJECT__ROTATING_PLATFORM,
		SFX_WORLD_OBJECT__SEESAW,
		SFX_WORLD_OBJECT__TRAIN_TRACKS,
		SFX_WORLD_OBJECT__CURRENTS,
		SFX_ZOMBIE_FISH_FOLK,
		SFX_FRONT_END__MOVE_CURSOR,	SFX_FRONT_END__SELECT,	SFX_FRONT_END__ERROR,	SFX_FRONT_END__OK,

		NUM_SFXIDS
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
