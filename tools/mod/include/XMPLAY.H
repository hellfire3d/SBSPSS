#ifndef XMPlayer
#define XMPlayer

extern int JPPer;

extern int XMTime1;
extern int XMTime2;
extern int XMTime3;

#define XM_PROCESSING 0
#define XM_NOT_PROCESSED 1

#define XM_NTSC 0				/* Machine type */
#define XM_PAL 1

#define XM_Loop 1				/* Looping song */
#define XM_NoLoop 0			/* Once off song */

#define XM_Music 0			/* Playback as music */
#define XM_SFX 1				/* Playback as SFX */

#define XM_UseXMPanning 0	/* S3M Panning switches */
#define XM_UseS3MPanning 1

#define XM_STOPPED 0			/* Song/SFX Status */
#define XM_PLAYING 1
#define XM_PAUSED  2

extern int XM_SCAN;			/* Scan lines used */
extern int JPError;			/* Test */


typedef struct XM_HeaderInfo
{
		u_short	BPM;
		u_short	Speed;
} XM_HeaderInfo;

typedef struct XM_VABInfo
{
		u_char*		Address;
		u_long		Size;
} XM_VABInfo;

typedef struct XM_Feedback
{
		u_char	Volume;
		short		Panning;
		int		CurrentStart;
		short		PlayNext;
		u_short	SongLength;
		u_char	Status;
		u_short	PatternPos;
		short		SongPos;
		u_short	CurrentPattern;
		u_short	SongSpeed;
		u_short	SongBPM;
		int		SongLoop;
		int		ActiveVoices;
} XM_Feedback;

#ifdef __cplusplus			/* Added by MikeA */
extern "C" {
#endif

void XM_Restart(int Song_ID);
void XM_Pause(int Song_ID);
void XM_Exit(void);
void XM_Update(void);
void XM_PlayStop(int Song_ID);
void XM_PlayStart(int Song_ID,int PlayMask);
void XM_SetSongPos(int Song_ID,u_short pos);
int InitXMData(u_char *mpp,int XM_ID,int S3MPan);
int XM_VABInit(u_char* VHData,u_char* VBData);
void XM_OnceOffInit(int PAL);
int XM_GetFeedback(int Song_ID,XM_Feedback* Feedback);
void XM_GetHeaderInfo(int XM_ID,XM_HeaderInfo* HeaderInfo);
int  XM_Init(int VabID,int XM_ID,int SongID, int FirstCh,
				 int Loop,int PlayMask,int PlayType,int SFXNum);
int XM_GetChVolume(int Song_ID,int Channel);
void XM_SetChVolume(int Song_ID,int Channel,int Volume);
void XM_PlayNext(int Song_ID,short SongPos);
void XM_CPlayNext(int Song_ID,short SongPos);
void XM_CloseVAB(int VabID);
void XM_SetMasterPan(int Song_ID,short Pan);
void XM_SetMasterVol(int Song_ID,u_char Vol);
int XM_SendVAGToSRAM(u_char *addr,int size);
int XM_SendVAGToSRAM_NoWait(u_char *addr,int size);
void XM_FreeVAG(int addr);
void UpdateWithTimer(int SC);
void XM_DoFullUpdate(int SC);

void XM_Quit(int SongID);
int XM_SetSFXRange(int FirstCh,int Amount);
void XM_ClearSFXRange(void);

void XM_StopSample(int channel);
void XM_PlaySample(int addr,int channel,int voll,int volr,int pitch);
int XM_GetSampleAddress(int vabid,int samplenum);
int XM_GetFreeVAB(void);
void XM_SetVAGAddress(int VabID,int slot,int addr);
int XM_GetVABSampleInfo(XM_VABInfo *VInfo,u_char *Header,u_char *Body,int slot);
void XM_CloseVAB2(int VabID);
void XM_PauseAll(void);
void XM_RestartAll(void);

void XM_SetSongAddress(u_char *Address);
int XM_GetSongSize(void);
void XM_FreeAllSongIDs(void);
void XM_FreeSongID(void);
void XM_SetSpeed(int Song_ID,u_short Speed);
void XM_SetBPM(int Song_ID,u_short BPM);

void XM_SetMono(void);
void XM_SetStereo(void);
void XM_FreeAllFileHeaderIDs(void);
void XM_FreeFileHeaderID(void);
void XM_SetFileHeaderAddress(u_char *Address);
int XM_GetFileHeaderSize(void);
void XM_Update2(int speed);


void SetTranspose(int a);

#ifdef __cplusplus			/* Added by MikeA */
}
#endif
#endif
