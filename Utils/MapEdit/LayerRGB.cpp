/*****************/
/*** Layer RGB ***/
/*****************/


#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"
#include	<io.h>
#include	<frame.hpp>

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"

#include	"Core.h"
#include	"Layer.h"
#include	"LayerRGB.h"
#include	"Utils.h"
#include	"Select.h"
#include	"Export.h"
#include	"GUILayerRGB.h"
#include	"Elem.h"

/*****************************************************************************/
char	*CLayerRGB::RGBModeName[CLayerRGB::GUI_MODE_MAX]={"Paint","Lighten","Darken"};
float	RGBAlpha=0.5f;
int		RateInc=5;

#define	MAX_UNDO	16

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerRGB::CLayerRGB(sLayerDef &Def)
{
		InitLayer(Def);

		ShadeRGB.R=224;
		ShadeRGB.G=224;
		ShadeRGB.B=224;
		CurrentMode=0;
		CurrentBrush=0;
		CurrentRate=0;
		LastCursPos.x=-1;
		LastCursPos.y=-1;
		CurrentUndo=0;
}


/*****************************************************************************/
CLayerRGB::~CLayerRGB()
{
}

/*****************************************************************************/
void	CLayerRGB::InitLayer(sLayerDef &Def)
{
		CLayer::InitLayer(Def);
		SetSize(Def.Width,Def.Height,true);
		BuildBrushList();
}

/*****************************************************************************/
void	CLayerRGB::Load(CFile *File,int Version)
{
		InitLayer(LayerDef);

		File->Read(&ShadeRGB,sizeof(sRGBElem));
		File->Read(&CurrentBrush,sizeof(int));
		File->Read(&CurrentMode,sizeof(int));
		File->Read(&SpareFlag,sizeof(bool));

// Read Map
		File->Read(&MapWidth,sizeof(int));
		File->Read(&MapHeight,sizeof(int));
		SetSize(MapWidth,MapHeight,false);
		for (int Y=0; Y<MapHeight; Y++)
		{
			for (int X=0; X<MapWidth; X++)
			{
				sRGBElem	&ThisElem=Map[X][Y];

				File->Read(&ThisElem,sizeof(sRGBElem));
			}
		}
}

/*****************************************************************************/
void	CLayerRGB::Save(CFile *File)
{
// Always Save current version
		File->Write(&ShadeRGB,sizeof(sRGBElem));
		File->Write(&CurrentBrush,sizeof(int));
		File->Write(&CurrentMode,sizeof(int));
		File->Write(&SpareFlag,sizeof(bool));

// Read Map
		File->Write(&MapWidth,sizeof(int));
		File->Write(&MapHeight,sizeof(int));
		for (int Y=0; Y<MapHeight; Y++)
		{
			for (int X=0; X<MapWidth; X++)
			{
				sRGBElem	&ThisElem=Map[X][Y];

				File->Write(&ThisElem,sizeof(sRGBElem));
			}
		}
}

/*****************************************************************************/
void	CLayerRGB::BuildBrushList()
{
GString		Path;
GString		Filename;
_finddata_t Find;
long		FileHandle;
int			Error=0;

			BrushList.clear();

			GetExecPath(Path);
			Path+="\\Brushes\\";
			Filename=Path+"*.bmp";

			if( (FileHandle= _findfirst( Filename, &Find)) == -1L ) 
			{
				ASSERT(!"No Brushes Found :o(\n");
				return;
			}

			while (Error==0)
			{
				GString	ThisFile=Path+Find.name;
				LoadBrush(ThisFile);
				Error=_findnext( FileHandle, &Find);
			}
			_findclose( FileHandle);

}

/*****************************************************************************/
void	CLayerRGB::LoadBrush(const char *Name)
{
GFName	Filename=Name;
int		BrushCount=BrushList.size();
		BrushList.resize(BrushCount+1);			
sRGBBrush	&NewBrush=BrushList[BrushCount];
Frame		Frm;
int		W,H;
		NewBrush.Name=Filename.File();
		Frm.LoadBMP(Name);
		W=Frm.GetWidth();
		H=Frm.GetHeight();
		NewBrush.W=W;
		NewBrush.H=H;
		NewBrush.XOfs=W/2;
		NewBrush.YOfs=H/2;
		NewBrush.Gfx.resize(W*H);

int		Ofs=0;
		for (int Y=0;Y<H; Y++)
		{
			for (int X=0;X<W; X++)
			{
				u8	Col=Frm.GetPixel(X,Y);

				Col=Frm.GetPal()[Col].GetR();
				NewBrush.Gfx[Ofs++]=Col;
			}
		}

		

		TRACE1("%s\n",NewBrush.Name);

}

/*****************************************************************************/
bool	CLayerRGB::Command(int CmdMsg,CCore *Core,int Param0,int Param1)
{
bool	Ret=false;

		switch(CmdMsg)
		{
		case CmdMsg_Undo:
			Undo();
			break;
		case CmdMsg_Filter:
			CreateUndo();
			if (Param0==0) BiFilter(Core);
			if (Param0==1) TriFilter(Core);
			if (Param0==2) STriFilter(Core);
			break;
		default:
			break;
		}

		return(Ret);

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerRGB::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();
int			StartX=(int)ThisCam.x;
int			StartY=(int)ThisCam.y;
float		ShiftX=ThisCam.x - (int)ThisCam.x;
float		ShiftY=ThisCam.y - (int)ThisCam.y;
CLayerTile	*ActionLayer=(CLayerTile*)Core->GetActionLayer();

		if (StartX<0) StartX=0;
		if (StartY<0) StartY=0;

int		DrawW=ZoomW+8;
int		DrawH=ZoomH+8;

		if (StartX+DrawW>MapWidth)	DrawW=MapWidth-StartX;
		if (StartY+DrawH>MapHeight)	DrawH=MapHeight-StartY;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
		glTranslatef(-ShiftX,ShiftY,0);		// Set scroll offset
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

		for (int YLoop=0; YLoop<DrawH; YLoop++)
		{
			for (int XLoop=0; XLoop<DrawW; XLoop++)
			{
				int	XPos=StartX+XLoop;
				int	YPos=StartY+YLoop;
				
				sRGBElem	&ThisElem=Map[XPos][YPos];
				sMapElem	&MapElem=ActionLayer->GetMapElem(XPos,YPos);
				if (MapElem.Tile)
				{
					float	fR=(1.0f/255.0f)*ThisElem.R;
					float	fG=(1.0f/255.0f)*ThisElem.G;
					float	fB=(1.0f/255.0f)*ThisElem.B;
					glLoadName (0);
					glBegin (GL_QUADS); 
						glColor4f(fR,fG,fB,RGBAlpha);
						BuildGLQuad(0,1,0,1,0);
					glEnd();
				}
				glTranslatef(1.0f,0,0);	// Next X
			}
			glTranslatef(-DrawW,-1,0); // Next y, rewind to start X
		}
		glPopMatrix();
}


/*****************************************************************************/
void	CLayerRGB::RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
CPoint		CursPos=Core->GetCursorPos();

			if (CursPos.x<0 || CursPos.y<0) return;
sRGBBrush	&ThisBrush=BrushList[CurrentBrush];

			CursPos.x-=ThisBrush.XOfs;
			CursPos.y-=ThisBrush.YOfs;
			CursPos.x-=(int)ThisCam.x;
			CursPos.y-=(int)ThisCam.y;

float			ZoomW=Core->GetZoomW();
float			ZoomH=Core->GetZoomH();
float			ScrOfsX=(ZoomW/2);
float			ScrOfsY=(ZoomH/2);
Vector3			&Scale=Core->GetScaleVector();
float			ShiftX=ThisCam.x - (int)ThisCam.x;
float			ShiftY=ThisCam.y - (int)ThisCam.y;
std::vector<u8>	&Gfx=ThisBrush.Gfx;
int				Ofs=0;
int				Rate=100-(CurrentRate*RateInc);
float			fRate=(1.0/100.f)*Rate;
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
		glTranslatef(-ShiftX,ShiftY,0);		// Set scroll offset
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner
		glTranslatef(CursPos.x,-CursPos.y,0);	// Bring to top left corner

		for (int YLoop=0; YLoop<ThisBrush.H; YLoop++)
		{
			for (int XLoop=0; XLoop<ThisBrush.W; XLoop++)
			{
				u8	B=Gfx[Ofs++];
				
				if (B)
				{
					float	fRGB=(1.0f/255.0f)*fRate*B;

					glBegin (GL_QUADS); 
					glColor4f(fRGB,fRGB,fRGB,RGBAlpha);
						BuildGLQuad(0,1,0,1,0);
					glEnd();

				}
				glTranslatef(1.0f,0,0);	// Next X
			}
			glTranslatef(-ThisBrush.W,-1,0); // Next y, rewind to start X
		}
		glPopMatrix();

}

/*****************************************************************************/
void	CLayerRGB::CheckLayerSize(int Width,int Height)
{
		if (Resize(Width,Height))
		{
			CString mexstr;
			mexstr.Format("%s Layer Resized to Correct Size\nPlease re-save\n", GetName());
			AfxMessageBox(mexstr,MB_OK | MB_ICONEXCLAMATION);
		}
}

/*****************************************************************************/
void	CLayerRGB::SetSize(int Width,int Height,BOOL ClearFlag)
{
int	m,i;
		MapWidth=Width;
		MapHeight=Height;
		Map.resize(Width);
		for (i=0;i<Width;i++)
		{
			Map[i].resize(Height);
		}

		UndoList.resize(MAX_UNDO);
		TRACE1("%i\n",UndoList.size());
		for (m=0; m<MAX_UNDO; m++) 
		{
			sRGBUndo	&ThisUndo=UndoList[m];
			ThisUndo.Valid=false;
			ThisUndo.Map.resize(Width);
			for (i=0;i<Width;i++)
			{
				ThisUndo.Map[i].resize(Height);
			}
		}

	if (ClearFlag)	Clear();
}

/*****************************************************************************/
void	CLayerRGB::Clear()
{
		for (int Y=0;Y<MapHeight;Y++)
		{
			for (int X=0;X<MapWidth;X++)
			{
				Map[X][Y].R=128;
				Map[X][Y].G=128;
				Map[X][Y].B=128;
			}
		}
}


/*****************************************************************************/
bool	CLayerRGB::Resize(int Width,int Height)
{
		if (MapWidth!= Width || MapHeight!=Height)
		{
			SetSize(Width,Height,true);
			return(true);
		}
		return(false);
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerRGB::GUIInit(CCore *Core)
{

int		ListSize,i;

		GUIRGB.DisableCallback();
		Core->GUIAdd(GUIRGB,IDD_LAYER_RGB);

// Init BrushList
		ListSize=BrushList.size();
		GUIRGB.m_BrushList.ResetContent();
		for (i=0; i<ListSize; i++)
		{
			GUIRGB.m_BrushList.AddString(BrushList[i].Name);
		}
// Init ModeList
		GUIRGB.m_ModeList.ResetContent();
		for (i=0; i<GUI_MODE_MAX; i++)
		{
			GUIRGB.m_ModeList.AddString(RGBModeName[i]);
		}

// Init RateList
		GUIRGB.m_RateList.ResetContent();
		for (i=0; i<100/RateInc; i++)
		{
			char	Str[8];
			sprintf(Str,"%i%",100-(i*RateInc));
			GUIRGB.m_RateList.AddString(Str);
		}

		GUIRGB.m_RSpin.SetRange(0,255);
		GUIRGB.m_GSpin.SetRange(0,255);
		GUIRGB.m_BSpin.SetRange(0,255);
		GUIRGB.EnableCallback();
		Core->RedrawView();
}

/*****************************************************************************/
void	CLayerRGB::GUIKill(CCore *Core)
{

		GUIChanged(Core);
		Core->GUIRemove(GUIRGB,IDD_LAYER_RGB);
}

/*****************************************************************************/
void	CLayerRGB::GUIUpdate(CCore *Core)
{
		GUIRGB.DisableCallback();

		GUIRGB.m_ModeList.SetCurSel(CurrentMode);
		GUIRGB.m_BrushList.SetCurSel(CurrentBrush);
		GUIRGB.m_RateList.SetCurSel(CurrentRate);
		GUIRGB.SetRGB(ShadeRGB.R,ShadeRGB.G,ShadeRGB.B);
		GUIRGB.EnableCallback();
}

/*****************************************************************************/
void	CLayerRGB::GUIChanged(CCore *Core)
{
		CurrentMode=GUIRGB.m_ModeList.GetCurSel();
		CurrentBrush=GUIRGB.m_BrushList.GetCurSel();
		CurrentRate=GUIRGB.m_RateList.GetCurSel();
		GUIRGB.GetRGB(ShadeRGB.R,ShadeRGB.G,ShadeRGB.B);

}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/

bool	CLayerRGB::LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
		if (DownFlag) 
		{
			if (CursorPos.x<0 || CursorPos.x>MapWidth) return(false);
			if (CursorPos.y<0 || CursorPos.y>MapHeight) return(false);
			CreateUndo();
			Paint(Core,CursorPos);
		}
		else
		{
			LastCursPos.x=-1;
			LastCursPos.y=-1;
		}
		return(true);
}

/*****************************************************************************/
bool	CLayerRGB::RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
		if (DownFlag) Grab(Core,CursorPos);
		return(true);
}

/*****************************************************************************/
bool	CLayerRGB::MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)
{
		if (nFlags & MK_LBUTTON)
		{
			Paint(Core,CursorPos);
		}
		return(true);
}

/*****************************************************************************/
void	CLayerRGB::CreateUndo()
{
sRGBUndo	&ThisUndo=UndoList[CurrentUndo];
		for (int y=0; y<MapHeight; y++)
		{
			for (int x=0; x<MapWidth; x++)
			{
				ThisUndo.Map[x][y]=Map[x][y];
			}
		}
		ThisUndo.Valid=true;
		CurrentUndo++;
		if (CurrentUndo>=MAX_UNDO) CurrentUndo=0;
}

/*****************************************************************************/
void	CLayerRGB::Undo()
{
int		Idx=CurrentUndo-1;

		if (Idx<0) Idx+=MAX_UNDO;
sRGBUndo	&ThisUndo=UndoList[Idx];
		if (ThisUndo.Valid)
		{
			CurrentUndo=Idx;
			for (int y=0; y<MapHeight; y++)
			{
				for (int x=0; x<MapWidth; x++)
				{
					Map[x][y]=ThisUndo.Map[x][y];
				}
			}
			ThisUndo.Valid=false;
		}

}
/*****************************************************************************/
void	CLayerRGB::Paint(CCore *Core,CPoint &CursorPos)
{

		if (CursorPos.x== LastCursPos.x && CursorPos.y==LastCursPos.y) return;
		LastCursPos=CursorPos;

sRGBBrush	&ThisBrush=BrushList[CurrentBrush];
std::vector<u8>		&Gfx=ThisBrush.Gfx;
CPoint		CursPos;
int			Ofs=0;

		CursPos.x=CursorPos.x-ThisBrush.XOfs;
		CursPos.y=CursorPos.y-ThisBrush.YOfs;

		for (int Y=0; Y<ThisBrush.H; Y++)
		{
			for (int X=0; X<ThisBrush.W; X++)
			{
				CPoint	Pos=CursPos;
				Pos.x+=X; 
				Pos.y+=Y;
				int	Blk=Gfx[Ofs++];

				if (Blk)
				if (Pos.x>=0 && Pos.x<MapWidth && Pos.y>=0 && Pos.y<MapHeight)
				{
					sRGBElem	MapElem=GetRGB(Pos.x,Pos.y);
					int			RGB;
					int			Rate=100-(CurrentRate*RateInc);
					float		fRate=(1.0/100.f)*Rate;
					int			RGBInc=(int)(fRate*(float)Blk);

					switch(CurrentMode)
					{
					case GUI_MODE_PAINT:
						RGB=RGBInc;
						break;
					case GUI_MODE_LIGHTEN:
						RGB=MapElem.R+RGBInc;
						break;
					case GUI_MODE_DARKEN:
						RGB=MapElem.R-RGBInc;
						break;
					}
					if (RGB<0) RGB=0; else if (RGB>255) RGB=255;
					Map[Pos.x][Pos.y].R=RGB;
					Map[Pos.x][Pos.y].G=RGB;
					Map[Pos.x][Pos.y].B=RGB;
				}
			}
		}

}

/*****************************************************************************/
void	CLayerRGB::Grab(CCore *Core,CPoint &CursorPos)
{
/*
		if (CursorPos.x<0 || CursorPos.x>MapWidth) return;
		if (CursorPos.y<0 || CursorPos.y>MapHeight) return;

		CurrentRGB=Map[CursorPos.x][CursorPos.y];
		GUIUpdate(Core);
*/
}

/*****************************************************************************/
void	CLayerRGB::BiFilter(CCore *Core)
{
		for(int Y=0;Y<MapHeight; Y++)
		{
			for(int X=0; X<MapWidth; X++)
			{	
				int	SCol=0,SCount=0;

//	            *p=(( (*p) + ( ( *(p+1)+*(p-1)+*(p-WIDTH)+*(p+WIDTH) ) >>2 ) )>>1);
				GetFilterCol(Core,X-1,Y+0,SCol,SCount);
				GetFilterCol(Core,X+1,Y+0,SCol,SCount);
				GetFilterCol(Core,X+0,Y-1,SCol,SCount);
				GetFilterCol(Core,X+0,Y+1,SCol,SCount);
				if (SCount)
				{
					SCol/=SCount;
					SetFilterCol(Core,X,Y,SCol,2);
				}
			}
		}
	
}

/*****************************************************************************/
void	CLayerRGB::TriFilter(CCore *Core)
{
		for(int Y=0;Y<MapHeight; Y++)
		{
			for(int X=0; X<MapWidth; X++)
			{	
				int	SCol=0,SCount=0;

//            *p=(( (*p) + ( (*(p+1)+*(p-1)+*(p-WIDTH)+*(p+WIDTH)+*(p-WIDTH-1)+*(p-WIDTH+1)+*(p+WIDTH-1)+*(p+WIDTH+1)) >>3 ) )>>1);
				GetFilterCol(Core,X-1,Y-1,SCol,SCount);
				GetFilterCol(Core,X+0,Y-1,SCol,SCount);
				GetFilterCol(Core,X+1,Y-1,SCol,SCount);
				GetFilterCol(Core,X-1,Y+0,SCol,SCount);
				GetFilterCol(Core,X+1,Y+0,SCol,SCount);
				GetFilterCol(Core,X-1,Y+1,SCol,SCount);
				GetFilterCol(Core,X+0,Y+1,SCol,SCount);
				GetFilterCol(Core,X+1,Y+1,SCol,SCount);
				if (SCount)
				{
					SCol/=SCount;
					SetFilterCol(Core,X,Y,SCol,2);
				}
			}
		}
	
}

/*****************************************************************************/
void	CLayerRGB::STriFilter(CCore *Core)
{
		for(int Y=0;Y<MapHeight; Y++)
		{
			for(int X=0; X<MapWidth; X++)
			{	
				int	SCol=0,SCount=0;
//            c=(( (*p) + ( (*(p+1)+*(p-1)+*(p-WIDTH)+*(p+WIDTH)+*(p-WIDTH-1)+*(p-WIDTH+1)+*(p+WIDTH-1)+*(p+WIDTH+1)) >>3 ) )>>1);
//            *(p-WIDTH-1)=c;
//            *(p-WIDTH+1)=c;
//            *(p+WIDTH-1)=c;
//            *(p+WIDTH+1)=c;

				GetFilterCol(Core,X-1,Y-1,SCol,SCount);
				GetFilterCol(Core,X+0,Y-1,SCol,SCount);
				GetFilterCol(Core,X+1,Y-1,SCol,SCount);
				GetFilterCol(Core,X-1,Y+0,SCol,SCount);
				GetFilterCol(Core,X+1,Y+0,SCol,SCount);
				GetFilterCol(Core,X-1,Y+1,SCol,SCount);
				GetFilterCol(Core,X+0,Y+1,SCol,SCount);
				GetFilterCol(Core,X+1,Y+1,SCol,SCount);
				if (SCount)
				{
					SCol/=SCount;
					SetFilterCol(Core,X-1,Y-1,SCol,1);
					SetFilterCol(Core,X+1,Y-1,SCol,1);
					SetFilterCol(Core,X-1,Y+1,SCol,1);
					SetFilterCol(Core,X+1,Y+1,SCol,1);
				}
			}
		}
	
}

/*****************************************************************************/
void	CLayerRGB::GetFilterCol(CCore *Core,int X,int Y,int &SumCol,int &Count)
{
CLayerTile	*ActionLayer=(CLayerTile*)Core->GetActionLayer();

		if (X>=0 && X<MapWidth && Y>=0 && Y<MapHeight)
		{
			sMapElem	&MapElem=ActionLayer->GetMapElem(X,Y);
			if (MapElem.Tile)
			{
				sRGBElem	&C=Map[X][Y];
				SumCol+=C.R;
				Count++;
			}
		}

}

/*****************************************************************************/
void	CLayerRGB::SetFilterCol(CCore *Core,int X,int Y,int Col,int Div)
{
CLayerTile	*ActionLayer=(CLayerTile*)Core->GetActionLayer();

		if (X>=0 && X<MapWidth && Y>=0 && Y<MapHeight)
		{
			sMapElem	&MapElem=ActionLayer->GetMapElem(X,Y);
			if (MapElem.Tile)
			{
				sRGBElem	&ThisElem=Map[X][Y];
				Col+=ThisElem.R;
				if (Div) Col/=Div;
				ThisElem.R=Col;
				ThisElem.G=Col;
				ThisElem.B=Col;
			}
		}
		
}
/*****************************************************************************/
void	CLayerRGB::Export(CCore *Core,CExport &Exp)
{
sRGBCol		RGB;

		Exp.ExportLayerHeader(LayerDef);//LAYER_TYPE_RGB,LayerDef.SubType,LayerDef.Width,LayerDef.Height);

		RGB.R=ShadeRGB.R;
		RGB.G=ShadeRGB.G;
		RGB.B=ShadeRGB.B;

		Exp.Write(&RGB,sizeof(sRGBCol));

		for (int Y=0; Y<MapHeight; Y++)
		{
			for (int X=0; X<MapWidth; X++)
			{
				sRGBElem	&ThisElem=Map[X][Y];

				RGB.R=ThisElem.R;
				RGB.G=ThisElem.G;
				RGB.B=ThisElem.B;

				Exp.Write(&RGB,sizeof(sRGBCol));
			}
		}
}
