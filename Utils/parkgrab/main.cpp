/*=========================================================================

	MAIN.CPP	

	Author:  Gary Liddon @ Fareham
	Created: 28th September 1998
	Project: Theme Park World PSX
	Purpose: Main file for tpw sprite grabber (it's sprgrab II!)

	Copyright (c) 1998 G R Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Version info
	
	1.0 - 1.1		Various changes by various authors
	1.2			GL	Added -r switch to allow \ disallow sprite rotation
	1.3			GL	Added -x option to allow saving out of non VRAM sprs
	1.31		GL	Added -f option so you can force offsets to a value
	1.4			TS	Added replacement of bad characters in filenames
	1.5			GL	Added support for BMP files
	1.6			GL	Made all out files lower case
					Checked for errrors when writing files (Doh!)
	1.7			GL	Added -q (turn on\of grabbers shrink to fit routine)
					Also Added -k option to report where each texture has gone in the VRAM,
					This will be read by mikes gin -> ars program for deciding what to do about shared
					textures
	1.71		GL	Added widths and heights of textures to report file
	1.72		GL	Actually made the -q switch do what it was supposed to. Doh!
	1.8			GL	Added -i switch to set output dir for inc files
	1.81		GL	Corrected bug in 1.8 that meant no .h was written
	1.9			GL	-e to set maximum size for textures
	1.91		GL	Changed so that files prefixed with a "+" have col zero as see-through
	2.0			GL	Added reading of gin files and import of rep files
	2.1			GL	Added spare tpage space defintions
	2.11		GL	Added the rather excellent -j option
	2.12		GL	Fixed a bug, stopped -j textures from being reduced
	2.13		GL	Removed the -j option and changed it so that all textures starting with ! are auto -j'd ;)
	2.14		GL	Replaced bit map loader, will move this code into Frame at some point
	2.15		GL	Restricted it so that only images up to 254x254 are allowed
	2.16		GL	Fixed bug that meant 256 col images were often shagged
	2.17		TS	Added '@' filename pre-fix to denote scrolling and transparent textures
	2.2			GL	Changed the autocropping so that if the image is blank it allocates a 1x1 texture instead of a
					texture the size of the input lbm
	2.21		GL	added -twisting_my_melon_man siwtch (don't ask)
	2.3			GL	added -y command to give us tpage compression
	2.31		GL	-y didn't work (not even a little bit) but it does now ;)
	2.4			GL	Added -~ switch to write out a raw tpage chunked version of the VRAM file
	2.41		GL	Added -^ switch to stop any tpages being written
	2.5			TS	Added -h half a tpage flag
	2.6			DO	Added -w only output animated texture frame headers
					Added -b dont output spare boxes
					Added -a align headers to 2k
					
/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <vector>
#include <conio.h>

/*	Glib
	---- */
#include <gtypes.h>
#include <gstring.hpp>
#include <misc.hpp>
#include <gfname.hpp>
#include <pcre.h>

/*	TP Lib
	------- */
#include <ginio.h>
#include <repread.h>


/*	Local
	----- */
#include "sprset.h"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
using namespace std;

/*----------------------------------------------------------------------
	Class defintions
	---------------- */

/*	Hold's a list of all the files to be processed on along with the options that apply to them
	------------------------------------------------------------------------------------------- */
class AllFiles : protected FileCycler
{
public:
	AllFiles(void)
		{
		RecurseFiles=false;
		CrossHair=false;
		ZeroColZero=false;
		MoveUVs=false;
		AllowRotate=true;
		ForceOffsets=false;
		ShrinkToFit=true;
		m_allocateAs16bit=false;
		MaxSize=0;
		m_noSort=false;
		}

	void	SetFileRecursion(bool NewRecurseFiles)
		{RecurseFiles=NewRecurseFiles;}

	void	SetCrossHair(bool NewCrossHair)
		{CrossHair=NewCrossHair;}

	void	SetZeroColZero(bool NewColZero)
		{ZeroColZero=NewColZero;}

	void	SetMoveUVs(bool NewMoveUVs)
		{MoveUVs=NewMoveUVs;}

	void	SetAllowRotate(bool NewAllowRotate)
		{AllowRotate=NewAllowRotate;}

	void	SetForceOffsets(bool NewForceOffsets)
		{ForceOffsets=NewForceOffsets;}

	void	SetShrinkToFit(bool NewSetShrinkToFit)
		{ShrinkToFit=NewSetShrinkToFit;}

	void	SetXOff(int NewXOff)
		{XOff=NewXOff;}

	void	SetYOff(int NewYOff)
		{YOff=NewYOff;}

	void	SetMaxSize(int New)
		{MaxSize=New;}

	int	GetMaxSize(void) const
		{return(MaxSize);}

	void	AddFile(const char * Name);

	FIVec const & GetFileInfoVector(void) const
		{return(AllFileInfos);}

	void	SortOrder();

	void ReadRepFile(char const * Name)
		{
		vector<RepItem>		MyItems;

		::readRepFile(Name,MyItems);

		for (int f=0;f<MyItems.size();f++)
			AllExternalSharedTextures.push_back(MyItems[f].m_texName);

		cout<<"Read report file "<<Name<<endl;
		}

	void setAllocateAs16Bit(bool newVal)
		{m_allocateAs16bit=newVal;}
	
	void setNoSort(void)
		{
		m_noSort=true;
		}

protected:
	vector<GString>		UniqueTexturesSoFar;
	vector<GString>		AllExternalSharedTextures;

	void	FileCallback(char const * FName,int FileNum);

	bool	ShrinkToFit;
	bool	RecurseFiles;
	bool	CrossHair;
	bool	ZeroColZero;
	bool	MoveUVs;
	bool	AllowRotate;
	bool	ForceOffsets;
	
	bool	m_allocateAs16bit;
	bool	m_noSort;

	int		XOff,YOff;

	FIVec	AllFileInfos;
	int		MaxSize;
};

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */
static char *	CycleCommands(char *String,int Num);
static void		Usage(void);
static bool		CheckPlusMinus(const char * Str);
static GString	CheckFileString(const char * Str);
static GString	OutFile;
static GString	OutLbm;
static GString	SprFile;
static GString	ReportFile;
static GString	IncOutFile;

static int		PageBase;
static int		WidthPages;
static int		HeightPages;
static bool		PagePlacements=false;
static bool		s_compressTpages=false;
static bool		s_noWriteTpages=false;
static bool		s_useHalfTpage=false;
static bool		s_AnimatedHeadersOnly=false;
static bool		s_DontOutputBoxes=false;
static bool		s_AlignHeaders=false;

static GString	s_rawTpageFile;

/*----------------------------------------------------------------------
	Vars
	---- */
static AllFiles		MyFiles;

/*----------------------------------------------------------------------
	Data
	---- */

static const float	Version = 2.6;

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int main(int argc,char ** argv)
{
	cerr<<"GRLiddon() PARKGRAB TPW Spr Grabber "<<Version<<endl;
	cerr<<"Copyright (c) 1998/1999G R Liddon. All rights reserved"<<endl<<endl;

	if (argc==1)
		Usage();
	else
		{
		/* TODO:
			Don't like this command line stuff, will change in glib at some point
		*/

		CommandLine(argc,argv,CycleCommands);

		SprSet	MySprSet;

		MySprSet.setHalfTpage(s_useHalfTpage);
		MySprSet.setAnimatedHeader(s_AnimatedHeadersOnly);
		MySprSet.setDontOutputBoxes(s_DontOutputBoxes);
		MySprSet.setAlignHeaders(s_AlignHeaders);
		MySprSet.setCompressTpages(s_compressTpages);
		MySprSet.setNoWriteTpages(s_noWriteTpages);

		MyFiles.SortOrder();

		if (MyFiles.GetMaxSize())
			MySprSet.SetMaxSize(MyFiles.GetMaxSize());

		MySprSet.AddFiles(MyFiles.GetFileInfoVector());

		if (IncOutFile)
			MySprSet.SetIncOutFile(IncOutFile);

		if (SprFile)
			MySprSet.WriteSprFile(SprFile);

		if (OutFile)
			{
			if (PagePlacements)
				{
				MySprSet.Write(OutFile,PageBase,WidthPages,HeightPages);

				if (OutLbm)
					MySprSet.WriteLBM(OutLbm);

				if (ReportFile)
					MySprSet.WriteReport(ReportFile);

				if (s_rawTpageFile)
					MySprSet.writeRawTPage(s_rawTpageFile);

				}
			else
				GObject::Error(ERR_FATAL,"-t option hasn't been specified");
			}
		}

	return(0);
}

/*----------------------------------------------------------------------
	Function:	int CountCommas(char const * Str)
	Purpose:	Count how many commas there are in this str
	Returns:	Number of commas
  ---------------------------------------------------------------------- */
int CountCommas(char const * Str)
{
	int	Len=strlen(Str);
	int	NumOfCommas=0;

	for (int f=0;f<Len;f++)
		{
		if (Str[f]==',')
			NumOfCommas++;
		}

	return(NumOfCommas);
}

/*----------------------------------------------------------------------
	Function:	int ZeroAndCountCommas(char * Str)
	Purpose:	Count how many commas there are in this str
				and change each one to a zero
	Returns:	Number of commas
  ---------------------------------------------------------------------- */
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

/*----------------------------------------------------------------------
	Function:	char *CycleCommands(char *String,int Num)
	Purpose:	Callback to cycle through command line items
	Params:		String = Text of item
				Num = Item number
  ---------------------------------------------------------------------- */
char * CycleCommands(char *String,int Num)
{
	if (GString(String)=="-twisting_my_melon_man")
		{
		MyFiles.setNoSort();
		}
	else
		{
		if (String[0]=='-' || String[0]=='/')
			{
			int			NumOfCommas;
			char		TpText[100];
			GString		TpStr;
			char *		MyText;

			switch (String[1])
				{
				/* Switches */
				case 'w':
					s_AnimatedHeadersOnly=CheckPlusMinus(String);
					break;
				case 'b':
					s_DontOutputBoxes=CheckPlusMinus(String);
					break;
				case 'a':
					s_AlignHeaders=CheckPlusMinus(String);
					break;
				case 'h':
					s_useHalfTpage=CheckPlusMinus(String);
					break;
				case 'y':
					s_compressTpages=CheckPlusMinus(String);
					break;

				case '^':
					s_noWriteTpages=CheckPlusMinus(String);
					break;

				case 'p':
					MyFiles.ReadRepFile(CheckFileString(String));
					break;

				case 'r':
					MyFiles.SetAllowRotate(CheckPlusMinus(String));
					break;

				case 's':
					MyFiles.SetFileRecursion(CheckPlusMinus(String));
					break;

				case 'c':
					MyFiles.SetCrossHair(CheckPlusMinus(String));
					break;

				case 'z':
					MyFiles.SetZeroColZero(CheckPlusMinus(String));
					break;

				case 'm':
					MyFiles.SetMoveUVs(CheckPlusMinus(String));
					break;

				case 'q':
					MyFiles.SetShrinkToFit(CheckPlusMinus(String));
					break;

				/* Options that set output files */

				case 'o':
					OutFile=CheckFileString(String);
					break;

				case 'x':
					SprFile=CheckFileString(String);
					break;

				case 'l':
					OutLbm=CheckFileString(String);
					break;

				case 'k':
					ReportFile=CheckFileString(String);
					break;

				case 'i':
					IncOutFile=CheckFileString(String);
					break;

				case 'e':
					{
					GString		MaxSize;
					MaxSize=CheckFileString(String);
					MyFiles.SetMaxSize(atoi(MaxSize));
					}
					break;

				/* More complex options */

				case 't':
					TpStr=CheckFileString(String);
					
					strcpy(TpText,TpStr);
					
					NumOfCommas=ZeroAndCountCommas(TpText);

					if (NumOfCommas != 2)
						GObject::Error(ERR_FATAL,"Problem with option %s",String);

					MyText=TpText;

					PageBase=atoi(MyText);
					MyText+=strlen(MyText)+1;

					WidthPages=atoi(MyText);
					MyText+=strlen(MyText)+1;

					HeightPages=atoi(MyText);

					PagePlacements=true;
					break;

				case 'f':
					TpStr=CheckFileString(String);
					
					strcpy(TpText,TpStr);
					
					NumOfCommas=ZeroAndCountCommas(TpText);

					if (NumOfCommas != 1)
						GObject::Error(ERR_FATAL,"Problem with option %s",String);

					MyText=TpText;

					MyFiles.SetXOff(atoi(MyText));
					MyText+=strlen(MyText)+1;

					MyFiles.SetYOff(atoi(MyText));
					MyText+=strlen(MyText)+1;

					MyFiles.SetForceOffsets(true);;
					break;
					
				case '~':
					s_rawTpageFile=CheckFileString(String);
					break;

				default:
					GObject::Error(ERR_FATAL,"Unknown switch %s",String);
					break;
				}
			}
		else
			{
			GString	UpperName(String);
			UpperName.Upper();
			MyFiles.AddFile(UpperName);
			}
		}



	return(String);
}

/*----------------------------------------------------------------------
	Function:	void CheckPlusMinus(const char * Str)
	Purpose:	Check to see if this cmd line item is of the type
				-<char>+|-. Aborts out of there was a problem.
	Params:		Str = Text of item
	Returns:	true if a plus option
  ---------------------------------------------------------------------- */
bool CheckPlusMinus(const char * Str)
{
	if (strlen(Str)==3 && (Str[0]=='-' || Str[0]=='/') && (Str[2]=='-' || Str[2]=='+'))
		{
		}
	else
		GObject::Error(ERR_FATAL,"Problem with cmd line option %s",Str);

	return(Str[2]=='+');
}

/*----------------------------------------------------------------------
	Function:	void CheckPlusMinus(const char * Str)
	Purpose:	Check to see if this cmd line item is of the type
				-<char>+|-. Aborts out of there was a problem.
	Params:		Str = Text of item
	Returns:	true if a plus option
  ---------------------------------------------------------------------- */
GString CheckFileString(const char * Str)
{
	GString	RetStr;

	if (strlen(Str)>3 && (Str[0]=='-' || Str[0]=='/') && (Str[2]==':' || Str[2]==':'))
		RetStr=&Str[3];
	else
		GObject::Error(ERR_FATAL,"Problem with cmd line option %s",Str);

	return(RetStr);
}


/*----------------------------------------------------------------------
	Function:		void Usage(void)
	Notes:			Usage of the util
  ---------------------------------------------------------------------- */
void Usage(void)
{
	cout<<"Usage: PARKGRAB <file> [ <file>.. ] [ switches.. ]\n";
	cout<<"Switches:\n"<<endl;
	cout<<"-s<+|->\t\t\tSet recurse sub dirs			(default off)"<<endl;
	cout<<"-c<+|->\t\t\tSet cross hair offset detection	(default off)"<<endl;
	cout<<"-z<+|->\t\t\tSet colour zero trans			(default off)"<<endl;
	cout<<"-m<+|->\t\t\tSet move in u/v coords			(default off)"<<endl;
	cout<<"-r<+|->\t\t\tSet if spr rotation allowed		(default  on)"<<endl;
	cout<<"-q<+|->\t\t\tShrink to bounds of lbm			(default  on)"<<endl;
	cout<<"-y<+|->\t\t\tCompress tpages					(default off)"<<endl;
	cout<<"-^<+|->\t\t\tDo not write a tpage			(default off)"<<endl;
	cout<<"-h<+|->\t\t\tForce tpage height to half		(default off)"<<endl;
	cout<<"-w<+|->\t\t\tOnly write animated headers		(default off)"<<endl;
	cout<<"-b<+|->\t\t\tDo not write spare boxes		(default off)"<<endl;
	cout<<"-a<+|->\t\t\tAlign headers to 2k chunk		(default off)"<<endl;
	cout<<endl;
	cout<<"-f:x,y\t\t\tForce offsets for sprs to x,y"<<endl;
	cout<<"-t:b,w,h\t\tt=base tp,x=# tps wide,h=# tps high"<<endl;
	cout<<"-e:<maxsize>\t\tSet maximum size for textures"<<endl;
	cout<<"-p:<rep file>\t\tRead in a shared texture file for gin files"<<endl;
	cout<<endl;
	cout<<"-o:<filename>\t\tOutput Vram filename"<<endl;
	cout<<"-x:<filename>\t\tOutput Sprfile filename"<<endl;
	cout<<"-l:<tplbmname>\t\tWrite out an lbm of the tpage "<<endl;
	cout<<"-k:<repname>\t\tWrite out a vram report"<<endl;
	cout<<"-~:<filename>\t\tWrite out raw VRAM image"<<endl;
	cout<<endl;
	cout<<"-twisting_my_melon_man"<<endl;
	cout<<endl;
}

/*----------------------------------------------------------------------
	Function:	void AllFiles::AddFile(const char * Name)
	Purpose:	Add a file to the list, it takes all the current
				properties that the AllFiles object has.
	Params:		Name of file to add
  ---------------------------------------------------------------------- */
void AllFiles::AddFile(const char * Name)
{
	DoCycle(Name,RecurseFiles);
}

/*----------------------------------------------------------------------
	Function:	void AllFiles::FileCallback(char const * FName,int FileNum)
  ---------------------------------------------------------------------- */
#include <gintex.h>
#include <algorithm>

void AllFiles::FileCallback(char const * FName,int FileNum)
{
	FileInfo	MyInfo;
	bool		ThisZeroColZero;


	GFName		FileName(FName);

	GString		Ext(FileName.Ext());
	
	Ext.Lower();

	if (Ext=="gin")
		{
		vector<GString> NonSharedTextures;

//		Gin4File		MyFile;
		CScene		MyFile;
		MyFile.Load(FName);

		MyFile.GetNonSharedTextures(AllExternalSharedTextures,NonSharedTextures);


		vector<GString> NewEntrys;

		for (int f=0;f<NonSharedTextures.size();f++)
			{
			if (find(UniqueTexturesSoFar.begin(),UniqueTexturesSoFar.end(),NonSharedTextures[f]) == UniqueTexturesSoFar.end())
				{
				NewEntrys.push_back(NonSharedTextures[f]);
				UniqueTexturesSoFar.push_back(NonSharedTextures[f]);
				}
			}


		for (f=0;f<NewEntrys.size();f++)
			FileCallback(NewEntrys[f],0);

		cout<<"Read gin file *"<<FName<<"*"<<endl;
		}
	else
		{
		GFName	Name(FName);

		if (Name.File()[0]=='+')
			ThisZeroColZero=true;
		else
			ThisZeroColZero=ZeroColZero;


		if (Name.File()[0]=='!')
			m_allocateAs16bit=true;
		else
			m_allocateAs16bit=false;

		if (Name.File()[0] == '@')
		{
			ThisZeroColZero = true;
			m_allocateAs16bit = true;
		}

		GString TheFile(FName);
		TheFile.Lower();
			
		MyInfo.SetInfo(TheFile,CrossHair,ThisZeroColZero,MoveUVs,AllowRotate,ShrinkToFit,m_allocateAs16bit);
		cout<<"Add image file "<<TheFile<<endl;

		if (ForceOffsets)
			MyInfo.SetForceOffsets(XOff,YOff);
		
		AllFileInfos.resize(AllFileInfos.size()+1);
		AllFileInfos[AllFileInfos.size()-1]=MyInfo;
		}
}


/*----------------------------------------------------------------------
	Function:	void AllFiles::FileCallback(char const * FName,int FileNum)
  ---------------------------------------------------------------------- */
void AllFiles::SortOrder()
{
	if (!m_noSort)
		{
		int i,j;
		FileInfo MyInfo;
		int count = AllFileInfos.size();

		for (i=0; i<count; i++)
			{
			for (j=0; j<count; j++)
				{
				if (strcmp(AllFileInfos[i].GetFileName(), AllFileInfos[j].GetFileName())<0)
					{
					MyInfo = AllFileInfos[i];
					AllFileInfos[i] = AllFileInfos[j];
					AllFileInfos[j] = MyInfo;
					}
				}
			}
		}
}
/*===========================================================================
 end */
