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
/*
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
*/
//***************************************************************************
void    PadFile(FILE *File)
{
int		Pad=ftell(File) & 3;

		while (ftell(File) &3)
		{
			fwrite(&Pad,1,1,File);
		}
//		if (Pad)
//		{
//			fwrite(&Pad,Pad,1,File);
//		}
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
/**************************************************************************************/
struct	sTgaHdr
{
	char	id;			   // 0
	char	colmaptype;	   // 1
	char	imagetype;	   // 2
	char	fei[2];		   // 3
	char	cml[2];		   // 5
	char	cmes;		   // 7
	short	xorig;		   // 8
	short	yorig;		   // 10
	short	width;		   // 12
	short	height;		   // 14
	char	depth;		   // 15
	char	imagedesc;	   // 16
};



void SaveTGA(char *Filename,int W,int H,u8 *Data,bool IsBGR)
{
FILE			*File;
sTgaHdr			FileHdr;

		File=fopen(Filename,"wb");
		
		memset(&FileHdr,0 ,sizeof(sTgaHdr));
	
		FileHdr.imagetype= 2;  //imagetype
		FileHdr.width = W;
		FileHdr.height= H;
		FileHdr.depth=24;

		fwrite(&FileHdr,sizeof(sTgaHdr),1,File);
		if (!IsBGR)
		{
			fwrite(Data,W*H*3,1,File);
		}
		else
		{
			int	Size=W*H;
			for (int i=0; i<Size;i++)
			{
				fwrite(&Data[2],1,1,File);
				fwrite(&Data[1],1,1,File);
				fwrite(&Data[0],1,1,File);
				Data+=3;

			}
		}

		fclose(File);
}

//***************************************************
Vector3 CalcNormal(Vector3 const &v0, Vector3 const &v1, Vector3 const &v2 )
{
		Vector3 dv1, dv2;
		Vector3 out;

		dv1.x = v1.x - v0.x;
		dv1.y = v1.y - v0.y;
		dv1.z = v1.z - v0.z;

		dv2.x = v2.x - v0.x;
		dv2.y = v2.y - v0.y;
		dv2.z = v2.z - v0.z;

		out.x = (dv1.z * dv2.y) - (dv1.y * dv2.z);
		out.y = (dv1.x * dv2.z) - (dv1.z * dv2.x);
		out.z = (dv1.y * dv2.x) - (dv1.x * dv2.y);

		out.Normalise();

		return out;
}

//***************************************************
//***************************************************
//***************************************************
/*
static const char BadFileChars[] =
{
        '+',    '-',    '*',    '/',    '\\',   '#',    ',',
        '.',    '(',    ')',    '!',    '"',    'ú',    '$',
        '%',    '^',    '&',    '=',    '#',    ':',    ';',    '<',
        '>',    '?',    '@',    '{',    '}',    '[',    ']',    '¼',
};

static const int        nbBadFileChars = (sizeof(BadFileChars) / sizeof(char));

GString  ReplaceBadFileChars(GString s)
{
        GString out;
        const char *textin = (const char*)s;
        char *textout = new char[s.Len()+1];
        memset(textout, 0, s.Len()+1);
        int i, p;

        p=0;
        for (i=0; i<s.Len(); i++)
        {
                char    ic;

                ic=textin[i];

                for (int c=0;c<nbBadFileChars;c++)
                {
                        if (ic == BadFileChars[c])      ic = '_';
                }
                textout[i] = ic;
        }
        out = textout;
        delete textout;
        return out;
}
*/