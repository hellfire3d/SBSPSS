/*=========================================================================

	gameover.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	
				
	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "backend\gameover.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef __ACTOR_HEADER__
#include "gfx\actor.h"
#endif

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __STRING_ENUMS__
#include <trans.h>
#endif

#ifndef	__ANIM_PATRICK_HEADER__
#include <actor_patrick_anim.h>
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

CGameOverScene	GameOverScene;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::init()
{
	m_image=LoadPakScreen(BACKDROP_GAMEOVER_GFX);
	ASSERT(m_image);
	SetScreenImage((u8*)m_image);

	m_font=new ("game over font") ScalableFontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(ScalableFontBank::JUST_CENTRE);
	m_font->setOt(5);

	CActorPool::Reset();
	m_anotherGfx=CActorPool::GetActor(ACTORS_SPONGEBOB_SBK);
	m_patrickGfx=CActorPool::GetActor(ACTORS_PATRICK_SBK);
	m_animFrame=0;
	CActorPool::SetUpCache();
	CActorPool::CleanUpCache();

	m_readyToExit=false;
	CFader::setFadingIn();

	initContinue();
	initGameOver();

	if(CGameSlotManager::getSlotData()->m_continues>=1)
	{
		m_state=STATE__CONTINUE;
	}
	else
	{
		m_state=STATE__GAME_OVER;
	}

	m_hasPlayedFirstSpeech=false;
	m_hasPlayedSecondSpeech=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::shutdown()
{
	CSoundMediator::stopSpeech();

	delete m_anotherGfx;
	delete m_patrickGfx;
	CActorPool::Reset();

	m_font->dump();				delete m_font;

	MemFree(m_image);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::render()
{
	switch(m_state)
	{
		case STATE__CONTINUE:
			renderContinue();
			break;
		case STATE__CONTINUE_TIMED_OUT:
			renderContinue();
			renderGameOver();
			break;
		case STATE__SPEECH_BEFORE_EXITING_TO_GAME:
			renderContinue();
			break;
		case STATE__GAME_OVER:
			renderGameOver();
			break;
		case STATE__EXITING_TO_GAME:
			renderContinue();
		case STATE__EXITING_TO_FRONT_END:
			renderGameOver();
			break;
	}

	DVECTOR ppos={255,210};
	m_patrickGfx->Render(ppos,0,m_animFrame);
	CActorPool::CleanUpCache();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::think(int _frames)
{
	int	animLength;
	m_animFrame+=_frames;
	animLength=m_patrickGfx->getFrameCount(0);
	while(m_animFrame>=animLength)m_animFrame-=animLength;

	switch(m_state)
	{
		case STATE__CONTINUE:
			thinkContinue(_frames);
			break;
		case STATE__CONTINUE_TIMED_OUT:
			thinkContinue(_frames);
			thinkGameOver(_frames);
			break;
		case STATE__GAME_OVER:
			thinkGameOver(_frames);
			break;
		case STATE__SPEECH_BEFORE_EXITING_TO_GAME:
			thinkContinue(_frames);
			break;
		case STATE__EXITING_TO_GAME:
			thinkContinue(_frames);
			break;
		case STATE__EXITING_TO_FRONT_END:
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CGameOverScene::readyToShutdown()
{
	return m_readyToExit&&!CFader::isFading();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CGameOverScene::initContinue()
{
	m_continueFontSin=0;
	m_continueFontOffset=100<<2;
	m_continueTimer=10*COUNTDOWN_TIME_SECOND_LENGTH;
}
void	CGameOverScene::thinkContinue(int _frames)
{
	int	move;

	// Slide the text onscreen
	if(m_state==STATE__CONTINUE)
	{
		if(!CFader::isFading())
		{
			if(!m_hasPlayedFirstSpeech)
			{
				CSoundMediator::playSpeech(SPEECH_085);
				m_hasPlayedFirstSpeech=true;
			}

			if(m_continueFontOffset)
			{
				move=m_continueFontOffset/10;
				if(move==0)
				{
					move=1;
				}
				m_continueFontOffset-=move;
				if(m_continueFontOffset<0)
				{
					m_continueFontOffset=0;
				}
			}
			else
			{
				// Countdown ( wait for text to stop first )
				if(m_continueTimer)
				{
					int	pad=PadGetDown(0);
					if(pad&PAD_CROSS)
					{
						m_continueTimer-=m_continueTimer%COUNTDOWN_TIME_SECOND_LENGTH;
					}
					else if(pad&PAD_START)
					{
//						m_readyToExit=true;
//						CFader::setFadingOut();
//						GameState::setNextScene(&MapScene);
//						CGameSlotManager::getSlotData()->m_lives=CGameSlotManager::INITIAL_LIVES;
//						CGameSlotManager::getSlotData()->m_continues--;
						m_state=STATE__SPEECH_BEFORE_EXITING_TO_GAME;
					}
					else
					{
						m_continueTimer-=_frames;
					}

					if(m_continueTimer<=0)
					{
						m_continueTimer=0;
						m_state=STATE__CONTINUE_TIMED_OUT;
					}
				}
			}
		}
	}
	else if(m_state==STATE__SPEECH_BEFORE_EXITING_TO_GAME||m_state==STATE__CONTINUE_TIMED_OUT||STATE__EXITING_TO_GAME)
	{
		if(m_state==STATE__SPEECH_BEFORE_EXITING_TO_GAME&&!m_hasPlayedSecondSpeech)
		{
			CSoundMediator::playSpeech(SPEECH_086);
			m_hasPlayedSecondSpeech=true;
		}

		// Slide text back off the screen
		m_continueFontOffset=(100<<2)-m_continueFontOffset;
		move=m_continueFontOffset/10;
		if(move==0)
		{
			move=1;
		}
		m_continueFontOffset-=move;
		if(m_continueFontOffset<0)
		{
			if(m_state==STATE__SPEECH_BEFORE_EXITING_TO_GAME&&!CSoundMediator::isSpeechPlaying())
			{
				m_readyToExit=true;
				CFader::setFadingOut();
				GameState::setNextScene(&MapScene);
				CGameSlotManager::getSlotData()->m_lives=CGameSlotManager::INITIAL_LIVES;
				CGameSlotManager::getSlotData()->m_continues--;
				m_state=STATE__EXITING_TO_GAME;
			}
			if(m_state==STATE__CONTINUE_TIMED_OUT)
			{
				m_state=STATE__GAME_OVER;
			}
		}
		m_continueFontOffset=(100<<2)-m_continueFontOffset;
	}

	// Change the text size
	m_continueFontSin=(m_continueFontSin+(_frames*CONTINUE_STROBE_SPEED))&4095;
}
void	CGameOverScene::renderContinue()
{
	int		yOfs;
	char	buf[100];

	m_font->setColour(242/2,245/2,15/2);
	yOfs=m_continueFontOffset>>2;

	sprintf(buf,TranslationDatabase::getString(STR__BACKEND__CONTINUE));
	m_font->setScale(((msin(m_continueFontSin)*CONTINUE_FONT_SCALE)>>12)+CONTINUE_FONT_BASE_SIZE);
	m_font->print(256,50-m_font->getStringHeight(buf)-yOfs,buf);

	sprintf(buf,"%d",m_continueTimer/COUNTDOWN_TIME_SECOND_LENGTH);
	m_font->setScale(CONTINUE_FONT_SCALE+CONTINUE_FONT_BASE_SIZE);
	m_font->print(256,80-m_font->getStringHeight(buf)-yOfs,buf);

	if(!CFader::isFading())
	{
		if(m_continueFontSin>512&m_state==STATE__CONTINUE)
		{
			sprintf(buf,TranslationDatabase::getString(STR__BACKEND__PRESS_START));
			m_font->setColour(255,0,0);
			m_font->print(256,200-m_font->getStringHeight(buf),buf);
		}
	}

	sprintf(buf,"%s%d",TranslationDatabase::getString(STR__BACKEND__CONTINUES_REMAINING),CGameSlotManager::getSlotData()->m_continues);
	m_font->setScale(256);
	m_font->setColour(100,100,100);
	m_font->print(490-(m_font->getStringWidth(buf)/2),210,buf);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CGameOverScene::initGameOver()
{
	m_gameOverTimer=0;
	m_finishedGrowingText=false;
}
void	CGameOverScene::thinkGameOver(int _frames)
{
	if(!CFader::isFading())
	{
		m_gameOverTimer+=_frames;

		if(m_finishedGrowingText&&!m_readyToExit)
		{
			if(PadGetDown(0)&(PAD_START|PAD_CROSS)||m_gameOverTimer>GAMEOVER_DISPLAY_TIME)
			{
				m_readyToExit=true;
				CFader::setFadingOut();
				GameState::setNextScene(&FrontEndScene);
				m_state=STATE__EXITING_TO_FRONT_END;
			}
		}
	}
}
void	CGameOverScene::renderGameOver()
{
	char	buf[100],*bufPtr;
	char	letter[]=" \0";
	int		i,len,step,x;

	m_font->setColour(80/2,143/2,248/2);

	sprintf(buf,TranslationDatabase::getString(STR__BACKEND__GAME_OVER));

	bufPtr=buf;
	len=strlen(buf);
	step=(400<<2)/len;
	x=(256<<2)-(((len-1)*step)/2);

	for(i=0;i<strlen(buf);i++)
	{
		int	sin,scale;

		sin=(m_gameOverTimer*GAMEOVER_FONT_GROWSPEED)-(i*GAMEOVER_FONT_GROWSPACING);
		if(sin>0)
		{
			if(sin>GAMEOVER_FONT_MAXSIN)
			{
				sin=GAMEOVER_FONT_MAXSIN;
				if(i==strlen(buf)-1)
				{
					m_finishedGrowingText=true;
				}
			}
			scale=(msin(sin)*GAMEOVER_FONT_SCALE)>>12;
			if(scale>50)
			{
				m_font->setScale(scale);
				letter[0]=*bufPtr;
				m_font->print(x>>2,100,letter);
			}
		}
		bufPtr++;
		x+=step;
	}
}

/*===========================================================================
 end */