/***************************************/
/*** PSX PC/CD Big Lump FileIO Stuff ***/
/***************************************/

#ifndef _FILEIO_HEADER_
#define _FILEIO_HEADER_

#include 	"BigLump.h"
#include	"fileio\filetab.h"

/*****************************************************************************/
#define FILEIO_MINCHUNKS		(1)		// 2k
#define FILEIO_MAXCHUNKS		(1)		// 2k	64k (only needed for small files, daft streams and end of files now)
#define FILEIO_CHUNKSIZE		(2048)
#define FILEIO_MINCHUNKSIZE		(FILEIO_CHUNKSIZE*FILEIO_MINCHUNKS)
#define FILEIO_MAXCHUNKSIZE		(FILEIO_CHUNKSIZE*FILEIO_MAXCHUNKS)

enum
{
FILEIO_MODE_NONE=0,
FILEIO_MODE_CHUNK,
FILEIO_MODE_STREAM,
};

enum	
{
BLStatusOffline=-1,
BLStatusReady,
BLStatusOpen,
BLStatusRead,
BLStatusFail
};

enum	
{
DataLump=0,
OverlayLump,
MaxLumps
};

struct	sFAT
{
	s32	FilePos;
	s32	FileSize;
};

struct	sBigLump
	{
	s16		Status;
	u32		Sector;
	u8		*ChunkPtr,*DstPtr;
	int		ChunkLeft;
	int		Length,LengthLeft,ReadSoFar;
	int		LoadLeft;
	int		LoadMode;
	u8		ChunkBuffer[FILEIO_MAXCHUNKSIZE];
	};


/*****************************************************************************/
enum DataBankEquate
{
DATABANK_MAX
};

struct	sDataBank
{
	FileEquate	Filename;
	sFAT		*Data;
};

/*****************************************************************************/
class CLOFileIO;
class CFileIO
{
public:

static	void		Init();
static	void 		OpenFile(FileEquate file);
static	long 		ReadFile(void *Buffer,s32 Length);
static	void 		CloseFile();
static	void 		AlignFile(int Length);
static	void 		SkipFile(int Skip);

static	int			GetStatus()	{return(BigLump.Status);}

static	void 		LoadFileSizeAtAddr(FileEquate Filename,void *Buffer,u32 Length);

static	u8			*loadFile( FileEquate file, char *allocName="CFileIO::loadFile" );
static	int			IsFromDataBank(void *Addr);
static	u8			*loadFileAtAddr(FileEquate file, u8* buffer);

static	void		loadDataBank(DataBankEquate Bank);
static	void		dumpDataBank();
static	u8			*isFileInDataBank(FileEquate File);

static s32			getFileSize( FileEquate file );
static s32			getFileSector( FileEquate file );
static s32			getFileOffset( FileEquate file );

static	int			GetFilePos(int File);
static	void		FindAllFilePos();
static	void		GetAllFilePos();
static	int			GetReadLeft();

private:
static	void   		CheckChunk();

static	sBigLump	BigLump;
static	CLOFileIO	*FileIO;
static	int			FilePosList[FILEPOS_MAX];

static	sDataBank		DataBank[DATABANK_MAX];
static	DataBankEquate	CurrentDataBank;

static	sFAT		*MainFAT;
};

/*****************************************************************************/
class CLOFileIO
{
public:
	virtual void 	Open()=0;
	virtual void	Read(u32 Length,void *Buffer)=0;
	virtual void 	Close()=0;
};

/*****************************************************************************/

#endif