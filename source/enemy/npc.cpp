#include "enemy\npc.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


CNpc::NPC_DATA CNpc::m_data[NPC_UNIT_TYPE_MAX] =
{
	{
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		false,
	},

	{
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		true,
	},
};


void CNpc::init()
{
	m_type = NPC_SANDY_CHEEKS;

	switch ( m_data[this->m_type].initFunc )
	{
		case NPC_INIT_DEFAULT:
			m_controlFunc = NPC_CONTROL_MOVEMENT;

			break;

		default:
			m_controlFunc = NPC_CONTROL_MOVEMENT;

			break;
	}
}


void CNpc::shutdown()
{
}


void CNpc::think(int _frames)
{
	m_controlFunc = NPC_CONTROL_MOVEMENT;

	switch ( this->m_controlFunc )
	{
		case NPC_CONTROL_MOVEMENT:
			if ( !processSensor() )
			{
				processMovement();
			}

			break;

		case NPC_CONTROL_SHOT:
			processShot();

			break;

		case NPC_CONTROL_CLOSE:
			processClose();

			break;

		case NPC_CONTROL_COLLISION:
			processCollision();

			break;
	}

	processTimer();
}


bool CNpc::processSensor()
{
	switch( m_data[this->m_type].sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		case NPC_SENSOR_USER_CLOSE:
			/*if ( user is close )
			{
				this->controlFunc = NPC_CONTROL_CLOSE;
				processClose();

				return( true );
			}
			else*/
			{
				return( false );
			}
	}

	return( false );
}

void CNpc::processMovement()
{
	switch( m_data[this->m_type].movementFunc )
	{
		case NPC_MOVEMENT_STATIC:
			switch( m_data[this->m_type].movementModifierFunc )
			{
				case NPC_MOVEMENT_MODIFIER_NONE:
					break;

				case NPC_MOVEMENT_MODIFIER_BOB:
					//staticBob();

					break;
			}

			break;

		case NPC_MOVEMENT_FIXED_PATH:

			break;

		case NPC_MOVEMENT_USER_SEEK:
			CPlayer *player;
			
			player = GameScene.getPlayer();

			break;

		case NPC_MOVEMENT_VERTICAL:
			Pos.vy--;

			break;

		default:

			break;
	}
}

void CNpc::processShot()
{
}

void CNpc::processClose()
{
}

void CNpc::processCollision()
{
}

void CNpc::processTimer()
{
}

void CNpc::render()
{
}

void CNpc::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
	CConversation *currentConversation = GameScene.getConversation();

	if ( m_data[this->m_type].canTalk )
	{
		DVECTOR sourcePos;
		int xDiffSqr, yDiffSqr;

		// check talk distance

		sourcePos = sourceThing->getPos();

		xDiffSqr = this->Pos.vx - sourcePos.vx;
		xDiffSqr *= xDiffSqr;

		yDiffSqr = this->Pos.vy - sourcePos.vy;
		yDiffSqr *= yDiffSqr;

		if ( xDiffSqr + yDiffSqr < 250 )
		{
			if( !currentConversation->isActive() )
			{
				currentConversation->trigger( SCRIPTS_SPEECHTEST_DAT );
			}
		}
	}
}
