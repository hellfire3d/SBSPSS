/*=========================================================================

	textdbase.cpp

	Author:  	G R Liddon @ Fareham
	Created:	Monday 23rd August 1999
	Project:	TPW PSX
	Purpose:	Language Database Handling Code

	Copyright (c) 1998 / 1999 Climax Development Ltd

===========================================================================*/

#ifndef __LOCALE_TEXTDBASE_H__
#define __LOCALE_TEXTDBASE_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

// May as well include the string IDs in here..
#ifndef __STRING_ENUMS__
#include <trans.h>
#endif


/*	Std Lib
	------- */

/*	Glib
	---- */

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
enum
	{
	ENGLISH,
	SWEDISH,
	DUTCH,
	ITALIAN,
	GERMAN,
	NUM_OF_LANGUAGES,
	};

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
class TranslationDatabase
{
public:
	enum{ NO_STRING=60000 };			// PKG - This is a special ID

	static void initialise(bool includeIds);
	static void loadLanguage(unsigned int langType);
	static char const * getString(unsigned int strNum);
	static void setShowIds(bool idShowVal);
	static int isLoaded();
	static int getLanguage();
};

/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */



/*---------------------------------------------------------------------- */

#endif	/* __LOCALE_TEXTDBASE_H__ */

/*===========================================================================
 end */
