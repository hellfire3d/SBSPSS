/*=========================================================================

	KANJICLASS.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 G R Liddon

===========================================================================*/

#ifndef __KANJI_CLASS__
#define __KANJI_CLASS__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <vector>

/*	Glib
	---- */
#include <gtypes.h>
#include <gobject.hpp>

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class KanjiClass : GObject
{
public:

	enum KanjiFontSize
		{
			FONT_SIZE_11,
			FONT_SIZE_13,
			FONT_SIZE_15,
		};
private:
	std::vector<u8> m_kanjiHit;

	void	initialise(void);

	void	addKanjiHit(uint hi,uint lo);
	bool	getKanjiHit(uint hi,uint lo);
	void	clearKanjiHit(uint hi,uint lo);
	int		findKanjiIndex(u16 kan) const;
	static void	saveKanjiLbm(char const * fileName,KanjiFontSize k,u16 kan);

public:


			KanjiClass(void);

	void	addStr(u8 const * newStr);

	int		howManyKanjiChars(void);
	void	saveKanjiLbms(char const * dir,KanjiFontSize k = KanjiClass::FONT_SIZE_11);
	void	saveKanjiLbmNames(char const * dir,char const *name);
	void	saveKanjiTable(char const *name);
	static void	writeEnglishFontLbms(char const * dir,KanjiFontSize k = KanjiClass::FONT_SIZE_11,char const * repFile= NULL);
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

#endif	/* XYZ */

/*===========================================================================
 end */
