/*=========================================================================

	animtest.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	
				
	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */
#if defined(__VERSION_DEBUG__)

#include "paul\animtest.h"

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __ACTOR_HEADER__
#include "gfx\actor.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_Anim.h>
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

CAnimTestScene	AnimTestScene;

static CActorGfx	*m_actorSpongebob;


// Anim tables from player.cpp
extern s8			s_animMapNet[NUM_PLAYER_ADDONS][NUM_ANIM_SPONGEBOB];
extern FileEquate	s_addonActorPoolNames[];



static const char	*s_animNames[NUM_ANIM_SPONGEBOB]=
{
	"ANIM_SPONGEBOB_BUTTBOUNCEEND",
	"ANIM_SPONGEBOB_BUTTBOUNCESTART",
	"ANIM_SPONGEBOB_FALL",
	"ANIM_SPONGEBOB_GETUP",
	"ANIM_SPONGEBOB_HITGROUND",
	"ANIM_SPONGEBOB_IDLEBREATH",
	"ANIM_SPONGEBOB_JUMPEND",
	"ANIM_SPONGEBOB_JUMP",
	"ANIM_SPONGEBOB_RUN",
	"ANIM_SPONGEBOB_RUNSTOP",
	"ANIM_SPONGEBOB_RUNSTART",
	"ANIM_SPONGEBOB_TEETERBACK",
	"ANIM_SPONGEBOB_TEETERFRONT",
	"ANIM_SPONGEBOB_SWIPE",
	"ANIM_SPONGEBOB_DEATHTAR",
	"ANIM_SPONGEBOB_BALLOONJUMP",
	"ANIM_SPONGEBOB_BLOWBUBBLE",
	"ANIM_SPONGEBOB_FIREEND",
	"ANIM_SPONGEBOB_FIRESTART",
	"ANIM_SPONGEBOB_IDLEWEAPON",
	"ANIM_SPONGEBOB_WALK",
	"ANIM_SPONGEBOB_KARATE",
	"ANIM_SPONGEBOB_GETHIT",
	"ANIM_SPONGEBOB_CROUCHDOWN",
	"ANIM_SPONGEBOB_CROUCHUP",
	"ANIM_SPONGEBOB_LOOKUP",
	"ANIM_SPONGEBOB_IDLEFACEUPSIDEDOWN",
	"ANIM_SPONGEBOB_IDLEBANDAID",
//NOIDLE	"ANIM_SPONGEBOB_IDLEINHAT",
	"ANIM_SPONGEBOB_IDLEMATURE",
	"ANIM_SPONGEBOB_IDLEMOUSTACHE",
	"ANIM_SPONGEBOB_IDLENOFACE",
	"ANIM_SPONGEBOB_IDLEZORRO",
	"ANIM_SPONGEBOB_CELEBRATE",
	"ANIM_SPONGEBOB_IDLEBLINK",
	"ANIM_SPONGEBOB_ELECTROCUTED",
	"ANIM_SPONGEBOB_DEATHBOUNCE",

};

static const char	*s_addonNames[NUM_PLAYER_ADDONS]=
{
	"NET",
	"CORALBLOWER",
	"JELLYLAUNCHER",
	"BUBBLEWAND",
	"JELLYFISHINNET",
	"GLOVE",
};

static CActorGfx	*s_addonActorGfx[NUM_PLAYER_ADDONS];


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CAnimTestScene::init()
{
	m_font=new ("game over font") ScalableFontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(ScalableFontBank::JUST_LEFT);
	m_font->setOt(5);

	CActorPool::Reset();
	m_actorSpongebob=CActorPool::GetActor(ACTORS_SPONGEBOB_SBK);
	m_actorSpongebob->SetOtPos(5);

	m_animNumber=0;
	m_animFrame=0;
	m_addonNumber=-1;

	for(int i=0;i<NUM_PLAYER_ADDONS;i++)
	{
		s_addonActorGfx[i]=CActorPool::GetActor(s_addonActorPoolNames[i]);
		s_addonActorGfx[i]->SetOtPos(4);
	}

	CActorPool::SetUpCache();
	CActorPool::CleanUpCache();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CAnimTestScene::render()
{
	POLY_F4	*f4;
	DVECTOR	sbpos={100,130};
	char	buf[100];
	int		addonAnimNumber=-1;


	f4=GetPrimF4();
	setXYWH(f4,0,0,512,256);
	setRGB0(f4,110,120,130);
	AddPrimToList(f4,15);

	if(m_addonNumber!=-1)
	{
		addonAnimNumber=s_animMapNet[m_addonNumber][m_animNumber];
		if(m_animFrame>=s_addonActorGfx[m_addonNumber]->getFrameCount(addonAnimNumber))
		{
			addonAnimNumber=-2;
		}
	}

	// Actor alone
	m_actorSpongebob->Render(sbpos,m_animNumber,m_animFrame,0,0);
	sbpos.vx+=150;

	// Addon alone
	if(addonAnimNumber>=0)
	{
		s_addonActorGfx[m_addonNumber]->Render(sbpos,addonAnimNumber,m_animFrame,0,0);
	}
	else
	{
		if(addonAnimNumber==-2)
		{
			m_font->setColour(255,0,0);
		}
		m_font->print(sbpos.vx,sbpos.vy-50,"x");
		m_font->setColour(255,255,255);
	}
	sbpos.vx+=150;

	// Actor+Addon
	m_actorSpongebob->Render(sbpos,m_animNumber,m_animFrame,0,0);
	if(addonAnimNumber>=0)
	{
		s_addonActorGfx[m_addonNumber]->Render(sbpos,addonAnimNumber,m_animFrame,0,0);
	}

	CActorPool::CleanUpCache();

	sprintf(buf,"Use: Up/Down, Left/Right, L1/L2, Cross and Circle");
	m_font->print(20,140,buf);
	sprintf(buf,"Anim: %s",s_animNames[m_animNumber]);
	m_font->print(20,180,buf);
	sprintf(buf,"Frame: %d",m_animFrame);
	m_font->print(20,195,buf);
	if(m_addonNumber!=-1)
	{
		sprintf(buf,"Addon: %s",s_addonNames[m_addonNumber]);
		m_font->print(20,210,buf);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CAnimTestScene::think(int _frames)
{
	int		padRepeat,padHeld,padDown;


	padRepeat=PadGetRepeat(0);
	padHeld=PadGetHeld(0);
	padDown=PadGetDown(0);

	if(padRepeat&PAD_UP)
	{
		if(++m_animNumber==NUM_ANIM_SPONGEBOB)
		{
			m_animNumber=0;
		}
		m_animFrame=0;
	}
	else if(padRepeat&PAD_DOWN)
	{
		if(--m_animNumber<0)
		{
			m_animNumber=NUM_ANIM_SPONGEBOB-1;
		}
		m_animFrame=0;
	}

	if(padHeld&PAD_CROSS)
	{
		int fc=m_actorSpongebob->getFrameCount(m_animNumber);
		m_animFrame+=_frames;
		while(m_animFrame>=fc){m_animFrame-=fc;}
	}
	else if(padHeld&PAD_CIRCLE)
	{
		if(padDown&PAD_CIRCLE)
		{
			m_animFrame=0;
		}
		else
		{
			int fc=m_actorSpongebob->getFrameCount(m_animNumber);
			m_animFrame+=_frames;
			if(m_animFrame>=fc)m_animFrame=fc-1;
		}
	}
	else if(padRepeat&PAD_RIGHT)
	{
		if(++m_animFrame==m_actorSpongebob->getFrameCount(m_animNumber))
		{
			m_animFrame=0;
		}
	}
	else if(padRepeat&PAD_LEFT)
	{
		if(--m_animFrame<0)
		{
			m_animFrame=m_actorSpongebob->getFrameCount(m_animNumber)-1;
		}
	}

	if(padRepeat&PAD_L1)
	{
		if(++m_addonNumber==NUM_PLAYER_ADDONS)
		{
			m_addonNumber=-1;
		}
	}
	else if(padRepeat&PAD_L2)
	{
		if(--m_addonNumber<-1)
		{
			m_addonNumber=NUM_PLAYER_ADDONS-1;
		}
	}
}

#else
void	AnimTestDummyStub()
{
}
#endif

/*===========================================================================
 end */
