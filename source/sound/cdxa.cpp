/*****************************/
/*** PSX CDXA Player Stuff ***/
/*** Another one by Dave   ***/
/*****************************/

// Note, need to add blanks in between tracks
#include	"system\global.h"
#include	"fileio/fileio.h"
#include	"fileio/filetab.h"
#include 	"sound\cdxa.h"

#include 	<libcd.h>
#include 	<libsnd.h>

//#ifndef __SOUND_SNDBANK_H__
//#include "sound\sndbank.h"
//#endif


// Add this to have CDXA on PC build!!
// You will need a CD with Track1 synced to something, oh, and a CD drive
#if	defined(__USER_paul__)//|| defined(__USER_daveo__)
#define	FORCE_XA	1	
#endif

#if		__FILE_SYSTEM__==CD | FORCE_XA 
#define	ENABLE_XA	
#endif

#if		defined(__USER_CDBUILD__)
#undef	FORCE_XA
#endif

/*****************************************************************************/
XA_MODE			CXAStream::Mode=XA_MODE_NOTINIT;
int				CXAStream::Status;
int				CXAStream::StartSector;
sXAStream		CXAStream::Stream[XA_STREAM_MAX];
int				CXAStream::CurrentStream;
int				CXAStream::PauseFlag;

// Speech
SpeechEquate	CXAStream::Queue[XA_QUEUE_MAX];
u16				CXAStream::QueueCount;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void 	XACDReadyCallback(int Intr, u8 *Result)
{
static int	ErrorRetry;
u32			XABuffer[8];
u16			ID,Track;
sXAStream	&ThisStream=CXAStream::Stream[CXAStream::CurrentStream];

	CXAStream::Status=Intr;
	if (CXAStream::Mode==XA_MODE_IDLE)	return;

	switch (Intr)
    	{
		case CdlNoIntr:
			break;
		case CdlDataReady:
// Check end of XA using video termination
			ErrorRetry=0;
			CdGetSector((u_long *)XABuffer,8);
			ID = *(unsigned short *)(XABuffer+3);
			Track = *((unsigned short *)(XABuffer+3)+1);
			Track = (Track&31744)>>10;
			if (Track==0)
				{
				ThisStream.Entry.CurrentSector++;					// track position
				}
			else
			{
			if (ID==352)
		        {
				if (Track==ThisStream.Entry.Channel)
			        {
//					DbgMsg0("TrackEnd\n");
					CXAStream::SetVolumeOff();
	  				CdControlF(CdlPause,0);
					CXAStream::Mode=XA_MODE_END;
			        }
				}
			}
			break;
		case CdlComplete:
			break;
		case CdlAcknowledge:
			break;
		case CdlDataEnd:
			break;
		case CdlDiskError:
			if (!ErrorRetry)
	            {
				ErrorRetry=25;
//				CXAStream::Mode=XA_MODE_RESUME;
	            }
			else
				ErrorRetry--;
			break;
		default:
			break;
	    }
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CXAStream::Init()
{
#ifdef	FORCE_XA
			SYSTEM_DBGMSG("FORCE XA\n");	
			while (!CdInit());
			CFileIO::FindAllFilePos();
			CXAStream::SetSector(CFileIO::GetFilePos(FILEPOS_TRACK1));

#endif
		SYSTEM_DBGMSG("XA INITIALISED");

// Set defaults
		CurrentStream=XA_STREAM_MUSIC;
		Stream[XA_STREAM_MUSIC].BaseChannel =XA_MUSIC_TRACK;
		Stream[XA_STREAM_SPEECH].BaseChannel=XA_SPEECH_TRACK;
		Reset();
}

/*****************************************************************************/
void 	CXAStream::Start(int Str,u32 Sector,u32 Channel,s32 LVol,s32 RVol)
{
#ifdef	ENABLE_XA
		if (Mode==XA_MODE_NOTINIT) Init();
		
sXAStream	&ThisStream=Stream[Str];
		SetVolumeOff();
		CdControlF(CdlPause,0);
		CurrentStream=Str;
		ThisStream.Entry.StartSector=Sector;
		ThisStream.Entry.CurrentSector=Sector;
		ThisStream.Entry.Channel=Channel+ThisStream.BaseChannel;
		ThisStream.Entry.LVol=LVol;
		ThisStream.Entry.RVol=RVol;
		Mode=XA_MODE_START;
		CdReadyCallback((CdlCB)XACDReadyCallback);
#endif
}

/*****************************************************************************/
void 	CXAStream::Stop()
{
		Reset();
		Mode=XA_MODE_STOP;
}

/*****************************************************************************/
void 	CXAStream::PlayMusic(u32 TrackNo)
{
		Start(XA_STREAM_MUSIC,0,0,XA_DEFAULT_VOL,XA_DEFAULT_VOL);
}

/*****************************************************************************/

void 	CXAStream::PlaySpeech(SpeechEquate SpeechNo,int ForcePlay)
{
u32		Channel=SpeechNo>>XA_CHANNEL_SHIFT;
u32		Speech=SpeechNo & XA_SPEECH_MASK;
u32		Sector=Speech*XA_TRACK_MAX;

		if (CurrentStream==XA_STREAM_SPEECH && Mode==XA_MODE_PLAY && !ForcePlay)
		    {
			// Check Current
			if (Stream[CurrentStream].Entry.Channel==Channel && Stream[CurrentStream].Entry.StartSector==Sector) return;
			// Check Queue
			if (QueueCount>=(int)XA_QUEUE_MAX) return;
			for (int Loop=0;Loop<QueueCount ; Loop++) if (Queue[Loop]==SpeechNo) return;
			Queue[QueueCount++]=SpeechNo;
			return;
			}
	SetVolumeOff();
	Start(XA_STREAM_SPEECH,Sector,Channel,XA_DEFAULT_VOL,XA_DEFAULT_VOL);
}

/*****************************************************************************/
void	CXAStream::Pause()
{
		Mode=XA_MODE_PAUSE;
		PauseFlag=1;
}

/*****************************************************************************/
void	CXAStream::Resume()
{
		if (PauseFlag)
	        {
			Mode=XA_MODE_RESUME;
			PauseFlag=0;
	        }
}

/*****************************************************************************/
void 	CXAStream::ControlXA()
{
#ifdef	ENABLE_XA
CdlFILTER	theFilter;
u8	 		Cmd[4];
sXAStream	&ThisStream=Stream[CurrentStream];

		switch(Mode)
			{
			case	XA_MODE_IDLE:
				break;
			case	XA_MODE_RESUME:
				Status=0;
			case	XA_MODE_START:
				SetVolumeOff();
				Cmd[0] = CdlModeSpeed|CdlModeRT|CdlModeSF|CdlModeSize1;
				CdControlB(CdlSetmode,Cmd, 0);
				CdIntToPos(ThisStream.Entry.CurrentSector+StartSector,&ThisStream.CDPos);
				theFilter.file=1;
//				theFilter.chan=ThisStream.BaseChannel+ThisStream.Entry.Channel;
				theFilter.chan=ThisStream.Entry.Channel;
				CdControlF(CdlSetfilter, (u8*)&theFilter);
				CdControlF(CdlReadS,(u8*)&ThisStream.CDPos);
				Mode=XA_MODE_PLAY;
				break;
			case	XA_MODE_PLAY:
				if (Status==CdlDiskError) Mode=XA_MODE_RESUME;
				SetVolume(XA_DEFAULT_VOL,XA_DEFAULT_VOL);
				break;
			case	XA_MODE_END:
				SetVolumeOff();
				if (CurrentStream==XA_STREAM_SPEECH)
	                {
					if (QueueCount)
	                    {
						PlaySpeech(Queue[0]);
						for (int Loop=0;Loop<(int)XA_QUEUE_MAX-1;Loop++) Queue[Loop]=Queue[Loop+1]; // shuffle queue in a crap way!!
						QueueCount--;
	                    }
					else
	                    {				
						Mode=XA_MODE_PAUSE;
	                    }
	                }
				else
	                {
					ThisStream.Entry.CurrentSector=ThisStream.Entry.StartSector;
					Mode=XA_MODE_START;
	                }
				break;
			case	XA_MODE_PAUSE:
				SetVolumeOff();
  				CdControlF(CdlPause,0);
				Mode=XA_MODE_IDLE;
				break;
			case	XA_MODE_STOP:
					ThisStream.Entry.CurrentSector=ThisStream.Entry.StartSector;
					SetVolumeOff();
	  				CdControlF(CdlPause,0);
				break;
			default :
				break;
			}
#endif
}

/*****************************************************************************/
void	CXAStream::Interrupt()
{
		Mode=XA_MODE_END;
}

/*****************************************************************************/
void	CXAStream::Reset()
{
#ifdef	ENABLE_XA
		SetVolumeOff();
//		if (Mode!=XA_MODE_NOTINIT) 
			{
			CdControlF(CdlPause,0);
			Mode=XA_MODE_IDLE;
			}
// Clear Queue
		for (int Loop=0;Loop<(int)XA_QUEUE_MAX; Loop++) Queue[Loop]=0;
		QueueCount=0;
#endif		
}

/*****************************************************************************/
void	CXAStream::SetVolume(s32 LVol,s32 RVol)
{
CdlATV			CDVol;
SpuCommonAttr	Attr;
//int				VolumeSetting;
//
//		if (CurrentStream==XA_STREAM_SPEECH)
//			VolumeSetting=CSfxFactory::SNDVOL_SFX;
//		else
//			VolumeSetting=CSfxFactory::SNDVOL_MUSIC;
//
//		LVol=(LVol*CSfxFactory::getVolumeLevel(VolumeSetting))/256;
//		RVol=(RVol*CSfxFactory::getVolumeLevel(VolumeSetting))/256;
		Attr.mask = (SPU_COMMON_CDVOLL|SPU_COMMON_CDVOLR|SPU_COMMON_CDMIX); 
		Attr.cd.volume.left	=LVol;
		Attr.cd.volume.right=RVol;
		Attr.cd.mix=SPU_ON;
		SpuSetCommonAttr(&Attr);
		CDVol.val0 = 127;		// CdL -> SpuL
		CDVol.val1 = 127;		// CdL -> SpuR
		CDVol.val2 = 127;		// CdR -> SpuR
		CDVol.val3 = 127;		// CdR -> SpuL
		CdMix(&CDVol);

}

/*****************************************************************************/
void	CXAStream::SetVolumeOff()
{
CdlATV			CDVol;
SpuCommonAttr	Attr;
//		SsSetSerialVol(SS_SERIAL_A,0,0);

		
		Attr.mask = (SPU_COMMON_CDVOLL|SPU_COMMON_CDVOLR|SPU_COMMON_CDMIX); 
		Attr.cd.volume.left	=0;
		Attr.cd.volume.right=0;
		Attr.cd.mix=SPU_ON;
		SpuSetCommonAttr(&Attr);
		CDVol.val0 = 0;		// CdL -> SpuL
		CDVol.val1 = 0;		// CdL -> SpuR
		CDVol.val2 = 0;		// CdR -> SpuR
		CDVol.val3 = 0;		// CdR -> SpuL
		CdMix(&CDVol);
		
}


