/*********************/
/*** TileSet Stuph ***/
/*********************/

#include	"stdafx.h"
//#include	"gl3d.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	"GLEnabledView.h"
#include	<Vector>
#include	<GFName.hpp>

#include	"Core.h"
#include	"TileSet.h"
#include	"GinTex.h"
#include	"utils.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"
#include	"LayerTileGui.h"


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
		for (int i=0; i<MaxBrush; i++) Brush[i].Delete();
		LastCursorPos=CursorPos=-1;
		ActiveBrush=0;
		SelStart=-1;
		SelEnd=-1;

}

/*****************************************************************************/
CTileBank::~CTileBank()
{
}

/*****************************************************************************/
void	CTileBank::Load(CFile *File,float Version)
{
int		ListSize;
GFName	RootPath=File->GetFilePath();
GString	FilePath=RootPath.Dir();
		FilePath.Append('\\');
	
	File->Read(&ListSize,sizeof(int));
	File->Read(&CurrentSet,sizeof(int));
	File->Read(&ActiveBrush,sizeof(int));
	Brush[0].Load(File,Version);
	Brush[1].Load(File,Version);

	if (Version<=1.00)
	{
		for (int i=0;i<ListSize;i++)
		{
			char	Filename[256+64];

			File->Read(Filename,256+64);
			AddTileSet(Filename);
		}
	}
	else
	{ // New Style rel storage
		for (int i=0;i<ListSize;i++)
		{
			char	c=1,RelName[256+64],FullName[256+64];
			int		Len=0;
			while (c)
			{
				File->Read(&c,1);
				RelName[Len++]=c;
			}
			RootPath.makeabsolute(FilePath,RelName,FullName);
			AddTileSet(FullName);
		}
	}
}

/*****************************************************************************/
void	CTileBank::Save(CFile *File)
{
int		ListSize=TileSet.size();
GFName	RootPath=File->GetFilePath();
GString	FilePath=RootPath.Dir();
		FilePath.Append('\\');

		File->Write(&ListSize,sizeof(int));
		File->Write(&CurrentSet,sizeof(int));
		File->Write(&ActiveBrush,sizeof(int));
		Brush[0].Save(File);
		Brush[1].Save(File);

		for (int i=0; i<ListSize; i++)
		{
			CTileSet	&ThisSet=TileSet[i];
			char	Filename[256+64];

		
			RootPath.makerelative(FilePath,ThisSet.GetFilename(),Filename);	
//			sprintf(Filename,"%s",ThisSet.GetFilename());
//			File->Write(Filename,256+64);		
			File->Write(Filename,strlen(Filename)+1);		
		}

}

/*****************************************************************************/
void	CTileBank::AddTileSet(char *Filename)
{
int		ListSize=TileSet.size();

		if (FindTileSet(Filename) ==-1)
			{
			TileSet.push_back(CTileSet(Filename,ListSize));
			LoadFlag=TRUE;
			}
}

/*****************************************************************************/
int		CTileBank::FindTileSet(char *Filename)
{
int		ListSize=TileSet.size();
CTileSet	FindSet(Filename,ListSize);

		for (int i=0; i<ListSize; i++)
		{
			CTileSet	&ThisSet=TileSet[i];

			if (IsStrSame((char*)FindSet.GetName(),(char*)ThisSet.GetName(),-1)) return(i);
		}
		return(-1);
}

/*****************************************************************************/
void	CTileBank::LoadTileSets(CCore *Core)
{
int		ListSize=TileSet.size();
	
		for (int i=0;i<ListSize;i++)
		{
			CTileSet	&ThisSet=TileSet[i];

			if (!ThisSet.IsLoaded()) ThisSet.Load(Core);
		}
		LoadFlag=FALSE;
}

/*****************************************************************************/
void	CTileBank::Delete()
{
//		List.erase(List.begin()+CurrentSet);
}

/*****************************************************************************/
void	CTileBank::Reload()
{
int		ListSize=TileSet.size();

		for (int i=0; i<ListSize; i++)
		{
			TileSet[i].Purge();
		}

		LoadFlag=TRUE;

}

/*****************************************************************************/
CTile	&CTileBank::GetTile(int Bank,int Tile)
{
		ASSERT(Bank>=0 && Tile>=0);
		return(TileSet[Bank].GetTile(Tile));
}

/*****************************************************************************/
void	CTileBank::RenderSet(CCore *Core,Vector3 &CamPos,BOOL Is3d)
{
		if (!TileSet.size()) return;	// No tiles, return

		if (Is3d)
		{
			glEnable(GL_DEPTH_TEST);
			TileSet[CurrentSet].Render(CamPos,GetLBrush(),GetRBrush(),TRUE);
			glDisable(GL_DEPTH_TEST);
		}
			else
		{
			TileSet[CurrentSet].Render(CamPos,GetLBrush(),GetRBrush(),FALSE);
		}

		TileSet[CurrentSet].RenderCursor(CamPos,CursorPos,SelStart,SelEnd);
		if (Core->IsGridOn()) TileSet[CurrentSet].RenderGrid(CamPos);
}

/*****************************************************************************/
void	CTileBank::FindCursorPos(CCore *Core,CMapEditView *View,Vector3 &CamPos,CPoint &MousePos)
{
		if (!TileSet.size()) return;	// No tiles, return
		
		CursorPos=TileSet[CurrentSet].FindCursorPos(Core,View,CamPos,MousePos);
		SelEnd=CursorPos;
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CTileBank::UpdateGUI(CCore *Core,BOOL IsTileView)
{
CMainFrame		*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CLayerTileGUI	*Dlg=(CLayerTileGUI*)Frm->GetDialog(IDD_LAYERTILE_GUI);
int				ListSize=TileSet.size();

		if (Dlg)
		{
			Dlg->m_List.ResetContent();
			if (ListSize)
			{
				for (int i=0; i<ListSize; i++)
				{
					Dlg->m_List.AddString(TileSet[i].GetName());
				}
				Dlg->m_List.SetCurSel(CurrentSet);
			}
			else
			{
				IsTileView=FALSE;
			}
			Dlg->m_List.EnableWindow(IsTileView);
		}
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
BOOL	CTileBank::Select(int BrushID,BOOL DownFlag)
{
		if (DownFlag && SelStart==-1)
		{
			if (CursorPos<0) return(FALSE);
			SelStart=CursorPos;
			if (CursorPos==0)
			{
				SetBrush(GetBrush(BrushID));
				SelStart=-1;
				TRACE0("Selected Blank\n");

			}
		}
		else
		if (!DownFlag && SelStart!=-1)
		{
			if (CursorPos==-1) return(SelectCancel());
	
			SetBrush(GetBrush(BrushID));

			SelStart=-1;
			TRACE0("END SEL\n");
		}

		return(TRUE);
}

/*****************************************************************************/
void	CTileBank::SetBrush(CMap &ThisBrush)
{
int			BW=TileSet[CurrentSet].GetTileBrowserWidth();
CPoint		S=IDToPoint(SelStart-1,BW);
CPoint		E=IDToPoint(SelEnd-1,BW);

int			Width=abs(E.x-S.x)+1;
int			Height=abs(E.y-S.y)+1;

sMapElem	ThisElem;
int			MaxTile=TileSet[CurrentSet].GetTileCount();

//		if (PointToID(End,BW)>=MaxTile) SelectCancel();	// Invalid selection

		ThisElem.Set=CurrentSet;
		ThisElem.Flags=0;
			
		ThisBrush.Delete();
		ThisBrush.SetSize(Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				ThisElem.Tile=SelStart+X+(Y*BW);
				if (!IsTileValid(CurrentSet,ThisElem.Tile))
				{
					ThisElem.Tile=-1;
				}
				ThisBrush.Set(X,Y,ThisElem,TRUE);
			}
		}
			
}

/*****************************************************************************/
BOOL	CTileBank::SelectCancel()
{
		SelStart=-1;
		TRACE0("Select Cancelled\n");
		return(TRUE);
}

/*****************************************************************************/
BOOL	CTileBank::IsTileValid(int Set,int Tile)
{
 		if (Set==-1 || Tile==-1) return(FALSE);
		
		return(TileSet[Set].IsTileValid(Tile));
}

/*****************************************************************************/
BOOL	CTileBank::IsTileValidGB(int Set,int Tile)
{
		if (Set==-1 || Tile==-1) return(FALSE);

		return(TileSet[Set].IsTileValidGB(Tile));
}

/*****************************************************************************/
/*****************************************************************************/
/*** TileSet *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CTileSet::CTileSet(char *_Filename,int Idx)
{
		Filename=_Filename;
		
		Loaded=FALSE;
		SetNumber=Idx;
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
GString	Ext=Filename.Ext();
		Ext.Upper();

		if (Ext=="GIN")
		{
			Load3d(Core);
		}
		else
		{
			Load2d(Core);
		}

		Loaded=TRUE;
}

/*****************************************************************************/

void	CTileSet::Load2d(CCore *Core)
{
CTexCache	&TexCache=Core->GetTexCache();

int		TexID=TexCache.ProcessTexture((char*)Filename.FullName(),0);
sTex	&ThisTex=TexCache.GetTex(TexID);

int		Width=ThisTex.TexWidth/16;
int		Height=ThisTex.TexHeight/16;

		TRACE3("Load 2d TileBank %s (%i,%i)\n",Filename.FullName(),Width,Height);

		Tile.push_back(CTile(0));	// Insert Blank		
		
		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				Tile.push_back(CTile(Core,this,TexID,X,Y));
			}
		}
		TileBrowserWidth=Width;
}

/*****************************************************************************/
BOOL	CTileSet::Create16x16Tile(sRGBData &Src,u8 *Dst,int XOfs,int YOfs)
{
BOOL	Data=FALSE;

		for (int Y=0; Y<16; Y++)
		{
			for (int X=0; X<16; X++)
			{
			u8	*SrcPtr=(u8*)&Src.RGB[((((YOfs*16)+Y)*Src.Width)+(X+(XOfs*16)))*3];
			u8	R=SrcPtr[0];
			u8	G=SrcPtr[1];
			u8	B=SrcPtr[2];

			if (R!=BlankRGB.rgbRed || G!=BlankRGB.rgbGreen || B!=BlankRGB.rgbBlue) Data=TRUE;

			*Dst++=R;
			*Dst++=G;
			*Dst++=B;
			}
		}
		return(Data);
}

/*****************************************************************************/
void	CTileSet::Load3d(CCore *Core)
{
CScene	Scene;

		Scene.Load(Filename.FullName());

CNode	&ThisNode=Scene.GetSceneNode(0);
int		ChildCount=ThisNode.GetPruneChildCount();
		
		Tile.push_back(CTile(0));	// Insert Blank		
		
		for (int Child=0; Child<ChildCount; Child++) 
		{
			Tile.push_back(CTile(Core,this,Scene,ThisNode.PruneChildList[Child]));
		}
		TileBrowserWidth=DefTileBrowserWidth;

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
CPoint	CTileSet::GetTilePos(int ID)
{
	if (ID==0)
		return(CPoint(-1,-1));
	else
		return(IDToPoint(ID-1,TileBrowserWidth));
}

/*****************************************************************************/
BOOL	CTileSet::IsTileValid(int No)			
{
		ASSERT(No<Tile.size());
		return(Tile[No].IsValid());
}

/*****************************************************************************/
BOOL	CTileSet::IsTileValidGB(int No)		
{
		ASSERT(No<Tile.size());
		return(Tile[No].IsValidGB());
}

/*****************************************************************************/
void	CTileSet::Render(Vector3 &CamPos,CMap &LBrush,CMap &RBrush,BOOL Render3d)
{
int			ListSize=Tile.size();
int			TileID=0;
sMapElem	ThisElem;
int			SelFlag;
BOOL		ValidTile=TRUE;

		ThisElem.Flags=0;
		ThisElem.Set=SetNumber;
		
		glMatrixMode(GL_MODELVIEW);

		while(TileID!=ListSize)
		{
			CPoint	Pos=GetTilePos(TileID);

			
			glLoadIdentity();
			glTranslatef(CamPos.x+Pos.x*(1+TileBrowserGap),CamPos.y-Pos.y*(1+TileBrowserGap),CamPos.z);

			ValidTile=IsTileValid(TileID);
			if (TileID && ValidTile) 
			{
				glColor3f(1,1,1);
				Tile[TileID].Render(0,Render3d);
			}

// Selection
			ThisElem.Tile=TileID;
			SelFlag=0;

			if (LBrush.DoesContainTile(ThisElem)) SelFlag|=1;
			if (RBrush.DoesContainTile(ThisElem)) SelFlag|=2;

			if (SelFlag)
			{
				glBegin(GL_QUADS); 
				switch(SelFlag)
				{
					case 1: // L
						glColor4f(1,0,0,0.5);
						BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
						break;
					case 2: // R
						glColor4f(0,0,1,0.5);
						BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
						break;
					case 3: // LR
						glColor4f(1,0,0,0.5);
						BuildGLQuad(TileBrowserX0,0.5,TileBrowserY0,TileBrowserY1,0);
						glColor4f(0,0,1,0.5);
						BuildGLQuad(0.5,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
						break;
				}
	
				glEnd();
			}
// Invalid tile?
			if (!ValidTile)
			{
				glBegin(GL_LINES); 
					glColor3f(1,1,1);
			
					glVertex3f( TileBrowserX0,TileBrowserY0,0);
					glVertex3f( TileBrowserX1,TileBrowserY1,0);

					glVertex3f( TileBrowserX1,TileBrowserY0,0);
					glVertex3f( TileBrowserX0,TileBrowserY1,0);

				glEnd();
			}
// Draw Box around bloody Blank so you can see it
			if (!TileID) 
			{
				CPoint	Pos=GetTilePos(TileID);
			
				glBegin(GL_LINES); 
					glColor3f(1,1,1);
			
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

			TileID++;
		}
	
}

/*****************************************************************************/
void	CTileSet::RenderCursor(Vector3 &CamPos,int CursorPos,int SelStart,int SelEnd)
{
int		ListSize=Tile.size();
CPoint	Start,End;
int		MaxTile=Tile.size();

		if (CursorPos<-1 || CursorPos>ListSize) return;		

		if (SelStart==-1)
		{
			Start=GetTilePos(CursorPos);
			End=Start;
		}
		else
		{

			CPoint	S=IDToPoint(SelStart-1,TileBrowserWidth);
			CPoint	E=IDToPoint(SelEnd-1,TileBrowserWidth);
			
			Start=CPoint(	min(S.x,E.x), min(S.y,E.y));
			End=CPoint(		max(S.x,E.x), max(S.y,E.y));
			if (PointToID(End,TileBrowserWidth)>=MaxTile) return;		// Invalid selection
		}

		glMatrixMode(GL_MODELVIEW);

		for (int Y=Start.y; Y<=End.y; Y++)
		{
			for (int X=Start.x; X<=End.x; X++)
			{
				glLoadIdentity();
				glTranslatef(CamPos.x+X*(1+TileBrowserGap),CamPos.y-Y*(1+TileBrowserGap),CamPos.z);
	
				glBegin(GL_QUADS); 
				glColor4f(1,1,0,0.5);
				BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
				glEnd();

			}
		}
}

/*****************************************************************************/
void	CTileSet::RenderGrid(Vector3 &CamPos)
{
int			ListSize=Tile.size();
int			TileID=1;	// Dont bother with blank, its sorted
		
		glMatrixMode(GL_MODELVIEW);

		while(TileID!=ListSize)
		{
			CPoint	Pos=GetTilePos(TileID);
			
			glLoadIdentity();
			glTranslatef(CamPos.x+Pos.x*(1+TileBrowserGap),CamPos.y-Pos.y*(1+TileBrowserGap),CamPos.z);

			glBegin(GL_LINES); 
				glColor3f(1,1,1);
			
				glVertex3f( TileBrowserX0,TileBrowserY0,0);
				glVertex3f( TileBrowserX1,TileBrowserY0,0);

				glVertex3f( TileBrowserX0,TileBrowserY1,0);
				glVertex3f( TileBrowserX1,TileBrowserY1,0);

				glVertex3f( TileBrowserX0,TileBrowserY0,0);
				glVertex3f( TileBrowserX0,TileBrowserY1,0);

				glVertex3f( TileBrowserX1,TileBrowserY0,0);
				glVertex3f( TileBrowserX1,TileBrowserY1,0);

			glEnd();

			TileID++;
		}
}

/*****************************************************************************/
int		CTileSet::FindCursorPos(CCore *Core,CMapEditView *View,Vector3 &CamPos,CPoint &MousePos)
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
			CPoint	Pos=GetTilePos(TileID);
			
			glLoadIdentity();
			glTranslatef(CamPos.x+Pos.x*(1+TileBrowserGap),CamPos.y-Pos.y*(1+TileBrowserGap),CamPos.z);

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

		TileID=-2;
		if (HitCount)	// Just take 1st		
		{
			TileID=HitPtr[3];
		}
		glMatrixMode(GL_MODELVIEW);	// <-- Prevent arse GL assert

		return(TileID);

}

	

