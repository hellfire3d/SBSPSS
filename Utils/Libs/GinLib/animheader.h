/*=========================================================================

	ANIMHEADER.H	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 G R Liddon

===========================================================================*/


#ifndef __ANIMHEADER_H__
#define __ANIMHEADER_H__

#pragma warning( disable : 4786 )

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <vector>
#include <map>

/*	Glib
	---- */
#include <gtypes.h>
#include <gstring.hpp>

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
struct AnimHeaderItem
{
	GString	m_AnimName;
	int		m_Frame;
};

class AnimHeaderFile
{

private:
	GString		m_lastLoadedFile;
	std::map<GString,AnimHeaderItem>	m_strToInfo;

	void	makeTexNameCompliant(GString & str);

public:
	void	load(char const * name);
	bool	find(GString const & str,AnimHeaderItem & result);

	
	char const *	getLastFileLoaded(void)
		{return(m_lastLoadedFile);}
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
void readAnimHeaderFile(char const * Name,std::vector<AnimHeaderItem> & Items);

/*---------------------------------------------------------------------- */

#endif	/* __ANIMHEADER_H__ */

/*===========================================================================
 end */

