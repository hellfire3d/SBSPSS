/******************/
/*** Layer Core ***/
/******************/

#ifndef	__LAYER_HEADER__
#define	__LAYER_HEADER__

#include	<Vector>
#include	"gl3d.h"
#include	"Map.h"

enum LAYER_TYPE
{
	LAYER_TYPE_BACK=0,
	LAYER_TYPE_MID,
	LAYER_TYPE_ACTION,
	LAYER_TYPE_FORE,

	LAYER_TYPE_MAX
};

/*
struct	sMapElem
{
	int		Bank;
	int		Tile;
	int		Flags;
};
*/

/*****************************************************************************/
class	CCore;
class	CMap;
class	CLayer
{
public:
		CLayer(){ASSERT(1);}
		CLayer(CCore *_Core);
		~CLayer();

		void	InitLayer(CCore *_Core);

// Virtual
virtual	void			Init()=0;
virtual	char			*GetName()=0;
virtual	void			Render(Vec &MapPos,BOOL Is3d);
virtual	void			Render2d(Vec &MapPos);
virtual	void			Render3d(Vec &MapPos);

virtual	float			GetLayerZPosDiv()=0;
virtual	BOOL			CanRender3d()=0;
virtual	void			SetTestColor()=0;

// Control


protected:
//		float			Width,Height;
		CCore			*Core;
//		std::vector< std::vector<sMapElem> > Map;
//		sMapElem		*Map;
		CMap			Map;

};


/*****************************************************************************/
#endif
