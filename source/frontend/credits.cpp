// PKG
// Temporay home of memcard stuff
// Controls:
//	UP		Save file ( this writes the save file to the memcard, must be done first time )
//	DOWN	Overwrite saved file
//	LEFT	Load saved file
/*=========================================================================

	credits.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\credits.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_FRONTEND_H__
#include <frontend.h>
#endif


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
#include "memcard\memcard.h"
#include "memcard\saveload.h"
CSaveLoadDatabase	*sl;
enum{mode_none,mode_save,mode_load};
int mode=mode_none;
void CFrontEndCredits::init()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndCredits::shutdown()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static u8 *s_image;
void CFrontEndCredits::select()
{
MemCard::Start();
sl=new ("sldb") CSaveLoadDatabase();
mode=mode_none;
	m_shuttingDown=false;
	s_image=CFileIO::loadFile(BACKDROP_CREDITS_GFX);
	ASSERT(s_image);
	SetScreenImage(s_image);
	CFader::setFadingIn();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndCredits::unselect()
{
MemCard::Stop();
delete sl;	
	ClearScreenImage();
	MemFree(s_image);	s_image=NULL;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndCredits::render()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndCredits::think(int _frames)
{
sl->think();
if(mode==mode_none)
{
	int pad=PadGetDown(0);
	if(pad&PAD_UP)
	{
		PAUL_DBGMSG("startSave(\"blah\")");
		sl->startSave("blah");
		mode=mode_save;
	}
	else if(pad&PAD_DOWN)
	{
		PAUL_DBGMSG("startSave(\"blah\",0)");
		sl->startSave("blah",0);
		mode=mode_save;
	}
	else if(pad&PAD_LEFT)
	{
		PAUL_DBGMSG("startLoad(0)");
		sl->startLoad(0);
		mode=mode_load;
	}
}
else if(mode==mode_save)
{
	int	status=sl->getSaveStatus();
	if(status!=CSaveLoadDatabase::IN_PROGRESS)
	{
		if(status==CSaveLoadDatabase::FINISHED_OK)
		{
			MEMCARD_DBGMSG("saved ok");
		}
		else
		{
			MEMCARD_DBGMSG("not saved ok");
		}
		mode=mode_none;
	}
}
else if(mode==mode_load)
{
	int	status=sl->getLoadStatus();
	if(status!=CSaveLoadDatabase::IN_PROGRESS)
	{
		if(status==CSaveLoadDatabase::FINISHED_OK)
		{
			MEMCARD_DBGMSG("loaded ok");
		}
		else
		{
			MEMCARD_DBGMSG("not loaded ok");
		}
		mode=mode_none;
	}
}

if(mode==mode_none)
	if(!m_shuttingDown&&!CFader::isFading())
	{
		if(PadGetDown(0)&(PAD_CROSS|PAD_START))
		{
			m_shuttingDown=true;
			CFader::setFadingOut();
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndCredits::isReadyToExit()
{
	return !CFader::isFading()&&m_shuttingDown;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CFrontEndScene::FrontEndMode CFrontEndCredits::getNextMode()
{
	return	CFrontEndScene::MODE__MAIN_TITLES;
}

/*===========================================================================
 end */