//--------------------------------------------------------------------------
//	Miscellaneous functions
//	Box Files
//
//	Gary Liddon @ Probe
//	21/11/92
//
//	(c)1992-1998 Gary Robert Liddon
//
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//	Includes
//	--------
#include "gtypes.h"
#include <vector>
#include <io.h>
#include <string.h>
#include <conio.h>

#include <fstream>

#include "misc.hpp"
#include "gfname.hpp"

using namespace std;

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */


bool copyFile(char const * Dest,char const * Source,bool Overwrite)
{
	ifstream		InFile;
	vector<u8>		Data;

	int		Size;

	Size=FileSize(Source);

	if (Size >= 0)
		{
		Data.resize(Size);

		ifstream	In;
		In.open(Source,ios::binary);
		In.read((char *)(&Data[0]),Size);
		In.close();

		if (In)
			{
			ofstream		OutFile;
			int				OpenMode;

			OpenMode=ios::binary;

			if (Overwrite)
				OpenMode|=ios::trunc;

			OutFile.open(Dest,OpenMode);

			if (OutFile)
				{
				OutFile.write((char*)&Data[0],Data.size());
				OutFile.close();
				}
			else
				GObject::Error(ERR_FATAL,"Unable to open out file %s",Dest);
			}
		else
			GObject::Error(ERR_FATAL,"Unable to open in file %s",Source);
		}

	return(true);
}

int FileCycler::DoCycle(char const *Spec,bool Recurse)
{
	int 	FileNum=0;
	int 	Handle;
	GFName  InName(Spec);
	char *	Speccy;

	if(!(Speccy=strdup(Spec)))
		GObject::Error(ERM_OUTOFMEM);

	_finddata_t FindBlock;

	/* Do dirs first */

	if (Recurse)
		{
		GFName	DirName(Spec);

		DirName.File("*");
		DirName.Ext("*");

		if ((Handle = _findfirst((char *) DirName.FullName(),&FindBlock))!=-1)
			{
			BOOL	Done;

			Done=FALSE;
	
			while (!Done)
				{
				if (FindBlock.attrib&_A_SUBDIR)
					{
					if (strcmp(FindBlock.name,".") && strcmp(FindBlock.name,".."))
						{
						GFName	TempName(InName.FullName());

						if (TempName.Dir())
							{
							char *	NewSub;

							if (!(NewSub=new char [strlen(TempName.Dir()) +strlen (FindBlock.name)+1+2] ))
								GObject::Error(ERM_OUTOFMEM);

							sprintf(NewSub,"%s\\%s",TempName.Dir(),FindBlock.name);

							TempName.Dir(NewSub);

							delete NewSub;
							}
						else
							TempName.Dir(FindBlock.name);

						DoCycle(TempName.FullName(),Recurse);
						}
					}
				Done=_findnext(Handle,&FindBlock);
				}
			}
		}	

	if ((Handle = _findfirst(Speccy,&FindBlock))!=-1)
		{
		BOOL	Done;
		Done=FALSE;

		while (!Done)
			{

			if (!(FindBlock.attrib&_A_SUBDIR))
				{
				GFName	TempName(InName.FullName());
				GFName	Temp2(FindBlock.name);
				TempName.File(Temp2.File());
				TempName.Ext(Temp2.Ext());
				FileCallback(TempName.FullName(),FileNum++);
				}

			Done=_findnext(Handle,&FindBlock);
			}
		}

	free(Speccy);

	return FileNum;
}


//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Cycle through files of a particular spec, Returns Number of Files found
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int	CycleFiles(char const *Spec,void (*Func)(char const *Fname,int Num),BOOL Recurse)
{
	int 	FileNum=0;
	int 	Handle;
	GFName  InName(Spec);
	char *	Speccy;

	if(!(Speccy=strdup(Spec)))
		GObject::Error(ERM_OUTOFMEM);

	_finddata_t FindBlock;

	/* Do dirs first */

	if (Recurse)
		{
		GFName	DirName(Spec);

		DirName.File("*");
		DirName.Ext("*");

		if ((Handle = _findfirst((char *) DirName.FullName(),&FindBlock))!=-1)
			{
			BOOL	Done;

			Done=FALSE;
	
			while (!Done)
				{
				if (FindBlock.attrib&_A_SUBDIR)
					{
					if (strcmp(FindBlock.name,".") && strcmp(FindBlock.name,".."))
						{
						GFName	TempName(InName.FullName());

						if (TempName.Dir())
							{
							char *	NewSub;

							if (!(NewSub=new char [strlen(TempName.Dir()) +strlen (FindBlock.name)+1+2] ))
								GObject::Error(ERM_OUTOFMEM);

							sprintf(NewSub,"%s\\%s",TempName.Dir(),FindBlock.name);

							TempName.Dir(NewSub);

							delete NewSub;
							}
						else
							TempName.Dir(FindBlock.name);

						CycleFiles(TempName.FullName(),Func,Recurse);
						}
					}
				Done=_findnext(Handle,&FindBlock);
				}
			}
		}	

	if ((Handle = _findfirst(Speccy,&FindBlock))!=-1)
		{
		BOOL	Done;
		Done=FALSE;

		while (!Done)
			{

			if (!(FindBlock.attrib&_A_SUBDIR))
				{
				GFName	TempName(InName.FullName());
				GFName	Temp2(FindBlock.name);
				TempName.File(Temp2.File());
				TempName.Ext(Temp2.Ext());
				Func(TempName.FullName(),FileNum++);
				}

			Done=_findnext(Handle,&FindBlock);
			}
		}

	free(Speccy);

	return FileNum;
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Get Switch Info
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
BOOL SwitchInfo(char *String)
{
	if (strlen(String)!=3 || (String[2] != '+' && String[2] != '-'))
		{
		GObject::Error(ERR_FATAL,"%s option formatted incorrectly",String);
		return 0;
		}
	else
		return String[2]=='+' ? 1 : 0;
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Command Line Constructor
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CommandLine::CommandLine(int argc,char **argv,char *(*Func)(char *String,int Num))
{
	LastItem=NULL;
	InStream=NULL;
	CommandItem=0;

	if (!(LastItem=new char[1000]))
		Error(ERM_OUTOFMEM);

	MyArgc=argc;
	MyArgv=argv;

	GetNextItem();			// Skip progname

	int x=0;

	char *SpamString;

	while ((SpamString=GetNextItem()))
		{
		Func(SpamString,x);
		x++;
		}
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Command Line Destructor
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CommandLine::~CommandLine()
{
	if (InStream)
		delete InStream;

	if (LastItem)
		delete LastItem;
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Get The Next Item in the Commandline string
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
char *CommandLine::GetNextItem()
{
	char *RetStuff=NULL;

	if (!InStream)
		RetStuff=GetNextCommandLineItem();
	else
		RetStuff=GetNextScriptFileItem();
			
	return RetStuff;
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Get The Next Item in the Argv list
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
char *CommandLine::GetNextCommandLineItem()
{
	char *RetStuff=NULL;

	if (CommandItem!=MyArgc)
		{
		strcpy(LastItem,MyArgv[CommandItem]);
		RetStuff=LastItem;

		CommandItem++;

		if (*RetStuff=='@')
			{
			if (InStream)
				delete InStream;

			if (!(InStream = new ifstream (&RetStuff[1])))
				Error(ERM_OUTOFMEM);

			if (!(*InStream))
				Error(ERR_FATAL,"Cannot open script file %s",&RetStuff[1]);

			if (!(RetStuff=GetNextScriptFileItem()))
				RetStuff=GetNextCommandLineItem();
			}
		}

	return RetStuff;
}



//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Get The Next Item from the currently open script file
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
char *CommandLine::GetNextScriptFileItem()
{
	char *RetStuff=NULL;

	(*InStream)>>LastItem;

	if (InStream->eof())
		{
		delete InStream;
		InStream=NULL;
		RetStuff=GetNextCommandLineItem();
		}
	else
		RetStuff=LastItem;

	return RetStuff;
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Does this file exist?
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
bool FileExists(const char *String)
{
	ifstream	InStream;
	bool		RetVal;

	InStream.open(String,ios::in);
	if (InStream)
		RetVal=true;
	else
		RetVal=false;

	InStream.close();
	return(RetVal);
}


//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	What is the size of this file?
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int FileSize(const char *String)
{
	ifstream	InStream;
	int			RetVal;

	RetVal=-1;

	InStream.open(String,ios::in);

	if (InStream)
		{
		InStream.seekg(0,ios::end);
		RetVal=InStream.tellg();
		InStream.close();
		}

	return(RetVal);
}



//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//ends
