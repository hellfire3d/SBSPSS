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

CNpcFriend::NPC_FRIEND_DATA CNpcFriend::m_data[NPC_FRIEND_UNIT_TYPE_MAX] =
{
	{	// NPC_FRIEND_BARNACLE_BOY
		ACTORS_CLAM_SBK,
		NPC_FRIEND_MOVEMENT_STATIC,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
	},

	{	// NPC_FRIEND_GARY
		ACTORS_CLAM_SBK,
		NPC_FRIEND_MOVEMENT_STATIC,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
	},

	{	// NPC_FRIEND_KRUSTY
		ACTORS_CLAM_SBK,
		NPC_FRIEND_MOVEMENT_STATIC,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
	},

	{	// NPC_FRIEND_MERMAID_MAN
		ACTORS_CLAM_SBK,
		NPC_FRIEND_MOVEMENT_STATIC,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
	},

	{	// NPC_FRIEND_PATRICK
		ACTORS_CLAM_SBK,
		NPC_FRIEND_MOVEMENT_STATIC,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
	},

	{	// NPC_FRIEND_SANDY_CHEEKS
		ACTORS_CLAM_SBK,
		NPC_FRIEND_MOVEMENT_STATIC,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
	},

	{	// NPC_FRIEND_SQUIDWARD
		ACTORS_SQUIDWARD_SBK,
		NPC_FRIEND_MOVEMENT_STATIC,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
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
};

