/*=========================================================================

	MAIN.CPP	

	Author:  Gary Liddon @ Fareham
	Created: 3rd March 1999
	Project: Theme Park World Playstation
	Purpose: Main module for Folio

	Copyright (c) 1998 G R Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <vector>
#include <conio.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <fcntl.h>
#include <io.h>

/*	Glib
	---- */
#include <misc.hpp>
#include <gfname.hpp>
#include <gstring.hpp>
#include <pcre.h>

/*	tplib
	---- */
#include <trans.h>
#include <lang.h>
#include <script.h>


/*	local
	---- */
#include "kanjiclass.h"

/*----------------------------------------------------------------------
	Version info
	
	1.0 - 1.2	GL	Initial working version
	1.3			GL	Longer line length (now 4096)
	1.4			GL	Added comments to file
	1.5			GL	Changed it so that \n are now returns
	1.6			GL	Added a slew of stuff to handle kanji
	1.7			GL	Chganged format of kanji table
	1.8			GL	Removed bug with kanji frame lookup
	1.9			GL	added -s so user can specify the size of output font
	1.91		GL  Transtext also writes out an ID text file (id.dat)
	1.92		GL	fixed bug with output kanji table being too short

/*----------------------------------------------------------------------

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
using namespace std;

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */
static void		usage(void);
static char *	cycleCommands(char *String,int Num);

/*----------------------------------------------------------------------
	Vars
	---- */
static const float	version=1.92f;
static GString		fileWithTranslations;
static GString		headerFile;
static GString		outDir;

static GString		g_japKanjiFile;
static GString		g_japLbmDir;
static GString		g_englishLbmsDir;
static GString		g_japLbmListFile;
static int			g_fontSize=0;

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int main(int argc, char ** argv)
{
	cerr<<"GRLiddon() TRANSTEXT text translation stuphh "<<version<<endl;
	cerr<<"Copyright (c) 1999 G R Liddon. All rights reserved"<<endl<<endl;

	if (argc==1)
		usage();
	else
		{
		CommandLine(argc,argv,cycleCommands);

		if (g_englishLbmsDir)
			{
			KanjiClass::writeEnglishFontLbms(g_englishLbmsDir,KanjiClass::KanjiFontSize(g_fontSize));
			}

		if (fileWithTranslations)
			{
			AllTextDatabases	myDatabases;
			myDatabases.readTranslationFile(fileWithTranslations);

			if (outDir)
				myDatabases.write(outDir);

			if (headerFile)
				myDatabases.writeHeaderFile(headerFile);


			/* Do some kanji jiggery pokery */


			if (g_japKanjiFile || g_japLbmDir || g_japLbmListFile) 
				{
				TextDatabase *		jDataBase;
				int					numOfStrings;
				KanjiClass			kClass;

				jDataBase=myDatabases.getTextDataBase(JAPANESE);
				numOfStrings=jDataBase->getNumOfStrings();

				for (int f=0;f<numOfStrings;f++)
					kClass.addStr((u8 const*)jDataBase->getText(f));

				if (g_japLbmDir)
					{
					if (g_fontSize >=0 && g_fontSize < 3)
						kClass.saveKanjiLbms(g_japLbmDir,KanjiClass::KanjiFontSize(g_fontSize));
					else
						GObject::Error(ERR_FATAL,"illegal font size parameter");
					}

				if (g_japKanjiFile)
					kClass.saveKanjiTable(g_japKanjiFile);

				if (g_japLbmListFile && g_japLbmDir)
					kClass.saveKanjiLbmNames(g_japLbmDir,g_japLbmListFile);

				cout<<"text uses "<<kClass.howManyKanjiChars()<<endl; 
				}
			}
		else
			GObject::Error(ERR_FATAL,"you must define a translation file");
		}

	return(0);
}


/*----------------------------------------------------------------------
	Function:		void Usage(void)
	Notes:			Usage of the util
  ---------------------------------------------------------------------- */
void usage(void)
{
	cout<<"Usage: TRANSTEXT <in file> [ switches.. ]\n";
	cout<<"Switches:\n"<<endl;
	cout<<"-o:<dir>            Dir to save translation strings to"<<endl;
	cout<<"-e:<dir>            Dir write ascii images to"<<endl;
	cout<<"-h:<file>           File to save lang enums to"<<endl;
	cout<<endl;
	cout<<"-d:<dir>            Dir to save kanji lbms to"<<endl;
	cout<<"-j:<file>           File to save kanji table to"<<endl;
	cout<<"-p:<file>           File to save list of saved lbms to"<<endl;
	cout<<endl;
	cout<<"-s:<0|1|2>          Set the size of the output font"<<endl;
	cout<<endl;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
  ---------------------------------------------------------------------- */
GString	getFileString(char const * Switch)
{
	pcre *			MyPattern;
	int				Matches;
	const int		NumOfOffsets=20;
	int				Offsets[NumOfOffsets];
	char const *	ErrTxt;
	int				ErrNum;
	char			SwitchText[100];

	MyPattern=pcre_compile("^-.:(.*)",0,&ErrTxt,&ErrNum,NULL);
	Matches=pcre_exec(MyPattern,NULL,Switch,strlen(Switch),0,Offsets,NumOfOffsets);

	if (Matches != 2)
		GObject::Error(ERR_FATAL,"Ill formatted switch %s",Switch);

	memcpy(SwitchText,&Switch[Offsets[2]],Offsets[3]-Offsets[2]);
	SwitchText[Offsets[3]-Offsets[2]]=0;

	return(GString(SwitchText));
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
  ---------------------------------------------------------------------- */
bool getPlusMinus(char const * Switch)
{
	pcre *			MyPattern;
	int				Matches;
	const int		NumOfOffsets=20;
	int				Offsets[NumOfOffsets];
	char const *	ErrTxt;
	int				ErrNum;

	MyPattern=pcre_compile("^-.([+|-])",0,&ErrTxt,&ErrNum,NULL);
	Matches=pcre_exec(MyPattern,NULL,Switch,strlen(Switch),0,Offsets,NumOfOffsets);

	if (Matches != 2)
		GObject::Error(ERR_FATAL,"Ill formatted switch %s",Switch);

	return(Switch[Offsets[2]]=='+');
}

/*----------------------------------------------------------------------
	Function:	char *CycleCommands(char *String,int Num)
	Purpose:	Callback to cycle through command line items
	Params:		String = Text of item
				Num = Item number
  ---------------------------------------------------------------------- */
char * cycleCommands(char * cmdString,int num)
{
	if (cmdString[0]=='-' || cmdString[0]=='/')
		{
		switch (cmdString[1])
			{
			case 'e':
				g_englishLbmsDir=getFileString(cmdString);
				break;

			case 'h':
				headerFile=getFileString(cmdString);
				break;

			case 'o':
				outDir=getFileString(cmdString);
				break;

			case 'j':
				g_japKanjiFile=getFileString(cmdString);
				break;

			case 'd':
				g_japLbmDir=getFileString(cmdString);
				break;

			case 'p':
				g_japLbmListFile=getFileString(cmdString);
				break;

			case 's':
				g_fontSize=atoi(getFileString(cmdString));
				break;

			default:
				GObject::Error(ERR_FATAL,"unknown switch %s",cmdString);
				break;
			}
		}
	else
		{
		if (fileWithTranslations)
			GObject::Error(ERR_FATAL,"translation file already defined as %s",(char const *)fileWithTranslations);
		else
			fileWithTranslations=cmdString;
		}

	return(cmdString);
}




/*===========================================================================
 end */
