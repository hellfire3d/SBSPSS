/************************/
/*** DataFile Creator ***/
/************************/

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
#include "Mkdata.h"

using namespace std;


//***********************************************************************
//***********************************************************************
//***********************************************************************
int		DebugFlag=0;
int		AlignSize=2048;
char	*AlignBuffer;

//***********************************************************************
CBank					Banks;
CFileList				Files;
char					*BaseDir=0;

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
	if (DebugFlag)
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
char	*LoadFileInMem(const char * FileName,int *OutSize)
{
FILE	*FileHandle;
char	*Buffer;
int		Size;
		
		FileHandle= fopen( FileName,"rb" );
		if(!FileHandle)
			{
			FATAL("ERROR : Could not open file %s\n",FileName);
			}	
		else
			{
			fseek(FileHandle,0,SEEK_END);
			Size=ftell(FileHandle);
			fseek(FileHandle,0,SEEK_SET);

			if( (Buffer=(char*)malloc(Size))==NULL ) {printf("Out of memory.\n");exit(123);}
			fread(Buffer, 1,Size,FileHandle);
			fclose(FileHandle);
		}
	if (OutSize) *OutSize=Size;
	return Buffer;
}


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
void	ConvertString(char *Str)	
{
		strupr(Str);
		while(*Str) convertIllegalChar(Str++);
}

//******************************************************************
void	MakeEqName(char *In,char *Out)
{
int		StrLen=strlen(In);
int		begin0 = 0;
int		begin1 = 0;

		for (int i=0;i<StrLen;i++)
			{
			if (In[i] == '/' || In[i] == '\\')
				{
				begin0 = begin1;
				begin1 = i;
				}
			}
	if (In[begin0]=='/' || In[begin0]=='\\') begin0++;
	strcpy(Out,In+begin0);
	ConvertString(Out);

}

//***********************************************************************
//*** CFileList *********************************************************
//***********************************************************************
CFileList::~CFileList()
{
int	ListSize=FileList.size();
	for (int i=0;i<ListSize;i++) if (!FileList[i].BankIdx) free(FileList[i].Data);

}
//***********************************************************************
int	CFileList::FindFileIdx(char *Filename)
{
int	ListSize=FileList.size();
	for (int i=0;i<ListSize;i++) if (!strcmp(Filename,FileList[i].Filename)) return(i);
	return(-1);

}
//***********************************************************************
int	CFileList::AddFile(char *BaseFilename,int BankIdx)
{
int		Idx;
sFile	File;

		File.BankIdx=BankIdx;
		strupr(BaseFilename);
		sprintf(File.Filename,"%s/%s",BaseDir,BaseFilename);
		MakeEqName(File.Filename,File.ShortName);
		Idx=FindFileIdx(File.Filename);

		if (Idx==-1)
			{
			Idx=FileList.size();
			if (!BankIdx)
				{
				File.Data=LoadFileInMem(File.Filename,&File.Size);
				}
			else
				{
				File.BankIdx=BankIdx;
				}
			FileList.push_back(File);
			}
	return(Idx);

}

//***********************************************************************
//*** CBank *************************************************************
//***********************************************************************
void	CBank::Start(char *Name,int Align)
{
sBank	B;
		if (Align==0) Align=AlignSize;
		strcpy(B.Name,Name);
		B.Align=Align;
		CurrentBank=BankList.size();
		BankList.push_back(B);
		if (CurrentBank) Files.AddFile(Name,CurrentBank);
}

//***********************************************************************
void	CBank::End()
{
		CurrentBank=0;
}

//***********************************************************************
int		CBank::FindEntry(int Idx)
{
sBank	&ThisBank=GetBank(CurrentBank);

int		ListSize=ThisBank.FileList.size();

	for (int i=0;i<ListSize;i++)
		{
		if (ThisBank.FileList[i].ListIdx==Idx) return(i);
		}
	return(-1);
}

//***********************************************************************
void	CBank::Add(char *Filename)
{
sBankEntry	Entry;

		if (Filename[0]==CHAR_SKIP)	{Files.AddFile(Filename+1); return;}
		Entry.ListIdx=Files.AddFile(Filename);
		if (FindEntry(Entry.ListIdx)==-1) BankList[CurrentBank].FileList.push_back(Entry);
}

//******************************************************************
//*** Script shite *************************************************
//******************************************************************
int		IsWhiteSpace(char c)
{
		if (c==' ') return(1);
		if (c==CHAR_TAB) return(1);
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
		while (*Ptr && *Ptr!=CHAR_EOL ) Ptr++;
		return(Ptr+2);
}

//******************************************************************
void	Line2Str(char *Ptr,char *EndPtr)
{
int		i=0;
		while (!IsWhiteSpace(*Ptr) && Ptr<EndPtr) Ptr++;
		*Ptr=0;
}

//******************************************************************
char	*ScriptComment(char *Ptr,char *EndPtr)
{
		return(GotoNextLine(Ptr));
}

//******************************************************************
char	*ScriptStartBank(char *Ptr,char *EndPtr)
{
int		Align;
char	*Name;

		Ptr=SkipWhiteSpace(Ptr+1);
		Line2Str(Ptr,EndPtr);
		Name=Ptr;
		Ptr+=strlen(Ptr)+1;
		Align=atoi(Ptr);
		DEBUG("[%s] (%i)\n\t{",Name,Align);
		Banks.Start(Name,Align);
		return(GotoNextLine(Ptr));
}

//******************************************************************
char	*ScriptEndBank(char *Ptr,char *EndPtr)
{
		DEBUG("\t}");
		Banks.End();
		return(GotoNextLine(Ptr));
}

//******************************************************************
char	*ScriptFile(char *Ptr,char *EndPtr)
{
		Line2Str(Ptr,EndPtr);
		Banks.Add(Ptr);
		if (DebugFlag)
			{
			if (Banks.GetCurrentBankNo()) printf("\t");
			printf("%s\n",Ptr);
			}
		return(GotoNextLine(Ptr));
}

//******************************************************************
void	ReadScript(char *Filename)
{
char	*Script,*Ptr,*EndPtr;
int		Size;
		Script = (char *)LoadFileInMem( Filename,&Size);
		Ptr=Script;
		EndPtr=Ptr+Size;

		printf("Reading %s\n",Filename);
		while (Ptr<EndPtr)
			{
			Ptr=SkipWhiteSpace(Ptr);
			if (Ptr>=EndPtr) return;
			switch (*Ptr)
				{
				case CHAR_COMMENT:
					Ptr=ScriptComment(Ptr,EndPtr);
					break;
				case CHAR_STARTBANK:
					Ptr=ScriptStartBank(Ptr,EndPtr);
					break;
				case CHAR_ENDBANK:
					Ptr=ScriptEndBank(Ptr,EndPtr);
					break;
				default:
					Ptr=ScriptFile(Ptr,EndPtr);
					break;
				}
			}		
		free(Script);
}

//***********************************************************************
//***********************************************************************
//***********************************************************************
void	ResetFAT(std::vector<sFAT> &FAT)
{
int		FATSize=Files.GetCount();
int		Loop;

		FAT.resize(FATSize);
		for (Loop=0; Loop<FATSize; Loop++)
			{
			sFile	&F=Files.GetFile(Loop);
			FAT[Loop].FilePos=-1;
			FAT[Loop].FileSize=F.Size;
			}
}

//***********************************************************************
void	AlignData(FILE *FileHandle,int Align)
{
int		Pos=ftell(FileHandle);
int		AlignPad= (Pos & (Align-1));
		if (AlignPad) fwrite(AlignBuffer,1,Align-AlignPad,FileHandle);
}

//***********************************************************************
int		WriteFAT(FILE *FileHandle,std::vector<sFAT> &FAT,int Align)
{
int		Pos=ftell(FileHandle);
int		FATSize=Files.GetCount();

	for (int Loop=0;Loop<FATSize;Loop++) 
		{
		fwrite(&FAT[Loop],1,sizeof(sFAT),FileHandle);
		}
	AlignData(FileHandle,Align);
	
	return(Pos);
}

//***********************************************************************
void	WriteFileData(FILE *FileHandle,sFile &File,int Align,sFAT &ParentFAT)
{
		ParentFAT.FilePos=ftell(FileHandle);
		ParentFAT.FileSize=File.Size;

		fwrite(File.Data,1,File.Size,FileHandle);
		AlignData(FileHandle,Align);
}

//***********************************************************************
void	WriteBankData(FILE *FileHandle,int BankIdx,sFAT &ParentFAT)
{
std::vector<sFAT> FAT;
sBank	&ThisBank=Banks.GetBank(BankIdx);
int		ThisListSize=ThisBank.FileList.size();
int		Loop;
long	StartFilePos,EndFilePos;

// reset FAT and write dummy FAT
		ResetFAT(FAT);
		ParentFAT.FilePos=WriteFAT(FileHandle,FAT,ThisBank.Align);
		StartFilePos=ftell(FileHandle);;
	
// Write files

		for (Loop=0; Loop<ThisListSize; Loop++)
			{
			int		Idx=ThisBank.FileList[Loop].ListIdx;
			sFile	&ThisFile=Files.GetFile(Idx);
			WriteFileData(FileHandle,ThisFile,ThisBank.Align,FAT[Idx]);
			FAT[Idx].FilePos-=ParentFAT.FilePos;
			DEBUG("\t %s %i %i",ThisFile.Filename,FAT[Idx].FilePos,FAT[Idx].FileSize);
			}

		EndFilePos=ftell(FileHandle);
		fseek(FileHandle,ParentFAT.FilePos,SEEK_SET);
		WriteFAT(FileHandle,FAT,ThisBank.Align);
		fseek(FileHandle,EndFilePos,SEEK_SET);

		ParentFAT.FileSize=EndFilePos-StartFilePos;
//		printf("%i %i\n",BankSize,ParentFAT.FileSize);
}

//***********************************************************************
void	WriteData(char *Filename)
{
FILE	*FileHandle;	
std::vector<sFAT> FAT;
int		FATSize=Files.GetCount(),Idx;

	FileHandle= fopen( Filename,"wb" );
	if(!FileHandle)	FATAL( "Could not write %s", Filename);
	ResetFAT(FAT);
	WriteFAT(FileHandle,FAT,AlignSize);
	for (Idx=0; Idx<FATSize; Idx++)
		{
		sFile	&ThisFile=Files.GetFile(Idx);
		if (ThisFile.BankIdx)
			{ // Databank
			DEBUG("Bank %s",ThisFile.Filename);
			WriteBankData(FileHandle,ThisFile.BankIdx,FAT[Idx]);
			FAT[Idx].FileSize+=FATSize*sizeof(FAT);
//			printf("%i\n",(int)FAT[Idx].FileSize);

			AlignData(FileHandle,AlignSize);
			}
		else
			{ // Normal File
			WriteFileData(FileHandle,ThisFile,AlignSize,FAT[Idx]);
			DEBUG("%s %i %i",ThisFile.Filename,FAT[Idx].FilePos,FAT[Idx].FileSize);
			}
		}

	fseek(FileHandle,0,SEEK_SET);
	WriteFAT(FileHandle,FAT,AlignSize);
	fclose(FileHandle);
}

//***********************************************************************
//***********************************************************************
//***********************************************************************
void	WriteFileHeader(FILE *FileHandle,char *Name)
{
static	Count=0;

		if (!Count)
			fprintf( FileHandle, "%s=%i,\n" ,Name,Count);
		else
			fprintf( FileHandle, "%s,\n" ,Name);

		Count++;
		
}

//***********************************************************************
void	WriteBankHeader(FILE *FileHandle,int BankIdx)
{
sBank	&ThisBank=Banks.GetBank(BankIdx);

		WriteFileHeader(FileHandle,ThisBank.Name);
}

//***********************************************************************
void	WriteHeader(char *Filename)
{
FILE	*FileHandle;	
int		FATSize=Files.GetCount(),Idx;

	
	FileHandle= fopen( Filename,"wb" );
	if(!FileHandle)	FATAL( "Could not write %s", Filename);
	fprintf( FileHandle, "#ifndef __FILE_EQUATES_H__\n");
	fprintf( FileHandle, "#define __FILE_EQUATES_H__\n\n");
	fprintf( FileHandle, "enum FileEquate\n" );
	fprintf( FileHandle, "{\n" );

	for (Idx=0; Idx<FATSize; Idx++)
		{
		sFile	&ThisFile=Files.GetFile(Idx);
		if (ThisFile.BankIdx)
			{ // Databank
			WriteBankHeader(FileHandle,ThisFile.BankIdx);
			}
		else
			{ // Normal File
			WriteFileHeader(FileHandle,ThisFile.ShortName);
			}
		}

	fprintf( FileHandle, "\nFileEquate_MAX,\n");
	fprintf( FileHandle, "};\n" );
	fprintf( FileHandle, "#endif\n" );

	fclose(FileHandle);
}

//***********************************************************************
//***********************************************************************
//***********************************************************************
int main(int argc, char *argv[])
{
char				*DataFilename=0;
char				*IncludeFilename=0;
std::vector<char*>	Scripts;
// Parse Input
		for (int Loop=1;Loop<argc;Loop++)
			{
			char *StrPtr=argv[Loop];
			if (StrPtr[0]=='-') StrPtr++;
			switch(StrPtr[0])
				{
				case 'o':	// Output
					if(*StrPtr=':')StrPtr++;			
					DataFilename=StrPtr+1;
					break;
				case 's':	// script
					if(*StrPtr=':')StrPtr++;			
					Scripts.push_back(StrPtr+1);
					break;
				case 'i':	// include file
					if(*StrPtr=':')StrPtr++;			
					IncludeFilename=StrPtr+1;
					break;
				case 'd':	// Cache script
					DebugFlag=1;
					break;
				case 'a':	// Align Size
					if(*StrPtr=':')StrPtr++;			
					AlignSize=atoi(StrPtr+1);
					break;
				case 'b':	// Base Dir
					if(*StrPtr=':')StrPtr++;			
					BaseDir=StrPtr+1;
					break;
				default:
					printf("Unknown Param [%s]\n",StrPtr);
					break;
				}
			}

		if (!AlignSize)	FATAL("ERROR: AlignSize is 0");

		if(!DataFilename || !Scripts.size() || !BaseDir)
		{
			printf("Error in command line\n");
			printf("Usage : MkData [files] -o:OutFile\n");
			printf("        -s\tIn Script\n");
			printf("        -i\tHeaderFile\n");
			printf("        -a\tAlignSize (Default 2048)\n");
			printf("        -d\tDebug Msgs\n");
			printf("        -b\tBase Dir\n");
			return(0);
		}

		if( (AlignBuffer=(char*)malloc(AlignSize))==NULL ) {printf("Couldnt Alloc AlignBuffer.\n");exit(123);}
		for (int i=0;i<AlignSize;i++) AlignBuffer[i]=0;

		strupr(DataFilename);
		printf("Creating Data File: %s\n",DataFilename);
		Banks.Start("",AlignSize);
		for (int Script=0;Script<Scripts.size();Script++) 
			{
			ReadScript(Scripts[Script]);
			}

		printf("Writing Data File: %s\n",DataFilename);
		WriteData(DataFilename);
		if (IncludeFilename) WriteHeader(IncludeFilename);
		printf("FATSize =%i\n",Files.GetCount()*sizeof(sFAT));
	
		return(0);
}

