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
	CurrentSet=0;
	LTile.Set=-1;
	RTile.Set=-1;

	AddTileSet("c:/temp/rockp/rockp.gin");
	LTile.Set=0;
	LTile.Tile=1;
	RTile.Set=0;
	RTile.Tile=2;
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
int		LT=LTile.Tile;
int		RT=RTile.Tile;

		if (LTile.Set!=CurrentSet) LT=-1;
		if (RTile.Set!=CurrentSet) RT=-1;

		if (!TileSet.size()) return;	// No tiles, return

		if (Is3d)
		{
			glEnable(GL_DEPTH_TEST);
			TileSet[CurrentSet].Render3d(CamPos,LT,RT,CursorPos,Core->IsGridOn());
			glDisable(GL_DEPTH_TEST);
		}
			else
		{
			TileSet[CurrentSet].Render2d(CamPos,LT,RT,CursorPos,Core->IsGridOn());
		}
}

/*****************************************************************************/
void	CTileBank::FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos)
{
		if (!TileSet.size()) return;	// No tiles, return
		
		CursorPos=TileSet[CurrentSet].FindCursorPos(Core,View,CamPos,MousePos);
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CTileBank::UpdateGUI(CCore *Core,BOOL IsTileView)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CTileSetDlg	*TileSetDlg=(CTileSetDlg*)Frm->GetDialog(IDD_TILESET_DIALOG);
int			ListSize=TileSet.size();

	TileSetDlg->TileSetList.ResetContent();
	if (ListSize)
	{
		for (int i=0; i<ListSize; i++)
		{
			TileSetDlg->TileSetList.AddString(TileSet[i].GetName());
		}
		TileSetDlg->TileSetList.SetCurSel(CurrentSet);
//		IsTileView=TRUE;
	}
	else
	{
		IsTileView=FALSE;
	}
	TileSetDlg->TileSetList.EnableWindow(IsTileView);
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
BOOL	CTileBank::TileSelect(sMapElem &ThisTile,sMapElem &OtherTile)
{
		if (CursorPos==-1) return(FALSE);
		if (CurrentSet==OtherTile.Set && OtherTile.Tile==CursorPos)
		{ // Dont assign if same as other Tile
			return(FALSE);
		}

		ThisTile.Set=CurrentSet;
		ThisTile.Tile=CursorPos;
		if (ThisTile.Tile==0) ThisTile.Set=0;	// Always make zero tile, bank 0 (dunno why, just seems handy)
		return(TRUE);
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

		sprintf(Filename,"%s%s.%s",Path,Name,"Gin");
		Scene.Load(Filename);

CNode	&ThisNode=Scene.GetSceneNode(0);
int		ChildCount=ThisNode.GetPruneChildCount();
		
		Tile.push_back(CTile());	// Insert Blank		
		for (int Child=0; Child<ChildCount; Child++) 
		{
			Tile.push_back(CTile(Core,this,Scene,ThisNode.PruneChildList[Child]));
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
void	CTileSet::Render2d(Vec &CamPos,int LTile,int RTile,int CursorPos,BOOL GridFlag)
{
}

/*****************************************************************************/
void	CTileSet::Render3d(Vec &CamPos,int LTile,int RTile,int CursorPos,BOOL GridFlag)
{
int			ListSize=Tile.size();
int			TileID=0;
		
		glMatrixMode(GL_MODELVIEW);

		while(TileID!=ListSize)
		{
			CTile	&ThisTile=Tile[TileID];
			int		XPos=TileID%TileBrowserWidth;
			int		YPos=TileID/TileBrowserWidth;

			glLoadIdentity();
			glTranslatef(CamPos.x+XPos*(1+TileBrowserGap),CamPos.y-YPos*(1+TileBrowserGap),CamPos.z);

			RenderMisc(TileID==LTile,TileID==RTile,TileID==CursorPos,GridFlag);

			glColor3f(0.5,0.5,0.5);
			ThisTile.Render();


			TileID++;
		}
	
}

/*****************************************************************************/
void	CTileSet::RenderMisc(BOOL LTileFlag,BOOL RTileFlag,BOOL CursorFlag,BOOL GridFlag)
{
		glDisable(GL_TEXTURE_2D);

		if (LTileFlag || RTileFlag || CursorFlag)
		{
			glBegin(GL_QUADS); 
			glNormal3f( 1,1,1);

			glColor3ub(255,255,0);

			if (LTileFlag)
			{
				glColor3ub(255,0,0);
			}
			if (RTileFlag)
			{
				glColor3ub(0,0,255);
			}
			BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
			glEnd();
		}

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

	

