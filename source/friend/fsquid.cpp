/*=========================================================================

	fsquid.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FSQUID_H__
#include "friend\fsquid.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef	__ANIM_SQUIDWARD_HEADER__
#include <ACTOR_SQUIDWARD_Anim.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSquidwardFriend::postInit()
{
	m_conversation = SCRIPTS_CH1L1_01_DAT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSquidwardFriend::think( int _frames )
{
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
	else
	{
		if ( m_animNo != m_data[m_type].idleAnim )
		{
			m_animNo = m_data[m_type].idleAnim;
		}
		else
		{
			m_animNo = getRnd() % NUM_ANIM_SQUIDWARD;
		}

		m_animPlaying = true;
		m_frame = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSquidwardFriend::startConderversation()
{
	if( !CConversation::isActive() )
	{
		CConversation::trigger( m_conversation );

		if ( m_conversation == SCRIPTS_CH1L1_01_DAT )
		{
			m_conversation = SCRIPTS_CH1L1_02_DAT;
		}
	}
}