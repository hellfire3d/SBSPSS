/*=========================================================================

	GSTRING.HPP

	Author:   Gary Liddon @ Farehame
	Created:  4th April 1997
	Purpose:  Generic string class

	Copyright (c) 1997 Gary Liddon

===========================================================================*/

#ifndef __GSTRING_HPP__
#define __GSTRING_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <string.h>
#include <fstream>

/*	Stl
	--- */

/*	Glib
	---- */
#include "gtypes.h"
#include "gobject.hpp"

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Class defintions
	---------------- */
class GLIB_API GString : public GObject
{
public:
	GString();
	~GString();
	GString(GString const &);

	GString(const char * Txt)
		{
		Init();
		*this=Txt;
		}

	GString(const char * Txt,int StrLength)
		{
		Init();
		Text=new char[StrLength+1];

		memcpy(Text,Txt,StrLength);
		Text[StrLength]=0;
		}

	bool operator==(GString const & Gs) const;
	bool operator==(char const * Txt) const
		{
		GString MyStr;
		MyStr=Txt;
		return(MyStr == *this);
		}

	GString & operator=(GString const & Gs)
		{
		/* Check for self assignment */

		if (this!=&Gs)
			AssignStr(Gs);

		return *this;
		}

	GString & operator=(char const * NewStr)
		{
		AssignStr(NewStr);
		return *this;
		}


	GString operator+(GString const &);
	GString & operator+=(GString const &);

	bool operator<(GString const & Str) const;

	friend std::ostream & operator<<(std::ostream & Out,GString const & G);

	int Len(void) const;

	operator char const *() const {return(Text);}
	bool	Empty()	const {return(Len()==0);}

	void Lower(void);
	void Upper(void);
	
	void	Filter(char const * CharsToFilterOut,char ReplacementChar='_');

	void	Append(char c);
	void	Replace(char const * SearchString,char const * ReplaceString);

protected:
	void Init(void);
	void Dump(void);
	void AssignStr(char const *NewStr);
	char * Text;
};



/*---------------------------------------------------------------------- */

#endif	/* __GSTRING_HPP__ */

/*===========================================================================
 end */
