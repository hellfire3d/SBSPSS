/******************/
/*** Layer Core ***/
/******************/

#ifndef	__LAYER_HEADER__
#define	__LAYER_HEADER__

#include	<Vector>
#include	"gl3d.h"

enum LAYER_TYPE
{
	LAYER_TYPE_BACK=0,
	LAYER_TYPE_MID,
	LAYER_TYPE_ACTION,
	LAYER_TYPE_FORE,

	LAYER_TYPE_MAX
};



/*****************************************************************************/
class	CLayer
{
public:
		CLayer();
		~CLayer();


// Virtual
virtual	void			Init()=0;
virtual	char			*GetName()=0;
virtual	void			Render(Vec &MapPos,BOOL Is3d);
virtual	void			Render2d(Vec &MapPos);
virtual	void			Render3d(Vec &MapPos);

virtual	float			GetLayerZPosDiv()=0;
virtual	float			GetLayerZPos()=0;
virtual	BOOL			CanRender3d()=0;
virtual	void			SetTestColor()=0;

// Control


protected:
		float			LayerWidth,LayerHeight;
};

/*****************************************************************************/
#endif
