/*=========================================================================

	gamebubs.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "game\gamebubs.h"


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

BubicleEmitterData CGameBubicleFactory::s_emitters[CGameBubicleFactory::NUM_TYPES]=
{
	// TYPE_SMALL
	{
		0,0,0,0,					// m_x,m_y,m_w,m_h
		2,3,						// m_birthRate,m_birthAmount
		5,							// m_life
		true,						// m_applyMapOffsets
		{							// m_bubicleBase
			10,							// m_life
			0,0,0,						// m_vx,m_vdx,m_vxmax
			-100,0,0,					// m_vy,m_vdy,m_vymax
			5,3,						// m_w,m_h
			0,							// m_dvSizeChange
			0,0,						// m_theta,m_vtheta
			0,0,0,						// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
			0,							// m_ot
			{200,200,200}				// m_colour
		},
		{							// m_bubicleRange
			50,							// m_life
			0,0,0,						// m_vx,m_vdx,m_vxmax
			50,0,0,						// m_vy,m_vdy,m_vymax
			3,3,						// m_w,m_h
			0,							// m_dvSizeChange
			0,100,						// m_theta,m_vtheta
			10,0,0,						// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
			0,							// m_ot
			{0,0,0}						// m_colour
		}
	},
	// TYPE_MEDIUM
	{
		0,0,0,0,					// m_x,m_y,m_w,m_h
		2,10,						// m_birthRate,m_birthAmount
		5,							// m_life
		true,						// m_applyMapOffsets
		{							// m_bubicleBase
			10,							// m_life
			0,0,0,						// m_vx,m_vdx,m_vxmax
			-100,0,0,					// m_vy,m_vdy,m_vymax
			5,3,						// m_w,m_h
			0,							// m_dvSizeChange
			0,0,						// m_theta,m_vtheta
			0,0,0,						// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
			0,							// m_ot
			{200,200,200}				// m_colour
		},
		{							// m_bubicleRange
			50,							// m_life
			0,0,0,						// m_vx,m_vdx,m_vxmax
			50,0,0,						// m_vy,m_vdy,m_vymax
			3,3,						// m_w,m_h
			0,							// m_dvSizeChange
			0,100,						// m_theta,m_vtheta
			10,0,0,						// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
			0,							// m_ot
			{0,0,0}						// m_colour
		}
	},
	// TYPE_LARGE
	{
		0,0,0,0,					// m_x,m_y,m_w,m_h
		1,10,						// m_birthRate,m_birthAmount
		15,							// m_life
		true,						// m_applyMapOffsets
		{							// m_bubicleBase
			10,							// m_life
			0,0,0,						// m_vx,m_vdx,m_vxmax
			-100,0,0,					// m_vy,m_vdy,m_vymax
			5,3,						// m_w,m_h
			0,							// m_dvSizeChange
			0,0,						// m_theta,m_vtheta
			0,0,0,						// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
			0,							// m_ot
			{200,200,200}				// m_colour
		},
		{							// m_bubicleRange
			50,							// m_life
			0,0,0,						// m_vx,m_vdx,m_vxmax
			50,0,0,						// m_vy,m_vdy,m_vymax
			3,3,						// m_w,m_h
			0,							// m_dvSizeChange
			0,100,						// m_theta,m_vtheta
			10,0,0,						// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
			0,							// m_ot
			{0,0,0}						// m_colour
		}
	},
	// TYPE_SPONGEBOBSOAKUP
	{
		0,0,0,0,					// m_x,m_y,m_w,m_h
		1,1,						// m_birthRate,m_birthAmount
		15,							// m_life
		true,						// m_applyMapOffsets
		{							// m_bubicleBase
			10,							// m_life
			0,0,0,						// m_vx,m_vdx,m_vxmax
			-100,0,0,					// m_vy,m_vdy,m_vymax
			10,7,						// m_w,m_h
			0,							// m_dvSizeChange
			0,0,						// m_theta,m_vtheta
			0,0,0,						// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
			0,							// m_ot
			{200,200,200}				// m_colour
		},
		{							// m_bubicleRange
			50,							// m_life
			0,0,0,						// m_vx,m_vdx,m_vxmax
			50,0,0,						// m_vy,m_vdy,m_vymax
			3,3,						// m_w,m_h
			0,							// m_dvSizeChange
			0,100,						// m_theta,m_vtheta
			10,0,0,						// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
			0,							// m_ot
			{0,0,0}						// m_colour
		}
	},
};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGameBubicleFactory::spawnBubicles(int _x,int _y,int _w,int _h,GAMEBUBICLETYPE _type)
{
	BubicleEmitterData	*emt;

	emt=&s_emitters[_type];

	emt->m_x=_x;
	emt->m_y=_y;
	emt->m_w=_w;
	emt->m_h=_h;

	CBubicleFactory::spawnEmitter(emt);
}


/*===========================================================================
 end */
