/*=========================================================================

	bubicles.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GFX_BUBICLES_H__
#define	__GFX_BUBICLES_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

// Uncomment to see the emitters on screen
//#define SHOW_BUBICLE_EMITTERS



/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */


//
// An individual particle and it's associated data
//

typedef struct RGBPack
{
	u8				m_r,m_g,m_b;
};

typedef struct
{
	s16				m_life;						// Frames to live for
	s16				m_vx,m_vdx,m_vxmax;			// Velocity, velocitydelta, max velocity
	s16				m_vy,m_vdy,m_vymax;
	s16				m_w,m_h;					// Size of bubicle
	s16				m_dvSizeChange;				// Speed at which bubbles changes height/width
	s16				m_theta,m_vtheta;
	s16				m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth;
	s16				m_ot;						// ot
	RGBPack			m_colour;					// Colour
} BubicleData;

class CBubicle
{
// We don't really want anyone to instantiate this themselves.. only the factory is allowed to
private:
	enum
	{
		ACCURACY_SHIFT=6,
	};

				CBubicle()											{;}

	void		create();
	void		init(BubicleData *_init,int _x,int _y,int _applyMapOffset);

	void		think(int _frames);
	void		render();

	int			isActive()											{return m_active;}


	u8			m_active;
	int			m_x;
	int			m_y;
	s16			m_typeSizeChange;				// 0=Width, 1=Height
	s16			m_vSizeChange;
	s16			m_frameCount;
	s16			m_applyMapOffset;
	sFrameHdr	*m_fhBub;

	BubicleData	m_data;

friend class CBubicleFactory;
friend class CBubicleEmitter;
};



//
// An emitter
//

typedef struct
{
	int			m_x,m_y,m_w,m_h;				// Size and position of emitter
	s16			m_birthRate,m_birthAmount;		// birthAmount bubicles born every birthRate frames
	s16			m_life;							// -1 for infinite life
	s16			m_applyMapOffset;				// Is bubicle position relative to the map or not?

	BubicleData	m_bubicleBase;
	BubicleData	m_bubicleRange;
} BubicleEmitterData;

class CBubicleEmitter
{
public:
	void		kill();
	void		setPos(int _x,int _y);
	void		setSize(int _w,int _h);
	void		setPosAndSize(int _x,int _y,int _w,int _h)			{setPos(_x,_y);setSize(_w,_h);}

	int			isActive()											{return m_active;}


private:
				CBubicleEmitter()									{;}
				
	void		create();
	void		init(BubicleEmitterData *_init);
	
	void		think(int _frames);
#ifdef SHOW_BUBICLE_EMITTERS
	void		render();
#endif
	

	u8					m_active;
	BubicleEmitterData	m_data;
	int					m_frameCount;
	s16					m_spawnFrameCount;

friend class CBubicleFactory;
};



//
// Particle factory
//

class CBubicleFactory
{
public:
	static void	init();
	static void	shutdown();
	static void	think(int _frames);
	static void	render();
	
	static CBubicleEmitter	*spawnEmitter(BubicleEmitterData *_init);
	static CBubicle			*spawnParticle(BubicleEmitterData *_init);
	static sFrameHdr		*getBubbleFrameHeader();

	static void				setMapOffset(DVECTOR *_offset);
	static const DVECTOR	*getMapOffset();


private:
	enum
	{
		NUM_EMITTERS=3,
		NUM_BUBICLES=50,
	};

							CBubicleFactory()						{;}

	static int				s_initialised;
	static CBubicleEmitter	*s_emitters;
	static CBubicle			*s_bubicles;

	static int				s_frameTypeCounter;
	static const int		s_frameTabSize;
	static const int		s_frameTabSizeMask;
	static sFrameHdr		*s_frameTab[];
	static const int		s_frameTabSrc[];

	static DVECTOR			s_mapPositionOffset;

};



/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GFX_BUBICLES_H__ */

/*===========================================================================
 end */
