/*=========================================================================

	demoplay.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_DEMOPLAY_H__
#define __PLAYER_DEMOPLAY_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __PLAYER_PLAYER_H__
#include "player/player.h"
#endif

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CDemoPlayer : public CPlayer
{
public:
	typedef struct
	{
		u8	m_inputValue;
		u8	m_length;
	}demoPlayerControl;

	void				loadControlData(FileEquate _fe);
	int					getFramesLeft();

	virtual void		shutdown();


protected:
	virtual PLAYERINPUT	readPadInput();


private:
	char				*m_demoData;
	
	demoPlayerControl	*m_controlData;
	int					m_controlDataSize;
	int					m_controlFrameCount;

	int					m_currentControlFrame;
	int					m_frameCount;
	int					m_totalFrameCount;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_DEMOPLAY_H__ */

/*===========================================================================
 end */
