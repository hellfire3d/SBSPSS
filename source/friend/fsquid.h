/*=========================================================================

	fsquid.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FSQUID_H__
#define __FRIEND_FSQUID_H__

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

class CNpcSquidwardFriend : public CNpcFriend
{
public:
	void				think(int _frames);
	void				postInit();
protected:
	void				startConderversation();

	FileEquate			m_conversation;
};

#endif