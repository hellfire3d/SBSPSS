/*=========================================================================

	FILENAME.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 G R Liddon

===========================================================================*/

#pragma warning( disable : 4786 )

#ifndef __SCRIPT_H__
#define __SCRIPT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <fstream>

/*	Glib
	---- */
#include <gstring.hpp>

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
class ScriptFile
{
private:
	std::ifstream		m_inStream;
	bool				m_isOpen;
	GString				m_fileName;
	int					m_lineCount;

	void		scriptError(void);

	bool		getNextNonBlankLine(char * Line,int LineLen);

	void		stripTrailingSpace(char * Line);
	GString		getCommandString(char * Com);
	bool		isCommand(char * Line);
	void		checkIsOpen(void);

protected:
	bool		eof(void);
	GString		peekNextCommand(void);
	bool		nextLineIsCommand(void);
	GString		getNextLine(void);

	void		open(char const * fileName);
	void		close(void);
	int			getLineCount(void)
		{return(m_lineCount);}



public:


	ScriptFile(void);
	~ScriptFile(void);
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

#endif	/* __SCRIPT_H__ */

/*===========================================================================
 end */

