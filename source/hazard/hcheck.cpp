/*=========================================================================

	hcheck.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HCHECK_H__
#include "hazard\hcheck.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::init()
{
	CNpcHazard::init();

	m_triggered = false;
//	m_spriteFrame = FRM__CHECKPOINT;
	m_spriteFrame = 0; // Change by dave cos the checkpoint gfx aint there no more
	m_timer = 0;
	m_flick = 0;

	m_scalableFont=new ("CheckpointFont") ScalableFontBank();
	m_scalableFont->initialise(&standardFont);
	m_scalableFont->setColour(255,255,0);
	m_scalableFont->setScale(511);

	if ( CLevel::getCurrentCheckpoint() == this )
	{
		m_triggered = true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::think(int _frames)
{
	if ( m_flick )
	{
		m_timer -= _frames;

		if ( m_timer <= 0 )
		{
			m_flick = 0;
		}
		else
		{
			int	HF=GameState::getOneSecondInFrames()>>1;
			if (m_timer<HF) m_flick^=2;
			if (m_flick & 2 && !GameScene.getIsPaused() )
			{
				m_scalableFont->setJustification(FontBank::JUST_CENTRE);
				m_scalableFont->print( 256, 50, "Checkpoint!" );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::shutdown()
{
	m_scalableFont->dump();	delete m_scalableFont;

	CNpcHazard::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int	CPXO=18;
static const int	CPYO=8;
static const int	CPOT=13;
static const int	CPA=3;
static const int	CPZ=50;

void CNpcCheckpointHazard::render()
{
	sFrameHdr	*frameHdr;
	POLY_FT4	*Ft4;

	if ( m_isActive )
	{
		CHazardThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);

			if ( CLevel::getCurrentCheckpoint() == this )
			{
				int		scrnWidth = VidGetScrW();
				int		scrnHeight = VidGetScrH();

				sBBox boundingBox = m_modelGfx->GetBBox();

				CRECT polyArea;

				polyArea.x1 = boundingBox.XMin;
				polyArea.y1 = boundingBox.YMin;
				polyArea.x2 = boundingBox.XMax;
				polyArea.y2 = boundingBox.YMax;
				polyArea.x1+=CPXO;
				polyArea.x2-=CPXO;
				polyArea.y1+=CPYO;

u8				*PrimPtr=GetPrimPtr();
TPOLY_F4		*F4=(TPOLY_F4 *)PrimPtr;
				PrimPtr+=sizeof(TPOLY_F4);
				SetPrimPtr((u8*)PrimPtr);
				setTPolyF4(F4);

//				F4->x0 = polyArea.x1; F4->y0 = polyArea.y1;
//				F4->x1 = polyArea.x2; F4->y1 = polyArea.y1;
//				F4->x2 = polyArea.x1; F4->y2 = polyArea.y2;
//				F4->x3 = polyArea.x2; F4->y3 = polyArea.y2;
long		Tmp;
SVECTOR		I;
				I.vz=CPZ;
				I.vy=polyArea.y1;
				I.vx=polyArea.x1; RotTransPers(&I,(long*)&F4->x0,&Tmp,&Tmp);
				I.vx=polyArea.x2; RotTransPers(&I,(long*)&F4->x1,&Tmp,&Tmp);
				I.vy=polyArea.y2; 
				I.vx=polyArea.x1; RotTransPers(&I,(long*)&F4->x2,&Tmp,&Tmp);
				I.vx=polyArea.x2; RotTransPers(&I,(long*)&F4->x3,&Tmp,&Tmp);
				
				setTSemiTrans(F4,1);
				setTABRMode(F4,CPA);

				setRGB0( F4, 255, 255, 0 );

				AddPrimToList( F4, CPOT );

			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::collidedWith(CThing *_thisThing)
{

//	if ( m_isActive && !m_triggered )
	if ( m_isActive && CLevel::getCurrentCheckpoint() != this )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CRECT const	&collisionArea=getCollisionArea();
				DVECTOR respawnPos;
				respawnPos.vx=collisionArea.x1+((collisionArea.x2-collisionArea.x1)/2);
				respawnPos.vy=collisionArea.y2;
				((CPlayer*)_thisThing)->setRespawnPosAndRingTelephone(respawnPos);
				m_triggered = true;
				m_timer = GameState::getOneSecondInFrames();
				m_flick = 3;
				CLevel::setCurrentCheckpoint( this );

				break;
			}

			default:
				break;
		}
	}
}
