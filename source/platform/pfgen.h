/*=========================================================================

	pfgen.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFGEN_H__
#define __PLATFORM_PFGEN_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcFallingPlatformGenerator : public CNpcPlatform
{
public:
	void				setTargetType( NPC_PLATFORM_UNIT_TYPE targetType )		{m_targetType = targetType;}
	virtual void		render();
	virtual	CRECT const		*getThinkBBox();
protected:
	virtual void		think( int _frames );
	virtual void		collidedWith(CThing *_thisThing);

	NPC_PLATFORM_UNIT_TYPE	m_targetType;
};

#endif