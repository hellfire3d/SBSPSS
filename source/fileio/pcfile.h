/***************************/
/*** PSX PC FileIO Stuff ***/
/***************************/

#ifndef __PCFILE_H__
#define __PCFILE_H__

#include	"fileio\fileio.h"
/*****************************************************************************/
enum
{
	F_READ,
	F_WRITE,
	F_READWRITE,
};

/*****************************************************************************/
class CPCFileIO : public CLOFileIO
{
public:
	CPCFileIO(char *Filename,sBigLump *BigLumpPtr);
	virtual ~CPCFileIO(){};

	void 		Open();
	void 		Read(u32 Length,void *Buffer);
	void 		Close();

private:
	sBigLump	*BigLump;
	s32			FileHandle;
	char		FullFilename[256];

};

/*****************************************************************************/
#endif