/*********************/
/*** TileSet Stuph ***/
/*********************/

#ifndef	__TILESET_HEADER__
#define	__TILESET_HEADER__

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<Vector>
#include	<gfname.hpp>

#include	"Layer.h"
#include	"TexCache.h"
#include	"Elem.h"

#include	"MapEdit.h"
#include	"GUITilebank.h"
#include	"GUIElemList.h"

/*****************************************************************************/

/*****************************************************************************/
class	CCore;
class	CTileBank : public CElemBank
{
public:
		CTileBank();
		~CTileBank();

		enum	BrushEnum
		{
			LBrush=0,
			RBrush,
			MaxBrush
		};
// Overloads
		int			GetType()							{return(0);}

		void		Render(CCore *Core,Vector3 &CamPos,bool Is3d);
//		void		RenderGrid(CCore *Core,Vector3 &CamPos,bool Active);
		void		RenderSelection(CCore *Core,Vector3 &ThisCam){};
		void		RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);
//		void		FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos);

		void		GUIInit(CCore *Core);
		void		GUIKill(CCore *Core);
		void		GUIUpdate(CCore *Core);
		void		GUIChanged(CCore *Core);

		void		Load(CFile *File,int Version);
		void		Save(CFile *File);

		void		Export(CCore *Core,CExport &Exp){};

		bool		LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag);
		bool		RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag);
		bool		MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos);
		bool		Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0);

// Local
		void		DeleteCurrent();
		
		CMap		&GetLBrush()											{return(Brush[LBrush]);}
		CMap		&GetRBrush()											{return(Brush[RBrush]);}
		CMap		&GetBrush(int i)										{return(Brush[i]);}
		CMap		&GetActiveBrush()										{return(GetBrush(ActiveBrush));}

		bool		CanClose()												{return(SelStart==-1);}

// Functions
		bool		SelectCancel();
		void		DeleteSet(CCore *Core);


protected:
		bool		Select(int BrushID,bool DownFlag);
		void		SetBrush(CMap &ThisBrush);

		CMap			Brush[2];
		int				ActiveBrush;
		int				SelStart;

		CGUIElemList	GUIElemList;
		CGUITileBank	GUITileBank;
};

/*****************************************************************************/
#endif