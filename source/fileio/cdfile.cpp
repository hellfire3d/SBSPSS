/***********************************/
/*** PSX CD BigLump FileIO Stuff ***/
/***********************************/

#include 	"system\global.h"
#include	"fileio\fileio.h"
#include 	"fileio\cdfile.h"
#include 	<libcd.h>
#include	"system\vid.h"
#include	"sound\cdxa.h"

/*****************************************************************************/
CCDFileIO::CCDFileIO(u32 FilePos,sBigLump *BigLumpPtr)
{
//		DbgMsg0("CD FILE SYSTEM\n");	
		while (!CdInit());

#if		!defined(__USER_CDBUILD__)
		CFileIO::FindAllFilePos();
#endif			
		BigLump=BigLumpPtr;
		FileStart=CFileIO::GetFilePos(FILEPOS_BIGLUMP);
		CXAStream::SetSector(CFileIO::GetFilePos(FILEPOS_TRACK1));
}

/*****************************************************************************/
void 	CCDFileIO::Open()
{
}

/*****************************************************************************/
void 	CCDFileIO::Read(u32 Count,void *Dst)
{
u8		Com=CdlModeSpeed;
int		Error=0;
// seek

		while (!Error)
	        {
			CdIntToPos(BigLump->Sector+FileStart,&CdPos);
	    	CdControlB(CdlSetloc, (u8*)&CdPos, 0);	// Set CD Pos
	    	CdControlB(CdlSetmode, &Com, 0);
			Error=CdRead(Count, (u32*)Dst, CdlModeSpeed);
	        }
		while (CdReadSync(1,0) > 0);
		BigLump->Sector+=Count;
}

/*****************************************************************************/
void 	CCDFileIO::Close()
{
}

