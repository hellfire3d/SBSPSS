/*=========================================================================

	GFNAME.HPP

	Author:  Gary Liddon @ Watford
	Created: 2nd March 1991
	Purpose: Filename manipulation class

	Copyright (c) 1991-1997 Gary Lidon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <fstream>

/*	Glib
	---- */


/*	Local
	----- */
#include "gfname.hpp"
#include "gstring.hpp"

using namespace std;

/*----------------------------------------------------------------------
	Function:	Destructor
  ---------------------------------------------------------------------- */
GFName::~GFName(void)
{
}

/*----------------------------------------------------------------------
	Function:	Constructor
  ---------------------------------------------------------------------- */
GFName::GFName(void)
{
	TDrive[0]=0;
	TPath[0]=0;
	TDir[0]=0;
	TFile[0]=0;
}


/*----------------------------------------------------------------------
	Function:	Constructor witn 
  ---------------------------------------------------------------------- */
GFName::GFName(const char *zfname)
{
	FullName(zfname);
}

void GFName::FullName(const char *zfname)
{
	if (zfname)
		{
		_splitpath(zfname,TDrive,TDir,TFile,TExt);

		if (strlen(TDir) > 1)
			{
			if (TDir[strlen(TDir)-1]=='\\')
				TDir[strlen(TDir)-1]=0;
			}

		Ext(TExt);
		}
	else
		{
		TDrive[0]=0;
		TPath[0]=0;
		TDir[0]=0;
		TFile[0]=0;
		}
}

/*----------------------------------------------------------------------
	Function:	Make a full name
  ---------------------------------------------------------------------- */
char const *GFName::FullName(void)
{
	_makepath(TPath,TDrive,TDir,TFile,TExt);
	return RetStr(TPath);
}

const char * GFName::RetStr(const char * Val)
{
	if (Val[0]=='\0')
		return(NULL);
	else
		return(Val);
}

void GFName::SetStr(char * Dest,char const * Source)
{
	if (Source)
		{
		char	Temp[1000];
		strcpy(Temp,Source);
		strcpy(Dest,Temp);
		}
	else
		Dest[0]=0;
}

/*----------------------------------------------------------------------
	Function:	Make a full name
  ---------------------------------------------------------------------- */
void GFName::AddDir(char const *add)
{
	GString	CurrDir;
	CurrDir=Dir();

	CurrDir+="\\";
	CurrDir+=add;
	Dir(CurrDir);
}

#define FF_PATHNAMEMAX _MAX_PATH+1

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
//	Convert relative to absolute paths
//	컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
char *GFName::makeabsolute(char const *basepath,char const *offset, char * outstr)
{
	int i, j, len;
	char basefilename[FF_PATHNAMEMAX];

	if (offset[1] == ':') // Drive letter makes it absolute
		strcpy(outstr, offset);
	else
		{
		// Find path of the file
		strcpy(outstr, basepath);
		j = 0;
		len = strlen(outstr);
		for (i = len - 1; i >= 0; i--)
			if (outstr[i] == '\\')
				break;
			else
				outstr[i] = 0;

		// Find the filename
		if (i > 0)
			strcpy(basefilename, &basepath[i+1]);
		else
			basefilename[0] = 0;
		len = strlen(basefilename);
		for (i = len - 1; i >= 0; i--)
			{
			if (basefilename[i] == '.')
				{
				basefilename[i] = 0;
				break;
				}
			else
				basefilename[i] = 0;
			}

		// Is it from the root directory?
		if (offset[0] == '\\')
			{
			outstr[2] = 0; // Leave only the drive letter
			}
		else
			{
			// Parse the ../ paths
			while ((offset[j] == '.') && (offset[j+1] == '.') &&
			  (offset[j+2] == '\\'))
				{
				len = strlen(outstr);
				if (len > 1)
					outstr[len-1] = 0;
				for (i = len - 2; i >= 0; i--)
					if (outstr[i] == '\\')
						break;
					else
						outstr[i] = 0;
				j += 3;
				}
			}

		// Add the remaining offset to the name

		len = strlen(outstr);


		while (offset[j])
			{
/*			if (offset[j] == '#') // Same filename
				{
				strcat(outstr, basefilename);
				len = strlen(outstr);
				}
			else
				{
				outstr[len++] = offset[j];
				outstr[len] = 0;
				}
*/
			outstr[len++] = offset[j];
			outstr[len] = 0;
			j++;
			}
		}
	return outstr;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
//	Convert absolute to relative paths
//	컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
char *GFName::makerelative(char const *basepath,char const *newpath, char *outstr)
{
	char temp1[FF_PATHNAMEMAX];
	char temp2[FF_PATHNAMEMAX];
	int i, j, match;

	// Are the filenames the same?
	match = 0;
	for (j = 0; j < FF_PATHNAMEMAX; j++)
		{
		if (basepath[j] != newpath[j])
			{
			match = 0;
			break;
			}
		else if (basepath[j] == '.') // Matching
			{
			match = 1;
			break;
			}
		else if (basepath[j] == 0) // Identical
			{
			match = 0;
			break;
			}
		}
	if (match) // Matching paths and filenames
		{
		strcpy(outstr, "#");
		strcat(outstr, &newpath[j]);
		}
	else if (basepath[0] != newpath[0]) // Drive letters are different
		strcpy(outstr, newpath);
	else
		{
		// Find the paths
		strcpy(temp1, basepath);
		for (i = strlen(temp1) - 1; i >= 0; i--)
			if (temp1[i] == '\\')
				break;
			else
				temp1[i] = 0;

		strcpy(temp2, newpath);
		for (i = strlen(temp2) - 1; i >= 0; i--)
			if (temp2[i] == '\\')
				break;
			else
				temp2[i] = 0;

		// Are the paths the same?
		strcpy(outstr, "");
		if (strcmp(temp1, temp2) == 0) // Paths are the same
			{
			j = strlen(temp1);
			}
		else // Paths are different
			{
			j = 2; // Drives are the same
			// Find different bits
			for (i = 0; i < (int) strlen(temp1); i++)
				{
				if ((temp1[i] == '\\') && (temp2[i] == '\\'))
					j = i + 1;
				else
					if (temp1[i] != temp2[i])
						break;
				}
			if (j > 3)
				{
				for (i = j; i < (int) strlen(temp1); i++)
					if (temp1[i] == '\\')
						strcat(outstr,"..\\");
				}
			else
				j = 2;
			}
		strcat(outstr, &newpath[j]);
		}
	return outstr;
}


ostream & operator<<(ostream & str,GFName & Name)
{
	str<<"!";

	if (Name.FullName())
		str<<Name.FullName();

	if (Name.Dir())
		str<<"!"<<Name.Dir();

	if (Name.File())
		str<<"!"<<Name.File();

	if (Name.Ext())
		str<<"!"<<Name.Ext();

	str<<"!";

	return(str);
}





/*===========================================================================
 end */
