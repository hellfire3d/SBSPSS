/*=========================================================================

	textdbase.cpp

	Author:  	G R Liddon @ Fareham
	Created:	Monday 23rd August 1999
	Project:	TPW PSX
	Purpose:	Language Database Handling Code

	Copyright (c) 1998 / 1999 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Module Header File
	------------------ */
#include "textdbase.h"

/*	Std Lib
	------- */

/*	Glib
	---- */

/*	Local
	----- */

#ifndef		__MEMORY_HEADER__
#include	"mem\memory.h"
#endif

#ifndef		__FILE_EQUATES_H__
#include	<biglump.h>
#endif

#ifndef		_FILEIO_HEADER_
#include	"fileio\fileio.h"
#endif


/*	Graphics
	-------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
struct TransHeader
{
	u32		m_numOfStrings;
	char *	m_stringPtrs[1];

	void relocate(void)
		{
		for (unsigned int f=0;f<m_numOfStrings;f++)
			m_stringPtrs[f]=(char *)((u32)m_stringPtrs[f]+(u32)this);
		}

	char const * getString(unsigned int stringNum) const
		{

#if defined(__VERSION_debug__)
		if (stringNum > m_numOfStrings)
		{
			SYSTEM_DBGMSG("stringNum %d > m_numOfStrings %d", stringNum, m_numOfStrings);
			ASSERT(0);
		}
#endif
		return(m_stringPtrs[stringNum]);

		}
};

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */
static void loadDatabase(FileEquate f);
static void dumpDatabase(void);

/*----------------------------------------------------------------------
	Vars
	---- */
static TransHeader *	s_database;
static bool				s_loaded;
static int				s_langType;
static TransHeader *	s_idDatabase;
static bool				s_idShow;
/*----------------------------------------------------------------------
	Data
	---- */
static FileEquate const s_languageFileEquates[NUM_OF_LANGUAGES]=
{
	TRANSLATIONS_ENG_DAT,
	TRANSLATIONS_SWE_DAT,
	TRANSLATIONS_DUT_DAT,
	TRANSLATIONS_ITA_DAT,
	TRANSLATIONS_GER_DAT,
};

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void TranslationDatabase::initialise(bool includeIds)
{
	int		largestSize;


	largestSize=0;

	/* first find which language is the largest */

	for (int f=0;f<NUM_OF_LANGUAGES;f++)
		{
		int		thisFileSize=CFileIO::getFileSize(s_languageFileEquates[f]);
		if (thisFileSize > largestSize)
			largestSize = thisFileSize;
		}

	/* now Allocate some mem for it */

	SYSTEM_DBGMSG("Translation Database allocating %d bytes string space",largestSize);

	s_database=(TransHeader*)MemAlloc(largestSize,"TextDB");
	s_loaded=false;

	if (includeIds)
		{
		int len=CFileIO::getFileSize(TRANSLATIONS_ID_DAT);
		s_idDatabase=(TransHeader*)MemAlloc(len,"TextID");
		CFileIO::OpenFile(TRANSLATIONS_ID_DAT);
		CFileIO::ReadFile(s_idDatabase,len);
		CFileIO::CloseFile();

		s_idDatabase->relocate();
		}
	else
		s_idDatabase=NULL;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void TranslationDatabase::setShowIds(bool idShowVal)
{
	if (s_idDatabase)
		s_idShow=idShowVal;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void TranslationDatabase::loadLanguage(unsigned int langType)
{
	ASSERT(langType < NUM_OF_LANGUAGES);

	if (s_loaded)
		dumpDatabase();

	loadDatabase(FileEquate(s_languageFileEquates[langType]));
	s_langType=langType;
	s_loaded=true;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
char const * TranslationDatabase::getString(unsigned int strNum)
{
	ASSERT(s_loaded);
	if(strNum!=NO_STRING)
	{
		if (s_idShow)
			return(s_idDatabase->getString(strNum));
		else
			return(s_database->getString(strNum));
	}
	else
	{
		return "\0";
	}
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int TranslationDatabase::isLoaded(void)
{
	return s_loaded;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int TranslationDatabase::getLanguage(void)
{
	ASSERT(s_loaded);

	return s_langType;
}



/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void loadDatabase(FileEquate f)
{
	ASSERT(!s_loaded);
	
	CFileIO::OpenFile(f);
	CFileIO::ReadFile(s_database,CFileIO::getFileSize(f));
	CFileIO::CloseFile();

	s_database->relocate();
	s_loaded=true;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void dumpDatabase(void)
{
	ASSERT(s_loaded);

	s_loaded=false;
}


/*===========================================================================
 end */
