/*=========================================================================

	paul.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "paul\paul.h"

#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif


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
static FontBank		s_fontBank;
char				*s_mem[3];


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::init()
{
	setActiveDbgChannels(DC_PAUL);
	PAUL_DBGMSG("this is a message..");
	PAUL_DBGMSG("this is a message.. 2");
	PAUL_DBGMSG("this is a message.. 3");

	s_fontBank.initialise(&standardFont);

}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::shutdown()
{
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::render()
{
	int		logCount;
	int		i,y,charHeight;

	logCount=getNumberOfDbgLinesInLog();
	y=20;
	charHeight=s_fontBank.getCharHeight();
	for(i=0;i<logCount;i++)
	{
		s_fontBank.print(20,y,getDbgLineFromLog(i));
		y+=charHeight;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int trashoff=-1;
int trash=false;
void CPaulScene::think()
{
//	static int	arse=0;
//	PAUL_DBGMSG("%d\n",arse++);
//	ASSERT(arse<100);

	int	i;
	int	size[3];

	for(i=0;i<3;i++)
	{
		size[i]=32763;//getRndRange(32768);
		s_mem[i]=MemAlloc(size[i],"Test");
	}
	PAUL_DBGMSG("%d %d %d",size[0],size[1],size[2]);

	if(trash)
	{
		*(s_mem[0]+trashoff)=123;
		trash=false;
	}

	for(i=0;i<3;i++)
	{
		MemFree(s_mem[i]);
	}
}


/*===========================================================================
 end */