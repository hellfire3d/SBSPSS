/**************************/
/*** Emitter Base Class ***/
/**************************/

#ifndef	__FX_FX_BASE_EMITTER_HEADER__
#define __FX_FX_BASE_EMITTER_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXBaseEmitter : public CFX
{
public:
		struct sFXData
		{
			u16				Rate,Amount;
			u16				Life;
			CFX::FX_TYPE	ParticleType;
		};

virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();
		void		setData(void *Data)				{DataPtr=(sFXData*)Data;}

virtual	void		setSize(int W,int H)			{Width=W; Height=H;}
virtual	void		createParticle(int _frames);


protected:
		sFXData		*DataPtr;
		s16			Width,Height;
		s16			CurrentCount;
};

#endif
