/***********************/
/*** Speech Compiler ***/
/***********************/

#include <windows.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <io.h>
#include <fstream.h>
#include <conio.h>
#include <vector>

using namespace std;

//************************************************
#define CHUNKSIZE			2336	// XA track size

#define	SPACE				' '
#define	MINUS				'-'
#define	COMMENT				'#'
#define	TAB					0x09
#define	EOL					0x0d
#define	InFileEXT			"Ixa"
#define	OutFileEXT			"XaB"
#define	BANK_SHIFT			16

//************************************************
struct	sLang
{
		char	*Dir;
		char	Prefix;
};

struct	sInFile
{
		char	Name[256];
		int		Chunks;
		int		Offset;
};

//************************************************
char	*Script;
char	*OutDir="",*ScriptFile=0;
int		IncFileSet=0;
char	IncludeFilename[256];
int		BankNo=0;

char	drive[_MAX_DRIVE];
char	dir[_MAX_DIR];
char	fname[_MAX_FNAME];
char	ext[_MAX_EXT];


std::vector<sLang>		LangList;
std::vector<sInFile>	FileList;

//***********************************************************************
//***********************************************************************

static void app_debug_msg(const char * pszFmt,va_list args)
{
	char szBuf[256];
	vsprintf(szBuf,pszFmt,args);

	printf("%s\n",szBuf);
}


//******************************************************************
//******************************************************************

void __cdecl DEBUG(const char * pszFmt,...)
{
	if (pszFmt)
	{
		va_list args;
		va_start(args,pszFmt);
		app_debug_msg(pszFmt,args);
		va_end(args);
	}
}

void __cdecl FATAL(const char * pszFmt,...)
{
	if (pszFmt)
	{
		va_list args;
		va_start(args,pszFmt);
		app_debug_msg(pszFmt,args);
		va_end(args);
	}
	exit(123);
}

//******************************************************************



char	*LoadFileInMem(const char * pszName)
{
HANDLE	FileHandle;
DWORD	SizeRead;
char	*Buffer;
char	TName[256];
int		Size;

	sprintf(TName,pszName);

    if( (FileHandle = CreateFile(TName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL)) == INVALID_HANDLE_VALUE )
		{
		FATAL("ERROR : Could not open file %s.",TName);
		return NULL;
		}
	else
		{
		Size = GetFileSize(FileHandle,NULL);

		if( (Buffer=(char*)malloc(Size))==NULL ) {printf("Out of memory.\n");exit(123);}
		ReadFile(FileHandle,(void*)Buffer,Size,&SizeRead,NULL);
		CloseHandle(FileHandle);
		}
	return Buffer;
}


//******************************************************************

int		FindFileSize(const char * FileName)
{
HANDLE	FileHandle;
DWORD	Size;
char	TName[256];

	sprintf(TName,FileName);

    if( (FileHandle = CreateFile(TName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL)) == INVALID_HANDLE_VALUE )
		{
		FATAL("ERROR : Could not open file %s.",TName);
		return (0);
		}
	else
		{
		Size = GetFileSize(FileHandle,NULL);
		CloseHandle(FileHandle);
		}

	return (Size);
}

int		FindFileSizeAlign(const char * FileName)
{
int		Size=FindFileSize(FileName);
		Size = ((Size + 3) & 0xfffffffc);
		return(Size);
}


//******************************************************************
//	Read pre-defined script file
//******************************************************************
int		IsWhiteSpace(char c)
{
		if (c==' ') return(1);
		if (c==TAB) return(1);
		if (c==0xd) return(1);
		if (c==0xa) return(1);
		return(0);
}

//******************************************************************
char	*SkipWhiteSpace(char *Ptr)
{
		while (IsWhiteSpace(*Ptr)) Ptr++;
		return(Ptr);
}

//******************************************************************
char	*GotoNextLine(char *Ptr)
{
		while (*Ptr!=EOL) Ptr++;
		return(Ptr+2);
}

//******************************************************************
void	ReadScript()
{
char	*Ptr,*EndPtr;
int		Size;
sInFile	InFile;

	Script = (char *)LoadFileInMem( ScriptFile);
	Size = FindFileSize(ScriptFile);
	Ptr=Script;
	EndPtr=Ptr+Size;

	while (Ptr<EndPtr)
		{
		Ptr=SkipWhiteSpace(Ptr);
		if (Ptr>=EndPtr) return;
		if (*Ptr==COMMENT) 
			{	// Comment
			Ptr=GotoNextLine(Ptr);
			}
		else
			{
			int	i=0;
			InFile.Chunks=0;
			while (!IsWhiteSpace(*Ptr) && Ptr<EndPtr) InFile.Name[i++]=*Ptr++;
			InFile.Name[i]=0;
			strupr(InFile.Name);
			FileList.push_back(InFile);
//			printf("%s\n",InFile.Name);
			}
		}

}

//******************************************************************
//*** Get File Sizes ***********************************************
//******************************************************************
void	GetFileSizes()
{
int		FileCount=FileList.size();
int		LangCount=LangList.size();
char	FileName[256];

//		DEBUG( "Parsing Files");
		for (int File=0; File<FileCount; File++)
			{
			sInFile	&ThisFile=FileList[File];
			ThisFile.Chunks=0;
			for (int Lang=0; Lang<LangCount; Lang++)
				{
				sLang	&ThisLang=LangList[Lang];
				int		ThisFileSize;
				int		ThisChunkSize;
				sprintf(FileName,"%s\\%c%s.%s", ThisLang.Dir, ThisLang.Prefix,  ThisFile.Name, InFileEXT);
				ThisFileSize=FindFileSize(FileName);
				ThisChunkSize=(ThisFileSize+(CHUNKSIZE-1))/CHUNKSIZE;
				if (ThisFile.Chunks<ThisChunkSize) ThisFile.Chunks=ThisChunkSize;
				}
			}
}

//******************************************************************
//*** Write Data ***************************************************
//******************************************************************
void WriteData(sLang &ThisLang)
{
char	*Buffer;
char	FileName[256];
FILE	*OutFile;
char	Blank[CHUNKSIZE];
int		FileCount=FileList.size();

		memset( Blank, 0xBA, CHUNKSIZE );	// clear Blank Buffer
		sprintf(FileName,"%s%c%s.%s",OutDir,ThisLang.Prefix ,fname,OutFileEXT);
		OutFile= fopen( FileName,"wb" );
		if(!OutFile)
			FATAL( "Could not write %s", FileName);
		else			
			DEBUG( "Created File - %s", FileName);

		for (int File=0; File<FileCount; File++)
			{
			sInFile	&ThisFile=FileList[File];
			int	ThisFileSize;
			sprintf(FileName,"%s\\%c%s.%s", ThisLang.Dir, ThisLang.Prefix,  ThisFile.Name, InFileEXT);
			Buffer=LoadFileInMem(FileName);
			ThisFileSize=FindFileSize(FileName);
			fwrite( Buffer, 1, ThisFileSize, OutFile);
			fwrite( Blank, 1, (ThisFile.Chunks*CHUNKSIZE)-ThisFileSize, OutFile); // Write Pad
			fwrite( Blank, 1, CHUNKSIZE, OutFile); // Force Pad
			ThisFile.Offset=ftell(OutFile);
			free(Buffer);
		}
		fclose(OutFile);
}


//******************************************************************
//*** Write Header File ********************************************
//******************************************************************
static char s_illegalChars[] = "\\/!£$%^&*()+-=-:. ";
static int	s_nbChars = sizeof(s_illegalChars);

void	convertIllegalChar( char *c )
{
	for (int i=0;i<s_nbChars;i++)
		{
		if (*c == s_illegalChars[i]) *c = '_';
		}
}

//******************************************************************
void	ConvertString(char *Str)	{while(*Str) convertIllegalChar(Str++);}

//******************************************************************
void WriteHeader(sLang &ThisLang)
{
FILE	*OutFile;
char	HdrStr[256];
char	EqStr[256];
int		FileCount=FileList.size();
int		Offset=0;

		sprintf(HdrStr,"__%s_SPEECH_DEFINES_H__",strupr(fname));
		sprintf(EqStr,"%s_SPEECH_",strupr(fname));

		OutFile= fopen( IncludeFilename,"wt" );
		if(!OutFile)
			FATAL( "Could not write %s", IncludeFilename);
		else			
			DEBUG( "Created File - %s", IncludeFilename);

		fprintf( OutFile, "#ifndef %s\n",HdrStr);
		fprintf( OutFile, "#define %s\n\n",HdrStr);
		fprintf( OutFile, "enum %sENUMS\n" ,EqStr);
		fprintf( OutFile, "{\n" );

		for (int File=0; File<FileCount; File++)
			{
			sInFile	&ThisFile=FileList[File];
			
			ConvertString(ThisFile.Name);
			fprintf( OutFile, "%s%s = 0x%x,\n", "SPEECH_",ThisFile.Name, Offset+(BankNo<<BANK_SHIFT));
			Offset+=ThisFile.Chunks+1; // +1 for pad
			}
		fprintf( OutFile, "};\n" );
		fprintf( OutFile, "#endif\n" );

		fclose(OutFile);
}

//***************************************************
//***************************************************
//***************************************************

int main(int argc, char *argv[])
{
int	CmdError=0;

// Parse Input
		for (int Loop=1;Loop<argc;Loop++)
			{
			char *StrPtr=argv[Loop];
			if (StrPtr[0]!='-') CmdError=1;
			StrPtr++;
			switch(StrPtr[0])
				{
				case 'o':	// Output
					OutDir=StrPtr+1;
					break;

				case 'd':	// Add In Dir
					{
					sLang	Dir;
					Dir.Prefix=StrPtr[1];
					Dir.Dir=StrPtr+2;
					LangList.push_back(Dir);
					DEBUG("Added Lang:%s",Dir.Dir);
					}
					break;

				case 's':	// script
					ScriptFile=StrPtr+1;
					break;
				case 'i':	// script
					IncFileSet=1;
					strcpy(IncludeFilename,StrPtr+1);
					break;
				case 'b':	// script
					BankNo=atoi(StrPtr+1);
					break;

				default:
					printf("Unknown Param [%s]\n",StrPtr);
					break;
				}

			}

		if(!ScriptFile || !LangList.size())
		{
			DEBUG("Error in command line");
			DEBUG("Usage : MkSpeech");
			DEBUG("        -d\tAdd Dir (Prefix & Dir= -dPddddd)");
			DEBUG("        -o\tOut Dir");
			DEBUG("        -s\tScript");
			DEBUG("        -I\tHeaderFile");
			DEBUG("        -B\tBankNo");
			return(0);
		}

		_splitpath( ScriptFile, drive, dir, fname, ext );

		if (!IncFileSet) sprintf(IncludeFilename,"%s%s.h",fname,"_Speech",OutFileEXT);

		ReadScript();
//		DEBUG( "Script Read.");
		GetFileSizes();

int	LangCount=LangList.size();
		for (int Lang=0;Lang<LangCount; Lang++)	WriteData(LangList[Lang]);
		WriteHeader(LangList[0]);

		DEBUG("Success.");

		return(0);
}
