/*=========================================================================

	platform.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#include "platform\platform.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __ENEMY_NPCPATH_H__
#include	"enemy\npcpath.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#include "Gfx\actor.h"

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __PLATFORM_PLINEAR_H__
#include "platform\plinear.h"
#endif

#ifndef __PLATFORM_PCIRCULR_H__
#include "platform\pcirculr.h"
#endif

#ifndef __PLATFORM_PBUBBLE_H__
#include "platform\pbubble.h"
#endif

#ifndef __PLATFORM_PCBUBBLE_H__
#include "platform\pcbubble.h"
#endif

#ifndef __PLATFORM_PFISHHK_H__
#include "platform\pfishhk.h"
#endif

#ifndef __PLATFORM_PRETRACT_H__
#include "platform\pretract.h"
#endif

#ifndef __PLATFORM_PGEYSER_H__
#include "platform\pgeyser.h"
#endif

#ifndef __PLATFORM_PBOB_H__
#include "platform\pbob.h"
#endif

#ifndef __PLATFORM_PFALLING_H__
#include "platform\pfalling.h"
#endif

#ifndef __PLATFORM_PPENDULM_H__
#include "platform\ppendulm.h"
#endif

#ifndef __PLATFORM_PBRANCH_H__
#include "platform\pbranch.h"
#endif

#ifndef __PLATFORM_PCART_H__
#include "platform\pcart.h"
#endif

#ifndef __PLATFORM_PSEESAW_H__
#include "platform\pseesaw.h"
#endif

#ifndef __PLATFORM_PPLAYER_H__
#include "platform\pplayer.h"
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// All platforms are fied to this width at the minute..
#define PLATFORMWIDTH				80

// The collision box is this high.. if SB keeps falling through platforms then it *should* be sufficient
// just to up this a bit
#define PLATFORMCOLLISIONHEIGHT		60


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CLayerCollision	*CNpcPlatform::m_layerCollision;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcPlatform	*CNpcPlatform::Create(sThingPlatform *ThisPlatform)
{
	CNpcPlatform *platform;

	NPC_PLATFORM_UNIT_TYPE platformType = getTypeFromMapEdit( ThisPlatform->Type );

	switch( platformType )
	{
		case NPC_LINEAR_PLATFORM:
		{
			platform = new ("linear platform") CNpcLinearPlatform;
			break;
		}

		case NPC_CIRCULAR_PLATFORM:
		{
			platform = new ("circular platform") CNpcCircularPlatform;
			break;
		}

		case NPC_BUBBLE_PLATFORM:
		{
			platform = new ("bubble platform") CNpcBubblePlatform;
			break;
		}

		case NPC_COLLAPSING_BUBBLE_PLATFORM:
		{
			platform = new ("collapsing bubble platform") CNpcCollapsingBubblePlatform;
			break;
		}

		case NPC_FISH_HOOK_PLATFORM:
		{
			platform = new ("fish hook platform") CNpcFishHookPlatform;
			break;
		}

		case NPC_RETRACTING_PLATFORM:
		{
			platform = new ("retracting platform") CNpcRetractingPlatform;
			break;
		}

		case NPC_GEYSER_PLATFORM:
		{
			platform = new ("geyser platform") CNpcGeyserPlatform;
			break;
		}

		case NPC_BOBBING_PLATFORM:
		{
			platform = new ("bobbing platform") CNpcBobbingPlatform;
			break;
		}

		case NPC_FALLING_PLATFORM:
		{
			platform = new ("falling platform") CNpcFallingPlatform;
			break;
		}

		case NPC_FISH_HOOK_2_PLATFORM:
		{
			platform = new ("fish hook 2 platform") CNpcPendulumPlatform;
			break;
		}

		case NPC_BRANCH_PLATFORM:
		{
			platform = new ("branch platform") CNpcBranchPlatform;
			break;
		}

		case NPC_CART_PLATFORM:
		{
			platform = new ("cart platform") CNpcCartPlatform;
			break;
		}
		
		case NPC_SEESAW_PLATFORM:
		{
			platform = new ("seesaw platform") CNpcSeesawPlatform;
			break;
		}

		default:
		{
			ASSERT( 0 );
			platform = new ("platform") CNpcPlatform;
			break;
		}
	}

	ASSERT(platform);
	platform->setType( platformType );
	platform->setGraphic( ThisPlatform );

	platform->setWaypoints( ThisPlatform );

	platform->setTiltable( false );

	return( platform );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	init( startPos );

	addWaypoint( newXPos, newYPos );

	if ( ThisPlatform->PointCount > 1 )
	{
		for ( pointNum = 1 ; pointNum < ThisPlatform->PointCount ; pointNum++ )
		{
			newXPos = (u16) *PntList;
			PntList++;
			newYPos = (u16) *PntList;
			PntList++;

			addWaypoint( newXPos, newYPos );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setGraphic( sThingPlatform *ThisPlatform )
{
	m_modelGfx = new ("ModelGfx") CModelGfx;
	m_modelGfx->SetModel( ThisPlatform->Gfx );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setGraphic( u8 graphicNum )
{
	m_modelGfx = new ("ModelGfx") CModelGfx;
	m_modelGfx->SetModel( graphicNum );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::init()
{
	CPlatformThing::init();


	m_animPlaying = true;
	m_animNo = m_data[m_type].initAnim;
	m_frame = 0;

	m_heading = 0;
	m_velocity = 0;
	m_rotation = 0;
	m_reversed = false;
	m_extension = 0;
	m_contact = false;
	m_timer = m_data[m_type].initTimer * GameState::getOneSecondInFrames();
	m_timerType = m_data[m_type].initTimerType;
	m_isActive = true;
	m_detectCollision = m_data[m_type].detectCollision;
	m_state = 0;
	m_tiltAngle = 0;
	m_tiltVelocity = 0;
	m_tiltable = false;

	m_layerCollision = NULL;

	m_lifetime = 0;
	m_lifetimeType = m_data[m_type].lifetimeType;

	m_isShuttingDown = false;

	m_npcPath.initPath();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::init( DVECTOR initPos )
{
	init();

	Pos = m_initPos = m_base = initPos;

	m_initLifetime = m_lifetime = GameState::getOneSecondInFrames() * m_data[m_type].lifetime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::init( DVECTOR initPos, s32 initLifetime )
{
	init( initPos );

	m_initLifetime = m_lifetime = GameState::getOneSecondInFrames() * initLifetime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::reinit()
{
	m_animPlaying = true;
	m_animNo = m_data[m_type].initAnim;
	m_frame = 0;

	m_heading = 0;
	m_velocity = 0;
	m_rotation = 0;
	m_reversed = false;
	m_contact = false;
	m_timer = m_data[m_type].initTimer * GameState::getOneSecondInFrames();
	m_timerType = m_data[m_type].initTimerType;
	m_isActive = true;
	m_detectCollision = m_data[m_type].detectCollision;
	m_state = 0;
	m_tiltAngle = 0;
	m_tiltVelocity = 0;

	m_lifetime = m_initLifetime;

	Pos = m_initPos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::postInit()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::shutdown()
{
	delete m_modelGfx;
	m_npcPath.removeAllWaypoints();

	// temporary
	CPlatformThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setToShutdown()
{
	m_isShuttingDown = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::processLifetime( int _frames )
{
	switch( m_lifetimeType )
	{
		case NPC_PLATFORM_FINITE_LIFE:
		{
			m_lifetime -= _frames;

			if ( m_lifetime <= 0 )
			{
				setToShutdown();

				return;
			}

			break;
		}

		case NPC_PLATFORM_FINITE_LIFE_RESPAWN:
		{
			m_lifetime -= _frames;

			if ( m_lifetime <= 0 )
			{
				reinit();
			}

			break;
		}

		case NPC_PLATFORM_INFINITE_LIFE_COLLAPSIBLE:
		{
			if ( m_contact )
			{
				m_lifetime -= _frames;

				if ( m_lifetime <= 0 )
				{
					m_isActive = false;
					m_timer = 3 * GameState::getOneSecondInFrames();
					m_timerType = NPC_PLATFORM_TIMER_RESPAWN;
				}
			}

			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::think(int _frames)
{

	if ( m_isActive )
	{
		if ( m_tiltable )
		{
			processTilt( _frames );
		}

		processLifetime( _frames );

		if ( m_animPlaying )
		{
/*			int frameCount = m_actorGfx->getFrameCount(m_animNo);

			if ( frameCount - m_frame > _frames )
			{
				m_frame += _frames;
			}
			else
			{
				m_frame = frameCount - 1;
				m_animPlaying = false;
			}
*/		}

		if ( m_heading > 1024 && m_heading < 3072 )
		{
			m_reversed = true;
		}
		else
		{
			m_reversed = false;
		}

		int moveFrames = _frames;

		if ( moveFrames > 2 )
		{
			// make sure platforms don't go berserk if too many frames are dropped

			moveFrames = 2;
		}

		processMovement( moveFrames );

		m_contact = false;
	}

	processTimer( _frames );

	CPlatformThing::think(_frames);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setCollisionAngle(int newAngle)
{
	CPlayer	*player;

	// Is the player stood on this platform as it rotates?
	player=GameScene.getPlayer();
	if(player->isOnPlatform()==this)
	{
		DVECTOR	playerPos;

		playerPos=player->getPos();
		if(getHeightFromPlatformAtPosition(playerPos.vx,playerPos.vy)==0)
		{
			// Ok.. currently stood on the platform - awkward bastard
			DVECTOR	centre;
			int		x,y;
			DVECTOR	shove;

			// Rotate backwards to find x position on platform
			centre=getCollisionCentre();
			x=-((centre.vx-playerPos.vx)*mcos(-getCollisionAngle()&4095)>>(12));

			// Rotate forwards to find new position *after* the platform has been rotated
			y=x*msin(newAngle&4095)>>(12);
			x=x*mcos(newAngle&4095)>>(12);

			// Shove the player to the new position
			shove.vx=0;//(x+centre.vx)-playerPos.vx;		pkg - can't get this to work :(
			shove.vy=(y+centre.vy)-playerPos.vy;

			// Finally, to cope with any innacuracies that have been introduced, we run this
			// new position through the getHeightFromPlatformAtPosition() code and use this to
			// make sure that the player is still on the platform
			playerPos.vx+=shove.vx;
			playerPos.vy+=shove.vy;
			y=(centre.vy-playerPos.vy)+((centre.vx-playerPos.vx)*msin(-newAngle&4095)>>12);
			if(y)
			{
				shove.vy+=y;
			}

			player->shove(shove);
		}
	}

	CPlatformThing::setCollisionAngle(newAngle);
	calculateBoundingBoxSize();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::calculateBoundingBoxSize()
{
	int		angle;
	DVECTOR	centre;
	int		halfLength;
	int		x1,y1,x2,y2;

	angle=getCollisionAngle();
	centre=getCollisionCentre();
	halfLength=PLATFORMWIDTH/2;

	x1=-halfLength*mcos(angle&4095)>>12;
	y1=-halfLength*msin(angle&4095)>>12;
	x2=+halfLength*mcos(angle&4095)>>12;
	y2=+halfLength*msin(angle&4095)>>12;

	setCollisionSize(abs(x2-x1),abs(y2-y1)+PLATFORMCOLLISIONHEIGHT);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::processTilt( int _frames )
{
	bool forceActing = false;

	if ( m_contact )
	{
		// user is touching platform, tilt accordingly

		CPlayer *player = GameScene.getPlayer();
		DVECTOR playerPos = player->getPos();

		if ( playerPos.vx > Pos.vx + 10 )
		{
			forceActing = true;

			// tilt clockwise

			if ( m_tiltVelocity < 2560 )
			{
				m_tiltVelocity += 64;
			}
		}
		else if ( playerPos.vx < Pos.vx - 10 )
		{
			forceActing = true;

			// tilt anticlockwise

			if ( m_tiltVelocity > -2560 )
			{
				m_tiltVelocity -= 64;
			}
		}
	}

	if ( !forceActing )
	{
		// no force acting, hence reduce velocity

		s32 reduction = abs( m_tiltVelocity );

		if ( reduction > 64 )
		{
			reduction = 64;
		}

		if ( m_tiltVelocity >= 0 )
		{
			reduction *= -1;
		}

		m_tiltVelocity += reduction;
	}

	m_tiltAngle += m_tiltVelocity;

	if ( m_tiltAngle > ( 512 << 8 ) )
	{
		m_tiltAngle = ( 512 << 8 );
		m_tiltVelocity = 0;
	}
	else if ( m_tiltAngle < -( 512 << 8 ) )
	{
		m_tiltAngle = -( 512 << 8 );
		m_tiltVelocity = 0;
	}

	setCollisionAngle( m_tiltAngle >> 8 );

	/*if ( getCollisionAngle() > 512 && getCollisionAngle() < 3584 )
	{
		m_detectCollision = false;
	}
	else
	{
		m_detectCollision = true;
	}*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::processTimer( int _frames )
{
	switch( m_timerType )
	{
		case NPC_PLATFORM_TIMER_NONE:
			break;

		case NPC_PLATFORM_TIMER_RESPAWN:
		{
			if ( m_timer > 0 )
			{
				m_timer -= _frames;
			}
			else
			{
				reinit();
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::collidedWith( CThing *_thisThing )
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			if ( m_detectCollision && m_isActive )
			{
				CPlayer *player;
				DVECTOR	playerPos;
				CRECT	collisionArea;

				// Only interested in SBs feet colliding with the box (pkg)
				player=(CPlayer*)_thisThing;
				playerPos=player->getPos();
				collisionArea=getCollisionArea();
				if(playerPos.vx>=collisionArea.x1&&playerPos.vx<=collisionArea.x2&&
				   playerPos.vy>=collisionArea.y1&&playerPos.vy<=collisionArea.y2)
				{
					player->setPlatform( this );

					m_contact = true;
				}
			}

			break;
		}

		default:
			ASSERT(0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::processMovement( int _frames )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		// Render
		DVECTOR renderPos;
		DVECTOR	offset = CLevel::getCameraPos();

		renderPos.vx = Pos.vx - offset.vx;
		renderPos.vy = Pos.vy - offset.vy;

		if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
		{
			if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
			{
				m_modelGfx->Render(renderPos);
//				POLY_F4	*F4=GetPrimF4();
//				setXYWH(F4,renderPos.vx-32,renderPos.vy-32,64,16);
//				setRGB0(F4,127,127,64);
//				AddPrimToList(F4,2);

#if defined (__USER_paul__) || defined (__USER_charles__)
	DVECTOR	centre;
	int		halfLength;
	int		x1,y1,x2,y2;

	centre=getCollisionCentre();
	halfLength=PLATFORMWIDTH/2;

	x1=-halfLength*mcos(getCollisionAngle()&4095)>>12;
	y1=-halfLength*msin(getCollisionAngle()&4095)>>12;
	x2=+halfLength*mcos(getCollisionAngle()&4095)>>12;
	y2=+halfLength*msin(getCollisionAngle()&4095)>>12;

	centre.vx-=offset.vx;
	centre.vy-=offset.vy;
	x1+=centre.vx;
	y1+=centre.vy;
	x2+=centre.vx;
	y2+=centre.vy;

	DrawLine(x1,y1,x2,y2,0,255,0,0);
#endif
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef REMOVETHIS
s32	CNpcPlatform::getNewYPos(CThing *_thisThing)
{
	CRECT	thisRect;
	DVECTOR thatPos = _thisThing->getPos();

	thisRect = getCollisionArea();

	// 'render' collision box at correct angle

	SVECTOR testPointsNonRel[4];
	VECTOR testPoints[4];

	testPointsNonRel[0].vx = thisRect.x1 - Pos.vx;
	testPointsNonRel[0].vy = thisRect.y1 - Pos.vy;

	testPointsNonRel[1].vx = thisRect.x2 - Pos.vx;
	testPointsNonRel[1].vy = thisRect.y1 - Pos.vy;

	testPointsNonRel[2].vx = thisRect.x2 - Pos.vx;
	testPointsNonRel[2].vy = thisRect.y2 - Pos.vy;

	testPointsNonRel[3].vx = thisRect.x1 - Pos.vx;
	testPointsNonRel[3].vy = thisRect.y2 - Pos.vy;

	MATRIX mtx;
	SetIdentNoTrans(&mtx );
	RotMatrixZ( getCollisionAngle(), &mtx );

	int i;

	for ( i = 0 ; i < 4 ; i++ )
	{
		ApplyMatrix( &mtx, &testPointsNonRel[i], &testPoints[i] );

		testPoints[i].vx += Pos.vx;
		testPoints[i].vy += Pos.vy;
	}

	// now find the highest y pos

	// first set highestY to lowest of the four points

	s16 highestY = testPoints[0].vy;

	for ( i = 1 ; i < 4 ; i++ )
	{
		if ( testPoints[i].vy > highestY ) // remember y is inverted
		{
			highestY = testPoints[i].vy;
		}
	}

	for ( i = 0 ; i < 4 ; i++ )
	{
		int j = i + 1;
		j %= 4;

		VECTOR highestX, lowestX;

		if ( testPoints[i].vx < testPoints[j].vx )
		{
			lowestX = testPoints[i];
			highestX = testPoints[j];
		}
		else
		{
			lowestX = testPoints[j];
			highestX = testPoints[i];
		}

		if ( highestX.vx == lowestX.vx )
		{
			// have to compare heights of both points to get highest

			if ( lowestX.vy < highestY )
			{
				highestY = lowestX.vy;
			}

			if ( highestX.vy < highestY )
			{
				highestY = highestX.vy;
			}
		}
		else
		{
			if ( thatPos.vx >= lowestX.vx && thatPos.vx <= highestX.vx )
			{
				// current position is above or below this line

				s16 testY;

				testY = lowestX.vy + ( ( thatPos.vx - lowestX.vx ) * ( highestX.vy - lowestX.vy ) ) /
							( highestX.vx - lowestX.vx );

				if ( testY < highestY )
				{
					highestY = testY;
				}
			}
		}
	}

	return( highestY );
}
#endif // REMOVETHIS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef REMOVETHIS
int	CNpcPlatform::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
	DVECTOR	pos,thisThingPos;
	int		radius;
	int		collided;

	MATRIX mtx;

	pos=getCollisionCentre();
	thisThingPos=_thisThing->getCollisionCentre();

	radius=getCollisionRadius()+_thisThing->getCollisionRadius();
	collided=false;
	if(abs(pos.vx-thisThingPos.vx)<radius&&
	   abs(pos.vy-thisThingPos.vy)<radius)
	{
		CRECT	thisRect,thatRect;

		thisRect=getCollisionArea();

		// ensure user 'sticks' to platform whilst it is moving along

		thatRect=_thisThing->getCollisionArea();

		// rotate thatPos opposite way to this CThing's collision angle, so that we can regard them both as being at 0 rotation

		// get target thing's position

		DVECTOR thatPos = _thisThing->getPos();

		// get target thing's position relative to this thing's position

		SVECTOR relativePos;
		relativePos.vx = thatPos.vx - Pos.vx;
		relativePos.vy = thatPos.vy - Pos.vy;

		VECTOR newPos;

		// get target thing's collision area relative to 0

		thatRect.x1 -= thatPos.vx;
		thatRect.y1 -= thatPos.vy;
		thatRect.x2 -= thatPos.vx;
		thatRect.y2 -= thatPos.vy;

		SetIdentNoTrans(&mtx );
		RotMatrixZ( -getCollisionAngle(), &mtx );

		// rotation target relative position back to 0 by this thing's collision angle

		ApplyMatrix( &mtx, &relativePos, &newPos );

		// add on this thing's position to get new target thing's position after rotation around this thing

		newPos.vx += Pos.vx;
		newPos.vy += Pos.vy;

		// reposition target thing's collision area
		// horrible, horrible +2 shite is to deal with useless PSX innacurracies in calculations, which can cause it to
		// believe that two collision areas are not *quite* colliding, even though they are

		thatRect.x1 += newPos.vx - 2;
		thatRect.y1 += newPos.vy - 2;
		thatRect.x2 += newPos.vx + 2;
		thatRect.y2 += newPos.vy + 2;

		// check to see if bounding boxes collide

		if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
		   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
		{
			collided=true;

			// check to see if centre point (i.e. where the object is standing) collides too

			if ( ( newPos.vx >= ( thisRect.x1 - 2 ) && newPos.vx <= ( thisRect.x2 + 2 ) ) &&
					( newPos.vy >= ( thisRect.y1 - 2 ) && newPos.vy <= ( thisRect.y2 + 2 ) ) )
			{
				thatPos.vy = getNewYPos( _thisThing );

				// vertical height change is the sum of the maximums of BOTH objects
				// potentially, one object could be falling down through another object that is moving up
				// hence we provide a certain leeway to compensate

				s32 thisDeltaX = abs( getPosDelta().vx );
				s32 thisDeltaY = abs( getPosDelta().vy );

				s32 thisDelta;

				if ( thisDeltaX > thisDeltaY )
				{
					thisDelta = thisDeltaX;
				}
				else
				{
					thisDelta = thisDeltaY;
				}

				s32 thatDeltaX = abs( _thisThing->getPosDelta().vx );
				s32 thatDeltaY = abs( _thisThing->getPosDelta().vy );

				s32 thatDelta;

				if ( thatDeltaX > thatDeltaY )
				{
					thatDelta = thatDeltaX;
				}
				else
				{
					thatDelta = thatDeltaY;
				}

				s32 verticalDelta = thisDelta + thatDelta;

				if ( thatPos.vy - _thisThing->getPos().vy >= -( verticalDelta ) )
				{
					if ( _thisThing->getHasPlatformCollided() )
					{
						// if this has already collided with a platform, check the current platform is
						// (a) within 10 units,
						// (b) higher

						DVECTOR oldCollidedPos = _thisThing->getNewCollidedPos();

						s32 oldY = abs( oldCollidedPos.vy - ( _thisThing->getPos().vy - verticalDelta ) );
						s32 currentY = abs( thatPos.vy - ( _thisThing->getPos().vy - verticalDelta ) );

						//if ( thatPos.vy < oldCollidedPos.vy )
						if ( currentY < oldY )
						{
							_thisThing->setNewCollidedPos( thatPos );
						}
					}
					else
					{
						_thisThing->setHasPlatformCollided( true );

						_thisThing->setNewCollidedPos( thatPos );
					}
				}
			}
		}
	}

	return collided;
}
#endif	// REMOVETHIS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setTiltable( bool isTiltable )
{
	m_tiltable = isTiltable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int	CNpcPlatform::getHeightFromPlatformAtPosition(int _x,int _y)
{
	DVECTOR	centre;
	int		y;

	// Rotate backwards to find height at current position
	centre=getCollisionCentre();
	y=(centre.vx-_x)*msin(-getCollisionAngle()&4095)>>12;

	return (centre.vy-_y)+y;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::addWaypoint( s32 xPos, s32 yPos )
{
	DVECTOR newPos;

	newPos.vx = xPos << 4;
	newPos.vy = yPos << 4;

	m_npcPath.addWaypoint( newPos );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setTypeFromMapEdit( u16 newType )
{
	m_type = mapEditConvertTable[newType];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcPlatform::NPC_PLATFORM_UNIT_TYPE CNpcPlatform::getTypeFromMapEdit( u16 newType )
{
	return( mapEditConvertTable[newType] );
}