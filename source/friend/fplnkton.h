/*=========================================================================

	fplnkton.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FPLNKTON_H__
#define __FRIEND_FPLNKTON_H__

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

class CNpcPlanktonFriend : public CNpcFriend
{
public:
	virtual void		init();
	virtual void		render();
	virtual void		think(int _frames);
	virtual void		shutdown();
protected:
	virtual void		startConderversation();
	SpriteBank		*m_spriteBank;
};

#endif