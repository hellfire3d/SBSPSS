/*=========================================================================

	hboat.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HBOAT_H__
#define __HAZARD_HBOAT_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcBoatHazard : public CNpcHazard
{
public:
	void				init();
	void				render();
protected:
	void				processMovement( int _frames );
	void				collidedWith(CThing *_thisThing);

	u8					m_reversed;
};

#endif