/*=========================================================================

	fpatrick.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FPATRICK_H__
#include "friend\fpatrick.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef	__ANIM_PATRICK_HEADER__
#include <ACTOR_PATRICK_Anim.h>
#endif


extern int s_globalLevelSelectThing;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPatrickFriend::postInit()
{
	switch( s_globalLevelSelectThing )
	{
		/*case 5:
		{
			m_conversation = SCRIPTS_CH2L1_01_DAT;
			break;
		}*/

		default:
		{
			m_conversation = SCRIPTS_CH2L2_01_DAT;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPatrickFriend::startConderversation()
{
	if ( m_data[this->m_type].canTalk )
	{
		if( !CConversation::isActive() )
		{
			CConversation::trigger( m_conversation );

			switch( s_globalLevelSelectThing )
			{
				/*case 5:
				{
					if ( m_conversation == SCRIPTS_CH2L1_01_DAT )
					{
						m_conversation = SCRIPTS_CH2L1_02_DAT;
					}

					break;
				}*/

				default:
				{
					if ( m_conversation == SCRIPTS_CH2L2_01_DAT )
					{
						m_conversation = SCRIPTS_CH2L2_02_DAT;
					}

					break;
				}
			}
		}
	}
}