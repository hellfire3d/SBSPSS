//#define MEM_CARD_TEST
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

#include "backend\credits.h"

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

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef	__FRONTEND_SCROLLBG_H__
#include "frontend\scrollbg.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

enum
{
	CC_BIG_TITLE,
	CC_TITLE,
	CC_NAME,
	CC_BLANK_LINE,
	CC_END_OF_PAGE,
	CC_END_OF_CREDITS,
}CREDIT_CONTROL;


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

static u16	s_creditData[]=
{
	CC_END_OF_PAGE,

	// ---- CLIMAX ----
	CC_BIG_TITLE,	STR__CREDITS__CLIMAX_GROUP,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__PRODUCER,
	CC_NAME,		STR__CREDITS__TREENA_SEYMOUR,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__DESIGNERS,
	CC_NAME,		STR__CREDITS__NICK_DRY,
	CC_NAME,		STR__CREDITS__MARK_DAVIES,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__LEAD_PROGRAMMER,
	CC_NAME,		STR__CREDITS__DAVE_OWENS,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__PROGRAMMING,
	CC_NAME,		STR__CREDITS__PAUL_GRENFELL,
	CC_NAME,		STR__CREDITS__CHARLES_BLAIR,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__LEAD_ARTIST,
	CC_NAME,		STR__CREDITS__KEVIN_MARTIN,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__SENIOR_ARTIST,
	CC_NAME,		STR__CREDITS__CHRIS_RUNDELL,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__ARTISTS,
	CC_NAME,		STR__CREDITS__SHAUN_PEARSON,
	CC_NAME,		STR__CREDITS__ROB_WILMOT,
	CC_NAME,		STR__CREDITS__WAI_HUNG_WAN,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__ADDITIONAL_ARTISTS,
	CC_NAME,		STR__CREDITS__RICHARD_EDWARDS,
	CC_NAME,		STR__CREDITS__KELVIN_LIEW,
	CC_NAME,		STR__CREDITS__NICK_THOMAS,
	CC_NAME,		STR__CREDITS__ANTONY_SMITH,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__ANIMATORS,
	CC_NAME,		STR__CREDITS__STEVE_CROCKER,
	CC_NAME,		STR__CREDITS__NIGEL_APPERLY,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__FMV,
	CC_NAME,		STR__CREDITS__ROB_WILMOT,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__LEAD_TESTER,
	CC_NAME,		STR__CREDITS__BARRY_MARTIN,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__MUSIC_SFX,
	CC_NAME,		STR__CREDITS__MATT_SIMMONDS,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__PRESIDENT,
	CC_NAME,		STR__CREDITS__KARL_JEFFREY,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__MANAGING_DIRECTOR,
	CC_NAME,		STR__CREDITS__GARY_LIDDON,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__DEVELOPMENT_DIRECTOR,
	CC_NAME,		STR__CREDITS__JIM_GARDNER,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__CREATIVE_DIRECTOR,
	CC_NAME,		STR__CREDITS__JOE_CAVALLA,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__VERY_SPECIAL_THANKS_INDEED_TO,
	CC_NAME,		STR__CREDITS__GARY_COLEMAN,
	CC_NAME,		STR__CREDITS__PETER_CHAFFEY_FOR_LISTENING_AND_FEEDING_CATS,
	CC_NAME,		STR__CREDITS__TEREZA_OWENS_FOR_PATIENCE_AND_A_BEAUTIFUL_WEDDING_DAY,
	CC_NAME,		STR__CREDITS__JENNIFER_PENN_FOR_TAMING_THE_NOMAD,
	CC_NAME,		STR__CREDITS__PERNILLA_BJARNE,
	CC_NAME,		STR__CREDITS__SARAH_RUNDELL,
	CC_NAME,		STR__CREDITS__NEIL_CAMPBELL,
	CC_NAME,		STR__CREDITS__MATT_COOPER_TRON,
	CC_NAME,		STR__CREDITS__LAURA_GALLARDO,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__EXTRA_THANKS_TO,
	CC_NAME,		STR__CREDITS__CLIMAX_LONDON,
	CC_END_OF_PAGE,


	// ---- THQ ----
	CC_BIG_TITLE,	STR__CREDITS__THQ,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__PRODUCER,
	CC_NAME,		STR__CREDITS__PETRO_PIASECKYJ,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__ASSISTANT_PRODUCER,
	CC_NAME,		STR__CREDITS__RYAN_CAMU,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__EXECUTIVE_PRODUCER,
	CC_NAME,		STR__CREDITS__CAROLINA_BEROZA,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__VP_OF_PRODUCT_DEVELOPMENT,
	CC_NAME,		STR__CREDITS__MICHAEL_RUBINELLI,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__QA_MANAGER,
	CC_NAME,		STR__CREDITS__JEREMY_BARNES,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__LEAD_TESTER,
	CC_NAME,		STR__CREDITS__SLAVEMONKEY_TRAVISTY_THOLEN,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__TESTERS,
	CC_NAME,		STR__CREDITS__JASON_DE_HERAS,
	CC_NAME,		STR__CREDITS__BUSTER_DEGEORGE,
	CC_NAME,		STR__CREDITS__IAN_SEDENSKY,
	CC_NAME,		STR__CREDITS__CHUCK_SHEFFIELD,
	CC_NAME,		STR__CREDITS__GJALEN_GIVHAN,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__SENIOR_GROUP_PRODUCT_MANAGER,
	CC_NAME,		STR__CREDITS__JOHN_ARDELL,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__PRODUCT_MANAGER,
	CC_NAME,		STR__CREDITS__DANIELLE_CONTE,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__CREATIVE_SERVICES,
	CC_NAME,		STR__CREDITS__HOWARD_LIEBESKIND,
	CC_NAME,		STR__CREDITS__KIRK_SOMDAL,
	CC_NAME,		STR__CREDITS__MELISSA_ROTH,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__VOICE_OVER,
	CC_NAME,		STR__CREDITS__MERMAID_MAN___ERNEST_BORGNINE,
	CC_NAME,		STR__CREDITS__BARNACLE_BOY___TIM_CONWAY,
	CC_NAME,		STR__CREDITS__THE_FLYING_DUTCHMAN___BRIAN_DOYLE_MURRAY,
	CC_NAME,		STR__CREDITS__SPONGEBOB_SQUAREPANTS_GARY_FRENCH_MAN___TOM_KENNY,
	CC_NAME,		STR__CREDITS__PATRICK_STAR___BILL_FAGERBAKKE,
	CC_NAME,		STR__CREDITS__SANDY_CHEEKS___CAROLYN_LAWRENCE,
	CC_NAME,		STR__CREDITS__SQUIDWARD___ROGER_BUMPASS,
	CC_NAME,		STR__CREDITS__PLANKTON___DOUG_LAWRENCE,
	CC_NAME,		STR__CREDITS__MR__KRABS___JOE_WHYTE,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__SPECIAL_THANKS_TO,
	CC_NAME,		STR__CREDITS__BRIAN_FARRELL,
	CC_NAME,		STR__CREDITS__JEFFERY_LAPIN,
	CC_NAME,		STR__CREDITS__ALISON_LOCKE,
	CC_NAME,		STR__CREDITS__GERMAINE_GIOIA,
	CC_NAME,		STR__CREDITS__PETER_DILLE,
	CC_NAME,		STR__CREDITS__TIFFANY_TERNAN,
	CC_NAME,		STR__CREDITS__BECKY_ALLEN,
	CC_NAME,		STR__CREDITS__AMANDA_WYATT,
	CC_NAME,		STR__CREDITS__BRIAN_FREDRICKSON,
	CC_NAME,		STR__CREDITS__STEVE_HILLENBURG,
	CC_NAME,		STR__CREDITS__ERIC_COLEMAN,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__THQ_AND_THE_THQ_LOGO_ARE_TRADEMARKS_AND_OR_REGISTERED_TRADEMARKS_OF_THQ_INC,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__C_2001_THQ_INC,
	CC_END_OF_PAGE,


	// ---- NICKELODEON ----
	CC_BIG_TITLE,	STR__CREDITS__NICKELODEON_INTERACTIVE,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__V_P__OF_INTERACTIVE_PRODUCT_AND_BOOK_PUBLISHING,
	CC_NAME,		STR__CREDITS__STEPHEN_YOUNGWOOD,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__DIRECTOR_OF_MARKETING_AND_NEW_BUSINESS_DEVELOPMENT,
	CC_NAME,		STR__CREDITS__SHERICE_GUILLORY,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__DIRECTOR_OF_DEVELOPMENT_AND_PRODUCTION,
	CC_NAME,		STR__CREDITS__ALY_SYLVESTER,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__PROJECT_COORDINATOR,
	CC_NAME,		STR__CREDITS__ERIKA__E__ORTIZ,
	CC_BLANK_LINE,
	CC_TITLE,		STR__CREDITS__MARKETING_COORDINATOR,
	CC_NAME,		STR__CREDITS__ERICA_DAVID,
	CC_END_OF_PAGE,

	CC_TITLE,		STR__CREDITS__NICK_INTERACTIVE_WOULD_LIKE_TO_THANK_,
	CC_NAME,		STR__CREDITS__TIM_BLANKLEY,
	CC_NAME,		STR__CREDITS__LEIGH_ANNE_BRODSKY,
	CC_NAME,		STR__CREDITS__JENNIFER_DAVIS,
	CC_NAME,		STR__CREDITS__MANNY_GALAN,
	CC_NAME,		STR__CREDITS__RUSSELL_HICKS,
	CC_NAME,		STR__CREDITS__DEB_KRASSNER,
	CC_NAME,		STR__CREDITS__GEORGE_LENTINO,
	CC_NAME,		STR__CREDITS__KATE_O_BRIEN,
	CC_NAME,		STR__CREDITS__ERIC_SQUIRES,
	CC_NAME,		STR__CREDITS__GEOFF_TODEBUSH,
	CC_NAME,		STR__CREDITS__STAVIT_YOUNG,
	CC_END_OF_PAGE,


	// ---- THE END! ----
	CC_END_OF_CREDITS,
};



const int LINE_BREAK_SIZE=10;
const int BIG_TITLE_FONT_SIZE=448;
const int TITLE_FONT_SIZE=352;
const int TITLE_FONT_OFS=4;
const int NAME_FONT_SIZE=256;
const int	DELAY_TIME=4*60;

CCreditsScene	CreditsScene;



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#ifdef MEM_CARD_TEST
#include "memcard\memcard.h"
#include "memcard\saveload.h"
CSaveLoadDatabase	*sl;
enum{mode_none,mode_save,mode_load};
int mode=mode_none;
#endif
void CCreditsScene::init()
{
#ifdef MEM_CARD_TEST
MemCard::Start();
sl=new ("sldb") CSaveLoadDatabase();
mode=mode_none;
#endif
	m_image=LoadPakScreen(BACKDROP_CREDITS_GFX);

	ASSERT(m_image);
	SetScreenImage(m_image);
	CFader::setFadingIn();

	m_font=new ("Credits font") ScalableFontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(ScalableFontBank::JUST_CENTRE);

	m_background1=new ("Options Background") CScrollyBackground();
	m_background1->init();
	m_background1->setOt(MAX_OT-2);
	m_background1->setFrame(FRM__CAUSTIC1);
	m_background1->setTheDrawMode(CScrollyBackground::DRAWMODE_ADDITIVE);
	m_background1->setColour(20,24,24);
	m_background1->setSpeed(1,0);
	m_background1->setSpeedScale(1);
	m_background1->setScale(511);

	m_background2=new ("Options Background") CScrollyBackground();
	m_background2->init();
	m_background2->setOt(MAX_OT-2);
	m_background2->setFrame(FRM__CAUSTIC2);
	m_background2->setTheDrawMode(CScrollyBackground::DRAWMODE_ADDITIVE);
	m_background2->setColour(20,24,24);
	m_background2->setSpeed(1,-1);
	m_background2->setSpeedScale(2);
	m_background2->setScale(511);

	m_currentPage=m_lastPage=s_creditData;
	m_frame=DELAY_TIME;

	m_shuttingDown=false;

	CSoundMediator::setSong(CSoundMediator::SONG_GAMECOMPLETE);
	CSoundMediator::playSong();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CCreditsScene::shutdown()
{
#ifdef MEM_CARD_TEST
MemCard::Stop();
delete sl;	
#endif
	CSoundMediator::dumpSong();

	m_background2->shutdown();		delete m_background2;
	m_background1->shutdown();		delete m_background1;

	m_font->dump();		delete m_font;

	ClearScreenImage();
	MemFree(m_image);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CCreditsScene::render()
{
	renderCreditsListTillEndOfPage(m_currentPage);
	m_background1->render();
	m_background2->render();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	creditsfr=40;
int	creditsfg=80;
int creditsfb=127;
void CCreditsScene::think(int _frames)
{
#ifdef MEM_CARD_TEST
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

//if(mode==mode_none)
#endif

	m_font->setColour(creditsfr,creditsfg,creditsfb);

	if(!CFader::isFading())
	{
		m_frame+=_frames;
		if(m_frame>DELAY_TIME/*PadGetDown(0)&PAD_CROSS*/)
		{
			m_lastPage=m_currentPage;
			m_frame=0;

			// Find start of next page
			while(*m_currentPage!=CC_END_OF_PAGE)
			{
				m_currentPage++;
			};
			m_currentPage++;
		}

#ifdef __USER_paul__
		// Make the credits repeat endlessly for me :)
		if(*m_currentPage==CC_END_OF_CREDITS)
		{
			m_currentPage=s_creditData;
		}
#endif
		
		if(PadGetDown(0)&PAD_START||
		   *m_currentPage==CC_END_OF_CREDITS)
		{
			m_shuttingDown=true;
			CFader::setFadingOut();
			GameState::setNextScene(&FrontEndScene);
		}
	}

	m_background1->think(_frames);
	m_background2->think(_frames);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CCreditsScene::readyToShutdown()
{
	return !CFader::isFading()&&m_shuttingDown;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CCreditsScene::renderCreditsListTillEndOfPage(u16 *_list)
{
	int	y;
	int	mode;

	enum
	{
		CALC,
		RENDER,
	};

	y=0;
	for(mode=CALC;mode<=RENDER;mode++)
	{
		u16	*listPtr;
		listPtr=_list;
		while(*listPtr!=CC_END_OF_PAGE&&*listPtr!=CC_END_OF_CREDITS)
		{
			switch(*listPtr++)
			{
				case CC_BIG_TITLE:
					{
						u16	data=*listPtr++;
						m_font->setScale(BIG_TITLE_FONT_SIZE);
						if(mode==RENDER)
						{
							m_font->print(256,y,data);
						}
						y+=m_font->getStringHeight(data);
					}
					break;

				case CC_TITLE:
					{
						u16	data=*listPtr++;
						m_font->setScale(TITLE_FONT_SIZE);
						if(mode==RENDER)
						{
							m_font->print(256,y-TITLE_FONT_OFS,data);
						}
						y+=m_font->getStringHeight(data);
					}
					break;

				case CC_NAME:
					{
						u16	data=*listPtr++;
						m_font->setScale(NAME_FONT_SIZE);
						if(mode==RENDER)
						{
							m_font->print(256,y,data);
						}
						y+=m_font->getStringHeight(data);
					}
					break;

				case CC_BLANK_LINE:
					y+=LINE_BREAK_SIZE;
					break;

				case CC_END_OF_PAGE:
				case CC_END_OF_CREDITS:
					break;
			}
		}

		if(mode==CALC)
		{
			y=(256-y)/2;
		}
	}
}

/*===========================================================================
 end */