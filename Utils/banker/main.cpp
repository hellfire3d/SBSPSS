#include <windows.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <io.h>

//#include "SfxList.h"


//************************************************

#define	MAX_SFX		1024

//************************************************

char path_buffer[_MAX_PATH];
char drive[_MAX_DRIVE];
char dir[_MAX_DIR];
char fname[_MAX_FNAME];
char ext[_MAX_EXT];


struct bank_entry
{
	WORD	looped;
	WORD	pitch;
	DWORD	offset;
};

typedef struct
{
	char	Name[ 100 ];
	WORD	SfxNo;
	BYTE	Hashed;
	BYTE	Looped;
	WORD	LoopPos;
} file_entry;


char		ThisDir[256];
file_entry	FileList[MAX_SFX];
int			NoFiles = 0;


//***********************************************************************
//***********************************************************************

static void app_debug_msg(const char * pszFmt,va_list args)
{
	char szBuf[256];

	vsprintf(szBuf,pszFmt,args);
	printf("\n%s\n",szBuf);
}


//******************************************************************
//******************************************************************

void __cdecl screen_debug_msg(const char * pszFmt,...)
{
	if (pszFmt)
	{
		va_list args;
		va_start(args,pszFmt);
		app_debug_msg(pszFmt,args);
		va_end(args);
	}
}

void __cdecl app_assert(const char * pszFmt,...)
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

void FreePtr(void *p)
{
	if(p) free(p);
}

//******************************************************************

BYTE* LoadFileInMem(const char * pszName)
{
	HANDLE hsFile;
	DWORD dwSize = NULL;
	DWORD dwSize2 = NULL;
	DWORD Temp;
	BYTE* Buffer;

    if( (hsFile = CreateFile(pszName, GENERIC_READ, FILE_SHARE_READ, NULL,
				  OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL)) == INVALID_HANDLE_VALUE )
	{
		screen_debug_msg("LFIM: Could not open file %s",pszName);
		return NULL;
	}else{
		dwSize = GetFileSize(hsFile,NULL);
		dwSize2 = dwSize;
		if(dwSize&0x7) dwSize2 = (dwSize-(dwSize&0x7))+8;
		if( (Buffer=(BYTE*)malloc(dwSize2))==NULL ) {printf("Out of memory.\n");exit(123);}
		ReadFile(hsFile,(void*)Buffer,dwSize,&Temp,NULL);
		CloseHandle(hsFile);
	}

	return Buffer;
}


//******************************************************************

DWORD FindFileSize(const char * pszName)
{
	HANDLE hsFile;
	DWORD dwSize=NULL;

    if( (hsFile = CreateFile(pszName, GENERIC_READ, FILE_SHARE_READ, NULL,
				  OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL)) == INVALID_HANDLE_VALUE )
	{
		screen_debug_msg("\nFFS: Could not open file %s",pszName);
		return NULL;
	}else{
		dwSize = GetFileSize(hsFile,NULL);
		if(dwSize&0x7) dwSize = (dwSize-(dwSize&0x7))+8;
		CloseHandle(hsFile);
	}

	return dwSize;
}


//***************************************************
//	Find files in directory
//***************************************************

int CalcHashNum(char* astring)
{
	int result = 0;
	WORD count;

	for (count=0; count<strlen(astring); count++)
	{
		result^=(int) astring[count];
		result++;
	}

	return result;
}



void FindDirFiles(char * Search)
{
	int i;
	FILE *fh;
	char loop;


	/* De-hash table */
	for(i=0;i<MAX_SFX;i++)
	{
		FileList[i].Name[ 0 ] = '\0';
		FileList[i].Hashed = false;
		FileList[i].Looped = false;
	}


	// Open source file
	if( ( fh = fopen( Search, "rb" ) ) == NULL )
	{
		printf( "\nCan't find source file!\n" );
		return;
	}

	// Scan source file
	while( !feof( fh ) )
	{
		if( fscanf( fh, "%s %c %d", FileList[NoFiles].Name, &loop, &FileList[NoFiles].LoopPos ) != 3 )
		{
			return;
		}
		if( loop == 'y' || loop == 'Y' )
		{
			FileList[NoFiles].Looped = true;
		}
		else
		{
			FileList[NoFiles].Looped = false;
		}
		FileList[NoFiles].Hashed = true;
		NoFiles++;
	}

}

//***************************************************
//	Write files to bank
//***************************************************

DWORD FlipLongWord(DWORD val)
{
	DWORD result;

	result = ((val&0x000000ff)<<24) + ((val&0x0000ff00)<<8) + ((val&0x00ff0000)>>8) + ((val&0xff000000)>>24);

	return(result);
}

void WriteBankData(char* Bank)
{
	FILE* BANKFILE = NULL;
	FILE* OFFSETFILE = NULL;
	BYTE* Buffer = NULL;
	WORD Size;
	DWORD filepos = 0;
	char file[256];
	bank_entry	BankInfo;
	WORD pitch;
	DWORD vagpitch;

	sprintf(file,"%s.bnk",Bank);
	BANKFILE = fopen(file,"wb");
	if(!BANKFILE) {printf("\nBank file error\n\n");return;}

	sprintf(file,"%s.bof",Bank);
	OFFSETFILE = fopen(file,"wb");
	if(!OFFSETFILE) {printf("\nBank offset error\n\n");return;}

	for(int i=0;i<NoFiles;i++)
	{
		if(FileList[i].Hashed)
		{
			char Name[256];
			sprintf(Name, "%s%s", ThisDir, FileList[i].Name);
			Buffer = LoadFileInMem(Name);
			if (Buffer)
			{
				Size = (WORD)FindFileSize(Name);
				Size -= 48;

				printf("%s\t\t", Name);

				if( FileList[i].Looped )
				{
					// Encode vag file as looped	
					int LoopBlock = (int)( (float)FileList[i].LoopPos / 3.5f );
					for(int p=48+16;p<(LoopBlock)-16;p+=16)
					{
						Buffer[p+1] = 0x0;
					}

					Buffer[p+1] = 0x6;
					for(p=p+16;p<(Size-16);p+=16)
					{
						Buffer[p+1] = 0x2;
					}
					Buffer[p+1] = 0x3;

				}
				else
				{
					// Encode vag file as non-looped
					for(int p=48;p<(Size-16);p+=16)
					{
						//	Buffer[p+1] = 0x2;
					}
					Buffer[p+1] = 0x1;
				}

				fwrite(Buffer+48, 1, Size, BANKFILE);

				_splitpath(Name, drive, dir, fname, ext);

				vagpitch = *(DWORD*)&Buffer[16];
				vagpitch = FlipLongWord(vagpitch);
				pitch=(int)((float)vagpitch*(1024.0/11025.0));
				printf(" pitch %dkhz (%d)\n",vagpitch,pitch);

				BankInfo.offset = filepos;
				BankInfo.pitch = pitch;
				BankInfo.looped = FileList[i].Looped;

				fwrite(&BankInfo, 1, sizeof(bank_entry), OFFSETFILE);

				filepos += Size;

				FreePtr(Buffer);
			}
		}
	}

	fclose(BANKFILE);
	fclose(OFFSETFILE);
}


static const char BadFileChars[] =
{
	'+',	'-',	'*',	'/',	'\\',	'#',	',',
	'.',	'(',	')',	'!',	'"',	'£',	'$',
	'%',	'^',	'&',	'=',	'#',	':',	';',	'<',
	'>',	'?',	'@',	'{',	'}',	'[',	']',	'¬',
};

static const int	nbBadFileChars = (sizeof(BadFileChars) / sizeof(char));


void ExportEnums(char * name)
{
	FILE *	ENUMFILE = NULL;
	char	file[256];

	_splitpath(name, drive, dir, fname, ext);
	strupr(fname);
	sprintf( file, "%s.h", name );
	ENUMFILE = fopen(file,"wt");
	if(!ENUMFILE) {printf("\nEnum file error\n\n");return;}

	fprintf( ENUMFILE, "enum\n{\n" );
	for (int i=0;i<NoFiles;i++)
	{
		char *	ostr = FileList[i].Name;//SFXNameTab[i];
		int		len = (int)strlen(ostr)+1;
		char *	nstr = new char[len];

		for (int t=len-1;t>=0;t--)
		{
			if( ostr[ t ] == '\\' )
			{
				t++;
				break;
			}
		}

		int o = 0;
		for (;t<len;t++)
		{
			nstr[o] = ostr[t];
			if (nstr[o] == '.')		nstr[o] = 0;
			for(int c=0;c<nbBadFileChars;c++)
			{
				if (nstr[o] == BadFileChars[c])	nstr[o] = '_';
			}
			o++;
		}

		strupr( nstr );

		if( FileList[i].Looped )
		{
			fprintf( ENUMFILE, "\tSFX_%s_%s,\t\t// Looped at %d\n", fname,nstr, FileList[i].LoopPos );
		}
		else
		{
			fprintf( ENUMFILE, "\tSFX_%s_%s,\n", fname,nstr );
		}
	}
	fprintf( ENUMFILE, "};\n" );
}

//***************************************************
//***************************************************
//***************************************************
//***************************************************

int main(int argc, char *argv[])
{

	if(argc!=3)
	{
		printf("\nBanker (C)limax 1997");
		printf("\nWritten by Tim Swann");
		printf("\nUsage: Banker <Source File> <Output Base>");
		printf("\nFor example, 'Banker level0.txt \\data\\snd\\level0' will take");
		printf("\n'level0.txt' as its input and produce three files ( \\data\\snd\\level0.bof,");
		printf("\n\\data\\snd\\level0.bnk and \\data\\snd\\level0.h ) as output.");
		printf("\n\n");
		return(0);
	}

	FindDirFiles(argv[1]);
	WriteBankData(argv[2]);
	ExportEnums(argv[2]);

	printf("\nBank files made\n\n");

	return(0);
}
