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
xmModId			CSoundMediator::s_songModId=NO_SONG;
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
	{	1,				0,				0	},										// SFX_TEST0
	{	1,				1,				0	},										// SFX_TEST1
	{	1,				2,				0	},										// SFX_TEST2
	{	1,				3,				0	},										// SFX_TEST3
	{	1,				4,				0	},										// SFX_TEST4
	{	1,				5,				0	},										// SFX_TEST5
	{	1,				6,				0	},										// SFX_TEST6
	{	1,				7,				0	},										// SFX_TEST7
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

	ASSERT(s_songSampleId==NO_SAMPLE);
	ASSERT(s_songModId==NO_SONG);

	song=&s_xmSongData[_songId];
	s_songModId=s_xmplaySound->loadModData(song->m_pxm);
	s_songSampleId=s_xmplaySound->loadSampleData(song->m_vh,song->m_vb);
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
void CSoundMediator::dumpSong()
{
	ASSERT(s_songSampleId!=NO_SAMPLE);
	ASSERT(s_songModId!=NO_SONG);

	if(s_songPlayingId!=NOT_PLAYING)
	{
		s_xmplaySound->stopPlayingId(s_songPlayingId);
		s_xmplaySound->unlockPlayingId(s_songPlayingId);
	}
	s_xmplaySound->dumpSampleData(s_songSampleId);
	s_xmplaySound->dumpModData(s_songModId);

	s_songSampleId=NO_SAMPLE;
	s_songModId=NO_SONG;
	s_songPlayingId=NOT_PLAYING;
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
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::playSpeech(SpeechEquate _speech)
{
	stopSpeech();
	CXAStream::PlaySpeech(_speech);
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
}


/*===========================================================================
 end */