/********************/
/*** Falling Tile ***/
/********************/

#ifndef	__FX_FX_FALLING_TILE_HEADER__
#define __FX_FX_FALLING_TILE_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXFallingTile : public CFX
{
public:

/*virtual */void		init(DVECTOR const &Pos);
/*virtual */void		think(int _frames);
/*virtual */void		render();
		void		SetTile(int T)		{Tile=T;}
protected:

		u16			Tile;
		DVECTOR		Velocity;

		sElem3d		*ElemBank3d;
		sTri		*TriList;
		sQuad		*QuadList;
		sVtx		*VtxList;

};

#endif
