/*=========================================================================

	fgary.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FGARY_H__
#define __FRIEND_FGARY_H__

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

class CNpcGaryFriend : public CNpcFriend
{
public:
	void				postInit();
	void				shutdown();
	void				think( int _frames );
	void				setupWaypoints( sThingActor *ThisActor );
	bool				alwaysThink()											{return(true);}
	DVECTOR const		&getTriggerPos()										{return( m_triggerPos );}
	void				start();
	void				stop( int xPos );
	void				startLeft();
	void				startRight();
	void				render();
	void				setObstructed()											{m_obstructed = true;}
	static void			setReachedDoor()										{m_hasReachedDoor = true;}
	static void			clearReachedDoor()										{m_hasReachedDoor = false;}
	static bool			hasReachedDoor()										{return( m_hasReachedDoor );}

protected:
	DVECTOR				m_triggerPos;
	u8					m_started;
	u8					m_startMoving;
	u8					m_fallDeath;
	s16					m_speed;
	s16					m_drawRotation;
	int					m_soundId;
	u8					m_obstructed;
	u8					m_garySB;
	u8					m_garyMeow;
	static u8			m_garySpeech;
	static u8			m_hasReachedDoor;
	u8					m_isStopping;
	int					m_xStopPos;
};

#endif