/*=========================================================================

	pclam.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCLAM_H__
#define __PLATFORM_PCLAM_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __ENEMY_NCLAM_H__
#include "enemy\nclam.h"
#endif

class CNpcClamPlatform : public CNpcPlatform
{
public:
	void				postInit();
	void				render();
	CRECT const			*getThinkBBox()									{return( CThing::getThinkBBox() );}
	void				setClam( CNpcStaticClamEnemy *newClam)			{m_clam=newClam;}
protected:
	void				collidedWith(CThing *_thisThing);

	CNpcStaticClamEnemy	*m_clam;
};

#endif