/*=========================================================================

	bosstext.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "game\bosstext.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad/pads.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __GFX_ACTOR__H__
#include "gfx/actor.h"
#endif

#ifndef	__ENEMY_NBOSS_H__
#include "enemy\nboss.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __STRING_ENUMS__
#include <trans.h>
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
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

const CBossText::BOSS_DATA	CBossText::s_bossData[]=
{
	{	STR__CHAPTER_1__BOSS_TITLE,	STR__CHAPTER_1__BOSS_SUB_TITLE,	STR__CHAPTER_1__BOSS_INSTRUCTIONS,	CSoundMediator::SONG_CHAPTER1_BOSS			},
	{	STR__CHAPTER_2__BOSS_TITLE,	STR__CHAPTER_2__BOSS_SUB_TITLE,	STR__CHAPTER_2__BOSS_INSTRUCTIONS,	CSoundMediator::SONG_CHAPTER2_BOSS			},
	{	STR__CHAPTER_3__BOSS_TITLE,	STR__CHAPTER_3__BOSS_SUB_TITLE,	STR__CHAPTER_3__BOSS_INSTRUCTIONS,	CSoundMediator::SONG_CHAPTER3_BOSS_AWAKE	},
	{	STR__CHAPTER_4__BOSS_TITLE,	STR__CHAPTER_4__BOSS_SUB_TITLE,	STR__CHAPTER_4__BOSS_INSTRUCTIONS,	CSoundMediator::SONG_CHAPTER4_BOSS			},
	{	STR__CHAPTER_5__BOSS_TITLE,	STR__CHAPTER_5__BOSS_SUB_TITLE,	STR__CHAPTER_5__BOSS_INSTRUCTIONS,	CSoundMediator::SONG_CHAPTER5_BOSS			},
};


// Evil global pointer to the boss character
extern CThing	*g_bossThing;

static const int s_bossTextYOffset=-8;



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBossText::init()
{
	m_scalableFontBank=new ("font") ScalableFontBank();
	m_scalableFontBank->initialise(&standardFont);
	m_scalableFontBank->setPrintArea(20,0,512-40,256);
	m_scalableFontBank->setJustification(FontBank::JUST_CENTRE);

	m_fontBank=new ("CGameScene::Init") FontBank();
	m_fontBank->initialise( &standardFont );
	m_fontBank->setPrintArea(20,0,512-40,256);
	m_fontBank->setJustification(FontBank::JUST_CENTRE);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBossText::shutdown()
{
	m_fontBank->dump();				delete m_fontBank;
	m_scalableFontBank->dump();		delete m_scalableFontBank;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBossText::select(int _dontChangeMusic)
{
	m_readyToExit=false;
	m_currentPage=0;

	if(!_dontChangeMusic)
	{
		CSoundMediator::stopSong();
		CSoundMediator::setSong(s_bossData[GameScene.GetLevel().getCurrentChapter()-1].m_songId);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBossText::think(int _frames)
{
	if(!CFader::isFading() && !m_readyToExit)
	{
		int	pad;
		pad=PadGetDown(0);

		if(pad&PAD_CROSS)
		{
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__SELECT);
			exit();
		}
		else if(pad&PAD_DOWN&&m_currentPage==0)
		{
			m_currentPage=1;
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__MOVE_CURSOR);
		}
		else if(pad&PAD_UP&&m_currentPage==1)
		{
			m_currentPage=0;
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__MOVE_CURSOR);
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const DVECTOR	pos={256,170};
void	CBossText::render()
{
	SpriteBank		*sb;
	sFrameHdr		*fhCorner,*fhSideBorder,*fhTopBorder;
	int				x,y;
	POLY_F4			*f4;
	POLY_FT4		*ft4;
	const BOSS_DATA	*bd;

	if(CFader::isFading() && m_readyToExit) return;
	// Scroll effect type thingy stuff
	sb=GameScene.getSpriteBank();
	fhCorner=sb->getFrameHeader(FRM__HELPBOX1);
	fhSideBorder=sb->getFrameHeader(FRM__HELPBOX2);
	fhTopBorder=sb->getFrameHeader(FRM__HELPBOX3);

	// Corners
	sb->printFT4(fhCorner,  0,  0,false,false,MAX_OT-2);
	sb->printFT4(fhCorner,512,  0,true ,false,MAX_OT-2);
	sb->printFT4(fhCorner,  0,256,false,true ,MAX_OT-2);
	sb->printFT4(fhCorner,512,256,true ,true ,MAX_OT-2);

	// Top/bottom
	for(x=fhCorner->W;x<512-fhCorner->W;x+=fhTopBorder->W)
	{
		sb->printFT4(fhTopBorder,x,  0,false,false,MAX_OT-2);
		sb->printFT4(fhTopBorder,x,256,false,true ,MAX_OT-2);
	}

	// Left/right
	for(y=fhCorner->H;y<256-fhCorner->H;y+=fhSideBorder->H)
	{
		sb->printFT4(fhSideBorder,  0,y,false,false,MAX_OT-2);
		sb->printFT4(fhSideBorder,512,y,true ,false,MAX_OT-2);
	}

	// Middle
	f4=GetPrimF4();
	setXYWH(f4,fhCorner->W,fhCorner->H,512-(fhCorner->W*2),256-(fhCorner->H*2));
	setRGB0(f4,224,184,107);
	AddPrimToList(f4,MAX_OT-1);

	// Background
	f4=GetPrimF4();
	setXYWH(f4,0,0,512,256);
	setRGB0(f4,0,0,0);
	AddPrimToList(f4,MAX_OT-1);

	// Text
	bd=&s_bossData[GameScene.GetLevel().getCurrentChapter()-1];
	m_fontBank->setColour(118,118,118);
	m_fontBank->print(256-20,30+s_bossTextYOffset,STR__BOSS_TEXT_TITLE);
	if(m_currentPage==0)
	{
		// Intro
		m_scalableFontBank->setScale(400);
		m_scalableFontBank->setColour(128,128,128);
		m_scalableFontBank->print(256-20,80+s_bossTextYOffset,bd->m_titleTextId);
/*
		m_fontBank->setColour(118,118,118);
		m_fontBank->print(256-20,190+s_bossTextYOffset,bd->m_subTitleTextId);
*/
		// Boss gfx
		DVECTOR	bossPoss;
		bossPoss=pos;
		bossPoss.vy+=s_bossTextYOffset;
		((CNpcBossEnemy*)g_bossThing)->renderOnMapScreen(bossPoss);
	}
	else
	{
		// Instructions
		m_fontBank->setColour(118,118,118);
		m_fontBank->print(256-20,80+s_bossTextYOffset,bd->m_instructionsTextId);
	}


	// Button texts..
	sFrameHdr		*fh1,*fh2;
	int				width;
	int				text;

	m_fontBank->setJustification(FontBank::JUST_LEFT);

	if(m_currentPage==0)
	{
		text=STR__BOSS__NEXT;
		fh1=sb->getFrameHeader(FRM__BUTD);
	}
	else
	{
		text=STR__BOSS__PREVIOUS;
		fh1=sb->getFrameHeader(FRM__BUTU);
	}
	width=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_fontBank->getStringWidth(text);
	x=128-(width/2);
	sb->printFT4(fh1,x,INSTRUCTIONS_Y_POS+INSTRUCTIONS_BUTTON_Y_OFFSET+s_bossTextYOffset,0,0,0);
	x+=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_fontBank->print(x,INSTRUCTIONS_Y_POS+s_bossTextYOffset,text);

	fh1=sb->getFrameHeader(FRM__BUTX);
	width=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT+m_fontBank->getStringWidth(STR__BOSS__CONTINUE);
	x=256+128-(width/2);
	sb->printFT4(fh1,x,INSTRUCTIONS_Y_POS+INSTRUCTIONS_BUTTON_Y_OFFSET+s_bossTextYOffset,0,0,0);
	x+=fh1->W+INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT;
	m_fontBank->print(x,INSTRUCTIONS_Y_POS+s_bossTextYOffset,STR__BOSS__CONTINUE);

	m_fontBank->setJustification(FontBank::JUST_CENTRE);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CBossText::isReadyToExit()
{
	return m_readyToExit&&!CFader::isFading();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBossText::exit()
{
	CFader::setFadingOut();
	m_readyToExit=true;
}

/*===========================================================================
 end */
