/***********************/
/*** ElemSotre Stuph ***/
/***********************/

#ifndef	__ELEMSTORE_HEADER__
#define	__ELEMSTORE_HEADER__

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
#include	"GUIElemList.h"

/*****************************************************************************/
class	CCore;
class	CElemStore : public CLayer, public CElemBank
{
public:
		CElemStore();
		~CElemStore();

// Overloads
virtual	int			GetType()							{return(0);}

virtual	void		Render(CCore *Core,Vector3 &CamPos,bool Is3d);
virtual	void		RenderGrid(CCore *Core,Vector3 &CamPos,bool Active){}
virtual	void		RenderSelection(CCore *Core,Vector3 &ThisCam){};
virtual	void		RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);
virtual	void		FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos){}

virtual	void		GUIInit(CCore *Core);
virtual	void		GUIKill(CCore *Core);
virtual	void		GUIUpdate(CCore *Core);
virtual	void		GUIChanged(CCore *Core);

virtual	void		Load(CFile *File,int Version);
virtual	void		Save(CFile *File);

virtual	void		Export(CCore *Core,CExport &Exp){};

virtual	bool		LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag);
virtual	bool		RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag);
virtual	bool		MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos);
virtual	bool		Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0);

// ElemSet Thruput
const	char		*GetSetName(int Set)									{return(SetList[Set].GetName());}
const	char		*GetSetFilename(int Set)								{return(SetList[Set].GetFilename());}
		CElem		&GetElem(int Set,int Elem)								{return(SetList[Set].GetElem(Elem));}
		void		RenderElem(int Set,int Elem,int Flags,bool Is3d);

// Local
virtual	bool		CanClose()												{return(true);}

protected:
		int				CurrentSet;
		bool			LoadFlag;
		int				CursorPos;
		CGUIElemList	GUIElemList;
};

/*****************************************************************************/
#endif