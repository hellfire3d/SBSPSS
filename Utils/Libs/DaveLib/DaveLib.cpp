/*************************/
/*** Dave Lib Routines ***/
/*************************/

#include <ginio.h>
#include <gintex.h>
#include "stdio.h"
#include <math.h>
#include <repread.h>
#include <misc.hpp>
#include <conio.h>
#include <iostream.h>
#include <vector>
#include <gfname.hpp>
#include "psxtypes.h"
#include "FaceStore.h"
#include <io.h>

#include "..\..\..\tools\data\include\dStructs.h"

#include "DaveLib.h"

GString OutStr,IncludeFile;
bool    DebugOn= false;
float	Scale=1.0;
int		StripLength=3;

FileCycle	MyFiles;

//***************************************************************************
void	DebugStr()						{if (DebugOn) std::cout<<endl;}
void	DebugStr(char *Txt)				{if (DebugOn) std::cout<<Txt<<endl;}
void	DebugStr(char *Txt,int Param)	{if (DebugOn) std::cout<<Txt<<" "<<Param<<endl;}
void	DebugStr2(char *Txt,int Param)	{if (DebugOn) std::cout<<Txt<<" "<<Param<<" ";}

//***************************************************************************
int		round(float f) 
{
		if (f<0) 
		{
			return (int)(f-0.5f);
		} 
		else 
		{
			return (int)(f+0.5f);
		}
}

//***************************************************************************
void alignFile( FILE *f, int align )
{
	long fp = ftell(f);
	if (fp&(align-1))
	{
		char buffer[] = "*********************************";
		int s = align - (fp & (align-1));
		fwrite(buffer, s, 1, f);
	}
}

//***************************************************************************
GString CheckFileString(const char * Str)
{
	GString	RetStr;

	if (strlen(Str)>3 && (Str[0]=='-' || Str[0]=='/') && (Str[2]==':' || Str[2]==':'))
		RetStr=&Str[3];
	else
		GObject::Error(ERR_FATAL,"Problem with cmd line option %s",Str);

	return(RetStr);
}

//***************************************************************************
bool CheckPlusMinus(const char * Str)
{
	if (strlen(Str)==3 && (Str[0]=='-' || Str[0]=='/') && (Str[2]=='-' || Str[2]=='+'))
		{
		}
	else
		GObject::Error(ERR_FATAL,"Problem with cmd line option %s",Str);

	return(Str[2]=='+');
}

//***************************************************************************
int ZeroAndCountCommas(char * Str)
{
int Len=strlen(Str);
int NumOfCommas=0;

	for (int f=0;f<Len;f++)
		{
		if (Str[f]==',')
			{
			NumOfCommas++;
			Str[f]=0;
			}
		}

	return(NumOfCommas);
}

//***************************************************************************
char	*FindFile(const char *Name)
{
int 		Handle;
GFName		DirName(Name);
GString		Filename,Path;
_finddata_t FindBlock;
int			Status;

// Does this file exist
FILE	*File=fopen(Name,"rb");
		if (File)
		{
			fclose(File);
			return((char*)Name);
		}

// Recurse dirs to find file
		Filename=DirName.File();
		Filename+=".";
		Filename+=DirName.Ext();
		DirName.File("*");
		DirName.Ext("*");
		Path=DirName.FullName();

		Name=0;
		Status=Handle= _findfirst(Path,&FindBlock);

		while (Status!=-1 && !Name)
		{
			if (FindBlock.attrib&_A_SUBDIR)
			{
				if (strcmp(FindBlock.name,".") && strcmp(FindBlock.name,".."))
				{
					GString	NewName;
					NewName=DirName.Drive();
					NewName+=DirName.Dir();
					NewName+=FindBlock.name;
					NewName+="/";
					NewName+=Filename;
					Name=FindFile(NewName);
				}
			}
		if (!Name) Status=_findnext(Handle,&FindBlock);
		}
		return((char*)Name);
}
