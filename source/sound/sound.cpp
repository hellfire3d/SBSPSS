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

typedef struct XMFILEDATA
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
xmSampleId		CSoundMediator::s_sfxSampleId=NO_SAMPLE;
xmModId			CSoundMediator::s_sfxModId=NO_SONG;

static CSpuSound		*s_spuSound;
static CXMPlaySound		*s_xmplaySound;


// Songs
static XMFILEDATA	s_xmSongData[CSoundMediator::NUM_SONGIDS]=
{
	{	MUSIC_HYPERMMX_VH,		MUSIC_HYPERMMX_VB,		MUSIC_HYPERMMX_PXM	},		// HYPERMMX
	{	MUSIC_DROPPOP_VH,		MUSIC_DROPPOP_VB,		MUSIC_DROPPOP_PXM	},		// DROPPOP
	{	MUSIC_MUSIC_VH,			MUSIC_MUSIC_VB,			MUSIC_MUSIC_PXM		},		// MUSIC
};

// SFX banks
static XMFILEDATA	s_xmSfxData[CSoundMediator::NUM_SFXBANKIDS]=
{
	{	SFX_INGAME_VH,			SFX_INGAME_VB,			SFX_INGAME_PXM		},		// INGAME
};

// Individual SFX details
static SFXDETAILS	s_sfxDetails[]=
{
	{	1,	6,	1	},
	{	1,	4,	0	},
	{	1,	5,	0	},
	{	1,	0,	1	},
};


// Reverb details
static ReverbDetails	s_reverbDetails[CSoundMediator::NUM_REVERBTYPES]=
{
	{	SPU_REV_MODE_OFF,	0,	0,		0	},								// NONE
	{	SPU_REV_MODE_ECHO,	75,	0x3000,	100	},								// ECHO_TEST
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
	setReverbType(NONE);//ECHO_TEST);

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
	if(s_volumeDirty[SONG])
	{
		s_xmplaySound->setMasterSongVolume(s_currentVolume[SONG]);
		s_volumeDirty[SONG]=false;
	}
	if(s_volumeDirty[SFX])
	{
		s_xmplaySound->setMasterSfxVolume(s_currentVolume[SFX]);
		s_volumeDirty[SFX]=false;
	}
	if(s_volumeDirty[SPEECH])
	{
		int vol=s_currentVolume[SPEECH]<<7;
		CXAStream::setMasterVolume(vol,vol);
		s_volumeDirty[SPEECH]=false;
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
	XMFILEDATA		*song;

	ASSERT(s_songSampleId==NO_SAMPLE);
	ASSERT(s_songModId==NO_SONG);

	song=&s_xmSongData[_songId];
	s_songModId=s_xmplaySound->loadModData(song->m_pxm);
	s_songSampleId=s_xmplaySound->loadSampleData(song->m_vh,song->m_vb);
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

	s_songPlayingId=s_xmplaySound->playSong(s_songSampleId,s_songModId);
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
	XMFILEDATA		*song;
	
	ASSERT(s_sfxSampleId==NO_SAMPLE);
	ASSERT(s_sfxModId==NO_SONG);
	
	song=&s_xmSfxData[_bankId];
	s_sfxModId=s_xmplaySound->loadModData(song->m_pxm);
	s_sfxSampleId=s_xmplaySound->loadSampleData(song->m_vh,song->m_vb);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		Pass _lock as true if you wanna keep hold of one-shot sfx.
				This'll be necessary if you have a *long* one-shot at the
				same time as *lots* of other sfx.
	Returns:
  ---------------------------------------------------------------------- */
xmPlayingId CSoundMediator::playSfx(int _sfxId,int _lock=false)
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