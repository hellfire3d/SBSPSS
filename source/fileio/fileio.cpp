/***************************************/
/*** PSX PC/CD Big Lump FileIO Stuff ***/
/***************************************/

#include	"system\global.h"
#include	"fileio\fileio.h"
#if			__FILE_SYSTEM__==PC
#include	"fileio\pcfile.h"
#else
#include	"fileio\cdfile.h"
#endif
#include	"utils\replace.h"
#include	"utils\utils.h"

//#define	FILEIO_DBG

#ifdef	FILEIO_DBG
#define	FILEIO_DBGMSG	SYSTEM_DBGMSG
#else
#define	FILEIO_DBGMSG	//

#endif

char	*LumpNames[]=
{
"BIGLUMP.BIN",
};

/*****************************************************************************/
sBigLump	CFileIO::BigLump;
CLOFileIO	*CFileIO::FileIO;
int			CFileIO::FilePosList[FILEPOS_MAX];

sFAT		*CFileIO::MainFAT=0;
sASyncQueue	CFileIO::ASyncQueue;
bool		CFileIO::ASyncFlag;
bool		CFileIO::LogFlag;

/*****************************************************************************/
sDataBank	CFileIO::DataBank[DATABANK_MAX]=
{
/*	
	{SYSTEM_CACHE	,0},
	{FRONTEND_CACHE	,0},
	{LEVEL1_CACHE	,0},
	{LEVEL2_CACHE	,0},
	{LEVEL3_CACHE	,0},
	{LEVEL4_CACHE	,0},
	{LEVEL5_CACHE	,0},
	{LEVEL6_CACHE	,0},
	{LEVEL7_CACHE	,0},
	{LEVEL8_CACHE	,0},
	{BACKEND_CACHE	,0},
*/	
};

DataBankEquate	CFileIO::CurrentDataBank=DATABANK_MAX;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//sFAT	*FAT;
void	CFileIO::Init()
{
#if		__FILE_SYSTEM__==PC
		FileIO=new ("CFileIO::FileIOInit") CPCFileIO(LumpNames[DataLump],&BigLump);
#else
		FileIO=new ("CFileIO::FileIOInit") CCDFileIO(0,&BigLump);
#endif
		BigLump.Status=BLStatusReady;

int		FATSize=FileEquate_MAX*sizeof(sFAT);
// Read Main FAT (special case load)
		MainFAT=(sFAT *)MemAlloc( FATSize,"MainFAT");

		FILEIO_DBGMSG("IoInit:FATSIZE=%i",FATSize);
		FileIO->Open();

		BigLump.Status = BLStatusOpen;
		BigLump.Sector=0;
		BigLump.Length=FATSize;
		BigLump.LengthLeft=BigLump.Length;
		BigLump.ReadSoFar=0;
		BigLump.LoadMode = FILEIO_MODE_NONE;
		BigLump.ChunkLeft=0;
		ReadFile(MainFAT,FATSize);
		CloseFile();

		for (int Bank=0;Bank<DATABANK_MAX; Bank++) DataBank[Bank].Data=0;
//		loadDataBank(DATABANK_SYSTEM);
//		CurrentDataBank=DATABANK_MAX;
		ASyncQueue.Status=BLStatusOffline;
		ASyncFlag=false;
		LogFlag=false;
#if	defined(__USER_daveo__)
		LogFlag=true;
#endif
}


/*****************************************************************************/
/*** Open ********************************************************************/
/*****************************************************************************/
void	CFileIO::OpenFile( FileEquate file )
{
	ASSERT(MainFAT);
	FileIO->Open();
	BigLump.Status = BLStatusOpen;
	BigLump.Sector = getFileSector( ( file ) );
	BigLump.Length = getFileSize( ( file ) );
	BigLump.LengthLeft = BigLump.Length;
	BigLump.ReadSoFar=0;
	BigLump.LoadMode = FILEIO_MODE_NONE;
	BigLump.ChunkLeft=0;
}

/*****************************************************************************/
/*** Read ********************************************************************/
/*****************************************************************************/
void   	CFileIO::CheckChunk()
{
int	ChunkCount;
	if (!BigLump.ChunkLeft)
		{
		ChunkCount=BigLump.LengthLeft/2048;
		if (ChunkCount>FILEIO_MAXCHUNKS) ChunkCount=FILEIO_MAXCHUNKS;
		else
		if (ChunkCount<FILEIO_MINCHUNKS) ChunkCount=FILEIO_MINCHUNKS;
	   	BigLump.ChunkPtr=BigLump.ChunkBuffer;
	   	BigLump.ChunkLeft=ChunkCount*2048;
	   	FileIO->Read(ChunkCount, (char*)BigLump.ChunkPtr);
		}
}

/*****************************************************************************/
long CFileIO::ReadFile( void * Buffer, s32 Length )
{
int	ThisLoadSize;

	BigLump.LoadLeft=Length;
	BigLump.DstPtr=(u8*)Buffer;

	while (BigLump.LoadLeft)
		{
		if (BigLump.LoadMode==FILEIO_MODE_NONE)
			{
			if (BigLump.LoadLeft>=FILEIO_CHUNKSIZE)
				BigLump.LoadMode = FILEIO_MODE_CHUNK;
			else
				BigLump.LoadMode = FILEIO_MODE_STREAM;
			}

		switch (BigLump.LoadMode)
		    {
		    case FILEIO_MODE_CHUNK:			// Load large chunk
				ThisLoadSize=BigLump.LoadLeft-(BigLump.LoadLeft%FILEIO_CHUNKSIZE);
			   	FileIO->Read(ThisLoadSize/FILEIO_CHUNKSIZE, (char*)BigLump.DstPtr);
				BigLump.DstPtr+=ThisLoadSize;
				BigLump.LoadLeft-=ThisLoadSize;
				BigLump.LengthLeft-=ThisLoadSize;
				BigLump.ReadSoFar+=ThisLoadSize;
				BigLump.LoadMode=FILEIO_MODE_NONE;
				break;
	
		    case FILEIO_MODE_STREAM:		// Stream chunk
				CheckChunk();
				ThisLoadSize=BigLump.LoadLeft;
				if (ThisLoadSize>BigLump.ChunkLeft) ThisLoadSize=BigLump.ChunkLeft;
	        	if (BigLump.DstPtr) MCmemcpy(BigLump.DstPtr, BigLump.ChunkPtr, ThisLoadSize );

				BigLump.ChunkLeft-=ThisLoadSize; 
				BigLump.DstPtr+=ThisLoadSize;
				BigLump.ChunkPtr+=ThisLoadSize;
				BigLump.LoadLeft-=ThisLoadSize;
				BigLump.LengthLeft-=ThisLoadSize;
				BigLump.ReadSoFar+=ThisLoadSize;
				if (!BigLump.ChunkLeft) BigLump.LoadMode=FILEIO_MODE_NONE;
				break;
			default:
				ASSERT(!"Unknown Load Type!");
		    }
		}

	return (BigLump.LoadLeft);
}

/*****************************************************************************/
/*** Load ********************************************************************/
/*****************************************************************************/
u8 * CFileIO::loadFile( FileEquate file, char *allocName )
{
	u8 *	buffer;
	s32		Length;

// Is Already loaded in a databank?
	buffer=isFileInDataBank(file);
	if (buffer) return(buffer);

	Length = getFileSize( file );

	if (LogFlag)
	{
		SYSTEM_DBGMSG("File: %i = %i",(int)file,Length);
	}

	buffer = (u8 *)MemAlloc( Length ,allocName);
	ASSERT( buffer );
#if defined(__CLIMAX_DEVKIT__)
	EnterCriticalSection();
#endif
	OpenFile( file );
	ReadFile( buffer, Length );
	CloseFile();
#if defined(__CLIMAX_DEVKIT__)
	ExitCriticalSection();
#endif

	return buffer;
}

/*****************************************************************************/
int		CFileIO::IsFromDataBank(void *Addr)
{
// See if we can find the file by its address
// Simply check if address is within a bank range
// Dont check start address, as this will prevent the bank from being freed
		for (int Bank=0;Bank<DATABANK_MAX; Bank++) 
			{
			sDataBank	&ThisBank=DataBank[Bank];
			if (ThisBank.Data)
				{
				int	Size=getFileSize(ThisBank.Filename);
				u8	*StartAddr=(u8*)ThisBank.Data;
				u8	*EndAddr=StartAddr+Size;
				if (Addr>StartAddr && Addr <=EndAddr) return(1);
				}
			}
	return(0);
}

/*****************************************************************************/
u8 * CFileIO::loadFileAtAddr( FileEquate file, u8* buffer)
{
	s32		Length;

	Length = getFileSize( file );

	ASSERT( buffer );
#if defined(__CLIMAX_DEVKIT__)
	EnterCriticalSection();
#endif
	OpenFile( file );
	ReadFile( buffer, Length );
	CloseFile();
#if defined(__CLIMAX_DEVKIT__)
	ExitCriticalSection();
#endif

	return buffer;
}

/*****************************************************************************/
/*** Close *******************************************************************/
/*****************************************************************************/
void 	CFileIO::CloseFile()
{
	FileIO->Close();
	BigLump.Status=BLStatusReady;
}


/*****************************************************************************/
void 	CFileIO::LoadFileSizeAtAddr(FileEquate Filename,void *Buffer,u32 Length)
{
#if defined(__CLIMAX_DEVKIT__)
	EnterCriticalSection();
#endif
	OpenFile(Filename);
	ReadFile(Buffer,Length);
	CloseFile();
#if defined(__CLIMAX_DEVKIT__)
	ExitCriticalSection();
#endif


}

/*****************************************************************************/
void 	CFileIO::AlignFile(int Align)
{
int		Length = ((BigLump.ReadSoFar+Align-1)&-Align)-BigLump.ReadSoFar;

	if (Length)
		{
		ReadFile( NULL, Length );
		}
}

/*****************************************************************************/

s32		CFileIO::getFileSize( FileEquate file )
{
s32	Ret=MainFAT[file].FileSize;
	return (Ret);
}


s32	CFileIO::getFileSector( FileEquate file )
{
s32	Ret=MainFAT[file].FilePos/2048;
	return (Ret);
}

s32	CFileIO::getFileOffset( FileEquate file )
{
s32	Ret=MainFAT[file].FilePos;
	return (Ret);
}

/*****************************************************************************/
void 	CFileIO::SkipFile(int Skip)
{
	if (Skip) ReadFile(0,Skip);
}

/*****************************************************************************/
int		CFileIO::GetFilePos(int File)
{
	return(FilePosList[File]);
}

/*****************************************************************************/
#if		!defined(__USER_CDBUILD__)
#include	"fileio/filetab.cpp"
#endif

void	CFileIO::FindAllFilePos()
{
#if		!defined(__USER_CDBUILD__)
		CalcFilePos(FilePosList);
#endif
}

/*****************************************************************************/
// File positions are passed by Bootstrap VIA Scratch Ram (nice!)
void	CFileIO::GetAllFilePos()
{
int	*Pos=(int*)SCRATCH_RAM;

	for (int Loop=0;Loop<FILEPOS_MAX;Loop++)	
		{
		FilePosList[Loop]=*Pos++;
		}

}

/****************************************************************************/
int	CFileIO::GetReadLeft()
{
	return(BigLump.LengthLeft);
}

/*****************************************************************************/
void	CFileIO::AddASyncFile(FileEquate file,u8 *Dst)
{
		ASyncQueue.Sector = getFileSector(file);
		ASyncQueue.Length = getFileSize(file )/2048;
		ASyncQueue.Dst=Dst;
		ASyncQueue.Status=BLStatusReady;
}

/*****************************************************************************/
void	CFileIO::LoadASyncFiles()
{
		if (ASyncQueue.Status!=BLStatusOffline && ASyncFlag)
		{
			FileIO->ReadAsync(ASyncQueue);
		}
}

/*****************************************************************************/
/*** Data Banks **************************************************************/
/*****************************************************************************/
void	CFileIO::loadDataBank(DataBankEquate Bank)
{
// Check if already loaded
		if (DataBank[Bank].Data) return;

// Check to see no more are loaded
		ASSERT(CurrentDataBank==DATABANK_MAX);

		DataBank[Bank].Data=(sFAT*)loadFile(DataBank[Bank].Filename,"DataBank");
		CurrentDataBank=Bank;
}

/*****************************************************************************/
void	CFileIO::dumpDataBank()
{
// Check if loaded
		ASSERT(CurrentDataBank!=DATABANK_MAX);
		MemFree(DataBank[CurrentDataBank].Data);
		DataBank[CurrentDataBank].Data=0;
		CurrentDataBank=DATABANK_MAX;
}

/*****************************************************************************/
u8		*CFileIO::isFileInDataBank(FileEquate File)
{
		for (int Bank=0;Bank<DATABANK_MAX; Bank++) 
			{
			sDataBank	&ThisBank=DataBank[Bank];
			if (ThisBank.Data)
				{
				int		Pos=ThisBank.Data[File].FilePos;
				if (Pos!=-1) return(MakePtr(ThisBank.Data,Pos));
				}
			}
	FILEIO_DBGMSG("File Not in Cache [%i]",File);

		return(0);
}
