/*=========================================================================

	GFNAME.HPP

	Author:  Gary Liddon @ Watford
	Created: 2nd March 1991
	Purpose: Filename manipulation class

	Copyright (c) 1991-1997 Gary Lidon

===========================================================================*/

#ifndef __GL_PC_GFNAME__
#define __GL_PC_GFNAME__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

/*	Glib
	---- */
#include "gtypes.h"

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Class defintions
	---------------- */
class GLIB_API GFName
{
protected:
	char TDrive[_MAX_DRIVE+1];
	char TDir[_MAX_DIR+1];
	char TPath[_MAX_PATH+1];
	char TFile[_MAX_FNAME+1];
	char TExt[_MAX_EXT+1];

public:

	GFName(void);
	GFName(char const *zfname);

	~GFName(void);

	char const *FullName(void);
	char const *Drive(){return RetStr(TDrive);}
	char const *Dir(){return RetStr(TDir);}
	char const *File(){return RetStr(TFile);}
	char const *Ext(){return RetStr(TExt);}

	void Drive(char const *new_drive){SetStr(TDrive,new_drive);}
	void Dir(char const *new_dir){SetStr(TDir,new_dir);}
	void File(char const *new_file){SetStr(TFile,new_file);}
	
	void Ext(char const *new_ext)
		{
		if (new_ext)
			{
			if (new_ext[0]=='.')
				SetStr(TExt,new_ext+1);
			else
				SetStr(TExt,new_ext);
			}
		else
			TExt[0]=0;
		}

	void FullName(const char *zfname);

	void operator=(const char * zfname)	{FullName(zfname);}

	void AddDir(char const *add);

	static char *makerelative(const char *basepath, const char *newpath, char *outstr);
	static char *makeabsolute(const char *basepath, const char *offset, char *outstr);

	friend	std::ostream &operator<<(std::ostream & str,GFName & Name);


private:
	const char *	RetStr(const char *);
	void			SetStr(char * Dest,char const * Source);

};


/*---------------------------------------------------------------------- */

#endif	/* __GL_PC_GFNAME__ */

/*===========================================================================
 end */



