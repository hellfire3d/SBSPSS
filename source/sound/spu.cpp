/*=========================================================================

	spu.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "spu.h"

#ifndef __SYSTEM_DBG_H__
#include "system\dbg.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif


/*	Std Lib
	------- */

#include 	<libspu.h>


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

char		CSpuSound::s_spuManagementTable[SPU_MALLOC_RECSIZ*(MAX_SPU_MANAGEMENT+1)];


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSpuSound::initialise()
{
	SpuEnv			env;

	// SPU setup
	SpuInit();
	SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
	SpuSetTransferStartAddr(0);
	SpuWrite0(512*1024);
	while(!SpuIsTransferCompleted(SPU_TRANSFER_PEEK))VSync(0);
	SpuInitMalloc(MAX_SPU_MANAGEMENT,s_spuManagementTable);
	SpuSetCommonMasterVolume(0x3fff,0x3fff);
	
	// Environment
	env.mask=SPU_ENV_EVENT_QUEUEING;
	env.queueing=SPU_OFF;
	SpuSetEnv(&env);
	SpuSetTransferCallback(0);

	SOUND_DBGMSG("SPU sound initialised");
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSpuSound::shutdown()
{
	SOUND_DBGMSG("Spu sound shutdown");
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSpuSound::setReverbActive(int _active)
{
	SpuReverbAttr	rev;

	if(_active)
	{
		// Reverb
		SpuSetReverbModeType(m_currentDetails.m_type);
		SpuSetReverb(SPU_ON);
		SpuReserveReverbWorkArea(SPU_ON);
		SpuSetReverbVoice(SPU_BIT,0xffffff);
		SpuSetReverbModeDelayTime(m_currentDetails.m_delay);
		SpuSetReverbModeDepth(m_currentDetails.m_depth,m_currentDetails.m_depth);
		SpuSetReverbModeFeedback(m_currentDetails.m_feedback);

		m_reverbActive=true;
	}
	else
	{
		SpuSetReverb(SPU_OFF);

		m_reverbActive=false;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSpuSound::setReverbDetails(ReverbDetails *_details)
{
	m_currentDetails=*_details;
}

/*===========================================================================
 end */