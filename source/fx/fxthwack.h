/***********************/
/*** Anim Base Class ***/
/***********************/

#ifndef	__FX_FX_ATTACH_ANIM_HEADER__
#define __FX_FX_ATTACH_ANIM_HEADER__

#include "fx/fx.h"
#include "fx/fxbaseAnim.h"

/*****************************************************************************/
class CFXAttachAnim : public CFXBaseAnim
{
public:
virtual void		init(DVECTOR const &Pos);
virtual void		render();

protected:

};

#endif
