/**************************/
/*** Emitter Base Class ***/
/**************************/

#ifndef	__FX_FX_EMITTER_HEADER__
#define __FX_FX_EMITTER_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXEmitter : public CFX
{
public:

virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();

virtual	void		setUp(int W,int H,int Rate,int Amount,int Life);

virtual	void		create(int _frames);

		void		SetSize(int W,int H)			{Width=W; Height=H;}
		void		SetRate(int Rate,int Amount)	{BirthRate=Rate; BirthAmount=Amount;}
		void		SetLife(int L)					{Life=L;}

protected:
		s16			Width,Height;
		s16			BirthRate,BirthAmount;
		s16			BirthCount;
		s16			Life;
		bool		MapEmitter;
};

#endif
/*
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
*/