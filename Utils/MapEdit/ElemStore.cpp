/************************/
/*** Elem Sotre Stuph ***/
/************************/

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"
#include	<Vector>

#include	<GFName.hpp>

#include	"Core.h"
#include	"ElemStore.h"
#include	"GinTex.h"
#include	"utils.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"
#include	"GUITileBank.h"

/*****************************************************************************/
const float	ElemBrowserGap=0.2f;
const float	ElemBrowserX0=0-ElemBrowserGap/2;
const float	ElemBrowserX1=1+ElemBrowserGap/2;
const float	ElemBrowserY0=0-ElemBrowserGap/2;
const float	ElemBrowserY1=1+ElemBrowserGap/2;

/*****************************************************************************/
CElemStore::CElemStore()
{
		LoadFlag=false;
		CurrentSet=0;
		VisibleFlag=true;
}

/*****************************************************************************/
CElemStore::~CElemStore()
{
}

/*****************************************************************************/
void	CElemStore::Load(CFile *File,int Version)
{
		File->Read(&LayerCam,sizeof(Vector3));
		File->Read(&CurrentSet,sizeof(int));
//		CElemBank::Load(File,Version);
}

/*****************************************************************************/
void	CElemStore::Save(CFile *File)
{
int		ListSize=GetSetCount();
GFName	RootPath=File->GetFilePath();
GString	SavePath;

		SavePath=RootPath.Drive();
		SavePath+=RootPath.Dir();
		SavePath.Append('\\');

		SavePath.Upper();

		File->Write(&LayerCam,sizeof(Vector3));
		File->Write(&CurrentSet,sizeof(int));
//		CElemBank::Save(File);
}

/*****************************************************************************/
void	CElemStore::RenderElem(int Set,int Elem,int Flags,bool Is3d)
{
		if (IsValid(Set,Elem))
		{
			SetList[Set].RenderElem(Elem,Flags,Is3d);
		}
		else
		{
			RenderInvalid();
		}
		
}
/*****************************************************************************/
void	CElemStore::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
/*
		if (!GetSetCount()) return;
CElemSet	&ThisSet=SetList[CurrentSet];
int			ListSize=ThisSet.GetCount();
int			BrowserWidth=ThisSet.GetBrowserWidth();
int			ElemID=0;
sMapElem	ThisElem;
int			SelFlag;
float		Scale=CamPos.z/(float)BrowserWidth/2.0;

		ThisElem.Flags=0;
		ThisElem.Set=CurrentSet;
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		while(ElemID!=ListSize)
		{
			CPoint	Pos=GetElemPos(ElemID,BrowserWidth);
			float	XPos=(float)Pos.x*(1+ElemBrowserGap);
			float	YPos=(float)Pos.y*(1+ElemBrowserGap);

			glLoadIdentity();
			glScalef(Scale,Scale,Scale);
			glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);

			glColor3f(1,1,1);
			ThisSet.RenderElem(ElemID,0,Is3d);
// Selection
			ThisElem.Elem=ElemID;
			SelFlag=0;

			if (Brush[0].DoesContainElem(ThisElem)) SelFlag|=1;
			if (Brush[1].DoesContainElem(ThisElem)) SelFlag|=2;

			if (SelFlag)
			{
				glBegin(GL_QUADS); 
				switch(SelFlag)
				{
					case 1: // L
						glColor4f(1,0,0,0.5);
						BuildGLQuad(ElemBrowserX0,ElemBrowserX1,ElemBrowserY0,ElemBrowserY1,0.01f);
						break;
					case 2: // R
						glColor4f(0,0,1,0.5);
						BuildGLQuad(ElemBrowserX0,ElemBrowserX1,ElemBrowserY0,ElemBrowserY1,0.01f);
						break;
					case 3: // LR
						glColor4f(1,0,0,0.5);
						BuildGLQuad(ElemBrowserX0,0.5,ElemBrowserY0,ElemBrowserY1,0.01f);
						glColor4f(0,0,1,0.5);
						BuildGLQuad(0.5,ElemBrowserX1,ElemBrowserY0,ElemBrowserY1,0.01f);
						break;
				}
	
				glEnd();
			}
			ElemID++;
		}
		glPopMatrix();
*/	
}

/*****************************************************************************/
void	CElemStore::RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d)
{
/*
		if (!GetSetCount()) return;
CElemSet	&ThisSet=SetList[CurrentSet];
int			ListSize=ThisSet.GetCount();
int			BrowserWidth=ThisSet.GetBrowserWidth();
CPoint		Start,End;
int			MaxElem=ListSize;
float		Scale=CamPos.z/(float)BrowserWidth/2.0;

		if (CursorPos<-1 || CursorPos>ListSize) return;		
		if (!ListSize) return;


		if (SelStart==-1)
		{
			Start=GetElemPos(CursorPos,BrowserWidth);
			End=Start;
		}
		else
		{

			CPoint	S=IDToPoint(SelStart-1,BrowserWidth);
			CPoint	E=IDToPoint(SelEnd-1,BrowserWidth);
			
			Start=CPoint(	min(S.x,E.x), min(S.y,E.y));
			End=CPoint(		max(S.x,E.x), max(S.y,E.y));
			if (PointToID(End,BrowserWidth)>=MaxElem) return;		// Invalid selection
		}

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		for (int Y=Start.y; Y<=End.y; Y++)
		{
			for (int X=Start.x; X<=End.x; X++)
			{
				float	XPos=(float)X*(1+ElemBrowserGap);
				float	YPos=(float)Y*(1+ElemBrowserGap);

				glLoadIdentity();
				glScalef(Scale,Scale,Scale);
				glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);

	
				glBegin(GL_QUADS); 
				glColor4f(1,1,0,0.5);
				BuildGLQuad(ElemBrowserX0,ElemBrowserX1,ElemBrowserY0,ElemBrowserY1,0);
				glEnd();

			}
		}
		glPopMatrix();
*/
}


/*****************************************************************************/
bool	CElemStore::LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
/*		if (nFlags & MK_RBUTTON)
		{
			SelectCancel();
			return(false);
		}
		Select(LBrush,DownFlag);
*/
		return(true);	
}

/*****************************************************************************/
bool	CElemStore::RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
/*		if (nFlags & MK_LBUTTON)
		{
			SelectCancel();
			return(false);
		}

		Select(RBrush,DownFlag);
*/
		return(false);	
}

/*****************************************************************************/
bool	CElemStore::MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)
{
	return(false);
}

/*****************************************************************************/
bool	CElemStore::Command(int CmdMsg,CCore *Core,int Param0,int Param1)
{
/*		switch(CmdMsg)
		{
		case CmdMsg_SubViewLoad:
			LoadNewSet(Core);
			GUIUpdate(Core);
			break;
		case CmdMsg_SubViewDelete:
			DeleteSet(Core);
			GUIUpdate(Core);
			break;
		case CmdMsg_SubViewUpdate:
			ReloadAllSets();
			Core->GetTexCache().Purge();
			GUIUpdate(Core);
			break;
		case CmdMsg_SubViewSet:
			CurrentSet=ElemStoreGUI.m_List.GetCurSel();
			GUIUpdate(Core);
			break;
		case CmdMsg_ActiveBrushLeft:
			ActiveBrush=LBrush;
			break;
		case CmdMsg_ActiveBrushRight:
			ActiveBrush=RBrush;
		default:
			TRACE3("ElemStore-Unhandled Command %i (%i,%i)\n",CmdMsg,Param0,Param1);
		}
*/		return(true);
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CElemStore::GUIInit(CCore *Core)
{
		Core->GUIAdd(GUIElemList,IDD_ELEMLIST);
}

/*****************************************************************************/
void	CElemStore::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUIElemList,IDD_ELEMLIST);
}

/*****************************************************************************/
void	CElemStore::GUIUpdate(CCore *Core)
{
int			ListSize=GetSetCount();
bool		IsSubView=Core->IsSubView();

			if (GUIElemList.m_List)
			{
				GUIElemList.m_List.ResetContent();
				if (ListSize)
				{
					for (int i=0; i<ListSize; i++)
					{
						GUIElemList.m_List.AddString(GetSetName(i));
					}
					GUIElemList.m_List.SetCurSel(CurrentSet);
				}
				else
				{
					IsSubView=FALSE;
				}
				GUIElemList.m_List.EnableWindow(IsSubView);
			}

}

/*****************************************************************************/
void	CElemStore::GUIChanged(CCore *Core)
{
}

/*****************************************************************************/
