/*=========================================================================

	scenesel.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#if defined(__VERSION_DEBUG__)

#include "paul\scenesel.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __DATA_STRUCTS_HEADER__
#include "Dstructs.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

// Scenes
#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __FMA_FMA_H__
#include "fma\fma.h"
#endif

#ifndef __SHOP_SHOP_H__
#include "shop\shop.h"
#endif

#ifndef __BACKEND_GAMEOVER_H__
#include "backend\gameover.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif

#ifndef __BACKEND_CREDITS_H__
#include "backend\credits.h"
#endif

#ifndef __PAUL_ANIMTEST_H__
#include "paul\animtest.h"
#endif

#ifndef __PAUL_SNDTEST_H__
#include "paul\sndtest.h"
#endif

#ifndef __SAVE_SAVE_H__
#include "save\save.h"
#endif

#ifndef __GFX_FADER_H__
#include "gfx\fader.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

// Uncomment this to make the selector automatically choose the default ( first ) scene
// You can still use L1&L2 to force the selection menu to appear
#if defined (__USER_art__)||defined (__USER_sbart__)
#define	AUTOMATICALLY_CHOOSE_DEFAULT
#endif

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

CScene	*CSceneSelector::s_sceneList[]=
{
	&FrontEndScene,				// First scene in the list is the default scene
	&GameScene,
	&GameOverScene,
	&FmaScene,
	&MapScene,
	&ShopScene,
	&CreditsScene,
	&AnimTestScene,
	&SaveScene,
	&SoundTestScene,
};
int		CSceneSelector::s_sceneCount=sizeof(s_sceneList)/sizeof(CScene*);


CSceneSelector	SceneSelector;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSceneSelector::init()
{
	m_font=new ("scene select font") FontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(FontBank::JUST_CENTRE);
	m_font->setOt(10);

	m_currentSelection=0;
	m_state=STATE_INIT;
	m_countdown=2;				// Need a couple of frames for the pad handler to wake up..

	CFader::setFadingIn();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSceneSelector::shutdown()
{
	m_font->dump();				delete m_font;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSceneSelector::render()
{
	if(m_state==STATE_SELECTING)
	{
		POLY_F4	*f4;
		int		i;

		f4=GetPrimF4();
		setXYWH(f4,0,0,512,256);
		setRGB0(f4,10,20,30);
		AddPrimToList(f4,15);

		m_font->setColour(255,255,255);
		m_font->print(256,50,"Select scene:");
		for(i=0;i<s_sceneCount;i++)
		{
			if(i==m_currentSelection)
			{
				m_font->setColour(100,255,100);
			}
			else
			{
				m_font->setColour(255,100,100);
			}
			m_font->print(256,80+(i*m_font->getCharHeight()),s_sceneList[i]->getSceneName());
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#include "pad\vibe.h"

void CSceneSelector::think(int _frames)
{
if(PadGetDown(0)&PAD_L1)	CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_LONG_STROBEY);
if(PadGetDown(0)&PAD_L2)	CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_LONG_STROBEY,127);

	switch(m_state)
	{
		case STATE_INIT:
		{
			if(--m_countdown==0)
			{
#ifdef AUTOMATICALLY_CHOOSE_DEFAULT
				int	pad;
				pad=PadGetHeld(0);
				if(pad&(PAD_L1|PAD_L2))
				{
					m_state=STATE_SELECTING;
				}
				else
				{
					m_state=STATE_SELECTED;
				}
#else
				m_state=STATE_SELECTING;
#endif
			}
			break;
		}

		case STATE_SELECTING:
		{
			int	pad;
			pad=PadGetDown(0);
			if(pad&PAD_UP)
			{
				if(--m_currentSelection==-1)m_currentSelection=s_sceneCount-1;
			}
			else if(pad&PAD_DOWN)
			{
				if(++m_currentSelection==s_sceneCount)m_currentSelection=0;
			}
			else if(pad&(PAD_CROSS|PAD_START))
			{
				m_state=STATE_SELECTED;
			}
			break;
		}
		
		case STATE_SELECTED:
		{
			selectScene(m_currentSelection);
			m_state=STATE_READY_TO_EXIT;
			break;
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CSceneSelector::readyToShutdown()
{
	return m_state==STATE_READY_TO_EXIT;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSceneSelector::selectScene(int _scene)
{
	GameState::setNextScene(s_sceneList[_scene]);
}

#else
void	SceneSelTestDummyStub()
{
}
#endif

/*===========================================================================
 end */