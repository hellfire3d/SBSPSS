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
	virtual void		postInit();
	virtual void		think( int _frames );
	virtual void		setupWaypoints( sThingActor *ThisActor );
	DVECTOR const		&getTriggerPos()										{return( m_triggerPos );}
	void				start()													{m_started = true;}
	void				stop()													{m_started = false;}
	void				startLeft();
	void				startRight();
	virtual void		render();

protected:
	DVECTOR				m_triggerPos;
	u8					m_started;
};

#endif