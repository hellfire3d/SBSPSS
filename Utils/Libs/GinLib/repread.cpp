/*=========================================================================

	FILENAME.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 G R Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <strstream>

/*	Glib
	---- */
#include <pcre.h>
#include <gobject.hpp>
#include <gfname.hpp>
/*	Local
	----- */
#include "repread.h"

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
static int GetHex(char const * Str);

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */
/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int GetHex(char const * Str)
{
	char	Buffer[1024];
	strcpy(Buffer,Str);

	strstream	i(Buffer,1024);

	int	Val;

	i>>hex>>Val;
	return(Val);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void readRepFile(char const * Name,std::vector<RepItem> & Items)
{
	char Line[1024];

	try
		{
		ifstream InStream;

		InStream.open(Name,ios::in);

		if (InStream)
			{
			int			frameNumber;

			frameNumber=0;


			InStream.getline(Line,1024);

			pcre *			MyPattern;
			int				Matches;
			const int		NumOfOffsets=60;
			int				Offsets[NumOfOffsets];
			char const *	ErrTxt;
			int				ErrNum;

			MyPattern=pcre_compile("(\\S+)\\s+\\$(\\S+)\\s+\\$(\\S+)\\s+\\$(\\S+)\\s+\\$(\\S+)\\s+\\$(\\S+)\\s+\\$(\\S+)\\s+(\\d+)",0,&ErrTxt,&ErrNum,NULL);

			while (!InStream.eof())
				{

				char			SwitchText[100];

				InStream.getline(Line,1024);

				if (strlen(Line))
					{
					Matches=pcre_exec(MyPattern,NULL,Line,strlen(Line),0,Offsets,NumOfOffsets);
					
					if (Matches != 9)
						throw("error parsing rep file");

					int		Tpage,Clut,u,v,w,h;
					bool	Rotated;
					
					Tpage=GetHex(&Line[Offsets[4]]);
					Clut=GetHex(&Line[Offsets[6]]);
					u=GetHex(&Line[Offsets[8]]);
					v=GetHex(&Line[Offsets[10]]);
					w=GetHex(&Line[Offsets[12]]);
					h=GetHex(&Line[Offsets[14]]);
					Rotated=atoi(&Line[Offsets[16]]) != 0;

					
					memcpy(SwitchText,&Line[Offsets[2]],Offsets[3]-Offsets[2]);
					SwitchText[Offsets[3]-Offsets[2]]=0;

					RepItem		MyItem;

					MyItem.m_texName=SwitchText;
					MyItem.m_texName.Lower();

					MyItem.m_tpage=Tpage;
					MyItem.m_clut=Clut;
					MyItem.m_u=u;
					MyItem.m_v=v;
					MyItem.m_w=w;
					MyItem.m_h=h;
					MyItem.m_rotated=Rotated;
					MyItem.m_frameNumber=frameNumber;

					Items.push_back(MyItem);
					
					frameNumber++;
					}

				}
			InStream.close();
			}
		else
			throw("Error opening report file");
		}

	catch (char const * E)
		{
		GObject::Error(ERR_FATAL,"Error in AddToTexList : %s",E);
		}

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void RepFile::load(char const * name)
{
	vector<RepItem>		items;

	readRepFile(name,items);

	int		numOfItems;

	numOfItems=items.size();

	for (int f=0;f<numOfItems;f++)
		{
		RepItem	dummyResult;

		if (find(items[f].m_texName,dummyResult))
			GObject::Error(ERR_FATAL,"%s already defined in rep file database",(char const *)items[f].m_texName);

		makeTexNameCompliant(items[f].m_texName);

		m_strToInfo[items[f].m_texName]=items[f];
		}

	m_lastLoadedFile=name;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void RepFile::makeTexNameCompliant(GString & str)
{
	str.Lower();
	str.Filter("\\",'/');
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void RepFileNoDir::makeTexNameCompliant(GString & str)
{
	GFName	texFileName(str);
	str=texFileName.File();
	str.Lower();
}
/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool RepFile::find(GString const & str,RepItem & result)
{
	GString							srchStr;
	map<GString,RepItem>::iterator	it;

	srchStr=str;

	makeTexNameCompliant(srchStr);

	it=m_strToInfo.find(srchStr);

	if (it == m_strToInfo.end())
		return(false);
	else
		{
		result=m_strToInfo[srchStr];
		return(true);
		}
}

/*===========================================================================
 end */


