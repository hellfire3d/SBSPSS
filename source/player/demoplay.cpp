/*=========================================================================

	demoplay.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\demoplay.h"


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CDemoPlayer::loadControlData(FileEquate _fe)
{
	int *hdr;

	m_demoData=(char*)CFileIO::loadFile(_fe);
	hdr=(int*)m_demoData;
	
	m_controlFrameCount=hdr[0];
	m_controlDataSize=hdr[1];
	m_controlData=(demoPlayerControl*)(m_demoData+(sizeof(int)*2));

	m_currentControlFrame=0;
	m_frameCount=0;
	m_totalFrameCount=0;

	PAUL_DBGMSG("Loaded demo data");
	PAUL_DBGMSG("%d actions, %d frames",m_controlDataSize,m_controlFrameCount);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CDemoPlayer::getFramesLeft()
{
	return m_controlFrameCount-m_totalFrameCount;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CDemoPlayer::shutdown()
{
	MemFree(m_demoData);
	CPlayer::shutdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
PLAYERINPUT CDemoPlayer::readPadInput()
{
	PLAYERINPUT	input;
	if(m_currentControlFrame>=m_controlDataSize)
	{
		// End of control data reached
		input=PI_NONE;
	}
	else
	{
		demoPlayerControl	*crnt;
		crnt=&m_controlData[m_currentControlFrame];
		input=(PLAYERINPUT)crnt->m_inputValue;
		if(++m_frameCount==crnt->m_length)
		{
			m_currentControlFrame++;
			m_frameCount=0;
		}
		m_totalFrameCount++;
	}

	return input;
}


/*===========================================================================
end */
