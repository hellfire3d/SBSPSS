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

virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();
		void		SetTile(int T)		{Tile=T;}
protected:

		u16			Tile;
		u16			Life;
		DVECTOR		Velocity;

		sTile3d		*TileBank3d;
		sTri		*TriList;
		sQuad		*QuadList;
		sVtx		*VtxList;

};

#endif
