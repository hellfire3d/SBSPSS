/*****************************/
/*** PSX CDXA Player Stuff ***/
/*** Another one by Dave   ***/
/*****************************/

#ifndef __CDXAHeader__
#define __CDXAHeader__

#include	"sound/Speech.h"

/*****************************************************************************/


/*****************************************************************************/

/*****************************************************************************/

class	CXAStream
{
public:
	enum
	{
		MIN_VOLUME=0,
		MAX_VOLUME=0x7fff,
	};
	
	CXAStream(){};
	~CXAStream(){};

	static	void 		Init();
	static	void 		Start(int Str,u32 Sector,u32 Channel,s32 LVol,s32 RVol);
	static	void 		Stop();

	static	void 		PlayMusic(u32 Track);
	static	void 		PlaySpeech(SpeechEquate SpeechNo,int ForcePlay=0);

	static	void 		ControlXA();
	static	int			IsPlaying()					{return(Mode==XA_MODE_PLAY);}

	static void			setMasterVolume(int _volumeL,int _volumeR);

	
//private: Paul, can you sort out the access functions
	enum	XA_ENUM
	{
		XA_MUSIC_TRACK		=1,
		XA_SPEECH_TRACK		=1,
//		XA_QUEUE_MAX		=8,
		XA_TRACK_MAX		=32,
		XA_DEFAULT_VOL		=32000,
		XA_CHANNEL_SHIFT	=16,
		XA_CHANNEL_MASK		=(0xffffffff<<XA_CHANNEL_SHIFT),
		XA_SPEECH_MASK		=(0xffffffff-XA_CHANNEL_MASK),
	};

	enum	XA_STREAMS
	{
		XA_STREAM_MUSIC=0,
		XA_STREAM_SPEECH,
			
		XA_STREAM_MAX
	};
	
	enum	XA_MODE
	{
			XA_MODE_NOTINIT=-1,
			XA_MODE_IDLE=0,
			XA_MODE_START,
			XA_MODE_PLAY,
			XA_MODE_END,
			XA_MODE_PAUSE,
			XA_MODE_RESUME,
			XA_MODE_STOP,
	};

	struct	sXAEntry
	{
			u32		Channel;
			u32		StartSector,CurrentSector;
			s32		LVol,RVol;
	};

	struct	sXAStream
	{
			int			BaseChannel;
			sXAEntry	Entry;
			CdlLOC 		CDPos;
	} ;


	static	void		SetVolumeOff();
	static	void		SetVolume(s32 LVol,s32 RVol);
	static	void		Interrupt();
	static	void		Pause();
	static	void		Resume();
	static	void		SetSector(u32 Sector)		{StartSector=Sector;}
	static	void		SetLanguage(int Lang)		{Stream[XA_STREAM_SPEECH].BaseChannel=XA_SPEECH_TRACK+Lang;}
	
	static	void		Reset();

	static	XA_MODE		Mode;
	static	int			Status;
	static	int			StartSector;
	static	sXAStream	Stream[XA_STREAM_MAX];
	static	int			CurrentStream;
	static	int			PauseFlag;
	
	static	int			CurrentChannel;
	
	// Speech
//	static SpeechEquate	Queue[XA_QUEUE_MAX];
//	static	u16			QueueCount;
	static	int			SpeechChannel;
	
	static int			s_masterVolumeL,s_masterVolumeR;

	
	static void 	XACDReadyCallback(int Intr, u8 *Result);

};

#endif
