/*=========================================================================

	fsandy.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FSANDY_H__
#include "friend\fsandy.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef	__ANIM_SANDY_HEADER__
#include <ACTOR_SANDY_Anim.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSandyFriend::startConderversation()
{
	if ( m_data[this->m_type].canTalk )
	{
		if( !CConversation::isActive() )
		{
			CConversation::trigger( SCRIPTS_CH1L2_03_DAT );
		}
	}
}