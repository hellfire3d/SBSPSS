/*=========================================================================

	hrrock.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HRROCK_H__
#define __HAZARD_HRROCK_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcRollingRockHazard : public CNpcHazard
{
public:
	void				init();
	void				render();
protected:
	void				processMovement( int _frames );

	s16					m_rotation;
	u8					m_jump;
	s32					m_jumpVel;
};

#endif