/*=========================================================================

	fkrusty.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FKRUSTY_H__
#include "friend\fkrusty.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef	__ANIM_KRUSTY_HEADER__
#include <ACTOR_KRUSTY_Anim.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcKrustyFriend::postInit()
{
	m_conversation = SCRIPTS_CH1L4_01_DAT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcKrustyFriend::startConderversation()
{
	if ( m_data[this->m_type].canTalk )
	{
		if( !CConversation::isActive() )
		{
			CConversation::trigger( m_conversation );

			if ( m_conversation == SCRIPTS_CH1L4_01_DAT )
			{
				m_conversation = SCRIPTS_CH1L4_02_DAT;
			}
		}
	}
}