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

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
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
	switch( CLevel::getCurrentChapter() )
	{
		case 3:
		{
			m_conversation = SCRIPTS_CH4L3_01_DAT;
			break;
		}

		case 2:
		{
			m_conversation = SCRIPTS_CH2L1_01_DAT;
			break;
		}

		default:
		{
			m_conversation = SCRIPTS_CH1L4_01_DAT;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcKrustyFriend::startConderversation()
{
	if( !CConversation::isActive() )
	{
		CConversation::trigger( m_conversation );

		switch( CLevel::getCurrentChapter() )
		{
			case 3:
			{
				if ( m_conversation == SCRIPTS_CH4L3_01_DAT )
				{
					m_conversation = SCRIPTS_CH4L3_02_DAT;
				}

				break;
			}

			case 2:
			{
				if ( m_conversation == SCRIPTS_CH2L1_01_DAT )
				{
					m_conversation = SCRIPTS_CH2L1_02_DAT;
				}

				break;
			}

			default:
			{
				if ( m_conversation == SCRIPTS_CH1L4_01_DAT )
				{
					m_conversation = SCRIPTS_CH1L4_02_DAT;
				}

				break;
			}
		}
	}
}