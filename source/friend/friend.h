/*=========================================================================

	friend.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRIEND_FRIEND_H__
#define	__FRIEND_FRIEND_H__

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

class	CNpcFriend : public CNpcThing
{
public:
	enum NPC_FRIEND_UNIT_TYPE
	{
		NPC_FRIEND_BARNACLE_BOY = 0,
		NPC_FRIEND_GARY = 1,
		NPC_FRIEND_KRUSTY,
		NPC_FRIEND_MERMAID_MAN,
		NPC_FRIEND_PATRICK,
		NPC_FRIEND_SANDY_CHEEKS,
		NPC_FRIEND_SQUIDWARD,
		NPC_FRIEND_UNIT_TYPE_MAX,
	};

	void				init();
	void				init( DVECTOR initPos );
	void				shutdown();
	virtual void		think(int _frames);
	void				render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );
	void				setLayerCollision( class CLayerCollision *_layer )		{m_layerCollision=_layer;}
	void				setType( NPC_FRIEND_UNIT_TYPE newType )					{m_type = newType;}

	static CNpcFriend	*Create(sThingActor *ThisActor);
	static NPC_FRIEND_UNIT_TYPE		getTypeFromMapEdit( u16 newType );

protected:
	class CLayerCollision	*m_layerCollision;

protected:
	typedef struct NPC_FRIEND_DATA_TYPE
	{
		int								skelType;
		bool							canTalk;
		u8								speed;
		u16								turnSpeed;
		bool							detectCollision;
		DAMAGE_TYPE						damageToUserType;
		u16								idleAnim;
	}
	NPC_FRIEND_DATA;

	// gary functions

	void				processGaryMovement( int _frames );

	// data

	static NPC_FRIEND_DATA		m_data[NPC_FRIEND_UNIT_TYPE_MAX];

	static NPC_FRIEND_UNIT_TYPE mapEditConvertTable[NPC_FRIEND_UNIT_TYPE_MAX];

	enum
	{
		EXTEND_RIGHT = true,
		EXTEND_LEFT = false,

		NPC_FRIEND_MAPEDIT_OFFSET = 1,
	};

	// internal variables
	
	NPC_FRIEND_UNIT_TYPE		m_type;
	s32							m_extension;

	int				m_frame;
	int				m_animNo;
	u8				m_animPlaying;
	CActorGfx		*m_actorGfx;
	DVECTOR			m_drawOffset;
	bool			m_reversed;
};

#endif