/*=========================================================================

	nboss.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NBOSS_H__
#include "enemy\nboss.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBossEnemy::postInit()
{
	m_meterOn=false;
	m_energyBar = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBossEnemy::shutdown()
{
	if ( m_energyBar )
	{
		m_energyBar->setToShutdown();
	}

	CNpcEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBossEnemy::addHealthMeter()
{
	if (!m_meterOn)
	{
		m_energyBar=(CFXNRGBar*)CFX::Create(CFX::FX_TYPE_NRG_BAR,this);
		m_energyBar->SetMax(m_data[m_type].initHealth);
		m_meterOn=true;
	}
}

