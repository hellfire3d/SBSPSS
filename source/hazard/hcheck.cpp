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
	m_flick = false;

	m_scalableFont=new ("CheckpointFont") ScalableFontBank();
	m_scalableFont->initialise(&standardFont);
	m_scalableFont->setColour(255,255,255);
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
			m_flick = false;
		}
		else
		{
			m_scalableFont->setJustification(FontBank::JUST_CENTRE);
			m_scalableFont->print( 256, 50, "Checkpoint!" );
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

				polyArea.x1 = boundingBox.XMin + renderPos.vx;
				polyArea.y1 = boundingBox.YMin + renderPos.vy;
				polyArea.x2 = boundingBox.XMax + renderPos.vx;
				polyArea.y2 = boundingBox.YMax + renderPos.vy;

				if ( polyArea.x1 < 0 )
				{
					polyArea.x1 = 0;
				}

				if ( polyArea.y1 < 0 )
				{
					polyArea.y1 = 0;
				}

				if ( polyArea.x1 > scrnWidth )
				{
					polyArea.x1 = scrnWidth;
				}

				if ( polyArea.y1 > scrnHeight )
				{
					polyArea.y1 = scrnHeight;
				}

				POLY_F4 *coverPoly;
				coverPoly = GetPrimF4();
				coverPoly->x0 = polyArea.x1;
				coverPoly->y0 = polyArea.y1;
				coverPoly->x1 = polyArea.x2;
				coverPoly->y1 = polyArea.y1;
				coverPoly->x2 = polyArea.x1;
				coverPoly->y2 = polyArea.y2;
				coverPoly->x3 = polyArea.x2;
				coverPoly->y3 = polyArea.y2;

				setRGB0( coverPoly, 255, 255, 0 );

				AddPrimToList( coverPoly, 0 );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::collidedWith(CThing *_thisThing)
{
	if ( m_isActive && !m_triggered )
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
				m_flick = true;
				CLevel::setCurrentCheckpoint( this );

				break;
			}

			default:
				break;
		}
	}
}
