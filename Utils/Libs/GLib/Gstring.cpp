/*=========================================================================

	GSTRING.CPP

	Author:   Gary Liddon @ Farehame
	Created:  4th April 1997
	Purpose:  Generic string class

	Copyright (c) 1997 Gary Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include "string.h"

/*	Glib
	---- */

/*	Local
	----- */
#include "gstring.hpp"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */
using namespace std;

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Function:		GString::GString()
	Notes:			Blank constructor
  ---------------------------------------------------------------------- */
GString::GString()
{
	Init();
}

/*----------------------------------------------------------------------
	Function:		GString::~GString()
	Notes:			Destructor
  ---------------------------------------------------------------------- */
GString::~GString()
{
	Dump();
}

/*----------------------------------------------------------------------
	Function:		GString::GString(GString const & Gs)
	Notes:			Copy constructor
  ---------------------------------------------------------------------- */
GString::GString(GString const & Gs)
{
	Init();
	(*this)=Gs;
}


/*----------------------------------------------------------------------
	Function:		GString & GString::operator+(GString const & Gs)
	Notes:			Add one string to another
  ---------------------------------------------------------------------- */
GString GString::operator+(GString const & Gs)
{
	GString	RetStr;

	if (Gs.Len())
		{
		if (Len())
			{
			char *	NewStr;
			if (!(NewStr=new char[Gs.Len()+Len()+1]))
				Error(ERM_OUTOFMEM);
			strcpy(NewStr,*this);
			strcat(NewStr,Gs);
			RetStr=NewStr;

			delete NewStr;
			}
		else
			RetStr=Gs;
		}
	else
		RetStr=*this;

	return(RetStr);
}

/*----------------------------------------------------------------------
	Function:
	Notes:
  ---------------------------------------------------------------------- */
GString & GString::operator+=(GString const & Str)
{
	*this=operator+(Str);
	return(*this);
}


/*----------------------------------------------------------------------
	Function:		int GString::Len(void)
	Notes:			Get the length
  ---------------------------------------------------------------------- */
int GString::Len(void) const
{
	if (Text)
		return(strlen(Text));
	else
		return(0);
}


/*----------------------------------------------------------------------
  ---------------------------------------------------------------------- */
bool GString::operator<(GString const & Str) const
{
	char	defaultString[]="";

	char const * stringOne;
	char const * stringTwo;

	if (Empty())
		stringOne=defaultString;
	else
		stringOne=*this;

	if (Str,Empty())
		stringTwo=defaultString;
	else
		stringTwo=Str;

	return(strcmp(stringOne,stringTwo) < 0 );
}

/*----------------------------------------------------------------------
	Function:		void GString::AssignStr(char const *NewStr)
	Notes:			Set this objs string
  ---------------------------------------------------------------------- */
void GString::AssignStr(char const *NewStr)
{
	/*	Chuck string we're copying from into a temp
		str in case	 the text being copied from
		is ours
	*/
	char * ReplaceStr=NULL;

	if (NewStr)
		{
		if (!(ReplaceStr=new char[strlen(NewStr)+1]))
			Error(ERM_OUTOFMEM);

		strcpy(ReplaceStr,NewStr);
		}

	Dump();

	Text=ReplaceStr;
}

/*----------------------------------------------------------------------
	Function:		void GString::Init(void)
	Notes:			Init this str
  ---------------------------------------------------------------------- */
void GString::Init(void)
{
	Text=NULL;
}


/*----------------------------------------------------------------------
	Function:		void GString::Init(void)
	Notes:			Init this str
  ---------------------------------------------------------------------- */
void GString::Filter(char const * CharsToFilterOut,char ReplacementChar)
{
	if (Text)
		{
		for (unsigned int f=0;f<strlen(Text);f++)
			{
			for (unsigned int g=0;g<strlen(CharsToFilterOut);g++)
				{
				if (Text[f]==CharsToFilterOut[g])
					Text[f]=ReplacementChar;
				}
			}
		}
}

/*----------------------------------------------------------------------
	Function:		void GString::Init(void)
	Notes:			Init this str
  ---------------------------------------------------------------------- */
void GString::Append(char c)
{
	if (Text)
		{
		char * NewText;
		NewText=new char[strlen(Text)+2];
		strcpy(NewText,Text);
		NewText[strlen(Text)]=c;
		NewText[strlen(Text)+1]=0;
		delete Text;
		Text=NewText;
		}
	else
		{
		Text=new char[2];
		Text[0]=c;
		Text[1]=0;
		}
}
/*----------------------------------------------------------------------
	Function:		void GString::Init(void)
	Notes:			Init this str
  ---------------------------------------------------------------------- */
void GString::Replace(char const * SearchString,char const * ReplaceString)
{
	if (Text && strlen(Text) >= strlen(SearchString))
		{
		GString	NewString;
		int		RepLen;
		bool	LastMatch;

		RepLen=strlen(ReplaceString);

		for (unsigned int Checks=0;Checks <= strlen(Text)-strlen(SearchString);Checks++)
			{
			bool	Failed;
			
			Failed=false;

			for (unsigned int f=0;f<strlen(SearchString) && !Failed;f++)
				{
				if (SearchString[f] != Text[Checks+f])
					Failed=true;
				}

			if (Failed)
				{
				NewString.Append(Text[Checks]);
				LastMatch=false;
				}
			else
				{
				NewString=NewString+GString(ReplaceString);
				Checks+=strlen(SearchString)-1;
				LastMatch=true;
				}
			}

		if ((strlen(Text)-Checks) > 0)
			NewString+=GString(&Text[Checks]);

		*this=NewString;
		}
}


/*----------------------------------------------------------------------
	Function:		void GString::Dump(void)
	Notes:			Dump all the data
  ---------------------------------------------------------------------- */
void GString::Dump(void)
{
	if (Text)
		delete Text;

	Init();
}

/*----------------------------------------------------------------------
	Function:		void GString::Dump(void)
	Notes:			Dump all the data
  ---------------------------------------------------------------------- */
void GString::Lower(void)
{
	if (!Empty())
		strlwr(Text);
}

/*----------------------------------------------------------------------
	Function:		void GString::Dump(void)
	Notes:			Dump all the data
  ---------------------------------------------------------------------- */
void GString::Upper(void)
{
	if (!Empty())
		strupr(Text);
}

/*----------------------------------------------------------------------
	Function:		void GString::Dump(void)
	Notes:			Dump all the data
  ---------------------------------------------------------------------- */
bool GString::operator==(GString const & Gs) const
{
	if (Empty() && Gs.Empty())
		return(true);

	if (Empty() || Gs.Empty())
		return(false);
	
	return(strcmp(Gs,Text)==0);
}


/*----------------------------------------------------------------------
	Function:		void GString::Dump(void)
	Notes:			Dump all the data
  ---------------------------------------------------------------------- */
ostream  & operator<<(ostream & Out,GString const & G)
{
	if (!G.Empty())
		Out<<G.Text;

	return(Out);
}

/*===========================================================================
 end */
