/*********************/
/*** TileSet Stuph ***/
/*********************/

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
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

// Reserve slot 0 for collision :o)
GString		ColFName="Collision.bmp";

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
GFName	ExePath;
GString	Filename;

// Get application path
#ifdef _DEBUG
		ExePath="C:\\Spongebob\\tools\\mapedit\\";
#else
char	ExeFilename[2048];
		GetModuleFileName(GetModuleHandle(NULL),ExeFilename,2048);
		ExePath=ExeFilename;
		ExePath.File(0);
 		ExePath.Ext(0);
#endif
		Filename=ExePath.FullName();
		Filename+=ColFName;

		LoadFlag=FALSE;
		CurrentSet=0; LastSet=0;
		for (int i=0; i<MaxBrush; i++) Brush[i].Delete();
		LastCursorPos=CursorPos=-1;
		ActiveBrush=0;
		SelStart=-1;
		SelEnd=-1;
		TileSet.push_back(CTileSet(Filename,0));
		LoadFlag=TRUE;
}

/*****************************************************************************/
CTileBank::~CTileBank()
{
}

/*****************************************************************************/
void	CTileBank::SetCollision(bool f)
{
		LastSet=CurrentSet;
		if (f)
		{ // Is collision
			CurrentSet=0;
		}
		else
		{
			CurrentSet=LastSet;
		}
}

/*****************************************************************************/
void	CTileBank::Load(CFile *File,int Version)
{
int		ListSize;
GFName	RootPath=File->GetFilePath();
GString	FilePath;
char	FixPath[1024];

		FilePath=RootPath.Drive();
		FilePath+=RootPath.Dir();
		FilePath.Append('\\');
		FilePath.Upper();
	
		File->Read(&ListSize,sizeof(int));
		File->Read(&CurrentSet,sizeof(int));
		File->Read(&ActiveBrush,sizeof(int));
		Brush[0].Load(File,Version);
		Brush[1].Load(File,Version);
		if (Version<2)
		{
			CurrentSet++;
		}

// New Style rel storage
		for (int i=0;i<ListSize;i++)
		{
			char	c=1;
			GString	FullName;//=FilePath;

			while (c)
			{
				File->Read(&c,1);
				FullName.Append(c);
			}
			FullName.Upper();
			GFName::makeabsolute(FilePath,FullName,FixPath);
			FullName=FixPath;
			_fullpath( FixPath, FullName, 1024);
			for (int z=0; z<strlen(FixPath); z++) 
			{// Invalidate any long name short cackness
				if (FixPath[z]=='~') FixPath[z]='_'; 
			}
			FullName=FixPath;

			CheckFilename(FullName);
			FullName.Upper();
			AddTileSet(FullName);
		}
}

/*****************************************************************************/
void	CTileBank::Save(CFile *File)
{
int		ListSize=TileSet.size();
int		NewListSize=ListSize-1;
GFName	RootPath=File->GetFilePath();
GString	SavePath;

		SavePath=RootPath.Drive();
		SavePath+=RootPath.Dir();
		SavePath.Append('\\');

		SavePath.Upper();

		File->Write(&NewListSize,sizeof(int));
		File->Write(&CurrentSet,sizeof(int));
		File->Write(&ActiveBrush,sizeof(int));
		Brush[0].Save(File);
		Brush[1].Save(File);

		for (int i=1; i<ListSize; i++)
		{
			CTileSet	&ThisSet=TileSet[i];
			char	Filename[256+64];

			RootPath.makerelative(SavePath,ThisSet.GetFilename(),Filename);	
			File->Write(Filename,strlen(Filename)+1);		
		}

}

/*****************************************************************************/
void	CTileBank::AddTileSet(const char *Filename)
{
int			ListSize=TileSet.size();
CTileSet	NewSet(Filename,ListSize);

			TileSet.Add(NewSet);
			if (TileSet.size()!=ListSize) LoadFlag=TRUE;
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
int		ListSize=TileSet.size();
// Remap Brushes
		for (int i=0; i<MaxBrush; i++) 
		{
			Brush[i].DeleteSet(CurrentSet);
		}
		for (int Set=CurrentSet; Set<ListSize; Set++)
		{
			for (int i=0; i<MaxBrush; i++) 
			{
				Brush[i].RemapSet(Set,Set);
			}
		}
//		TileSet.erase(TileSet.begin()+CurrentSet);
		TileSet.erase(CurrentSet);
		CurrentSet=0;
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
			TileSet[CurrentSet].Render(Core,CamPos,GetLBrush(),GetRBrush(),TRUE);
			glDisable(GL_DEPTH_TEST);
		}
			else
		{
			TileSet[CurrentSet].Render(Core,CamPos,GetLBrush(),GetRBrush(),FALSE);
		}

		TileSet[CurrentSet].RenderCursor(CamPos,CursorPos,SelStart,SelEnd);
		if (Core->IsGridOn()) TileSet[CurrentSet].RenderGrid(CamPos);
}

/*****************************************************************************/
void	CTileBank::FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos)
{
		if (!TileSet.size()) return;	// No tiles, return
		
		CursorPos=TileSet[CurrentSet].FindCursorPos(Core,CamPos,MousePos);
		SelEnd=CursorPos;
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CTileBank::GUIInit(CCore *Core)
{
		Core->GUIAdd(TileBankGUI,IDD_LAYERTILE_GUI);
}

/*****************************************************************************/
void	CTileBank::GUIKill(CCore *Core)
{
		Core->GUIRemove(TileBankGUI,IDD_LAYERTILE_GUI);
}

/*****************************************************************************/
void	CTileBank::GUIUpdate(CCore *Core)
{
int			ListSize=TileSet.size();
BOOL		IsTileView=Core->IsTileView();

			if (TileBankGUI.m_List)
			{
				TileBankGUI.m_List.ResetContent();
				if (ListSize-1)
				{
					for (int i=1; i<ListSize; i++)
					{
						TileBankGUI.m_List.AddString(TileSet[i].GetName());
					}
					TileBankGUI.m_List.SetCurSel(CurrentSet-1);
				}
				else
				{
					IsTileView=FALSE;
				}
				TileBankGUI.m_List.EnableWindow(IsTileView);
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
 		if (Set<0 || Tile<0) return(FALSE);
		if (Tile==0) return(TRUE);
		ASSERT(Set<TileSet.size());
		
		return(TileSet[Set].IsTileValid(Tile));
}

/*****************************************************************************/
/*****************************************************************************/
/*** TileSet *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CTileSet::CTileSet(const char *_Filename,int Idx)
{
GFName	FName=_Filename;

		Filename=_Filename;
		Name=FName.File();
		
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
GFName	FName=Filename;
GString	Ext=FName.Ext();
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
GString		ColTest;
int			TexID=TexCache.ProcessTexture(Filename,0);
sTex		&ThisTex=TexCache.GetTex(TexID);

int		Width=ThisTex.TexWidth/16;
int		Height=ThisTex.TexHeight/16;

//		TRACE3("Load 2d TileBank %s (%i,%i)\n",Filename.FullName(),Width,Height);

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
void	CTileSet::Load3d(CCore *Core)
{
CScene	Scene;

		Scene.Load(Filename);

CNode	&ThisNode=Scene.GetSceneNode(0);
int		ChildCount=ThisNode.GetPruneChildCount();
		
		Tile.push_back(CTile(0));	// Insert Blank		
		
		for (int Child=0; Child<ChildCount; Child++) 
		{
			Tile.push_back(CTile(Core,this,Scene,ThisNode.PruneChildList[Child]));
		}
		TileBrowserWidth=DefTileBrowserWidth;
		TRACE1("%i\n",Tile.size());

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
//		ASSERT(No<Tile.size());
		if (No>Tile.size()) return(FALSE);
		{return(Tile[No].IsValid());}
}

/*****************************************************************************/
void	CTileSet::Render(CCore *Core,Vector3 &CamPos,CMap &LBrush,CMap &RBrush,BOOL Render3d)
{
int			ListSize=Tile.size();
int			TileID=0;
sMapElem	ThisElem;
int			SelFlag;
BOOL		ValidTile=TRUE;
//float		Scale=1.0f/(float)TileBrowserWidth/CamPos.z;
float		Scale=CamPos.z/(float)TileBrowserWidth/2.0;

		ThisElem.Flags=0;
		ThisElem.Set=SetNumber;
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		while(TileID!=ListSize)
		{
			CPoint	Pos=GetTilePos(TileID);
			float	XPos=(float)Pos.x*(1+TileBrowserGap);
			float	YPos=(float)Pos.y*(1+TileBrowserGap);

			glLoadIdentity();
			glScalef(Scale,Scale,Scale);
			glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);

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
						BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0.01f);
						break;
					case 2: // R
						glColor4f(0,0,1,0.5);
						BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0.01f);
						break;
					case 3: // LR
						glColor4f(1,0,0,0.5);
						BuildGLQuad(TileBrowserX0,0.5,TileBrowserY0,TileBrowserY1,0.01f);
						glColor4f(0,0,1,0.5);
						BuildGLQuad(0.5,TileBrowserX1,TileBrowserY0,TileBrowserY1,0.01f);
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
		glPopMatrix();
	
}

/*****************************************************************************/
void	CTileSet::RenderCursor(Vector3 &CamPos,int CursorPos,int SelStart,int SelEnd)
{
int			ListSize=Tile.size();
CPoint		Start,End;
int			MaxTile=Tile.size();
//float		Scale=1.0f/(float)TileBrowserWidth/CamPos.z;
float		Scale=CamPos.z/(float)TileBrowserWidth/2.0;

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
		glPushMatrix();

		for (int Y=Start.y; Y<=End.y; Y++)
		{
			for (int X=Start.x; X<=End.x; X++)
			{
				float	XPos=(float)X*(1+TileBrowserGap);
				float	YPos=(float)Y*(1+TileBrowserGap);

				glLoadIdentity();
				glScalef(Scale,Scale,Scale);
				glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);

	
				glBegin(GL_QUADS); 
				glColor4f(1,1,0,0.5);
				BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
				glEnd();

			}
		}
		glPopMatrix();
}

/*****************************************************************************/
void	CTileSet::RenderGrid(Vector3 &CamPos)
{
int			ListSize=Tile.size();
int			TileID=1;	// Dont bother with blank, its sorted
//float		Scale=1.0f/(float)TileBrowserWidth/CamPos.z;
float		Scale=CamPos.z/(float)TileBrowserWidth/2.0;
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		while(TileID!=ListSize)
		{
			CPoint	Pos=GetTilePos(TileID);
			float	XPos=(float)Pos.x*(1+TileBrowserGap);
			float	YPos=(float)Pos.y*(1+TileBrowserGap);

			glLoadIdentity();
			glScalef(Scale,Scale,Scale);
			glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);
			
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
		glPopMatrix();

}

/*****************************************************************************/
int		CTileSet::FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos)
{
int		ListSize=Tile.size();
GLint	Viewport[4];
GLuint	SelectBuffer[SELECT_BUFFER_SIZE];
int		HitCount;
int		TileID=0;
//float		Scale=1.0f/(float)TileBrowserWidth/CamPos.z;
float		Scale=CamPos.z/(float)TileBrowserWidth/2.0;
		
		glGetIntegerv(GL_VIEWPORT, Viewport);
		glSelectBuffer (SELECT_BUFFER_SIZE, SelectBuffer );
		glRenderMode (GL_SELECT);

	    glInitNames();
		glPushName(-1);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix( MousePos.x ,(Viewport[3]-MousePos.y),5.0,5.0,Viewport);
		Core->GetView()->SetupPersMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		while(TileID!=ListSize)
		{
			CPoint	Pos=GetTilePos(TileID);
			float	XPos=(float)Pos.x*(1+TileBrowserGap);
			float	YPos=(float)Pos.y*(1+TileBrowserGap);

			glLoadIdentity();
			glScalef(Scale,Scale,Scale);
			glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);

			glLoadName (TileID);
			glBegin (GL_QUADS); 
				BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
			glEnd();
			TileID++;
		}

		HitCount= glRenderMode (GL_RENDER);
		glPopMatrix();
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

	

