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

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
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
		NPC_FRIEND_PLANKTON,
		NPC_FRIEND_UNIT_TYPE_MAX,
	};
	enum
	{ // For Dynamic ThingCache
		MAX_SUBTYPE	=NPC_FRIEND_UNIT_TYPE_MAX,
	};

	virtual void		init();
	void				init( DVECTOR initPos );
	virtual void		postInit();
	virtual void		shutdown();
	virtual void		think(int _frames);
	virtual void		render();


	void				setType( NPC_FRIEND_UNIT_TYPE newType )					{m_type = newType;}
	void				setPlatform( CNpcPlatform *platform )					{m_platform = platform;}
	void				clearPlatform()											{m_platform = NULL;}
	virtual void		setupWaypoints( sThingActor *ThisActor );
	virtual void		setObstructed()											{}

	static CNpcFriend	*Create(sThingActor *ThisActor);
	static NPC_FRIEND_UNIT_TYPE		getTypeFromMapEdit( u16 newType );

protected:
	typedef struct NPC_FRIEND_DATA_TYPE
	{
		int								skelType;
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

	virtual void		startConderversation();

	virtual void		collidedWith(CThing *_thisThing);

	enum
	{
		EXTEND_RIGHT = true,
		EXTEND_LEFT = false,

		NPC_FRIEND_MAPEDIT_OFFSET = 1,
	};

	// internal variables

	NPC_FRIEND_UNIT_TYPE		m_type;
	s32							m_extension;
	CNpcPlatform				*m_platform;

	int				m_frame;
	int				m_animNo;
	u8				m_animPlaying;
	CActorGfx		*m_actorGfx;
	bool			m_reversed;

	int				m_hasSpokenToSbYet;
	int				m_iconStrobe;
};

#endif