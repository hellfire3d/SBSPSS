/*=========================================================================

	save.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	
				
	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "save\save.h"

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __GUI_GFACTORY_H__
#include "gui\gfactory.h"
#endif

#ifndef __GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef __GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __MEMCARD_MEMCARD_H__
#include "memcard\memcard.h"
#endif

#ifndef __MEMCARD_SAVELOAD_H__
#include "memcard\saveload.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __STRING_ENUMS__
#include <trans.h>
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

CSaveScene	SaveScene;


static const int	NORMAL__FRAME_W=416;
static const int	NORMAL__FRAME_H=160;
static const int	WANT_TO_SAVE__FRAME_W=NORMAL__FRAME_W;
static const int	WANT_TO_SAVE__FRAME_H=96;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSaveScene::init()
{
	CGUITextBox			*tb;

	
	m_wantToSaveFrame=new ("saveframe") CGUIControlFrame();
	m_wantToSaveFrame->init(NULL);
	m_wantToSaveFrame->setObjectXYWH((512-WANT_TO_SAVE__FRAME_W)/2,(256-WANT_TO_SAVE__FRAME_H)/2,WANT_TO_SAVE__FRAME_W,WANT_TO_SAVE__FRAME_H);
		tb=new ("textbox") CGUITextBox();
		tb->init(m_wantToSaveFrame);
		tb->setObjectXYWH(0,0,WANT_TO_SAVE__FRAME_W,40);
		tb->setText(STR__SAVE__DO_YOU_WANT_TO_SAVE);
		CGUIFactory::createValueButtonFrame(m_wantToSaveFrame,
											0,50,WANT_TO_SAVE__FRAME_W,20,
											STR__YES,
											&m_userResponse,USERRESPONSE__YES);
		CGUIFactory::createValueButtonFrame(m_wantToSaveFrame,
											0,70,WANT_TO_SAVE__FRAME_W,20,
											STR__NO,
											&m_userResponse,USERRESPONSE__NO);

	m_blankFrame=new ("saveframe") CGUIGroupFrame();
	m_blankFrame->init(NULL);
	m_blankFrame->setObjectXYWH((512-NORMAL__FRAME_W)/2,(256-NORMAL__FRAME_H)/2,NORMAL__FRAME_W,NORMAL__FRAME_H);
	m_blankFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);
		m_blankTextBox=new ("textbox") CGUITextBox();
		m_blankTextBox->init(m_blankFrame);
		m_blankTextBox->setObjectXYWH(0,0,NORMAL__FRAME_W,80);
		m_blankTextBox->setText(STR__BLANK);

	m_okResponseFrame=new ("saveframe") CGUIControlFrame();
	m_okResponseFrame->init(NULL);
	m_okResponseFrame->setObjectXYWH((512-NORMAL__FRAME_W)/2,(256-NORMAL__FRAME_H)/2,NORMAL__FRAME_W,NORMAL__FRAME_H);
		m_okResponseTextBox=new ("textbox") CGUITextBox();
		m_okResponseTextBox->init(m_okResponseFrame);
		m_okResponseTextBox->setObjectXYWH(0,0,NORMAL__FRAME_W,80);
		m_okResponseTextBox->setText(STR__BLANK);
		CGUIFactory::createValueButtonFrame(m_okResponseFrame,
											0,NORMAL__FRAME_H-30,WANT_TO_SAVE__FRAME_W,20,
											STR__OK,
											&m_userResponse,USERRESPONSE__OK);
	
	m_yesNoResponseFrame=new ("saveframe") CGUIControlFrame();
	m_yesNoResponseFrame->init(NULL);
	m_yesNoResponseFrame->setObjectXYWH((512-NORMAL__FRAME_W)/2,(256-NORMAL__FRAME_H)/2,NORMAL__FRAME_W,NORMAL__FRAME_H);
		m_yesNoResponseTextBox=new ("textbox") CGUITextBox();
		m_yesNoResponseTextBox->init(m_yesNoResponseFrame);
		m_yesNoResponseTextBox->setObjectXYWH(0,0,NORMAL__FRAME_W,80);
		m_yesNoResponseTextBox->setText(STR__BLANK);
		CGUIFactory::createValueButtonFrame(m_yesNoResponseFrame,
											0,NORMAL__FRAME_H-50,WANT_TO_SAVE__FRAME_W,20,
											STR__YES,
											&m_userResponse,USERRESPONSE__YES);
		CGUIFactory::createValueButtonFrame(m_yesNoResponseFrame,
											0,NORMAL__FRAME_H-30,WANT_TO_SAVE__FRAME_W,20,
											STR__NO,
											&m_userResponse,USERRESPONSE__NO);

	CFader::setFadingIn();

	setMode(MODE__CONFIRMSAVE);

	MemCard::Start();
	m_saveLoadDatabase=new ("saveloaddb") CSaveLoadDatabase();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSaveScene::shutdown()
{
	m_yesNoResponseFrame->shutdown();
	m_okResponseFrame->shutdown();
	m_blankFrame->shutdown();
	m_wantToSaveFrame->shutdown();

	MemCard::Stop();
	delete m_saveLoadDatabase;	
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSaveScene::render()
{
	POLY_F4	*f4;

	f4=GetPrimF4();
	setXYWH(f4,0,0,512,256);
	setRGB0(f4,  0,  0,  0);
	AddPrimToList(f4,MAX_OT-1);

	switch(m_mode)
	{
		case MODE__CONFIRMSAVE:
			m_wantToSaveFrame->render();
			break;

		case MODE__CHECKING:
		case MODE__FORMATTING:
		case MODE__CHECKINGFORMAT:
		case MODE__SAVING:
			m_blankFrame->render();
			break;

		case MODE__UNFORMATTED:
		case MODE__CONFIRMOVERWRITE:
			m_yesNoResponseFrame->render();
			break;

		case MODE__FORMATOK:
		case MODE__FORMATERROR:
		case MODE__NOCARD:
		case MODE__NOSPACE:
		case MODE__SAVEOK:
		case MODE__SAVEERROR:
			m_okResponseFrame->render();
			break;

		case MODE__READYTOEXIT:
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSaveScene::think(int _frames)
{
	if(!CFader::isFading())
	{
		m_saveLoadDatabase->think();

		switch(m_mode)
		{
			case MODE__CONFIRMSAVE:
				m_wantToSaveFrame->think(_frames);
				if(m_userResponse==USERRESPONSE__NO)
				{
					setMode(MODE__READYTOEXIT);
					CFader::setFadingOut();
				}
				else if(m_userResponse==USERRESPONSE__YES)
				{
					setMode(MODE__CHECKING);
				}
				break;

			case MODE__CHECKING:
				if(MemCard::GetCardStatus(0)==MemCard::CS_CardInserted)
				{
					// Scanning a new card..
					m_timeInMode=0;
				}
				else if(m_timeInMode>60)
				{
					// Wait for card status to settle for one second before trusting it
					if(MemCard::GetCardStatus(0)==MemCard::CS_NoCard)
					{
						setMode(MODE__NOCARD);
					}
					else if(MemCard::GetCardStatus(0)==MemCard::CS_UnformattedCard)
					{
						setMode(MODE__UNFORMATTED);
					}
					else if(MemCard::GetFileCountOnCard(0))
					{
						setMode(MODE__CONFIRMOVERWRITE);
					}
					else if(MemCard::GetFreeBlocksOnCard(0)==0)
					{
						setMode(MODE__NOSPACE);
					}
					else
					{
						setMode(MODE__SAVING);
						if(!m_saveLoadDatabase->startSave("blah"))
						{
							setMode(MODE__SAVEERROR);
						}
					}
				}
				break;

			case MODE__UNFORMATTED:
				if(MemCard::GetCardStatus(0)!=MemCard::CS_UnformattedCard)
				{
					setMode(MODE__CHECKING);
				}
				else
				{
					m_yesNoResponseFrame->think(_frames);
					if(m_userResponse==USERRESPONSE__YES)
					{
						setMode(MODE__FORMATTING);
						if(!m_saveLoadDatabase->startFormat())
						{
							setMode(MODE__FORMATERROR);
						}
					}
					else if(m_userResponse==USERRESPONSE__NO)
					{
						setMode(MODE__CONFIRMSAVE);
					}
				}
				break;

			case MODE__FORMATTING:
				{
				int stat=m_saveLoadDatabase->getFormatStatus();
				if(stat==CSaveLoadDatabase::FINISHED_OK)
				{
					MemCard::InvalidateCard(0);
					setMode(MODE__CHECKINGFORMAT);
				}
				else if(stat==CSaveLoadDatabase::FAILED)
				{
					setMode(MODE__FORMATERROR);
				}
				}
				break;

			case MODE__CHECKINGFORMAT:
				if(MemCard::GetCardStatus(0)==MemCard::CS_CardInserted)
				{
					// Scanning a new card..
					m_timeInMode=0;
				}
				else if(m_timeInMode>240)
				{
					// Wait for card status to settle for one second before trusting it
					if(MemCard::GetCardStatus(0)==MemCard::CS_NoCard)
					{
						setMode(MODE__NOCARD);
					}
					else if(MemCard::GetCardStatus(0)==MemCard::CS_UnformattedCard)
					{
						setMode(MODE__UNFORMATTED);
					}
					else if(MemCard::GetFileCountOnCard(0))
					{
						setMode(MODE__CONFIRMOVERWRITE);
					}
					else if(MemCard::GetFreeBlocksOnCard(0)==0)
					{
						setMode(MODE__NOSPACE);
					}
					else
					{
						setMode(MODE__FORMATOK);
					}
				}
				break;

			case MODE__FORMATOK:
				if(MemCard::GetCardStatus(0)!=MemCard::CS_ValidCard)
				{
					setMode(MODE__CHECKING);
				}
				else
				{
					m_okResponseFrame->think(_frames);
					if(m_userResponse==USERRESPONSE__OK)
					{
						setMode(MODE__SAVING);
						if(!m_saveLoadDatabase->startSave("blah"))
						{
							setMode(MODE__SAVEERROR);
						}
					}
				}
				break;

			case MODE__FORMATERROR:
			case MODE__NOSPACE:
			case MODE__SAVEERROR:
				m_okResponseFrame->think(_frames);
				if(m_userResponse==USERRESPONSE__OK)
				{
					setMode(MODE__CONFIRMSAVE);
				}
				break;

			case MODE__NOCARD:
				if(MemCard::GetCardStatus(0)!=MemCard::CS_NoCard)
				{
					setMode(MODE__CHECKING);
				}
				else
				{
					m_okResponseFrame->think(_frames);
					if(m_userResponse==USERRESPONSE__OK)
					{
						setMode(MODE__CONFIRMSAVE);
					}
				}
				break;

			case MODE__CONFIRMOVERWRITE:
				if(MemCard::GetCardStatus(0)!=MemCard::CS_ValidCard)
				{
					setMode(MODE__CHECKING);
				}
				else
				{
					m_yesNoResponseFrame->think(_frames);
					if(m_userResponse==USERRESPONSE__YES)
					{
						setMode(MODE__SAVING);
						if(!m_saveLoadDatabase->startSave("blah",0))
						{
							setMode(MODE__SAVEERROR);
						}
					}
					else if(m_userResponse==USERRESPONSE__NO)
					{
						setMode(MODE__CONFIRMSAVE);
					}
				}
				break;

			case MODE__SAVING:
				{
				int stat=m_saveLoadDatabase->getSaveStatus();
				if(stat==CSaveLoadDatabase::FINISHED_OK)
				{
					setMode(MODE__SAVEOK);
				}
				else if(stat==CSaveLoadDatabase::FAILED)
				{
					setMode(MODE__SAVEERROR);
				}
				}
				break;

			case MODE__SAVEOK:
				m_okResponseFrame->think(_frames);
				if(m_userResponse==USERRESPONSE__OK)
				{
					setMode(MODE__READYTOEXIT);
				}
				break;

			case MODE__READYTOEXIT:
				break;
		}
	}

	m_timeInMode+=_frames;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CSaveScene::readyToShutdown()
{
	return m_mode==MODE__READYTOEXIT&&!CFader::isFading();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSaveScene::setMode(MODE _newMode)
{
	if(m_wantToSaveFrame->isSelected())m_wantToSaveFrame->unselect();
	if(m_okResponseFrame->isSelected())m_okResponseFrame->unselect();
	if(m_yesNoResponseFrame->isSelected())m_yesNoResponseFrame->unselect();
	
	switch(_newMode)
	{
		case MODE__CONFIRMSAVE:
			m_wantToSaveFrame->select();
			break;

		case MODE__CHECKING:
			m_blankTextBox->setText(STR__MEMCARD__CHECKING_MEM_CARD);
			break;

		case MODE__UNFORMATTED:
			m_yesNoResponseTextBox->setText(STR__SAVE__UNFORMATTEDCARD);
			m_yesNoResponseFrame->select();
			break;

		case MODE__FORMATTING:
			m_blankTextBox->setText(STR__MEMCARD__FORMATTINGPLEASEWIAT);
			break;

		case MODE__CHECKINGFORMAT:
			m_blankTextBox->setText(STR__MEMCARD__FORMATTINGPLEASEWIAT);
			break;

		case MODE__FORMATOK:
			m_okResponseTextBox->setText(STR__MEMCARD__FORMATTINGOK);
			m_okResponseFrame->select();
			break;

		case MODE__FORMATERROR:
			m_okResponseTextBox->setText(STR__MEMCARD__FORMATTINGERROR);
			m_okResponseFrame->select();
			break;

		case MODE__NOCARD:
			m_okResponseTextBox->setText(STR__MEMCARD__NOCARDINSLOT);
			m_okResponseFrame->select();
			break;

		case MODE__NOSPACE:
			m_okResponseTextBox->setText(STR__MEMCARD__NOSPACEONCARD);
			m_okResponseFrame->select();
			break;

		case MODE__CONFIRMOVERWRITE:
			m_yesNoResponseTextBox->setText(STR__MEMCARD__DUPLICATESAVENAME);
			m_yesNoResponseFrame->select();
			break;

		case MODE__SAVING:
			m_blankTextBox->setText(STR__MEMCARD__SAVINGPLEASEWAIT);
			break;

		case MODE__SAVEOK:
			m_okResponseTextBox->setText(STR__MEMCARD__SAVEOK);
			m_okResponseFrame->select();
			break;

		case MODE__SAVEERROR:
			m_okResponseTextBox->setText(STR__MEMCARD__SAVEERROR);
			m_okResponseFrame->select();
			break;

		case MODE__READYTOEXIT:
			GameState::setNextScene(&MapScene);
			break;
	}

	
	m_mode=_newMode;
	m_timeInMode=0;
	m_userResponse=USERRESPONSE__NONE;


//////////
static const char *text[]=
{
		"MODE__CONFIRMSAVE",
		"MODE__CHECKING",
		"MODE__UNFORMATTED",
		"MODE__FORMATTING",
		"MODE__CHECKINGFORMAT",
		"MODE__FORMATOK",
		"MODE__FORMATERROR",
		"MODE__NOCARD",
		"MODE__NOSPACE",
		"MODE__CONFIRMOVERWRITE",
		"MODE__SAVING",
		"MODE__SAVEOK",
		"MODE__SAVEERROR",
		"MODE__READYTOEXIT",
};
PAUL_DBGMSG("newmode: %s",text[m_mode]);
//////////
}


/*===========================================================================
 end */