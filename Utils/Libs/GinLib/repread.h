/*=========================================================================

	FILENAME.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 G R Liddon

===========================================================================*/

#pragma warning( disable : 4786 )

#ifndef __REPREAD_H__
#define __REPREAD_H__

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
struct RepItem
{
	GString		m_texName;
	u16			m_tpage;
	u16			m_clut;
	u8			m_u,m_v,m_w,m_h;
	bool		m_rotated;
	int			m_frameNumber;
};

class RepFile
{

private:
	GString						m_lastLoadedFile;
	std::map<GString,RepItem>	m_strToInfo;

	virtual void	makeTexNameCompliant(GString & str);

public:
	void	load(char const * name);
	bool	find(GString const & str,RepItem & result);

	
	char const *	getLastFileLoaded(void)
		{return(m_lastLoadedFile);}
};

class RepFileNoDir : public RepFile
{
private:
	virtual void makeTexNameCompliant(GString & str);

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
void readRepFile(char const * Name,std::vector<RepItem> & Items);

/*---------------------------------------------------------------------- */

#endif	/* __REPREAD_H__ */

/*===========================================================================
 end */

