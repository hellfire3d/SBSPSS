/*=========================================================================

	fma.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__FMA_FMA_H__
#define __FMA_FMA_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CFmaScene : public CScene
{
public:
	typedef enum
	{
		FMA_SCRIPT__INTRO,
		FMA_SCRIPT__CH1FINISHED,
		FMA_SCRIPT__CH2FINISHED,
		FMA_SCRIPT__CH3FINISHED,
		FMA_SCRIPT__CH4FINISHED,
		FMA_SCRIPT__CH5FINISHED,
		FMA_SCRIPT__PLANKTON,
		FMA_SCRIPT__PARTY,
		NUM_FMA_SCRIPTS,
		FMA_SCRIPT__NONE,
	}FMA_SCRIPT_NUMBER;


	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"FMA";}

	static void	selectFma(FMA_SCRIPT_NUMBER _fma);

private:
	void	startShutdown();

	void	startNextScriptCommand();
	void	processCurrentScriptCommand();



	class CLevel		*m_level;
	class CPartyScene	*m_party;

	DVECTOR			m_cameraPos;
	
	int				m_cameraMoving;
	int				m_startCameraFrame;
	DVECTOR			m_startCameraPos;
	int				m_endCameraFrame;
	DVECTOR			m_endCameraPos;

	int const		*m_pc;
	int				m_scriptRunning;
	int				m_stillProcessingCommand;
	int				m_doOtherProcessing;
	int				m_timer;
	int				m_frameCount;

	int				m_tuneLoaded;
	int				m_musicPlaying;

	int				m_globalAnimationFlag;

	int				m_readyToShutdown;

	int				m_drawScreenAsWhite;
};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CFmaScene	FmaScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FMA_FMA_H__ */

/*===========================================================================
 end */
