/*=========================================================================

	fpatrick.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FPATRICK_H__
#define __FRIEND_FPATRICK_H__

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

class CNpcPatrickFriend : public CNpcFriend
{
public:
	void				postInit();
protected:
	void				startConderversation();

	FileEquate			m_conversation;
};

#endif