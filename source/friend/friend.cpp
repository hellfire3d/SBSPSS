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

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __FRIEND_FSQUID_H__
#include "friend\fsquid.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Friend NPCs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcFriend	*CNpcFriend::Create(sThingActor *ThisActor)
{
	CNpcFriend *friendNpc;

	NPC_FRIEND_UNIT_TYPE friendType = CNpcFriend::getTypeFromMapEdit( ThisActor->Type );

	switch( friendType )
	{
		case CNpcFriend::NPC_FRIEND_SQUIDWARD:
		{
			friendNpc = new ("squidward") CNpcSquidwardFriend;
			break;
		}

		default:
		{
			printf("UNKNOWN %i\n",friendType);
			friendNpc = NULL;
			ASSERT(0);
			break;
		}
	}

	ASSERT( friendNpc );

	friendNpc->setType( friendType );

	u16	*PntList=(u16*)MakePtr(ThisActor,sizeof(sThingActor));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

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

	DVECTOR ofs = getCollisionSize();

	m_drawOffset.vx = 0;
	m_drawOffset.vy = -( ofs.vy >> 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::init( DVECTOR initPos )
{
	init();
	Pos = initPos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::shutdown()
{
	//m_spriteBank->dump();		delete m_spriteBank;

	delete m_actorGfx;
	CNpcThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::think(int _frames)
{
	if ( _frames > 2 )
	{
		// make sure enemies don't go berserk if too many frames are dropped

		_frames = 2;
	}

	CNpcThing::think(_frames);

	if ( m_animPlaying )
	{
		s32 frameCount;

		frameCount = m_actorGfx->getFrameCount( m_animNo );

		s32 frameShift = ( _frames << 8 ) >> 1;

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

	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s8 groundHeight;

	// check vertical collision

	groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

	if ( groundHeight <= 0 )
	{
		// make sure we are on the ground, not below it

		Pos.vy += groundHeight;
	}
	else
	{
		// above ground

		if ( groundHeight < yMovement )
		{
			// colliding with ground

			Pos.vy += groundHeight;
		}
		else
		{
			Pos.vy += yMovement;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::render()
{
	CNpcThing::render();

	// Render
	DVECTOR renderPos;
	DVECTOR	offset = CLevel::getCameraPos();

	renderPos.vx = Pos.vx - offset.vx;
	renderPos.vy = Pos.vy - offset.vy;

	if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
	{
		if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
		{
			m_actorGfx->Render(renderPos,m_animNo,(m_frame>>8),m_reversed);

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
	switch( evt )
	{
		case USER_REQUEST_TALK_EVENT:
		{
			if ( m_data[this->m_type].canTalk )
			{
				DVECTOR sourcePos;
				s32 xDiffSqr, yDiffSqr;

				// check talk distance

				sourcePos = sourceThing->getPos();

				xDiffSqr = this->Pos.vx - sourcePos.vx;
				xDiffSqr *= xDiffSqr;

				yDiffSqr = this->Pos.vy - sourcePos.vy;
				yDiffSqr *= yDiffSqr;

				if ( xDiffSqr + yDiffSqr < 10000 )
				{
					if( !CConversation::isActive() )
					{
						CConversation::trigger( SCRIPTS_SPEECHTEST_DAT );
					}
				}
			}

			break;
		}

		default:
			// ignore

			break;
	}
}
