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

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */


typedef struct RGBPack
{
	unsigned char	m_r,m_g,m_b;
};


// particle
typedef struct
{
	int			m_life;
	int			m_vx,m_vdx,m_vxmax;
	int			m_vy,m_vdy,m_vymax;
	int			m_w,m_h;
	int			m_dvSizeChange;
	int			m_theta,m_vtheta;
	int			m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth;
	int			m_ot;
	RGBPack		m_colour;
} BubicleData;


class CBubicle
{
public:
	enum
	{
		ACCURACY_SHIFT=6,
	};


	void		init(BubicleData *_init,int _x,int _y);

	void		think();
	void		render();

	int			isDead();

private:
	int			m_x;
	int			m_y;
	int			m_typeSizeChange;		// 0=Width, 1=Height
	int			m_vSizeChange;
	int			m_frameCount;
	void		*m_fhBub;				// FFS! Go on.. try and make this an sFrameHdr *

	BubicleData	m_data;


};


// emitter
typedef struct
{
	int				m_x,m_y,m_w,m_h;
	int				m_birthRate,m_birthAmount;

	BubicleData		m_bubicleBase;
	BubicleData		m_bubicleRange;
} BubicleEmitterData;

class CBubicleEmitter
{
public:
	enum{NUM_BUB=400};

	void	init(BubicleEmitterData *_init);

	void	think();
	void	render();

	void	setPos(int _x,int _y);
	void	setSize(int _w,int _h);
	void	setPosAndSize(int _x,int _y,int _w,int _h)				{setPos(_x,_y);setSize(_w,_h);}


private:

	static CBubicle	*createNewBubicle(BubicleEmitterData *_init);
//	static void		removeBubicle();


	BubicleEmitterData	m_data;
	int			m_x,m_y,m_w,m_h;
	int			m_birthRate;
	int			m_frameCount;

	CBubicle	*m_bubicles[NUM_BUB];
//	static int		s_numBubble
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
