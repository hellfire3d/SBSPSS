/*=========================================================================

	hcheck.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HCHECK_H__
#define __HAZARD_HCHECK_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif


class CNpcCheckpointHazard : public CNpcHazard
{
public:
	void				init();
	void				render();
	void				think(int _frames);
	void				shutdown();
	bool				alwaysThink()											{return(m_flick);}
protected:
	void				collidedWith(CThing *_thisThing);

	u8					m_triggered;
	int					m_spriteFrame;
	int					m_timer;
	u8					m_flick;
	class ScalableFontBank			*m_scalableFont;
};

#endif