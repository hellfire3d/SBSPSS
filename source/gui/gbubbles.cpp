/*=========================================================================

	gbubbles.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gui\gbubbles.h"


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

BubicleEmitterData CGUIBubbles::s_emitterData=
{
	200,200,312,25,		// m_x,m_y,m_w,m_h
	4,1,				// m_birthRate,m_birthAmount
	-1,					// m_life
	false,				// m_applyMapOffsets
	{					// m_bubicleBase
		100,				// m_life
		0,0,0,				// m_vx,m_vdx,m_vxmax
		-40,-15,-100,		// m_vy,m_vdy,m_vymax
		5,3,				// m_w,m_h
		10,					// m_dvSizeChange
		0,100,				// m_theta,m_vtheta
		100,2,0,			// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
		40,					// m_ot
		{ 127,127,127,	}	// m_colour
	},
	{					// m_bubicleRange
		100,				// m_life
		0,0,0,				// m_vx,m_vdx,m_vxmax
		20,10,0,			// m_vy,m_vdy,m_vymax
		3,2,				// m_w,m_h
		10,					// m_dvSizeChange
		4095,50,			// m_theta,m_vtheta
		100,5,0,			// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
		0,					// m_ot
		{   0,  0,  0,	}	// m_colour
	}
};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBubbles::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObject::init(_parent,_id);
	m_emitter=NULL;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBubbles::setFlags(GUI_FLAGS _flags)
{
	CGUIObject::setFlags(_flags);
	if(_flags&FLAG_SELECTED)
	{
		startEmitter();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBubbles::clearFlags(GUI_FLAGS _flags)
{
	CGUIObject::clearFlags(_flags);
	if(_flags&FLAG_SELECTED)
	{
		stopEmitter();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBubbles::startEmitter()
{
	if(!m_emitter)
	{
		s_emitterData.m_x=getParentX()+getX();
		s_emitterData.m_y=getParentY()+getY();
		s_emitterData.m_w=getW();
		s_emitterData.m_h=getH();
		s_emitterData.m_bubicleBase.m_ot=getOt()+1;
		m_emitter=CBubicleFactory::spawnEmitter(&s_emitterData);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBubbles::stopEmitter()
{
	m_emitter->kill();
	m_emitter=NULL;
}


/*===========================================================================
 end */