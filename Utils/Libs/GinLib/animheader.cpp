/*=========================================================================

	ANIMHEADER.CPP	

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

/*	Local
	----- */
#include "animheader.h"

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

void MakeAnimHeader( char * line, AnimHeaderItem * item )
{
	int		len;
	int		pos;
	char	anim[1024];
	char	animno[8];

	pos = 0;
	len = strlen( line );

	if (len > 4)
	{
		int	count = 0;

		// FIND ANIM
		while(count != 3)
		{
			while(line[pos] != '_')
			{
				pos++;
				if (pos >= len)
				{
					GObject::Error(ERR_FATAL,"Header scan overrun line - %s\n", line);
				}
			}
			pos++;
			count++;
		}

		// GET ANIM NAME
		count = 0;
		while(line[pos] != ' ')
		{
			anim[count++] = line[pos++];
			if (pos > len)
			{
				GObject::Error(ERR_FATAL,"Header scan overrun line - %s\n", line);
			}
		}
		anim[count] = 0;

		// GET ANIM NUMBER
		pos += 3;
		count = 0;
		while(line[pos] != ',')
		{
			animno[count++] = line[pos++];
			if (pos > len)
			{
				GObject::Error(ERR_FATAL,"Header scan overrun line - %s\n", line);
			}
		}
		animno[count] = 0;

		strupr( anim );
		item->m_AnimName = anim;
		item->m_Frame = atoi( animno );
	}
	else
	{
		GObject::Error(ERR_FATAL,"Incorrect header line - %s\n", line);
	}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void readAnimHeaderFile(char const * Name, std::vector<AnimHeaderItem> & Items)
{
	char Line[1024];

	try
	{
		ifstream InStream;

		InStream.open(Name,ios::in);

		if (InStream)
		{
			InStream.getline(Line,1024);		// HEADER GUARD
			InStream.getline(Line,1024);		// HEADER GUARD
			InStream.getline(Line,1024);		// ENUM
			InStream.getline(Line,1024);		// {
			InStream.getline(Line,1024);		// ANIM COUNT

			while (!InStream.eof() && Line[0] != '}')
			{
				InStream.getline(Line,1024);

				if (strlen(Line) && Line[0] != '}')
				{
					AnimHeaderItem	MyItem;

					MakeAnimHeader( Line, &MyItem );

					InStream.getline(Line,1024);	// NUMBER OF FRAMES

					Items.push_back( MyItem );
				}
			}
			InStream.close();
		}
		else
		{
			throw("Error opening anim header file");
		}
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
void AnimHeaderFile::load(char const * name)
{
	vector<AnimHeaderItem>	items;

	readAnimHeaderFile(name,items);

	int		numOfItems;

	numOfItems=items.size();

	for (int f=0;f<numOfItems;f++)
	{
		AnimHeaderItem	dummyResult;

//		if (find(items[f].m_texName,dummyResult))
//			GObject::Error(ERR_FATAL,"%s already defined in anim header file database",(char const *)items[f].m_texName);

//		makeTexNameCompliant(items[f].m_texName);

//		m_strToInfo[items[f].m_texName]=items[f];
	}

	m_lastLoadedFile=name;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void AnimHeaderFile::makeTexNameCompliant(GString & str)
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
bool AnimHeaderFile::find(GString const & str,AnimHeaderItem & result)
{
	GString		srchStr;
	map<GString,AnimHeaderItem>::iterator	it;

	srchStr=str;

	makeTexNameCompliant(srchStr);

	it=m_strToInfo.find(srchStr);

	if (it == m_strToInfo.end())
	{
		return(false);
	}
	else
	{
		result=m_strToInfo[srchStr];
		return(true);
	}
}

/*===========================================================================
 end */


