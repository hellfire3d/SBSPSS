/*=========================================================================

	nboss.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NBOSS_H__
#include "enemy\nboss.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBossEnemy::postInit()
{
	m_meterOn=false;
	m_energyBar = NULL;
	m_invulnerableTimer = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBossEnemy::shutdown()
{
	if ( m_energyBar )
	{
		m_energyBar->killFX();
	}

	CNpcEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBossEnemy::think( int _frames )
{
	if ( m_invulnerableTimer > 0 )
	{
		m_invulnerableTimer -= _frames;
	}

	CNpcEnemy::think( _frames );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBossEnemy::addHealthMeter()
{
	if (!m_meterOn)
	{
		m_energyBar=(CFXNRGBar*)CFX::Create(CFX::FX_TYPE_NRG_BAR,this);
		m_energyBar->SetMax(m_data[m_type].initHealth);
		m_meterOn=true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBossEnemy::processShot( int _frames )
{
	switch( m_data[m_type].shotFunc )
	{
		case NPC_SHOT_NONE:
		{
			// do nothing
			m_controlFunc = m_oldControlFunc;

			break;
		}

		case NPC_SHOT_GENERIC:
		{
			switch ( m_state )
			{
				case NPC_GENERIC_HIT_CHECK_HEALTH:
				{
					m_health--;

					if ( m_health <= 0 )
					{
						m_state = NPC_GENERIC_HIT_DEATH_START;
						m_isDying = true;
						m_health = 0;
						CGameScene::setBossHasBeenKilled();
					}
					else
					{
						m_state = NPC_GENERIC_HIT_RECOIL;

						m_animPlaying = true;
						m_animNo = m_data[m_type].recoilAnim;
						m_frame = 0;
						m_invulnerableTimer = 2 * GameState::getOneSecondInFrames();
					}

					break;
				}

				case NPC_GENERIC_HIT_RECOIL:
				{
					processShotRecoil( _frames );

					break;
				}

				case NPC_GENERIC_HIT_DEATH_START:
				{
					processShotDeathStart( _frames );

					break;
				}

				case NPC_GENERIC_HIT_DEATH_END:
				{
					processShotDeathEnd( _frames );

					break;
				}
			}

			break;
		}
	}
}
