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
	SpuReverbAttr	rev;
	SpuEnv			env;

	// SPU setup
	SpuInit();
	SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
	SpuSetTransferStartAddr(0);
	SpuWrite0(512*1024);
	while(!SpuIsTransferCompleted(SPU_TRANSFER_PEEK))VSync(0);
	SpuInitMalloc(MAX_SPU_MANAGEMENT,s_spuManagementTable);
	SpuSetCommonMasterVolume(0x3fff,0x3fff);
	
	// Reverb
	rev.mask=(SPU_REV_MODE|SPU_REV_DEPTHL|SPU_REV_DEPTHR);
	rev.mode=SPU_REV_MODE_SPACE;
	rev.depth.left=0x1000;
	rev.depth.right=0x1000;
	SpuSetReverbModeParam(&rev);
	SpuSetReverb(SPU_ON);
	SpuReserveReverbWorkArea(SPU_ON);
	SpuSetReverbVoice(SPU_BIT,0x000000);
	rev.mask=(SPU_REV_DEPTHL|SPU_REV_DEPTHR);
	SpuSetReverbDepth(&rev);

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


/*===========================================================================
 end */