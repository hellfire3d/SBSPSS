/***************************/
/*** PSX CD FileIO Stuff ***/
/***************************/

#ifndef __CDFILE_H__
#define __CDFILE_H__

#include	"fileio\fileio.h"

/*****************************************************************************/
class CCDFileIO : public CLOFileIO
{
public:
	CCDFileIO(u32 FilePos,sBigLump *BigLumpPtr);
	virtual ~CCDFileIO(){};

	void 		Open();
	void 		Read(u32 Length,void *Buffer);
	void		ReadAsync(sASyncQueue &File);
	void 		Close();

private:
	sBigLump	*BigLump;
	u32 		FileStart;
	CdlLOC		CdPos;
	static	int	HasInit;
};

/*****************************************************************************/
#endif

