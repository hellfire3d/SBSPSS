/***********************************/
/*** PSX PC BigLump FileIO Stuff ***/
/***********************************/

#include 	"system\global.h"
#include	"fileio\fileio.h"
#include 	"fileio\pcfile.h"

/*****************************************************************************/
CPCFileIO::CPCFileIO(char *Filename,sBigLump *BigLumpPtr)
{
int		NoPath=0;
		PCinit();
		sprintf(FullFilename,"out\\%s\\%s\\version\\%s\\%s",INF_Territory,INF_Version,INF_FileSystem,Filename);
		BigLump=BigLumpPtr;
		FileHandle=-1;
}

/*****************************************************************************/
void 	CPCFileIO::Open()
{
		FileHandle=PCopen(FullFilename,F_READ,0);
		ASSERT(FileHandle!=-1);
}

/*****************************************************************************/
void 	CPCFileIO::Read(u32 Count,void *Dst)
{
		PClseek(FileHandle,BigLump->Sector*FILEIO_CHUNKSIZE,0);
		PCread(FileHandle,(char*)Dst,Count*FILEIO_CHUNKSIZE);
		BigLump->Sector+=Count;
}

/*****************************************************************************/
void 	CPCFileIO::Close()
{
		if (FileHandle!=-1) PCclose(FileHandle);
		FileHandle=-1;

}
