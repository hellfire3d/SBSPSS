/*=========================================================================

	friend.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif
/*
#ifndef __FRIEND_FBBOY_H__
#include "friend\fbboy.h"
#endif
*/
#ifndef __FRIEND_FGARY_H__
#include "friend\fgary.h"
#endif

#ifndef __FRIEND_FKRUSTY_H__
#include "friend\fkrusty.h"
#endif
/*
#ifndef __FRIEND_FMMAN_H__
#include "friend\fmman.h"
#endif
*/
#ifndef __FRIEND_FPATRICK_H__
#include "friend\fpatrick.h"
#endif

#ifndef __FRIEND_FSANDY_H__
#include "friend\fsandy.h"
#endif

#ifndef __FRIEND_FSQUID_H__
#include "friend\fsquid.h"
#endif

#ifndef __FRIEND_FPLNKTON_H__
#include "friend\fplnkton.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif


#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif


// These are temporary.. I'll clean them up when I can test them (pkg)
int	strobespeed=500;
int strobebase=300;
int strobescale=200;
// These are temporary.. I'll clean them up when I can test them (pkg)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Friend NPCs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcFriend	*CNpcFriend::Create(sThingActor *ThisActor)
{
	CNpcFriend *friendNpc;

	NPC_FRIEND_UNIT_TYPE Type = CNpcFriend::getTypeFromMapEdit( ThisActor->Type );

	friendNpc = (CNpcFriend*)CThingManager::GetThing(CThing::TYPE_NPC,Type);
	if (!friendNpc)
	switch( Type )
	{
		case CNpcFriend::NPC_FRIEND_BARNACLE_BOY:
		{
			ASSERT(!"Friend removed");
//			friendNpc = new ("barnacle boy") CNpcBarnacleBoyFriend;
			break;
		}

		case CNpcFriend::NPC_FRIEND_GARY:
		{
			friendNpc = new ("gary") CNpcGaryFriend;
			break;
		}

		case CNpcFriend::NPC_FRIEND_KRUSTY:
		{
			friendNpc = new ("krusty") CNpcKrustyFriend;
			break;
		}

		case CNpcFriend::NPC_FRIEND_MERMAID_MAN:
		{
			ASSERT(!"Friend removed");
//			friendNpc = new ("mermaid man") CNpcMermaidManFriend;
			break;
		}

		case CNpcFriend::NPC_FRIEND_PATRICK:
		{
			friendNpc = new ("patrick") CNpcPatrickFriend;
			break;
		}

		case CNpcFriend::NPC_FRIEND_SANDY_CHEEKS:
		{
			friendNpc = new ("sandy cheeks") CNpcSandyFriend;
			break;
		}

		case CNpcFriend::NPC_FRIEND_SQUIDWARD:
		{
			friendNpc = new ("squidward") CNpcSquidwardFriend;
			break;
		}

		case CNpcFriend::NPC_FRIEND_PLANKTON:
		{
			ASSERT(!"Friend removed");
//			friendNpc = new ("plankton") CNpcPlanktonFriend;
			break;
		}

		default:
		{
			SYSTEM_DBGMSG("UNKNOWN %i\n",Type);
			friendNpc = NULL;
			ASSERT(0);
			break;
		}
	}

	ASSERT( friendNpc );

	friendNpc->setType( Type );
	friendNpc->setThingSubType(Type);

	u16	*PntList=(u16*)MakePtr(ThisActor,sizeof(sThingActor));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	friendNpc->init( startPos );

	return( friendNpc );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcFriend::NPC_FRIEND_UNIT_TYPE CNpcFriend::getTypeFromMapEdit( u16 newType )
{
	return( mapEditConvertTable[newType - NPC_FRIEND_MAPEDIT_OFFSET] );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::init()
{
	CNpcThing::init();

	m_extension = EXTEND_RIGHT;

	m_actorGfx = CActorPool::GetActor( (FileEquate) m_data[m_type].skelType );

	m_animPlaying = true;
	m_animNo = m_data[m_type].idleAnim;
	m_frame = 0;
	m_reversed = false;
	m_platform = NULL;

	// Get rid of the bloody auto-talk-to-npc stuff except in cd builds..
#if defined(__USER_CDBUILD__)
	m_hasSpokenToSbYet=false;
#else
	m_hasSpokenToSbYet=true;
#endif

	m_iconStrobe=0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::init( DVECTOR initPos )
{
	init();
	Pos = initPos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::postInit()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::shutdown()
{
	delete m_actorGfx;
	CNpcThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::think(int _frames)
{
	if ( m_animPlaying )
	{
		s32 frameCount;

		frameCount = m_actorGfx->getFrameCount( m_animNo );

		s32 frameShift = _frames << 8;

		if ( ( frameCount << 8 ) - m_frame > frameShift )
		{
			m_frame += frameShift;
		}
		else
		{
			m_frame = ( frameCount - 1 ) << 8;
			m_animPlaying = false;
		}
	}
	else
	{
		m_animNo = m_data[m_type].idleAnim;
		m_animPlaying = true;
		m_frame = 0;
	}

	m_iconStrobe=(m_iconStrobe+(strobespeed))&4095;

	CNpcThing::think(_frames);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::render()
{
	CNpcThing::render();

// Render

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		m_actorGfx->Render(renderPos,m_animNo,(m_frame>>8),m_reversed);

		sBBox boundingBox = m_actorGfx->GetBBox();
		boundingBox.YMax = 0;
		setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
		setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::collidedWith( CThing *_thisThing )
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			CPlayer *player = (CPlayer *) _thisThing;

			if ( !m_hasSpokenToSbYet||player->isTryingToConversateWithFriend() )
			{
				m_hasSpokenToSbYet=true;
				startConderversation();
			}

			break;
		}

		default:
			ASSERT(0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::startConderversation()
{
	// I am 'avin a fayg
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::setupWaypoints( sThingActor *ThisActor )
{
	// do nothing
}
