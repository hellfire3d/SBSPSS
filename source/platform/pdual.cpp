/*=========================================================================

	pdual.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PDUAL_H__
#include "platform\pdual.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setMaster( u8 isMaster )
{
	m_isMaster = isMaster;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setOtherPlatform( CNpcDualPlatform *other )
{
	m_otherPlatform = other;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	ASSERT( ThisPlatform->PointCount >= 3 );

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	// get master platform init pos

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	init( startPos );

	m_extension = 0;

	// get master platform max vertical extension

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_maxExtension = ( newYPos << 4 ) - startPos.vy;

	ASSERT( m_maxExtension >= 0 );

	// get slave platform init x pos

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR slavePos;
	slavePos.vx = newXPos << 4;
	slavePos.vy = startPos.vy + m_maxExtension;

	m_otherPlatform->init( slavePos );
	m_otherPlatform->postInit();

	if ( ThisPlatform->PointCount > 3 )
	{
		DVECTOR base;

		base.vx = (u16) *PntList;
		base.vx <<= 4;
		PntList++;
		base.vy = (u16) *PntList;
		base.vy <<= 4;
		PntList++;

		m_otherPlatform->setLineBase( base );

		base.vx = (u16) *PntList;
		base.vx <<= 4;
		PntList++;
		base.vy = (u16) *PntList;
		base.vy <<= 4;
		PntList++;

		setLineBase( base );
	}
	else
	{
		setLineBase( Pos );
		m_otherPlatform->setLineBase( slavePos );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 CNpcDualPlatform::canDrop()
{
	if ( m_isMaster )
	{
		s32 extensionLeft = m_maxExtension - m_extension;

		if ( extensionLeft == 0 )
		{
			return( false );
		}
		else
		{
			return( true );
		}
	}
	else
	{
		return( false );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::processMovement( int _frames )
{
	if ( m_isMaster )
	{
		s32 extensionChange = 0;
		s32 extensionLeft;

		if ( m_contact )
		{
			extensionLeft = m_maxExtension - m_extension;

			extensionChange = 3 * _frames;

			if ( extensionChange > extensionLeft )
			{
				extensionChange = extensionLeft;
			}
		}
		else
		{
			extensionLeft = m_extension;

			extensionChange = -3 * _frames;

			if ( -extensionChange > extensionLeft )
			{
				extensionChange = -extensionLeft;
			}
		}

		m_extension += extensionChange;

		Pos.vy = m_base.vy + m_extension;

		DVECTOR slaveMove;
		slaveMove.vx = 0;
		slaveMove.vy = -extensionChange;

		m_otherPlatform->setMovement( slaveMove );
		m_otherPlatform->think(_frames);
		m_otherPlatform->updateCollisionArea();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setMovement( DVECTOR move )
{
	Pos.vx += move.vx;
	Pos.vy += move.vy;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::render()
{
	int		x1,y1,x2,y2;

	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);

#if defined (__USER_paul__) || defined (__USER_charles__)
	DVECTOR size;
	DVECTOR	centre;
	int		halfLength;

	centre=getCollisionCentre();
	size=getCollisionSize();
	halfLength=size.vx>>1;

	x1=-halfLength*mcos(getCollisionAngle()&4095)>>12;
	y1=-halfLength*msin(getCollisionAngle()&4095)>>12;
	x2=+halfLength*mcos(getCollisionAngle()&4095)>>12;
	y2=+halfLength*msin(getCollisionAngle()&4095)>>12;

	centre.vx-=offset.vx;
	centre.vy-=offset.vy;
	x1+=centre.vx;
	y1+=centre.vy;
	x2+=centre.vx;
	y2+=centre.vy;

	DrawLine(x1,y1,x2,y2,0,255,0,0);
#endif
/*
		x1 = Pos.vx - offset.vx;
		x2 = m_lineBase.vx - offset.vx;

		if ( x1 > x2 )
		{
			int tempX = x1;
			x1 = x2;
			x2 = tempX;
		}

		y1 = Pos.vy - offset.vy;
		y2 = m_lineBase.vy - offset.vy;

		if ( y1 > y2 )
		{
			int tempY = y1;
			y1 = y2;
			y2 = tempY;
		}

		if ( y1 < 0 )
		{
			y1 = 0;
		}
		
		if ( y2 > VidGetScrH() )
		{
			y2 = VidGetScrH();
		}

		if ( x2 >= 0 && x1 <= VidGetScrW() )
		{
			if ( y2 >= 0 && y1 <= VidGetScrH() )
			{
				DrawLine( x1, y1, x2, y2, 0, 0, 0, 0 );
			}
		}

		if ( m_isMaster )
		{
			DVECTOR otherLineBase = m_otherPlatform->getLineBase();

			x1 = otherLineBase.vx - offset.vx;
			x2 = m_lineBase.vx - offset.vx;

			if ( x1 > x2 )
			{
				int tempX = x1;
				x1 = x2;
				x2 = tempX;
			}

			y1 = otherLineBase.vy - offset.vy;
			y2 = m_lineBase.vy - offset.vy;

			if ( y1 > y2 )
			{
				int tempY = y1;
				y1 = y2;
				y2 = tempY;
			}

			if ( x1 < 0 )
			{
				x1 = 0;
			}
			
			if ( x2 > VidGetScrW() )
			{
				x2 = VidGetScrW();
			}

			if ( x2 >= 0 && x1 <= VidGetScrW() )
			{
				if ( y2 >= 0 && y1 <= VidGetScrH() )
				{
					DrawLine( x1, y1, x2, y2, 0, 0, 0, 0 );
				}
			}
*/
		}
	}
}
