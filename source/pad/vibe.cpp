/*=========================================================================

	vibe.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	
				
	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "pad\vibe.h"
#include "pad\pads.h"

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

s16		s_vibe_short[]=				{	 64,128,255,255,255,255,128, 64,-1	};
s16		s_vibe_medium[]=			{	 64,128,255,255,255,255,255,255,255,255,255,255,255,255, 64,128,-1		};
s16		s_vibe_long[]=				{	 64,128,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,128, 64,-1		};
s16		s_vibe_long_strobey[]=		{	 64, 64, 64, 64,128,128,128,128,255,255,255,255,128,128,128,128, 64, 64, 64, 64,128,128,128,128,255,255,255,255,128,128,128,128, 64, 64, 64, 64,128,128,128,128,255,255,255,255,128,128,128,128, 64, 64, 64, 64,-1		};
s16		s_vibe_coral_blower_suck[]=	{	 20, 40, 15, 50, 30, 40,-1		};
s16		s_vibe_chop[]=				{	255,255,255,200,200,200,-1		};

s16										*CPadVibrationManager::s_vibeData[VIBE_MAX_TYPES]=
{
	s_vibe_short,				// VIBE_SHORT
	s_vibe_medium,				// VIBE_MEDIUM
	s_vibe_long,				// VIBE_LONG
	s_vibe_long_strobey,		// VIBE_LONG_STROBEY
	s_vibe_coral_blower_suck,	// VIBE_CORAL_BLOWER_SUCK
	s_vibe_chop,				// VIBE_CHOP
};

CPadVibrationManager::VIBE_CONTROL		CPadVibrationManager::s_vibeControls[2][MAX_VIBES_PER_PAD];
u8										CPadVibrationManager::s_currentIntensityValues[2];


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPadVibrationManager::init()
{
	stopAllVibration();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPadVibrationManager::think(int _frames)
{
	u8				*intensity;
	int				tempIntensity;
	int				i,j,f;
	VIBE_CONTROL	*vc;

	intensity=s_currentIntensityValues;
	for(i=0;i<2;i++)
	{
		*intensity=0;
		tempIntensity=0;
		vc=s_vibeControls[i];
		for(j=0;j<MAX_VIBES_PER_PAD;j++)
		{
			if(vc->m_active)
			{
				int	amount=-1;
				for(f=0;f<_frames;f++)
				{
					if(*vc->m_vibeData==-1)
					{
						vc->m_active=false;
						amount=-1;
						break;
					}
					else
					{
						amount=*(vc->m_vibeData++);
					}
				}
				if(amount!=-1)
				{
					tempIntensity+=(amount*vc->m_ferocity)>>8;
				}
			}
			vc++;
		}

		if(tempIntensity>254)
		{
			tempIntensity=254;
		}
		*intensity=tempIntensity;
		intensity++;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPadVibrationManager::setVibration(int _port,VIBE_TYPE _type,u8 _ferocity)
{
	VIBE_CONTROL	*vc;
	int				i;
	int				foundSlot=false;

	// Look for a spare slot
	vc=s_vibeControls[_port];
	for(i=0;i<MAX_VIBES_PER_PAD;i++)
	{
		if(!vc->m_active)
		{
			foundSlot=true;
			break;
		}
		vc++;
	}

	// No more slots so re-use one
	if(!foundSlot)
	{
		// Could probly find a better way to choose a spare slot, but shouldn't need to..
		vc=&s_vibeControls[_port][getRndRange(MAX_VIBES_PER_PAD)];
	}

	// Fill in details
	vc->m_active=true;
	vc->m_vibeData=s_vibeData[_type];
	vc->m_ferocity=_ferocity;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPadVibrationManager::stopAllVibration()
{
	int		i,j;

	for(i=0;i<2;i++)
	{
		for(j=0;j<MAX_VIBES_PER_PAD;j++)
		{
			s_vibeControls[i][j].m_active=false;
		}
		s_currentIntensityValues[i]=0;
	}
	PadUpdate();	// Ensure that pads ARE stopped
}

/*===========================================================================
 end */