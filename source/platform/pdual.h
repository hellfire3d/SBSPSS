/*=========================================================================

	pdual.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PDUAL_H__
#define __PLATFORM_PDUAL_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcDualPlatform : public CNpcPlatform
{
public:
	void				setMaster( u8 isMaster );
	void				setOtherPlatform( CNpcDualPlatform *other );
	void				setMovement( DVECTOR move );
	virtual u8			canDrop();
	void				setLineBase( DVECTOR base )							{m_lineBase = base;}
	DVECTOR				getLineBase()										{return( m_lineBase );}
	virtual void		render();
protected:
	virtual void		setWaypoints( sThingPlatform *ThisPlatform );
	virtual void		processMovement( int _frames );

	u8					m_isMaster;
	CNpcDualPlatform	*m_otherPlatform;
	s32					m_maxExtension;
	s32					m_extension;
	DVECTOR				m_lineBase;
};

#endif