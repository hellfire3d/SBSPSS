/*********************/
/*** TileSet Stuph ***/
/*********************/

#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	"GLEnabledView.h"
#include	<Vector>

#include	"Core.h"
#include	"TileSet.h"
#include	"GinTex.h"
#include	"utils.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"
#include	"TileSetDlg.h"


/*****************************************************************************/
/*****************************************************************************/
/*** TileBank ****************************************************************/
/*****************************************************************************/

const float	TileBrowserGap=0.2f;
const float	TileBrowserX0=0-TileBrowserGap/2;
const float	TileBrowserX1=1+TileBrowserGap/2;
const float	TileBrowserY0=0-TileBrowserGap/2;
const float	TileBrowserY1=1+TileBrowserGap/2;

/*****************************************************************************/
CTileBank::CTileBank()
{
	LoadFlag=FALSE;
}

/*****************************************************************************/
CTileBank::~CTileBank()
{
}

/*****************************************************************************/
void	CTileBank::AddTileSet(char *Filename)
{
	TileSet.push_back(CTileSet(Filename));
	LoadFlag=TRUE;
}

/*****************************************************************************/
void	CTileBank::LoadTileSets(CCore *Core)
{
int	ListSize=TileSet.size();
	
	for (int i=0;i<ListSize;i++)
	{
		CTileSet	&ThisSet=TileSet[i];

		if (!ThisSet.IsLoaded()) ThisSet.Load(Core);
	}
	LoadFlag=FALSE;
}

/*****************************************************************************/
void	CTileBank::Reload()
{
int	ListSize=TileSet.size();

	for (int i=0; i<ListSize; i++)
	{
		TileSet[i].Purge();
	}

	LoadFlag=TRUE;
}

/*****************************************************************************/
CTile	&CTileBank::GetTile(int Bank,int Tile)
{
	return(TileSet[Bank].GetTile(Tile));
}

/*****************************************************************************/
void	CTileBank::RenderSet(CCore *Core,Vec &CamPos,BOOL Is3d)
{
int		CurrentBank=Core->GetCurrentTileBank();
sMapElem	&LSelTile=Core->GetMouseTileL();
sMapElem	&RSelTile=Core->GetMouseTileR();
int			LTile=-1,RTile=-1;

		if (LSelTile.Bank==CurrentBank || LSelTile.Tile==0) LTile=LSelTile.Tile;
		if (RSelTile.Bank==CurrentBank || RSelTile.Tile==0) RTile=RSelTile.Tile;

		if (Is3d)
		{
			glEnable(GL_DEPTH_TEST);
			TileSet[CurrentBank].Render3d(Core,CamPos,CursPos,LTile,RTile);
			glDisable(GL_DEPTH_TEST);
		}
			else
		{
			TileSet[CurrentBank].Render2d(Core,CamPos,CursPos,LTile,RTile);
		}
}

/*****************************************************************************/
void	CTileBank::FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos)
{
int		CurrentBank=Core->GetCurrentTileBank();
		
		CursPos=TileSet[CurrentBank].FindCursorPos(Core,View,CamPos,MousePos);
}

/*****************************************************************************/
void	CTileBank::LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
		if (!DownFlag) return;

sMapElem	&SelTile=		Core->GetMouseTileL();
sMapElem	&OtherSelTile=	Core->GetMouseTileR();
int			CurrentBank=	Core->GetCurrentTileBank();

		if (CursPos==-1) return;
		if (CurrentBank==OtherSelTile.Bank && OtherSelTile.Tile==CursPos)
		{ // Dont assign if same as other mouse
		}
		else
		{
			SelTile.Bank=CurrentBank;
			SelTile.Tile=CursPos;
			if (SelTile.Tile==0) SelTile.Bank=0;	// Always make zero tile, bank 0 (dunno why, just seems handy)
			View->Invalidate();
		}

}

/*****************************************************************************/
void	CTileBank::RButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
		if (!DownFlag) return;

sMapElem	&SelTile=		Core->GetMouseTileR();
sMapElem	&OtherSelTile=	Core->GetMouseTileL();
int			CurrentBank=	Core->GetCurrentTileBank();

		if (CursPos==-1) return;
		if (CurrentBank==OtherSelTile.Bank && OtherSelTile.Tile==CursPos)
		{ // Dont assign if same as other mouse
		}
		else
		{
			SelTile.Bank=CurrentBank;
			SelTile.Tile=CursPos;
			View->Invalidate();
		}
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CTileBank::InitGUI(CCore *Core)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();

		ParamBar->RemoveAll();
		ParamBar->Add(Frm->GetTileSetDlg(),IDD_TILESET_DIALOG,TRUE);
		ParamBar->Update();
		UpdateGUI(Core);
}

/*****************************************************************************/
void	CTileBank::UpdateGUI(CCore *Core)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CTileSetDlg	*TileSetDlg=(CTileSetDlg*)Frm->GetDialog(IDD_TILESET_DIALOG);
int			ListSize=TileSet.size();

	TRACE1("%i\n",TileSetDlg->TileSetList.GetCurSel());
	TileSetDlg->TileSetList.ResetContent();

	for (int i=0; i<ListSize; i++)
	{
		TileSetDlg->TileSetList.AddString(TileSet[i].GetName());
	}
	TileSetDlg->TileSetList.SetCurSel(Core->GetCurrentTileBank());
}

/*****************************************************************************/
/*****************************************************************************/
/*** TileSet *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CTileSet::CTileSet(char *_Filename)
{
char	Drive[_MAX_DRIVE];
char	Dir[_MAX_DIR];
char	Fname[_MAX_FNAME];
char	Ext[_MAX_EXT];

	_splitpath(_Filename,Drive,Dir,Fname,Ext);
	sprintf(Path,"%s%s",Drive,Dir);
	sprintf(Name,"%s",Fname);
	Loaded=FALSE;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CTileSet::~CTileSet()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CTileSet::Load(CCore *Core)
{
CScene	Scene;
char	Filename[256+64];
CTile	NewTile;

		sprintf(Filename,"%s%s.%s",Path,Name,"Gin");
		Scene.Load(Filename);

CNode	&ThisNode=Scene.GetSceneNode(0);
int		ChildCount=ThisNode.GetPruneChildCount();
		
		NewTile.CreateBlank(Core,this);		
		Tile.push_back(NewTile);

		for (int Child=0; Child<ChildCount; Child++) 
		{
			NewTile.Load(Core,this,Scene,ThisNode.PruneChildList[Child]);
			Tile.push_back(NewTile);
		}

	Loaded=TRUE;
}

/*****************************************************************************/
void	CTileSet::Purge()
{
int	ListSize=Tile.size();
	
		for (int i=0; i<ListSize; i++)
		{
			Tile[i].Purge();
		}
		Tile.clear();
		Loaded=FALSE;

}

/*****************************************************************************/
void	CTileSet::Render2d(CCore *Core,Vec &CamPos,int CursorPos,int LTile,int RTile)
{
}

/*****************************************************************************/
void	CTileSet::Render3d(CCore *Core,Vec &CamPos,int CursorPos,int LTile,int RTile)
{
int			ListSize=Tile.size();
CTexCache	&TexCache=Core->GetTexCache();
int			TileID=0;
BOOL		GridFlag=Core->IsGridOn();
		
		glMatrixMode(GL_MODELVIEW);

		while(TileID!=ListSize)
		{
			CTile	&ThisTile=Tile[TileID];
			int		XPos=TileID%TileBrowserWidth;
			int		YPos=TileID/TileBrowserWidth;

			glLoadIdentity();
			glTranslatef(CamPos.x+XPos*(1+TileBrowserGap),CamPos.y-YPos*(1+TileBrowserGap),CamPos.z);

			RenderMisc(TileID==LTile,TileID==RTile,GridFlag,TileID==CursorPos);

			glColor3f(0.5,0.5,0.5);
			ThisTile.Render();

			TileID++;
		}
	
}

/*****************************************************************************/
void	CTileSet::RenderMisc(BOOL LSelFlag,BOOL RSelFlag, BOOL GridFlag,BOOL CursorFlag)
{
		
			glDisable(GL_TEXTURE_2D);

			if (LSelFlag || RSelFlag || CursorFlag)
			{ // Show selected tile
				glBegin(GL_QUADS); 
					glNormal3f( 1,1,1);
					if (LSelFlag) 
						glColor3ub(255,0,0);
					else
					if (RSelFlag) 
						glColor3ub(0,0,255);
					else
						glColor3ub(255,255,0);

					BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
				glEnd();
			}
			else
			if (GridFlag)
			{
				glBegin(GL_LINES); 
					glNormal3f( 1,1,1);
					glColor3ub(255,255,255);
				
					glVertex3f( TileBrowserX0,TileBrowserY0,0);
					glVertex3f( TileBrowserX1,TileBrowserY0,0);
	
					glVertex3f( TileBrowserX0,TileBrowserY1,0);
					glVertex3f( TileBrowserX1,TileBrowserY1,0);
	
					glVertex3f( TileBrowserX0,TileBrowserY0,0);
					glVertex3f( TileBrowserX0,TileBrowserY1,0);
	
					glVertex3f( TileBrowserX1,TileBrowserY0,0);
					glVertex3f( TileBrowserX1,TileBrowserY1,0);
	
				glEnd();
			}

			glEnable(GL_TEXTURE_2D);
	
}

/*****************************************************************************/
int		CTileSet::FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos)
{
int		ListSize=Tile.size();
GLint	Viewport[4];
GLuint	SelectBuffer[SELECT_BUFFER_SIZE];
int		HitCount;
int		TileID=0;
		
		glGetIntegerv(GL_VIEWPORT, Viewport);
		glSelectBuffer (SELECT_BUFFER_SIZE, SelectBuffer );
		glRenderMode (GL_SELECT);

	    glInitNames();
		glPushName(-1);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix( MousePos.x ,(Viewport[3]-MousePos.y),5.0,5.0,Viewport);
		View->SetupPersMatrix();

		glMatrixMode(GL_MODELVIEW);

		while(TileID!=ListSize)
		{
			int		XPos=TileID%TileBrowserWidth;
			int		YPos=TileID/TileBrowserWidth;

			glLoadIdentity();
			glTranslatef(CamPos.x+XPos*(1+TileBrowserGap),CamPos.y-YPos*(1+TileBrowserGap),CamPos.z);

			glLoadName (TileID);
			glBegin (GL_QUADS); 
				BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
			glEnd();
			TileID++;
		}

		HitCount= glRenderMode (GL_RENDER);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

// Process hits

GLuint	*HitPtr=SelectBuffer;

		TileID=-1;
		if (HitCount)	// Just take 1st		
		{
			TileID=HitPtr[3];
		}
		glMatrixMode(GL_MODELVIEW);	// <-- Prevent arse GL assert
		return(TileID);

}

	

