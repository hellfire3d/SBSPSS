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

#include "script.h"

/*	Std Lib
	------- */

/*	Glib
	---- */

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
using namespace std;

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
const int DEFAULT_LINE_LENGTH=8192;

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

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
ScriptFile::ScriptFile(void)
{
	m_isOpen=false;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
ScriptFile::~ScriptFile(void)
{
	close();
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void ScriptFile::close(void)
{
	if (m_isOpen)
		m_inStream.close();

	m_isOpen=false;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void ScriptFile::open(char const * fileName)
{
	if (m_isOpen)
		close();

	m_inStream.open(fileName,ios::binary|ios::in);

	if (m_inStream)
		{	
		m_fileName=fileName;

		}
	else
		GObject::Error(ERR_FATAL,"unable to open script file %s",fileName);

	m_isOpen=true;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void ScriptFile::scriptError(void)
{
	GObject::Error(ERR_FATAL,"Ill formatted script file %s, line %d",(char const *)m_fileName,m_lineCount);
}


/*----------------------------------------------------------------------
	Function:	ak
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void ScriptFile::checkIsOpen(void)
{
	if (!m_isOpen)
		GObject::Error(ERR_FATAL,"file operation needed by ScriptFile but no file is open");
}
/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool ScriptFile::getNextNonBlankLine(char * line,int lineLen)
{
	checkIsOpen();

	do
		{
		if (m_inStream.eof())
			return(false);

		m_inStream.getline(line,lineLen);

		
		int		lineLength;

		lineLength=strlen(line);

		for (int f=0;f<lineLength;f++)
			{
			if (line[f] == ';')
				line[f]=0;
			}
		
		if (strlen(line))
			line[strlen(line)-1]=0;

		m_lineCount++;
			
//		stripTrailingSpace(line);
		}
	while (!strlen(line));

	return(true);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool ScriptFile::nextLineIsCommand(void)
{
	checkIsOpen();

	bool		retVal;
	streampos	thisPos;
	const int	lineLength=DEFAULT_LINE_LENGTH;
	char		line[lineLength];
	int			oldLineCount;

	oldLineCount=m_lineCount;

	retVal=false;	

	thisPos=m_inStream.tellg();

	if (getNextNonBlankLine(line,lineLength))
		{
		stripTrailingSpace(line);
		retVal=isCommand(line);
		}

	m_inStream.seekg(thisPos);

	m_lineCount=oldLineCount;

	return(retVal);

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
GString ScriptFile::peekNextCommand(void)
{
	checkIsOpen();

	GString		retStr;

	if (nextLineIsCommand())
		{
		const int	lineLength=DEFAULT_LINE_LENGTH;
		char		line[lineLength];
		int			oldLineCount;
		streampos	thisPos;

		oldLineCount=m_lineCount;

		thisPos=m_inStream.tellg();

		getNextNonBlankLine(line,lineLength);
		stripTrailingSpace(line);
		retStr=getCommandString(line);

		m_inStream.seekg(thisPos);
	
		m_lineCount=oldLineCount;
		}

	return(retStr);

}




/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void ScriptFile::stripTrailingSpace(char * line)
{
	if (strlen(line))
		{
		bool done=false;


		for (int f=strlen(line)-1;f<=0 && !done ;f--)
			{
			if ((!isspace(line[f])) )
				done=true;
			else
				line[f]=0;
			}
		}
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
GString ScriptFile::getCommandString(char * com)
{
	char *		newStr;

	newStr=strdup(com);

	newStr[strlen(newStr)-1]=0;
	GString	retStr(&newStr[1]);

	free(newStr);

	return(retStr);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool ScriptFile::isCommand(char * line)
{
	return ((line[0]=='[') && (line[strlen(line)-1]==']'));
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
GString ScriptFile::getNextLine(void)
{
	const int	lineLength=DEFAULT_LINE_LENGTH;
	char		line[lineLength];

	getNextNonBlankLine(line,lineLength);

	return(line);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool ScriptFile::eof(void)
{
	checkIsOpen();
	return(m_inStream.eof());
}

/*===========================================================================
 end */
