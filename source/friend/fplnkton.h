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
	void				init();
	void				render();
	void				think(int _frames);
	void				shutdown();
protected:
	void				startConderversation();
};

#endif