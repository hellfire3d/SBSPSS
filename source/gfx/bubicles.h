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
#ifdef __USER_paul__
#define SHOW_BUBICLE_EMITTERS
#endif



/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */


//
// An individual particle and it's associated data
//

typedef struct RGBPack
{
	unsigned char	m_r,m_g,m_b;
};

typedef struct
{
	int				m_life;
	int				m_vx,m_vdx,m_vxmax;
	int				m_vy,m_vdy,m_vymax;
	int				m_w,m_h;
	int				m_dvSizeChange;
	int				m_theta,m_vtheta;
	int				m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth;
	int				m_ot;
	RGBPack			m_colour;
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
	void		init(BubicleData *_init,int _x,int _y);

	void		think(int _frames);
	void		render();

	int			isActive()											{return m_active;}


	int			m_active;

	int			m_x;
	int			m_y;
	int			m_typeSizeChange;		// 0=Width, 1=Height
	int			m_vSizeChange;
	int			m_frameCount;
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
	int			m_x,m_y,m_w,m_h;
	int			m_birthRate,m_birthAmount;
	int			m_life;							// -1 for infinite life

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
	

	int					m_active;
	BubicleEmitterData	m_data;
	int					m_frameCount;
	int					m_spawnFrameCount;

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
	static void	think();
	static void	render();
	
	static CBubicleEmitter	*spawnEmitter(BubicleEmitterData *_init);
	static CBubicle			*spawnParticle(BubicleEmitterData *_init);

	static SpriteBank		*getSprites()							{return s_sprites;}


private:
	enum
	{
		NUM_EMITTERS=10,
		NUM_BUBICLES=200,
	};

							CBubicleFactory()						{;}

	static int				s_initialised;
	static CBubicleEmitter	*s_emitters;
	static CBubicle			*s_bubicles;
	static SpriteBank		*s_sprites;

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
