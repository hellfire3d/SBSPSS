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


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

static XMFILEDATA	s_xmSongData[CSoundMediator::NUM_SONGIDS]=
{
	{	HYPERMMX_HYPERMMX_VH,		HYPERMMX_HYPERMMX_VB,		HYPERMMX_HYPERMMX_PXM	},		// HYPERMMX
	{	DROPPOP_DROPPOP_VH,		DROPPOP_DROPPOP_VB,		DROPPOP_DROPPOP_PXM	},		// DROPPOP
};

static XMFILEDATA	s_xmSfxData[CSoundMediator::NUM_SFXBANKIDS]=
{
//	{	SFX_INGAME_VH,			SFX_INGAME_VB,			SFX_INGAME_PXM		},		// INGAME
	{	DROPPOP_DROPPOP_VH,		DROPPOP_DROPPOP_VB,		DROPPOP_DROPPOP_PXM	},		// DROPPOP
};


int			CSoundMediator::s_initialised=false;
int			/*CSoundMediator::*/s_currentVolume[CSoundMediator::NUM_VOLUMETYPES];
int			/*CSoundMediator::*/s_targetVolume[CSoundMediator::NUM_VOLUMETYPES];
int			/*CSoundMediator::*/s_volumeDirty[CSoundMediator::NUM_VOLUMETYPES];


static CSpuSound		*s_spuSound;

static CXMPlaySound		*s_xmplaySound;

static xmSampleId		s_songSampleId=NO_SAMPLE;
static xmSongId			s_songDataId=NO_SONG;
static xmPlayingSongId	s_songPlayingId=NOT_PLAYING;

static xmSampleId		s_sfxSampleId=NO_SAMPLE;
static xmSongId			s_sfxDataId=NO_SONG;




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
	s_xmplaySound=new  ("XMPlaySound") CXMPlaySound();	s_xmplaySound->initialise();
	CXAStream::Init();
	

	for(i=0;i<NUM_VOLUMETYPES;i++)
	{
		s_currentVolume[i]=MIN_VOLUME;
		s_targetVolume[i]=INITIAL_VOLUME;
		s_volumeDirty[i]=true;
	}
	setSongToFadedIn();
//	ASSERT(CXAStream::MIN_VOLUME==0);			// Just incase someone decides to change any of these.. things in here will break ( PKG )
//	ASSERT(CXAStream::MAX_VOLUME==32767);



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
int tgt=255;
int val;
static int	s_spuChannelUse[24]={-1,-1,-1,-1,-1,-1,
									-1,-1,-1,-1,-1,-1,
									-1,-1,-1,-1,-1,-1,
									-1,-1,-1,-1,-1,-1};

void CSoundMediator::think(int _frames)
{
	int			i;
	int			diff,speed;
	int			*current,*target,*dirty;

	ASSERT(s_initialised);


{
int id;

id=-1;
for(i=10;i<24;i++)
{
	if(id!=s_spuChannelUse[i])
	{
		id=s_spuChannelUse[i];
		if(id!=-1)
		{
			if(!s_xmplaySound->isSfxActive((xmPlayingSongId)id))
			{
PAUL_DBGMSG("%d end.. ( was on chnl %d )",id,i);
				while(s_spuChannelUse[i]==id&&i<24)
				{
					s_spuChannelUse[i++]=-1;
				}
			}
		}
	}

}
}






	// Fade to target volume
	speed=(_frames*VOLUME_CHANGE_SPEED)>>10;
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

	// Manual update of anything that needs it
//	CXAStream::ControlXA();

	// Push through any changes in volume
	if(s_volumeDirty[SONG]||s_volumeDirty[SONGFADE])
	{
		if(s_songPlayingId!=NOT_PLAYING)
		{
			int vol=(s_currentVolume[SONG]*((s_currentVolume[SONGFADE]>>1)+128))>>8;
			s_xmplaySound->setVolume(s_songPlayingId,(char)vol);
		}
		s_volumeDirty[SONG]=s_volumeDirty[SONGFADE]=false;
	}
	if(s_volumeDirty[SFX])
	{
		s_volumeDirty[SFX]=false;
	}
//	if(s_volumeDirty[SPEECH])
//	{
//		int vol=s_currentVolume[SPEECH]<<7;
//		CXAStream::SetVolume(vol,vol);
//		s_volumeDirty[SPEECH]=false;
//	}
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
	ASSERT(s_songDataId==NO_SONG);

	song=&s_xmSongData[_songId];
	s_songDataId=s_xmplaySound->loadSongData(song->m_pxm);
	s_songSampleId=s_xmplaySound->loadSamples(song->m_vh,song->m_vb);
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
	ASSERT(s_songDataId!=NO_SONG);
	ASSERT(s_songPlayingId==NOT_PLAYING);

	s_songPlayingId=s_xmplaySound->playSong(s_songSampleId,s_songDataId,SONG_BASE_CHANNEL);
	s_volumeDirty[SONG]=true;		// Force a volume update
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
	ASSERT(s_songDataId!=NO_SONG);

	if(s_songPlayingId!=NOT_PLAYING)s_xmplaySound->stopSong(s_songPlayingId);
	s_xmplaySound->dumpSamples(s_songSampleId);
	s_xmplaySound->dumpSongData(s_songDataId);

	s_songSampleId=NO_SAMPLE;
	s_songDataId=NO_SONG;
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
	ASSERT(s_sfxDataId==NO_SONG);
	
	song=&s_xmSfxData[_bankId];
	s_sfxDataId=s_xmplaySound->loadSongData(song->m_pxm);
	s_sfxSampleId=s_xmplaySound->loadSamples(song->m_vh,song->m_vb);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */


int CSoundMediator::playSfx(int _sfxId)
{
	ASSERT(s_sfxSampleId!=NO_SAMPLE);
	ASSERT(s_sfxDataId!=NO_SONG);

	int			baseChannel;
	int			channelCount=0;
	int			sfxChannelMask,maskCopy;
	int			i,j;
	int			valid;
	int			maskCheck=1;
	int			playId;

sfxChannelMask=3;

	// Count channels
	maskCopy=sfxChannelMask;
	for(i=0;i<24&&maskCopy;i++)
	{
		if(maskCopy&1)
			channelCount++;
		maskCopy>>=1;
	}

	// Find some spare channels to play on
	valid=true;
	for(i=SFX_BASE_CHANNEL;i<NUM_SPU_CHANNELS-channelCount+1;i++)
	{
		valid=true;
		for(j=i;j<i+channelCount&&valid;j++)
		{
			if(s_spuChannelUse[j]!=-1) valid=false;
		}
		if(valid==true) break;
	}
	ASSERT(valid!=false);
	baseChannel=i;

	// Play!
	playId=s_xmplaySound->playSfx(s_sfxSampleId,s_sfxDataId,baseChannel,_sfxId,sfxChannelMask);
	s_volumeDirty[SFX]=true;		// Force a volume update

// Clear any dead channels
//for(i=SFX_BASE_CHANNEL;i<NUM_SPU_CHANNELS;i++)
//	if(s_spuChannelUse[i]==playId)s_spuChannelUse[i]=-1;

	// Mark channels as active
	for(i=baseChannel;i<baseChannel+channelCount;i++)
	{
		s_spuChannelUse[i]=playId;
	}

	return 0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSoundMediator::playSpeech(SpeechEquate _speech)
{
//	if(CXAStream::IsPlaying())
//		CXAStream::Stop();
//	CXAStream::PlaySpeech(_speech);
//	s_volumeDirty[SPEECH]=true;		// Force a volume update
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


/*===========================================================================
 end */