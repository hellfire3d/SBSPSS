/*****************/
/*** Layer RGB ***/
/*****************/


#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"

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
char	*CLayerRGB::RGBModeName[CLayerRGB::GUI_MODE_MAX]={"Paint","Tint","Lighten","Darken"};

u8	BrushGfx1[]=
{
	1
};

u8	BrushGfx2[]=
{
	1,1,
	1,1,
};
u8	BrushGfx3[]=
{
	0,1,0,
	1,2,1,
	0,1,0,
};
u8	BrushGfx4[]=
{
	0,1,1,0,
	1,2,2,1,
	1,2,2,1,
	0,1,1,0,
};
u8	BrushGfx5[]=
{
	0,0,1,0,0,
	0,1,2,1,0,
	1,2,3,2,1,
	0,1,2,1,0,
	0,0,1,0,0,
};
u8	BrushGfx6[]=
{
	0,0,1,1,0,0,
	0,1,2,2,1,0,
	1,2,3,3,2,1,
	1,2,3,3,2,1,
	0,1,2,2,1,0,
	0,0,1,1,0,0,
};
u8	BrushGfx7[]=
{
	0,0,1,1,1,0,0,
	0,1,2,2,2,1,0,
	1,2,3,3,3,2,1,
	1,2,3,4,3,2,1,
	1,2,3,3,3,2,1,
	0,1,2,2,2,1,0,
	0,0,1,1,1,0,0,
};
u8	BrushGfx8[]=
{
	0,0,1,1,1,1,0,0,
	0,1,2,2,2,2,1,0,
	1,2,3,3,3,3,2,1,
	1,2,3,4,4,3,2,1,
	1,2,3,4,4,3,2,1,
	1,2,3,3,3,3,2,1,
	0,1,2,2,2,2,1,0,
	0,0,1,1,1,1,0,0,
};

CLayerRGB::sRGBBrush	CLayerRGB::RGBBrushTable[CLayerRGB::RGB_BRUSH_MAX]=
{
	{1,0,BrushGfx1},
	{2,1,BrushGfx2},
	{3,1,BrushGfx3},
	{4,2,BrushGfx4},
	{5,2,BrushGfx5},
	{6,3,BrushGfx6},
	{7,3,BrushGfx7},
	{8,4,BrushGfx8},
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerRGB::CLayerRGB(sLayerDef &Def)
{
		InitLayer(Def);

		CurrentRGB.R=128;
		CurrentRGB.G=128;
		CurrentRGB.B=128;
		CurrentMode=0;
		CurrentBrush=0;
		ShadeFlag=false;
		LastCursPos.x=-1;
		LastCursPos.y=-1;

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
}
/*****************************************************************************/
void	CLayerRGB::Load(CFile *File,int Version)
{
		InitLayer(LayerDef);

		File->Read(&CurrentRGB,sizeof(sRGBElem));
		File->Read(&CurrentBrush,sizeof(int));
		File->Read(&CurrentMode,sizeof(int));
		File->Read(&ShadeFlag,sizeof(bool));

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
		File->Write(&CurrentRGB,sizeof(sRGBElem));
		File->Write(&CurrentBrush,sizeof(int));
		File->Write(&CurrentMode,sizeof(int));
		File->Write(&ShadeFlag,sizeof(bool));

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
						glColor4f(fR,fG,fB,0.5);
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
sRGBBrush	&ThisBrush=RGBBrushTable[CurrentBrush];

			CursPos.x-=ThisBrush.XYOfs;
			CursPos.y-=ThisBrush.XYOfs;
			CursPos.x-=(int)ThisCam.x;
			CursPos.y-=(int)ThisCam.y;

float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();
float		ShiftX=ThisCam.x - (int)ThisCam.x;
float		ShiftY=ThisCam.y - (int)ThisCam.y;
u8			*Gfx=ThisBrush.Gfx;
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
		glTranslatef(-ShiftX,ShiftY,0);		// Set scroll offset
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner
		glTranslatef(CursPos.x,-CursPos.y,0);	// Bring to top left corner

		for (int YLoop=0; YLoop<ThisBrush.WH; YLoop++)
		{
			for (int XLoop=0; XLoop<ThisBrush.WH; XLoop++)
			{
				u8	B=*Gfx++;
				float	fR,fG,fB,Bi;
				Bi=(1.0f/8.0f)*(float)B;
				
				if (B)
				{
					glLoadName (0);
					glBegin (GL_QUADS); 
						switch(CurrentMode)
						{
						case GUI_MODE_PAINT:
							fR=CurrentRGB.R;
							fG=CurrentRGB.G;
							fB=CurrentRGB.B;
							break;
						case GUI_MODE_TINT:
							fR=((1.0f/255.0f)*CurrentRGB.R)+Bi;
							fG=((1.0f/255.0f)*CurrentRGB.G)+Bi;
							fB=((1.0f/255.0f)*CurrentRGB.B)+Bi;
							break;
						case GUI_MODE_LIGHTEN:
						case GUI_MODE_DARKEN:
							fR=1;
							fG=1;
							fB=1;
							break;
						}
						glColor4f(fR,fG,fB,0.5);
						BuildGLQuad(0,1,0,1,0);
					glEnd();

				}
				glTranslatef(1.0f,0,0);	// Next X
			}
			glTranslatef(-ThisBrush.WH,-1,0); // Next y, rewind to start X
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
	MapWidth=Width;
	MapHeight=Height;
	Map.resize(Width);
	for (int i=0;i<Width;i++)
	{
		Map[i].resize(Height);
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

int		i;

		GUIRGB.DisableCallback();
		Core->GUIAdd(GUIRGB,IDD_LAYER_RGB);

// Init ModeList
		GUIRGB.m_ModeList.ResetContent();
		for (i=0; i<GUI_MODE_MAX; i++)
		{
			GUIRGB.m_ModeList.AddString(RGBModeName[i]);
		}
		GUIRGB.m_RSpin.SetRange(0,255);
		GUIRGB.m_GSpin.SetRange(0,255);
		GUIRGB.m_BSpin.SetRange(0,255);
		GUIRGB.m_BrushSpin.SetRange(0,RGB_BRUSH_MAX-1);

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
		GUIRGB.SetRGB(CurrentRGB.R,CurrentRGB.G,CurrentRGB.B);
		GUIRGB.SetVal(GUIRGB.m_Brush,CurrentBrush);
		GUIRGB.m_Shade.SetCheck(ShadeFlag);
		

		GUIRGB.EnableCallback();
}

/*****************************************************************************/
void	CLayerRGB::GUIChanged(CCore *Core)
{
		CurrentMode=GUIRGB.m_ModeList.GetCurSel();
		GUIRGB.GetRGB(CurrentRGB.R,CurrentRGB.G,CurrentRGB.B);
		GUIRGB.GetVal(GUIRGB.m_Brush,CurrentBrush);
		ShadeFlag=GUIRGB.m_Shade.GetCheck()!=0;
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/

bool	CLayerRGB::LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
		if (DownFlag) 
		{
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
void	CLayerRGB::Paint(CCore *Core,CPoint &CursorPos)
{
		if (CursorPos.x<0 || CursorPos.x>MapWidth) return;
		if (CursorPos.y<0 || CursorPos.y>MapHeight) return;

		if (CursorPos.x== LastCursPos.x && CursorPos.y==LastCursPos.y) return;
		LastCursPos=CursorPos;

sRGBBrush	&ThisBrush=RGBBrushTable[CurrentBrush];
u8			*Gfx=ThisBrush.Gfx;
CPoint		CursPos;

		CursPos.x=CursorPos.x-ThisBrush.XYOfs;
		CursPos.y=CursorPos.y-ThisBrush.XYOfs;

		for (int Y=0; Y<ThisBrush.WH; Y++)
		{
			for (int X=0; X<ThisBrush.WH; X++)
			{
				CPoint	Pos=CursPos;
				Pos.x+=X; 
				Pos.y+=Y;
				int	Blk=*Gfx++;

				if (Blk)
				if (Pos.x>=0 && Pos.x<MapWidth &&
					Pos.y>=0 && Pos.y<MapHeight)
				{
					sRGBElem	MapElem=GetRGB(Pos.x,Pos.y);
					int			R=CurrentRGB.R;
					int			G=CurrentRGB.G;
					int			B=CurrentRGB.B;
					int			Br=(R/8)*Blk;
					int			Bg=(G/8)*Blk;
					int			Bb=(B/8)*Blk;

					switch(CurrentMode)
					{
					case GUI_MODE_PAINT:
						break;
					case GUI_MODE_TINT:
						R=(MapElem.R+Br);
						G=(MapElem.G+Bg);
						B=(MapElem.B+Bb);
						break;
					case GUI_MODE_LIGHTEN:
						R=MapElem.R+4;
						G=MapElem.G+4;
						B=MapElem.B+4;
						break;
					case GUI_MODE_DARKEN:
						R=MapElem.R-4;
						G=MapElem.G-4;
						B=MapElem.B-4;
						break;
					}
					if (R<0) R=0; else if (R>255) R=255;
					if (G<0) G=0; else if (G>255) G=255;
					if (B<0) B=0; else if (B>255) B=255;
					Map[Pos.x][Pos.y].R=R;
					Map[Pos.x][Pos.y].G=G;
					Map[Pos.x][Pos.y].B=B;
				}
			}
		}

}

/*****************************************************************************/
void	CLayerRGB::Grab(CCore *Core,CPoint &CursorPos)
{
		if (CursorPos.x<0 || CursorPos.x>MapWidth) return;
		if (CursorPos.y<0 || CursorPos.y>MapHeight) return;

		CurrentRGB=Map[CursorPos.x][CursorPos.y];
		GUIUpdate(Core);

}
/*****************************************************************************/
void	CLayerRGB::Export(CCore *Core,CExport &Exp)
{
		Exp.ExportLayerHeader(LayerDef);//LAYER_TYPE_RGB,LayerDef.SubType,LayerDef.Width,LayerDef.Height);

int		f=ShadeFlag;
		Exp.Write(&f,sizeof(int));

		for (int Y=0; Y<MapHeight; Y++)
		{
			for (int X=0; X<MapWidth; X++)
			{
				sRGBElem	&ThisElem=Map[X][Y];
				sRGBCol		RGB;

				RGB.R=ThisElem.R;
				RGB.G=ThisElem.G;
				RGB.B=ThisElem.B;

				Exp.Write(&RGB,sizeof(sRGBCol));
			}
		}
}
