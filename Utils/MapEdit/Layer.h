/******************/
/*** Layer Core ***/
/******************/

#ifndef	__LAYER_HEADER__
#define	__LAYER_HEADER__

#include	<Vector>
#include	"gl3d.h"
#include	"Map.h"

/*****************************************************************************/
enum	LAYER_ENUMS
{
SELECT_BUFFER_SIZE=16,
};

/*****************************************************************************/
enum LAYER_TYPE
{
	LAYER_TYPE_BACK=0,
	LAYER_TYPE_MID,
	LAYER_TYPE_ACTION,
	LAYER_TYPE_FORE,

	LAYER_TYPE_MAX
};


/*****************************************************************************/
class	CCore;
class	CMapEditView;
class	CLayer
{
public:
		CLayer(){};
virtual	~CLayer(){};

virtual	char			*GetName();
virtual	void			SetName(char *_Name);

virtual	float			GetLayerZPosDiv()		{return(ZPosDiv);}

virtual	void			Render(CCore *Core,Vec &MapPos,BOOL Is3d)=0;
virtual	void			RenderGrid(CCore *Core,Vec &MapPos)=0;

virtual	void			FindCursorPos(CCore *Core,CMapEditView *View,Vec &MapPos,CPoint &MousePos)=0;

protected:
		char			Name[256];
		BOOL			Render3dFlag;
		float			ZPosDiv;
};


/*****************************************************************************/
#endif
