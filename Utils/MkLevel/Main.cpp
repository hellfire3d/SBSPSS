/*******************************/
/*** SpongeBob Level Creator ***/
/*******************************/

#include	"stdio.h"
#include	<misc.hpp>
#include	<vector>
#include	<DaveLib.h>
#include	<GFName.hpp>

#include	"MkLevel.h"

//***************************************************************************
CMkLevel	Level;
int			TPBase=-1,TPW=-1,TPH=-1;

//***************************************************************************
char * CycleCommands(char *String,int Num)
{
char	Text[256],*TextPtr;
int		Count;

		if (String[0]=='-' || String[0]=='/')
		{
			GString		TpStr;
			TpStr= String;
			TpStr.Upper();
			switch (String[1])
			{// Switches 
				case 'o':
					OutStr = CheckFileString(String);
					break;
				case 'd':
					DebugOn =true;
					break;
				case 's':
					TpStr= CheckFileString(String);
					Scale=atof(TpStr);
					break;
				case 't':
					TpStr= CheckFileString(String);
					TextPtr=Text;
					strcpy(TextPtr,TpStr);
					Count=ZeroAndCountCommas(TextPtr);
					if (Count!=2)
							GObject::Error(ERR_FATAL,"Problem with option %s\n",String);
					TPBase=atol(TextPtr);
					TextPtr+=strlen(TextPtr)+1;
					TPW=atol(TextPtr);
					TextPtr+=strlen(TextPtr)+1;
					TPH=atol(TextPtr);
					break;
				case 'q':
					StripLength=4;
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

		return(String);
}

//***************************************************************************

void Usage(char *ErrStr)
{
	printf("\nMkLevel: by Dave\n");
	printf("Usage: MkLevel <file> [ <file>.. ] [ switches.. ]\n");
	printf("Switches:\n");
	printf("   -o:[FILE]       Set output File (AND Dir)\n");
	printf("   -s:nn           Set Scaling value\n");
	printf("   -t:p,w,h        Set TPage No,Width,Height\n");
//	printf("   -c:             Set Chapter Name\n");
//	printf("   -l:             Set Level Name\n");
	printf("   -d:             Enable Debug output\n");
//	printf("   -q:             Enable Quadding\n");
	GObject::Error(ERR_FATAL,ErrStr);
}

//***************************************************************************

int		main(int argc, char *argv[])
{
		CommandLine(argc,argv,CycleCommands);

std::vector<GString> const &Files = MyFiles.GetFileInfoVector();

		if (Files.size()==0)	Usage("No Levels specified\n");
		if (Files.size()>1)		Usage("Too many Levels specified\n");

		Level.Init(Files[0],OutStr,TPBase,TPW,TPH);
		Level.Load();
		Level.Process();
		Level.Write();
		return 0;
}



