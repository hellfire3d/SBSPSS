/*=========================================================================

	fdata.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef	__ANIM_BARNACLEBOY_HEADER__
#include <ACTOR_BARNACLEBOY_Anim.h>
#endif

#ifndef	__ANIM_KRUSTY_HEADER__
#include <ACTOR_KRUSTY_Anim.h>
#endif

#ifndef	__ANIM_SQUIDWARD_HEADER__
#include <ACTOR_SQUIDWARD_Anim.h>
#endif

#ifndef	__ANIM_SANDY_HEADER__
#include <ACTOR_SANDY_Anim.h>
#endif

#ifndef	__ANIM_PATRICK_HEADER__
#include <ACTOR_PATRICK_Anim.h>
#endif
/*
#ifndef	__ANIM_BARNACLEBOY_HEADER__
#include <ACTOR_BARNACLEBOY_Anim.h>
#endif

#ifndef	__ANIM_MERMAIDMAN_HEADER__
#include <ACTOR_MERMAIDMAN_Anim.h>
#endif
*/
#ifndef __ANIM_GARY_HEADER__
#include <ACTOR_GARY_Anim.h>
#endif


CNpcFriend::NPC_FRIEND_DATA CNpcFriend::m_data[NPC_FRIEND_UNIT_TYPE_MAX] =
{
	{	// NPC_FRIEND_BARNACLE_BOY
		ACTORS_BARNACLEBOY_SBK,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		0,//ANIM_BARNACLEBOY_IDLE,
	},

	{	// NPC_FRIEND_GARY
		ACTORS_GARY_SBK,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		0,
	},

	{	// NPC_FRIEND_KRUSTY
		ACTORS_KRUSTY_SBK,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		ANIM_KRUSTY_IDLEBREATHE,
	},

	{	// NPC_FRIEND_MERMAID_MAN
		ACTORS_MERMAIDMAN_SBK,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		0,//ANIM_MERMAIDMAN_IDLE,
	},

	{	// NPC_FRIEND_PATRICK
		ACTORS_PATRICK_SBK,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		0,
	},

	{	// NPC_FRIEND_SANDY_CHEEKS
		ACTORS_SANDY_SBK,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		0,
	},

	{	// NPC_FRIEND_SQUIDWARD
		ACTORS_SQUIDWARD_SBK,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		ANIM_SQUIDWARD_IDLEBREATHE,
	},
};

CNpcFriend::NPC_FRIEND_UNIT_TYPE CNpcFriend::mapEditConvertTable[NPC_FRIEND_UNIT_TYPE_MAX] =
{
	NPC_FRIEND_BARNACLE_BOY,
	NPC_FRIEND_GARY,
	NPC_FRIEND_KRUSTY,
	NPC_FRIEND_MERMAID_MAN,
	NPC_FRIEND_PATRICK,
	NPC_FRIEND_SANDY_CHEEKS,
	NPC_FRIEND_SQUIDWARD,
	NPC_FRIEND_PLANKTON,
};

