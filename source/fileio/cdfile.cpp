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
void	CCDFileIO::ReadAsync(sASyncQueue &File)
{
u8		Com=CdlModeSpeed;
static	int Error=0;
/*
		switch(File.Status)
		{
		case BLStatusReady:
			CdIntToPos(File.Sector+FileStart,&CdPos);
	    	CdControlF(CdlSetloc, (u8*)&CdPos);	// Set CD Pos
			File.Status=BLStatusOpen;
			break;
		case BLStatusOpen:
			if (CdReadSync(1,0) > 0) return;
	    	CdControlB(CdlSetmode, &Com,0);
			CdRead(File.Length, (u32*)File.Dst, CdlModeSpeed);
			File.Status=BLStatusRead;
			break;
		case BLStatusRead:
			if (CdReadSync(1,0) > 0) return;
			File.Status=BLStatusOffline;

			break;
		}
*/

			CdIntToPos(File.Sector+FileStart,&CdPos);
	    	CdControlB(CdlSetloc, (u8*)&CdPos, 0);	// Set CD Pos
	    	CdControlB(CdlSetmode, &Com, 0);
			Error=CdRead(File.Length, (u32*)File.Dst, CdlModeSpeed);
			if (Error!=0) File.Status=BLStatusOffline;

}

/*****************************************************************************/
void 	CCDFileIO::Close()
{
}

