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


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBossText::init()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBossText::shutdown()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBossText::select()
{
//	CSoundMediator::stopSong();
//	CSoundMediator::setSong(s_bossData[Level.getCurrentChapter()-1].m_songId);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBossText::think(int _frames)
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBossText::render()
{
	SpriteBank		*sb;
	sFrameHdr		*fhCorner,*fhSideBorder,*fhTopBorder;
	int				x,y;
	POLY_F4			*f4;
	POLY_G4			*g4;
	const BOSS_DATA	*bd;

	// Scroll effect type thingy stuff
	sb=GameScene.getSpriteBank();
	fhCorner=sb->getFrameHeader(FRM__HELPBOX1);
	fhSideBorder=sb->getFrameHeader(FRM__HELPBOX2);
	fhTopBorder=sb->getFrameHeader(FRM__HELPBOX3);

	// Corners
	sb->printFT4(fhCorner,  0,  0,false,false,4);
	sb->printFT4(fhCorner,512,  0,true ,false,4);
	sb->printFT4(fhCorner,  0,256,false,true ,4);
	sb->printFT4(fhCorner,512,256,true ,true ,4);

	// Top/bottom
	for(x=fhCorner->W;x<512-fhCorner->W;x+=fhTopBorder->W)
	{
		sb->printFT4(fhTopBorder,x,  0,false,false,4);
		sb->printFT4(fhTopBorder,x,256,false,true ,4);
	}

	// Left/right
	for(y=fhCorner->H;y<256-fhCorner->H;y+=fhSideBorder->H)
	{
		sb->printFT4(fhSideBorder,  0,y,false,false,4);
		sb->printFT4(fhSideBorder,512,y,true ,false,4);
	}

	// Middle
	f4=GetPrimF4();
	setXYWH(f4,fhCorner->W,fhCorner->H,512-(fhCorner->W*2),256-(fhCorner->H*2));
	setRGB0(f4,224,184,107);
	AddPrimToList(f4,5);

	// Background
	g4=GetPrimG4();
	setXYWH(g4,0,0,512,256);
	setRGB0(g4,70,50,60);
	setRGB1(g4,70,50,60);
	setRGB2(g4,50,60,70);
	setRGB3(g4,50,60,70);
	AddPrimToList(g4,5);

	// Instructions..
	bd=&s_bossData[GameScene.GetLevel().getCurrentChapter()-1];

	m_scalableFontBank->setTrans(0);
	m_scalableFontBank->setSMode(0);
	m_scalableFontBank->setPrintArea(20,0,512-40,256);
	m_scalableFontBank->setJustification(FontBank::JUST_CENTRE);
	m_scalableFontBank->setColour(128,128,128);
	m_fontBank->setTrans(0);
	m_fontBank->setSMode(0);
	m_fontBank->setPrintArea(20,0,512-40,256);
	m_fontBank->setJustification(FontBank::JUST_CENTRE);


	m_fontBank->setColour(118,118,118);
	m_fontBank->print(256-20,25,STR__BOSS_TEXT_TITLE);
	m_scalableFontBank->setScale(300);
	m_fontBank->setColour(128,128,128);
	m_scalableFontBank->print(256-20,60,bd->m_titleTextId);
	m_fontBank->setColour(118,118,118);
	m_fontBank->print(256-20,80,bd->m_subTitleTextId);
	m_fontBank->setColour(118,118,118);
	m_fontBank->print(256-20,105,bd->m_instructionsTextId);


	m_fontBank->setPrintArea(0,0,256,512);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CBossText::isReadyToExit()
{
	return false;
}



			m_gamestate=GAMESTATE_FADING_OUT_OF_BOSS_INTRO;
			CFader::setFadingOut();

/*===========================================================================
 end */
