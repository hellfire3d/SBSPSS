/**************************/
/*** Emitter Base Class ***/
/**************************/

#ifndef	__FX_FX_EMITTER_HEADER__
#define __FX_FX_EMITTER_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXParticle;
class CFXEmitter : public CFX
{
public:

virtual void		init();
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();

protected:

		CFXParticle	*ParticleList;
};

/*****************************************************************************/
class CFXParticle : public CFX
{
public:
virtual void		init();
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();

protected:
};
#endif
