/*=========================================================================

	gameover.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	Game Over scene. Shown when out of contiunes or player chooses
				not to continue any longer

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

CGameOverScene	GameOverScene;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::init()
{
	m_image=CFileIO::loadFile(BACKDROP_GAMEOVER_GFX);
	ASSERT(m_image);
	SetScreenImage((u8*)m_image);

	m_font=new ("game over font") ScalableFontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(ScalableFontBank::JUST_CENTRE);
	m_font->setOt(5);

	m_readyToExit=false;
	CFader::setFadingIn(CFader::BLACK_FADE);

	initContinue();
	initGameOver();

	m_state=STATE__CONTINUE;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::shutdown()
{
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
		case STATE__GAME_OVER:
			renderGameOver();
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameOverScene::think(int _frames)
{
	if(!CFader::isFading()&&!m_readyToExit)
	{
		if(PadGetDown(0)&PAD_START)
		{
			m_readyToExit=true;
			CFader::setFadingOut(CFader::BLACK_FADE);
			GameState::setNextScene(&FrontEndScene);
		}
	}

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
	m_continueTimer=10*GameState::getOneSecondInFrames()*COUNTDOWN_TIME_MODIFIER;
}
void	CGameOverScene::thinkContinue(int _frames)
{
	int	move;

	// Slide the text onscreen
	if(m_state==STATE__CONTINUE)
	{
		if(!CFader::isFading())
		{
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
					if(PadGetDown(0)&PAD_CROSS)
					{
						m_continueTimer-=m_continueTimer%(GameState::getOneSecondInFrames()*COUNTDOWN_TIME_MODIFIER);
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
	else if(m_state==STATE__CONTINUE_TIMED_OUT)
	{
		// Slide text back off the screen
		m_continueFontOffset=m_continueFontOffset-(100<<2);
		move=m_continueFontOffset/10;
		if(move==0)
		{
			move=1;
		}
		m_continueFontOffset-=move;
		if(m_continueFontOffset>(100<<2))
		{
			m_continueFontOffset=0;
			m_state=STATE__GAME_OVER;
		}
		m_continueFontOffset=m_continueFontOffset+(100<<2);
	}

	// Change the text size
	m_continueFontSin=(m_continueFontSin+(_frames*CONTINUE_STROBE_SPEED))&4095;
}
void	CGameOverScene::renderContinue()
{
	if(!CFader::isFading())
	{
		int		yOfs;
		char	buf[100];

		m_font->setColour(255,255,255);
		yOfs=m_continueFontOffset>>2;

		sprintf(buf,TranslationDatabase::getString(STR__BACKEND__CONTINUE));
		m_font->setScale(((msin(m_continueFontSin)*CONTINUE_FONT_SCALE)>>12)+CONTINUE_FONT_BASE_SIZE);
		m_font->print(256,50-m_font->getStringHeight(buf)-yOfs,buf);

		sprintf(buf,"%d",m_continueTimer/(GameState::getOneSecondInFrames()*COUNTDOWN_TIME_MODIFIER));
		m_font->setScale(CONTINUE_FONT_SCALE+CONTINUE_FONT_BASE_SIZE);
		m_font->print(256,80-m_font->getStringHeight(buf)-yOfs,buf);

		if(m_continueFontSin>512&m_state==STATE__CONTINUE)
		{
			sprintf(buf,TranslationDatabase::getString(STR__BACKEND__PRESS_START));
			m_font->setColour(255,0,0);
			m_font->print(256,200-m_font->getStringHeight(buf),buf);
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CGameOverScene::initGameOver()
{
}
void	CGameOverScene::thinkGameOver(int _frames)
{
}
int bigscale=510;
void	CGameOverScene::renderGameOver()
{
	char	buf[100],*bufPtr;
	char	letter[]=" \0";
	int		i,len,step,x;

	m_font->setColour(255,255,255);

	sprintf(buf,"PARTIE TERMINEE");//TranslationDatabase::getString(STR__BACKEND__GAME_OVER));
	m_font->setScale(bigscale);

	bufPtr=buf;
	len=strlen(buf);
	step=(400<<2)/len;
	x=256-(((len*step)/2)>>2);
	for(i=0;i<strlen(buf);i++)
	{
		letter[0]=*bufPtr++;
		m_font->print(x-(m_font->getStringWidth(letter)/2),100,letter);
		DrawLine(x,20,x,236,255,255,255,0);
		x+=step>>2;
	}
}

/*===========================================================================
 end */