/*
position
adjust channels ( watery-mario64 style music changes )
*/


/*=========================================================================

	sound.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "sound.h"

#ifndef __SYSTEM_DBG_H__
#include "system\dbg.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
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

typedef struct XMSONGDATA
{
	FileEquate	m_vh,m_vb,m_pxm;
	int			m_startPattern;
};


typedef struct XMSFXFILEDATA
{
	FileEquate	m_vh,m_vb,m_pxm;
};
typedef struct SFXDETAILS
{
	int			m_channelMask;
	int			m_pattern;			// ..or instrument number for loopers
	int			m_looping;
};


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

// Static stuff for CSoundMediator
int				CSoundMediator::s_initialised=false;

int				CSoundMediator::s_currentVolume[CSoundMediator::NUM_VOLUMETYPES];
int				CSoundMediator::s_targetVolume[CSoundMediator::NUM_VOLUMETYPES];
int				CSoundMediator::s_volumeDirty[CSoundMediator::NUM_VOLUMETYPES];

xmSampleId		CSoundMediator::s_songSampleId=NO_SAMPLE;
FileEquate		CSoundMediator::s_songVh=(FileEquate)-1;
xmModId			CSoundMediator::s_songModId=NO_SONG;
FileEquate		CSoundMediator::s_songPxm=(FileEquate)-1;
xmPlayingId		CSoundMediator::s_songPlayingId=NOT_PLAYING;
int				CSoundMediator::s_songStartPattern=0;
xmSampleId		CSoundMediator::s_sfxSampleId=NO_SAMPLE;
xmModId			CSoundMediator::s_sfxModId=NO_SONG;

static CSpuSound		*s_spuSound;
static CXMPlaySound		*s_xmplaySound;


// Songs
static XMSONGDATA	s_xmSongData[CSoundMediator::NUM_SONGIDS]=
{
	//	m_vh					m_vb					m_pxm					m_startPattern;
	{	MUSIC_SB_TITLE_VH,		MUSIC_SB_TITLE_VB,		MUSIC_SB_TITLE_PXM,		0x00	},		// SONG_TITLE
	{	MUSIC_SB_TITLE_VH,		MUSIC_SB_TITLE_VB,		MUSIC_SB_TITLE_PXM,		0x0a	},		// SONG_OPTIONS
	{	MUSIC_SB_TITLE_VH,		MUSIC_SB_TITLE_VB,		MUSIC_SB_TITLE_PXM,		0x13	},		// SONG_MEMORYCARD
	{	MUSIC_SB_TITLE_VH,		MUSIC_SB_TITLE_VB,		MUSIC_SB_TITLE_PXM,		0x14	},		// SONG_GAMECOMPLETE
	{	MUSIC_SB_TITLE_VH,		MUSIC_SB_TITLE_VB,		MUSIC_SB_TITLE_PXM,		0x23	},		// SONG_GAMEOVER
	{	MUSIC_CHAPTER1_VH,		MUSIC_CHAPTER1_VB,		MUSIC_CHAPTER1_PXM,		0x19	},		// SONG_CHAPTER1_LEVEL1,
	{	MUSIC_CHAPTER1_VH,		MUSIC_CHAPTER1_VB,		MUSIC_CHAPTER1_PXM,		0x2d	},		// SONG_CHAPTER1_LEVEL2,
	{	MUSIC_CHAPTER1_VH,		MUSIC_CHAPTER1_VB,		MUSIC_CHAPTER1_PXM,		0x00	},		// SONG_CHAPTER1_LEVEL3,
	{	MUSIC_CHAPTER1_VH,		MUSIC_CHAPTER1_VB,		MUSIC_CHAPTER1_PXM,		0x2c	},		// SONG_CHAPTER1_LEVEL4,
	{	MUSIC_CHAPTER1_VH,		MUSIC_CHAPTER1_VB,		MUSIC_CHAPTER1_PXM,		0x40	},		// SONG_CHAPTER1_BOSS,
	{	MUSIC_CHAPTER2_VH,		MUSIC_CHAPTER2_VB,		MUSIC_CHAPTER2_PXM,		0x00	},		// SONG_CHAPTER2_LEVEL1,
	{	MUSIC_CHAPTER2_VH,		MUSIC_CHAPTER2_VB,		MUSIC_CHAPTER2_PXM,		0x16	},		// SONG_CHAPTER2_LEVEL2,
	{	MUSIC_CHAPTER2_VH,		MUSIC_CHAPTER2_VB,		MUSIC_CHAPTER2_PXM,		0x00	},		// SONG_CHAPTER2_LEVEL3,
	{	MUSIC_CHAPTER2_VH,		MUSIC_CHAPTER2_VB,		MUSIC_CHAPTER2_PXM,		0x3e	},		// SONG_CHAPTER2_LEVEL4,
	{	MUSIC_CHAPTER2_VH,		MUSIC_CHAPTER2_VB,		MUSIC_CHAPTER2_PXM,		0x40	},		// SONG_CHAPTER2_BOSS,
	{	MUSIC_CHAPTER3_VH,		MUSIC_CHAPTER3_VB,		MUSIC_CHAPTER3_PXM,		0x18	},		// SONG_CHAPTER3_LEVEL1,
	{	MUSIC_CHAPTER3_VH,		MUSIC_CHAPTER3_VB,		MUSIC_CHAPTER3_PXM,		0x00	},		// SONG_CHAPTER3_LEVEL2,
	{	MUSIC_CHAPTER3_VH,		MUSIC_CHAPTER3_VB,		MUSIC_CHAPTER3_PXM,		0x00	},		// SONG_CHAPTER3_LEVEL3,
	{	MUSIC_CHAPTER3_VH,		MUSIC_CHAPTER3_VB,		MUSIC_CHAPTER3_PXM,		0x0b	},		// SONG_CHAPTER3_LEVEL4,
	{	MUSIC_CHAPTER3_VH,		MUSIC_CHAPTER3_VB,		MUSIC_CHAPTER3_PXM,		0x21	},		// SONG_CHAPTER3_BOSS_ASLEEP,
	{	MUSIC_CHAPTER3_VH,		MUSIC_CHAPTER3_VB,		MUSIC_CHAPTER3_PXM,		0x22	},		// SONG_CHAPTER3_BOSS_AWAKE,
	{	MUSIC_CHAPTER4_VH,		MUSIC_CHAPTER4_VB,		MUSIC_CHAPTER4_PXM,		0x1b	},		// SONG_CHAPTER4_LEVEL1,
	{	MUSIC_CHAPTER4_VH,		MUSIC_CHAPTER4_VB,		MUSIC_CHAPTER4_PXM,		0x00	},		// SONG_CHAPTER4_LEVEL2,
	{	MUSIC_CHAPTER4_VH,		MUSIC_CHAPTER4_VB,		MUSIC_CHAPTER4_PXM,		0x2c	},		// SONG_CHAPTER4_LEVEL3,
	{	MUSIC_CHAPTER4_VH,		MUSIC_CHAPTER4_VB,		MUSIC_CHAPTER4_PXM,		0x00	},		// SONG_CHAPTER4_LEVEL4,
	{	MUSIC_CHAPTER4_VH,		MUSIC_CHAPTER4_VB,		MUSIC_CHAPTER4_PXM,		0x3c	},		// SONG_CHAPTER4_BOSS,
	{	MUSIC_CHAPTER5_VH,		MUSIC_CHAPTER5_VB,		MUSIC_CHAPTER5_PXM,		0x00	},		// SONG_CHAPTER5_LEVEL1,
	{	MUSIC_CHAPTER5_VH,		MUSIC_CHAPTER5_VB,		MUSIC_CHAPTER5_PXM,		0x14	},		// SONG_CHAPTER5_LEVEL2,
	{	MUSIC_CHAPTER5_VH,		MUSIC_CHAPTER5_VB,		MUSIC_CHAPTER5_PXM,		0x00	},		// SONG_CHAPTER5_LEVEL3,
	{	MUSIC_CHAPTER5_VH,		MUSIC_CHAPTER5_VB,		MUSIC_CHAPTER5_PXM,		0x2a	},		// SONG_CHAPTER5_LEVEL4,
	{	MUSIC_CHAPTER5_VH,		MUSIC_CHAPTER5_VB,		MUSIC_CHAPTER5_PXM,		0x3e	},		// SONG_CHAPTER5_BOSS,
	{	MUSIC_CHAPTER6_VH,		MUSIC_CHAPTER6_VB,		MUSIC_CHAPTER6_PXM,		0x00	},		// SONG_CHAPTER6_ROLLERCOASTER
	{	MUSIC_CHAPTER6_VH,		MUSIC_CHAPTER6_VB,		MUSIC_CHAPTER6_PXM,		0x00	},		// SONG_CHAPTER6_SNAILRACE
	{	MUSIC_CHAPTER6_VH,		MUSIC_CHAPTER6_VB,		MUSIC_CHAPTER6_PXM,		0x0c	},		// SONG_CHAPTER6_BIGWHEEL
	{	MUSIC_CHAPTER6_VH,		MUSIC_CHAPTER6_VB,		MUSIC_CHAPTER6_PXM,		0x0c	},		// SONG_CHAPTER6_TUNNELOFLOVE
	{	MUSIC_CHAPTER6_VH,		MUSIC_CHAPTER6_VB,		MUSIC_CHAPTER6_PXM,		0x15	},		// SONG_CHAPTER6_GHOSTTRAIN
	{	MUSIC_CHAPTER6_VH,		MUSIC_CHAPTER6_VB,		MUSIC_CHAPTER6_PXM,		0x2b	},		// SONG_CHAPTER6_BIGTOP
};

// SFX banks
static XMSFXFILEDATA	s_xmSfxData[CSoundMediator::NUM_SFXBANKIDS]=
{
	//	m_vh					m_vb					m_pxm
	{	SFX_INGAME_VH,			SFX_INGAME_VB,			SFX_INGAME_PXM		},		// SFX_INGAME
};

// Individual SFX details
static SFXDETAILS	s_sfxDetails[CSoundMediator::NUM_SFXIDS]=
{
	//	m_channelMask	m_pattern		m_looping
	{	1,				0,				0	},										// SFX_SPONGEBOB_WALK_1
	{	1,				1,				0	},										// SFX_SPONGEBOB_WALK_2
	{	1,				2,				0	},										// SFX_SPONGEBOB_SQUEAKY_SHOES_1
	{	1,				3,				0	},										// SFX_SPONGEBOB_SQUEAKY_SHOES_2
	{	1,				4,				0	},										// SFX_SPONGEBOB_JUMP
	{	1,				5,				0	},										// SFX_SPONGEBOB_BUTTBOUNCE
	{	1,				6,				0	},										// SFX_SPONGEBOB_BLINK
	{	3,				7,				0	},										// SFX_BUBBLE_WAND*
	{	1,				8,				0	},										// SFX_SPONGEBOB_DEFEATED_JINGLE
	{	1,				9,				0	},										// SFX_BALLOON_INFLATE
	{	1,				21,				0	},										// SFX_BALLOON_POP
	{	1,				10,				0	},										// SFX_GLASSES_ON
	{	1,				11,				0	},										// SFX_JELLY_LAUNCHER
	{	1,				12,				0	},										// SFX_SPONGEBOB_KARATE_1
	{	1,				13,				0	},										// SFX_SPONGEBOB_KARATE_2
	{	1,				14,				0	},										// SFX_SPONGEBOB_ATTACK_IMPACT_WITH_ENEMY_1
	{	1,				15,				0	},										// SFX_SPONGEBOB_ATTACK_IMPACT_WITH_ENEMY_2
	{	1,				16,				0	},										// SFX_SPONGEBOB_LAND_AFTER_FALL
	{	1,				17,				0	},										// SFX_SPONGEBOB_NET
	{	1,				18,				0	},										// SFX_ANEMONE_ATTACK_LEVEL1
	{	1,				19,				0	},										// SFX_ANEMONE_ATTACK_LEVEL2
	{	1,				20,				0	},										// SFX_ANEMONE_ATTACK_LEVEL3
	{	1,				21,				0	},										// SFX_ANEMONE_DEFEATED_LEVEL1
	{	1,				21,				0	},										// SFX_ANEMONE_DEFEATED_LEVEL2
	{	1,				22,				0	},										// SFX_ANEMONE_DEFEATED_LEVEL3
	{	1,				23,				0	},										// SFX_ANEMONE_MOVE
	{	1,				24,				0	},										// SFX_ANGLER_FISH_MOVE
	{	1,				25,				0	},										// SFX_BALL_BLOB_MOVE
	{	1,				26,				0	},										// SFX_BALL_BLOB_ATTACK
	{	1,				27,				0	},										// SFX_BEANIE_MOVE
	{	3,				28,				0	},										// SFX_BOOGER_MOVE*
	{	1,				29,				0	},										// SFX_CLAM_ATTACK
	{	1,				30,				0	},										// SFX_CLAM_DEFEATED
	{	1,				31,				0	},										// SFX_CLAM_MOVE
	{	3,				32,				0	},										// SFX_EYEBALL_SENTRY_ATTACK*
	{	3,				33,				0	},										// SFX_EYEBALL_SENTRY_MOVE*
	{	1,				34,				0	},										// SFX_FISH_HOOK_MOVE
	{	3,				35,				0	},										// SFX_FLAMING_SKULL_MOVE*
	{	1,				36,				0	},										// SFX_FLAMING_SKULL_DEFEATED
	{	3,				37,				0	},										// SFX_FLAMING_SKULL_ATTACK*
	{	1,				38,				0	},										// SFX_GARY_DE_SNAIL
	{	3,				39,				0	},										// SFX_GHOST*
	{	1,				40,				0	},										// SFX_FISH_FOLK_MOVE_1
	{	1,				41,				0	},										// SFX_FISH_FOLK_MOVE_2
	{	1,				42,				0	},										// SFX_CANNING_MACHINE
	{	3,				43,				0	},										// SFX_LAVA*
	{	1,				44,				0	},										// SFX_ROLLING_ROCK
	{	1,				45,				0	},										// SFX_SAW
	{	3,				46,				0	},										// SFX_WHIRLPOOL_DRAIN*
	{	3,				47,				0	},										// SFX_WHIRLPOOL_WHIRL*
	{	1,				48,				0	},										// SFX_HERMIT_CRAB_ATTACK
	{	1,				49,				0	},										// SFX_HERMIT_CRAB_MOVE
	{	3,				50,				0	},										// SFX_HAZARD__ACID_DROP*
	{	3,				51,				0	},										// SFX_HAZARD__ACORN_LAND*
	{	3,				52,				0	},										// SFX_HAZARD__BLADE_ATTACK*
	{	3,				53,				0	},										// SFX_HAZARD__BLADE_MOVE*
	{	1,				54,				0	},										// SFX_HAZARD__ROPE_MOVE
	{	3,				55,				0	},										// SFX_HAZARD__ROPE_SNAP*
	{	1,				56,				0	},										// SFX_ANY_OBJECT_FALLING
	{	3,				57,				0	},										// SFX_HAZARD__FIREBALL_LAND*
	{	3,				58,				0	},										// SFX_HAZARD__FIREBALL_LAUNCH*
	{	3,				59,				0	},										// SFX_HAZARD__FALLING_ROCK_FALL*
	{	3,				60,				0	},										// SFX_HAZARD__FALLING_ROCK_LAND*
	{	1,				61,				0	},										// SFX_HAZARD__MINE
	{	1,				62,				0	},										// SFX_SPONGEBOB_SPLASH_INTO_WATER
	{	1,				63,				0	},										// SFX_SPONGEBOB_DIVING_HELMET
	{	1,				64,				0	},										// SFX_HAZARD__STALACTITE_LAND
	{	1,				65,				0	},										// SFX_HAZARD__STALACTITE_RATTLE
	{	3,				66,				0	},										// SFX_HAZARD__STEAM*
	{	3,				67,				0	},										// SFX_HAZARD__SWAMP_GAS*
	{	3,				68,				0	},										// SFX_ITEM__CORAL_BLOWER*
	{	1,				69,				0	},										// SFX_ITEM__DISGUISE
	{	1,				70,				0	},										// SFX_ITEM__JAMJAR_AMMO
	{	3,				71,				0	},										// SFX_ITEM__LEVEL_ITEM*
	{	1,				72,				0	},										// SFX_ITEM__ANY_OTHER_ITEM
	{	3,				73,				0	},										// SFX_ITEM__POWER_UP_ITEM*
	{	3,				74,				0	},										// SFX_ITEM__SKILL_ITEM*
	{	1,				75,				0	},										// SFX_JELLYFISH_MOVE
	{	1,				76,				0	},										// SFX_JELLYFISH_DEFEATED
	{	3,				77,				0	},										// SFX_JELLYFISH_ATTACK*
	{	1,				78,				0	},										// SFX_NAUTILUS_ATTACK
	{	1,				79,				0	},										// SFX_NAUTILUS_MOVE
	{	1,				80,				0	},										// SFX_NINJASTARFISH_ATTACK
	{	1,				81,				0	},										// SFX_PRICKLY_BUG_ATTACK
	{	1,				82,				0	},										// SFX_PRICKLY_BUG_DEFEATED
	{	1,				83,				0	},										// SFX_PRICKLY_BUG_MOVE
	{	3,				84,				0	},										// SFX_PUFFAFISH_MOVE*
	{	3,				85,				0	},										// SFX_SKULL_ATTACK*
	{	1,				86,				0	},										// SFX_SPIDERCRAB_ATTACK
	{	1,				87,				0	},										// SFX_SPIDERCRAB_MOVE
	{	1,				88,				0	},										// SFX_SQUIDDART_ATTACK
	{	3,				89,				0	},										// SFX_SQUIDDART_DEFEATED*
	{	1,				90,				0	},										// SFX_SQUIDDART_MOVE
	{	1,				91,				0	},										// SFX_SEASNAKE_ATTACK
	{	1,				92,				0	},										// SFX_SEASNAKE_MOVE
	{	1,				93,				0	},										// SFX_TELEPHONE_BOX
	{	3,				94,				0	},										// SFX_WORLD_OBJECT__BARRIER*
	{	3,				95,				0	},										// SFX_WORLD_OBJECT__BIG_WHEEL*
	{	3,				96,				0	},										// SFX_WORLD_OBJECT__GEYSER*
	{	3,				97,				0	},										// SFX_WORLD_OBJECT__GHOST_TRAIN_DOORS*
	{	1,				98,				0	},										// SFX_WORLD_OBJECT__DISAPPEARING_PLATFORM
	{	1,				99,				0	},										// SFX_WORLD_OBJECT__RESPAWNING_PLATFORM
	{	1,				100,			0	},										// SFX_WORLD_OBJECT__ROTATING_PLATFORM
	{	1,				101,			0	},										// SFX_WORLD_OBJECT__SEESAW
	{	1,				102,			0	},										// SFX_WORLD_OBJECT__TRAIN_TRACKS
	{	1,				103,			0	},										// SFX_WORLD_OBJECT__CURRENTS
	{	1,				104,			0	},										// SFX_ZOMBIE_FISH_FOLK
	{	1,				105,			0	},										// SFX_FRONT_END__MOVE_CURSOR
	{	1,				106,			0	},										// SFX_FRONT_END__SELECT
	{	1,				107,			0	},										// SFX_FRONT_END__ERROR
	{	1,				108,			0	},										// SFX_FRONT_END__OK
};

// Reverb details
static ReverbDetails	s_reverbDetails[CSoundMediator::NUM_REVERBTYPES]=
{
	//	m_type					m_delay		m_depth		m_feedback
	{	SPU_REV_MODE_OFF,		0,			0,			0	},						// REV_NONE
	{	SPU_REV_MODE_ECHO,		75,			0x3000,		100	},						// REV_ECHOTEST
};



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::initialise()
{
	int		i;

	ASSERT(!s_initialised);

	s_spuSound=new ("SPUSound")	CSpuSound();			s_spuSound->initialise();
	s_xmplaySound=new ("XMPlaySound") CXMPlaySound();	s_xmplaySound->initialise();
	CXAStream::Init();

	for(i=0;i<NUM_VOLUMETYPES;i++)
	{
		s_currentVolume[i]=MIN_VOLUME;
		s_targetVolume[i]=INITIAL_VOLUME;
		s_volumeDirty[i]=true;
	}

	ASSERT(CXAStream::MIN_VOLUME==0);			// Just incase someone decides to change any of these.. things in here will break ( PKG )
	ASSERT(CXAStream::MAX_VOLUME==32767);

	// Initial reverb settings
	setReverbType(REV_NONE);//REV_ECHOTEST);

	SOUND_DBGMSG("Sound mediator initialised");
	s_initialised=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::shutdown()
{
ASSERT(0);

	ASSERT(s_initialised);

	s_xmplaySound->shutdown();			delete s_xmplaySound;
	s_spuSound->shutdown();				delete s_spuSound;

	SOUND_DBGMSG("Sound mediator shutdown");
	s_initialised=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::think(int _frames)
{
	int			i;
	int			diff,speed;
	int			*current,*target,*dirty;

	ASSERT(s_initialised);


	// Fade to target volume
ASSERT(_frames!=0);
if(_frames==0)_frames=1;
	speed=_frames*VOLUME_CHANGE_SPEED;
	current=s_currentVolume;
	target=s_targetVolume;
	dirty=s_volumeDirty;
	for(i=0;i<NUM_VOLUMETYPES;i++)
	{
		diff=*target-*current;
		if(diff<0)
		{
			*current-=speed;
			if(*current<*target)*current=*target;
			*dirty=true;
		}
		else if(diff>0)
		{
			*current+=speed;
			if(*current>*target)*current=*target;
			*dirty=true;
		}
		current++;
		target++;
		dirty++;
	}

	// Update of anything that needs it
	CXAStream::ControlXA();
	s_xmplaySound->think();
	
	// Push through any changes in volume
	if(s_volumeDirty[VOL_FADE])
	{
		s_volumeDirty[VOL_SONG]=true;
		s_volumeDirty[VOL_SFX]=true;
		s_volumeDirty[VOL_SPEECH]=true;
		s_volumeDirty[VOL_FADE]=false;
	}
	if(s_volumeDirty[VOL_SONG])
	{
		s_xmplaySound->setMasterSongVolume((s_currentVolume[VOL_SONG]*s_currentVolume[VOL_FADE])>>8);
		s_volumeDirty[VOL_SONG]=false;
	}
	if(s_volumeDirty[VOL_SFX])
	{
		s_xmplaySound->setMasterSfxVolume((s_currentVolume[VOL_SFX]*s_currentVolume[VOL_FADE])>>8);
		s_volumeDirty[VOL_SFX]=false;
	}
	if(s_volumeDirty[VOL_SPEECH])
	{
		int vol=((s_currentVolume[VOL_SPEECH]*s_currentVolume[VOL_FADE])>>8)<<7;
		CXAStream::setMasterVolume(vol,vol);
		s_volumeDirty[VOL_SPEECH]=false;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::setReverbType(REVERBTYPE _type)
{
	s_spuSound->setReverbDetails(&s_reverbDetails[_type]);
	s_spuSound->setReverbActive(true);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::setSong(SONGID _songId)
{
	XMSONGDATA		*song;

	song=&s_xmSongData[_songId];
	
	if(s_songSampleId!=NO_SAMPLE&&s_songVh!=song->m_vh)
	{
		s_xmplaySound->dumpSampleData(s_songSampleId);
		s_songSampleId=NO_SAMPLE;
	}
	if(s_songSampleId==NO_SAMPLE)
	{
		s_songSampleId=s_xmplaySound->loadSampleData(song->m_vh,song->m_vb);
	}
	s_songVh=song->m_vh;

	if(s_songModId!=NO_SONG&&s_songPxm!=song->m_pxm)
	{
		s_xmplaySound->dumpModData(s_songModId);
		s_songModId=NO_SONG;
	}
	if(s_songModId==NO_SONG)
	{
		s_songModId=s_xmplaySound->loadModData(song->m_pxm);
	}
	s_songPxm=song->m_pxm;

	s_songStartPattern=song->m_startPattern;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::playSong()
{
	ASSERT(s_songSampleId!=NO_SAMPLE);
	ASSERT(s_songModId!=NO_SONG);
	ASSERT(s_songPlayingId==NOT_PLAYING);

	s_songPlayingId=s_xmplaySound->playSong(s_songSampleId,s_songModId,s_songStartPattern);
//	s_volumeDirty[SONG]=true;		// Force a volume update
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::stopSong()
{
	ASSERT(s_songSampleId!=NO_SAMPLE);
	ASSERT(s_songModId!=NO_SONG);

	if(s_songPlayingId!=NOT_PLAYING)
	{
		s_xmplaySound->stopPlayingId(s_songPlayingId);
		s_xmplaySound->unlockPlayingId(s_songPlayingId);
	}
	s_songPlayingId=NOT_PLAYING;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::dumpSong()
{
	ASSERT(s_songSampleId!=NO_SAMPLE);
	ASSERT(s_songModId!=NO_SONG);

	stopSong();
	s_xmplaySound->dumpSampleData(s_songSampleId);
	s_songSampleId=NO_SAMPLE;
	s_xmplaySound->dumpModData(s_songModId);
	s_songModId=NO_SONG;
	s_songVh=(FileEquate)-1;
	s_songPxm=(FileEquate)-1;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::setSfxBank(SFXBANKID _bankId)
{
	XMSFXFILEDATA		*sfx;
	
	ASSERT(s_sfxSampleId==NO_SAMPLE);
	ASSERT(s_sfxModId==NO_SONG);
	
	sfx=&s_xmSfxData[_bankId];
	s_sfxModId=s_xmplaySound->loadModData(sfx->m_pxm);
	s_sfxSampleId=s_xmplaySound->loadSampleData(sfx->m_vh,sfx->m_vb);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		Pass _lock as true if you wanna keep hold of one-shot sfx.
				This'll be necessary if you have a *long* one-shot at the
				same time as *lots* of other sfx.
	Returns:
  ---------------------------------------------------------------------- */
xmPlayingId CSoundMediator::playSfx(SFXID _sfxId,int _lock=false)
{
	int			sfxChannelMask;
	xmPlayingId	playId;
	SFXDETAILS	*sfx;

	ASSERT(s_sfxSampleId!=NO_SAMPLE);
	ASSERT(s_sfxModId!=NO_SONG);

	// Play!
	sfx=&s_sfxDetails[_sfxId];
	if(sfx->m_looping)
	{
		playId=s_xmplaySound->playLoopingSfx(s_sfxSampleId,s_sfxModId,sfx->m_pattern,10);
	}
	else
	{
		playId=s_xmplaySound->playSfx(s_sfxSampleId,s_sfxModId,sfx->m_pattern,sfx->m_channelMask,20);
		if(!_lock&&playId!=NOT_PLAYING)
		{
			s_xmplaySound->unlockPlayingId(playId);		// We really don't care about one-shot sfx..
			playId=NOT_PLAYING;
		}
	}
//	s_volumeDirty[SFX]=true;		// Force a volume update

	return playId;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Calculate volume and panning for a sound base upon its
				position in space. Takes a position in space which describes
				the sound position relative to the camera/microphone.
				Volume comes from distance from origin and panning comes from
				the position along the x axis.
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::setposition(xmPlayingId _playingId,VECTOR *pos)
{
	int		volume,pan;

	volume=CXMPlaySound::MAX_VOLUME-(CalcLengthV(pos));
	if(volume<CXMPlaySound::MIN_VOLUME)volume=CXMPlaySound::MIN_VOLUME;

	pan=(pos->vx/2)+CXMPlaySound::PAN_CENTRE;
	if(pan<CXMPlaySound::PAN_LEFT)pan=CXMPlaySound::PAN_LEFT;
	else if(pan>CXMPlaySound::PAN_RIGHT)pan=CXMPlaySound::PAN_RIGHT;

	s_xmplaySound->setVolume(_playingId,volume);
	s_xmplaySound->setPanning(_playingId,pan);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::stopSfx(xmPlayingId _playingId)
{
	s_xmplaySound->stopPlayingId(_playingId);
	s_xmplaySound->unlockPlayingId(_playingId);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	NB: Will invalidate any locked sfx!
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::stopAllSfx()
{
	s_xmplaySound->stopAndUnlockAllSfx();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::playSpeech(SpeechEquate _speech)
{
	stopSpeech();
	CXAStream::PlaySpeech(_speech,1);
//	s_volumeDirty[SPEECH]=true;		// Force a volume update
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::stopSpeech()
{
	if(CXAStream::IsPlaying())
		CXAStream::Stop();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::setVolume(VOLUMETYPE _type,int _val)
{
	ASSERT(_val>=MIN_VOLUME&&_val<=MAX_VOLUME);
	
	s_targetVolume[_type]=_val;
	s_volumeDirty[_type]=true;		// Otherwise we could miss volume changes under rare conditions
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CSoundMediator::getVolume(VOLUMETYPE _type)
{
	return s_targetVolume[_type];
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::stopAllSound()
{
	s_xmplaySound->stopAndUnlockAllSound();
	CXAStream::Stop();
	s_songPlayingId=NOT_PLAYING;
}


/*===========================================================================
 end */