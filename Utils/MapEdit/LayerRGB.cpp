/*******************/
/*** Layer Shade ***/
/*******************/


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
#include	"LayerShade.h"
#include	"Utils.h"
#include	"Select.h"
#include	"Export.h"
#include	"GUILayerShade.h"
#include	"Elem.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerShade::CLayerShade(sLayerDef &Def)
{
		InitLayer(Def);
GString	ExecPath;
GString	ScriptName;

		GfxBank=new CElemBank(-1,-1,false,CElem::CentreModeLR | CElem::CentreModeTB);
		LoadGfx();
		Cursor.Gfx=-1;
		Cursor.Ofs[0].x=-2;	Cursor.Ofs[0].y=-2;
		Cursor.Ofs[1].x=+2;	Cursor.Ofs[1].y=-2;
		Cursor.Ofs[2].x=-2;	Cursor.Ofs[2].y=+2;
		Cursor.Ofs[3].x=+2;	Cursor.Ofs[3].y=+2;
		Cursor.TransMode=0;

		for(int i=0; i<4; i++)
		{
			Cursor.RGB[i].R=Cursor.RGB[i].G=Cursor.RGB[i].B=255;
		}
		CurrentGfx=-1;
}


/*****************************************************************************/
CLayerShade::~CLayerShade()
{
		GfxBank->CleanUp();
		delete GfxBank;
}

/*****************************************************************************/
void	CLayerShade::LoadGfx()
{
GString	ExecPath;
GString	ScriptName;
		GetExecPath(ExecPath);
		ScriptName=ExecPath+theApp.GetConfigStr("LayerScript","BackGfxScript");
		Script.LoadAndImport(ScriptName);

int		i,ListSize=Script.GetGroupCount();
		BankList.resize(ListSize);

		for (i=0; i<ListSize; i++)
		{
			sBackList	&ThisGfx=BankList[i];

			char	*Name=Script.GetGroupName(i);
			char	*Gfx=Script.GetStr(Name,"gfx");

			ThisGfx.Name=Name;
			if (Gfx) 
			{
				char	Filename[512];
				GFName::makeabsolute(ExecPath,Gfx,Filename);
				ThisGfx.ElemID=GfxBank->AddSet(Filename);
			}
		}
}

/*****************************************************************************/
void	CLayerShade::InitLayer(sLayerDef &Def)
{
		CLayer::InitLayer(Def);
//		LayerDef.Width=TileLayerMinWidth+(Def.Width-TileLayerMinWidth)/GetScaleFactor();
//		LayerDef.Height=TileLayerMinHeight+(Def.Height-TileLayerMinHeight)/GetScaleFactor();

		ShadeRGB[0].R=255;	ShadeRGB[0].G=255;	ShadeRGB[0].B=255;
		ShadeRGB[1].R=255;	ShadeRGB[1].G=0;		ShadeRGB[1].B=0;
		ShadeRGB[2].R=0;	ShadeRGB[2].G=255;	ShadeRGB[2].B=0;
		ShadeRGB[3].R=0;	ShadeRGB[3].G=0;		ShadeRGB[3].B=255;

		ShadeCount=2;

}
/*****************************************************************************/
void	CLayerShade::Load(CFile *File,int Version)
{
int		i;
		InitLayer(LayerDef);

		File->Read(&ShadeCount,sizeof(int));
		
		if (Version<9)
		{ // GOD I HATE FILE VERSIONS NOW!!
			int		DummyInt;

			for (i=0; i<LAYER_SHADE_RGB_MAX; i++)
			{
				RGBQUAD	RGB;
				File->Read(&DummyInt,sizeof(int));
				File->Read(&RGB,sizeof(RGBQUAD));
				ShadeRGB[i].R=RGB.rgbRed;
				ShadeRGB[i].G=RGB.rgbGreen;
				ShadeRGB[i].B=RGB.rgbBlue;
			}
			File->Read(&DummyInt,sizeof(int));
			File->Read(&DummyInt,sizeof(int));
			if (Version==8)
			{
			File->Read(&DummyInt,sizeof(int));
			File->Read(&DummyInt,sizeof(int));
			File->Read(&DummyInt,sizeof(int));
			File->Read(&DummyInt,sizeof(int));
			}
		}
		else
		{
			int	GfxCount;
			for (i=0; i<LAYER_SHADE_RGB_MAX; i++)
			{
				File->Read(&ShadeRGB[i],sizeof(sRGBCol));
			}
// Load GfxList
			File->Read(&GfxCount,sizeof(int));
			GfxList.resize(GfxCount);
			for (i=0; i<GfxCount; i++)
			{
				sLayerShadeGfx	&ThisGfx=GfxList[i];
				File->Read(&ThisGfx,sizeof(sLayerShadeGfx));
			}
			if (GfxCount) CurrentGfx=0;

		}
}

/*****************************************************************************/
void	CLayerShade::Save(CFile *File)
{
// Always Save current version
		File->Write(&ShadeCount,sizeof(int));
		for (int i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			File->Write(&ShadeRGB[i],sizeof(sRGBCol));
		}
		int	GfxCount=GfxList.size();
		File->Write(&GfxCount,sizeof(int));
		
		for (i=0; i<GfxCount; i++)
		{
			sLayerShadeGfx	&ThisGfx=GfxList[i];
			File->Write(&ThisGfx,sizeof(sLayerShadeGfx));
		}

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerShade::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();
int			ThisCount=ShadeCount-1;
float		X0=0;
float		X1=LayerDef.Width;
float		Y=(0+1);
float		YInc=(float)LayerDef.Height/(float)ThisCount;
int			i,ListSize;

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glScalef(Scale.x,Scale.y,Scale.z);
			glTranslatef(-ThisCam.x,ThisCam.y,0);		// Set scroll offset
			glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

			glBegin (GL_QUADS);
			for (i=0; i<ThisCount; i++)
			{
					glColor3ub(ShadeRGB[i+0].R,ShadeRGB[i+0].G,ShadeRGB[i+0].B);
					glVertex3f( X0,Y,0.0f);
					glVertex3f( X1,Y,0.0f);
					Y-=YInc;
					glColor3ub(ShadeRGB[i+1].R,ShadeRGB[i+1].G,ShadeRGB[i+1].B);
					glVertex3f( X1,Y,0.0f);
					glVertex3f( X0,Y,0.0f);
			}
			glEnd();
			glPopMatrix();
// Render Gfx
		ListSize=GfxList.size();
		for (i=0; i<ListSize; i++)
		{
			sLayerShadeGfx	&ThisGfx=GfxList[i];
			RenderBackGfx(Core,ThisCam,ThisGfx);
		}

}


/*****************************************************************************/
void	CLayerShade::RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
CPoint		&CursPos=Core->GetCursorPos();

		Cursor.Pos.x=CursPos.x;
		Cursor.Pos.y=CursPos.y;
		if (CursPos.x<0 || CursPos.y<0) return;
		if (Cursor.Gfx==-1) return;
		RenderBackGfx(Core,ThisCam,Cursor);
}

/*****************************************************************************/
void	CLayerShade::RenderBackGfx(CCore *Core,Vector3 &ThisCam,sLayerShadeGfx &ThisGfx)
{
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
Vector3		&Scale=Core->GetScaleVector();
Vector3		ScrOfs(ZoomW/2,ZoomH/2,0);
int			i;

CElem		Elem=GfxBank->GetElem(ThisGfx.Gfx,0);

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glScalef(Scale.x,Scale.y,Scale.z);
			glTranslatef(-ThisCam.x,ThisCam.y,0);					// Set scroll offset
			glTranslatef(-ScrOfs.x,ScrOfs.y,0);						// Bring to top left corner

			glTranslatef(ThisGfx.Pos.x,-ThisGfx.Pos.y,0);		// Set Pos
			glColor3ub(255,255,255);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Elem.GetTexID());
			glBegin (GL_QUADS);
				i=0; glColor3ub(ThisGfx.RGB[i].R,ThisGfx.RGB[i].G,ThisGfx.RGB[i].B); glTexCoord2f(0.0f,1.0f); glVertex3f(ThisGfx.Ofs[i].x ,-ThisGfx.Ofs[i].y,0.0f);
				i=1; glColor3ub(ThisGfx.RGB[i].R,ThisGfx.RGB[i].G,ThisGfx.RGB[i].B); glTexCoord2f(1.0f,1.0f); glVertex3f(ThisGfx.Ofs[i].x ,-ThisGfx.Ofs[i].y,0.0f);
				i=3; glColor3ub(ThisGfx.RGB[i].R,ThisGfx.RGB[i].G,ThisGfx.RGB[i].B); glTexCoord2f(1.0f,0.0f); glVertex3f(ThisGfx.Ofs[i].x ,-ThisGfx.Ofs[i].y,0.0f);
				i=2; glColor3ub(ThisGfx.RGB[i].R,ThisGfx.RGB[i].G,ThisGfx.RGB[i].B); glTexCoord2f(0.0f,0.0f); glVertex3f(ThisGfx.Ofs[i].x ,-ThisGfx.Ofs[i].y,0.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
}

/*****************************************************************************/
void	CLayerShade::CheckLayerSize(int Width,int Height)
{
		if (Resize(Width,Height))
		{
			CString mexstr;
			mexstr.Format("%s Layer Resized to Correct Size\nPlease re-save\n", GetName());
			AfxMessageBox(mexstr,MB_OK | MB_ICONEXCLAMATION);
		}
}

/*****************************************************************************/
bool	CLayerShade::Resize(int Width,int Height)
{

		Width=TileLayerMinWidth+(Width-TileLayerMinWidth)/GetScaleFactor();
		Height=TileLayerMinHeight+(Height-TileLayerMinHeight)/GetScaleFactor();
		if (LayerDef.Width!=Width || LayerDef.Height!=Height)
		{
			LayerDef.Width=Width;
			LayerDef.Height=Height;
			return(true);
		}

		return(false);
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerShade::GUIInit(CCore *Core)
{
int		i,ListSize;

		GUIShade.DisableCallback();
		Core->GUIAdd(GUIShade,IDD_LAYER_SHADE);

// Init BankList
		ListSize=BankList.size();
		GUIShade.m_GfxBankList.ResetContent();
		GUIShade.m_DefList.ResetContent();
		for (i=0; i<ListSize; i++)
		{
			GUIShade.m_GfxBankList.AddString(BankList[i].Name);
			GUIShade.m_DefList.AddString(BankList[i].Name);
		}

		GUIShade.m_GfxTransList.ResetContent();
		GUIShade.m_GfxTransList.AddString("Normal");
		GUIShade.m_GfxTransList.AddString("50%");
		GUIShade.m_GfxTransList.AddString("Subtractive");
		GUIShade.m_GfxTransList.AddString("Another one");

		GUIShade.m_GfxPosXSpin.SetRange(0,32000); GUIShade.m_GfxPosYSpin.SetRange(0,32000);
		GUIShade.m_GfxSpinx0.SetRange(-32,+32);	GUIShade.m_GfxSpiny0.SetRange(-32,+32);
		GUIShade.m_GfxSpinx1.SetRange(-32,+32);	GUIShade.m_GfxSpiny1.SetRange(-32,+32);
		GUIShade.m_GfxSpinx2.SetRange(-32,+32);	GUIShade.m_GfxSpiny2.SetRange(-32,+32);
		GUIShade.m_GfxSpinx3.SetRange(-32,+32);	GUIShade.m_GfxSpiny3.SetRange(-32,+32);
		GUIShade.EnableCallback();
		Core->RedrawView();
}

/*****************************************************************************/
void	CLayerShade::GUIKill(CCore *Core)
{
		GUIChanged(Core);
		Core->GUIRemove(GUIShade,IDD_LAYER_SHADE);
}

/*****************************************************************************/
void	CLayerShade::GUIUpdate(CCore *Core)
{
int		i,ListSize;

// Shades
		GUIShade.DisableCallback();

		for (i=0; i<LAYER_SHADE_MAX; i++)
		{
			GUIShade.SetShadeRGB(i,ShadeRGB[i].R,ShadeRGB[i].G,ShadeRGB[i].B);
		}
		GUIShade.m_ShadeCountSpin.SetRange(2,4);
		GUIShade.SetVal(GUIShade.m_ShadeCount,ShadeCount,2,4);

// Gfx
		ListSize=GfxList.size();
		if (!ListSize)
		{
			GUIShade.m_GfxCurrent.SetWindowText("");
			GUIShade.m_GfxCurrentSpin.SetRange(0,0);
			return;
		}

sLayerShadeGfx	&ThisGfx=GfxList[CurrentGfx];

		GUIShade.m_GfxCurrentSpin.SetRange(0,ListSize);
		GUIShade.SetVal(GUIShade.m_GfxCurrent,CurrentGfx,0,ListSize-1);
		GUIShade.SetVal(GUIShade.m_GfxPosX,ThisGfx.Pos.x);
		GUIShade.SetVal(GUIShade.m_GfxPosY,ThisGfx.Pos.y);
		GUIShade.m_GfxBankList.SetCurSel(ThisGfx.Gfx);
		
		for ( i=0;i<4; i++)
		{
			GUIShade.SetGfxRGB(i,ThisGfx.RGB[i].R,ThisGfx.RGB[i].G,ThisGfx.RGB[i].B);
		}

		GUIShade.SetVal(GUIShade.m_Gfxx0,ThisGfx.Ofs[0].x); GUIShade.SetVal(GUIShade.m_Gfxy0,ThisGfx.Ofs[0].y);
		GUIShade.SetVal(GUIShade.m_Gfxx1,ThisGfx.Ofs[1].x); GUIShade.SetVal(GUIShade.m_Gfxy1,ThisGfx.Ofs[1].y);
		GUIShade.SetVal(GUIShade.m_Gfxx2,ThisGfx.Ofs[2].x); GUIShade.SetVal(GUIShade.m_Gfxy2,ThisGfx.Ofs[2].y);
		GUIShade.SetVal(GUIShade.m_Gfxx3,ThisGfx.Ofs[3].x); GUIShade.SetVal(GUIShade.m_Gfxy3,ThisGfx.Ofs[3].y);

		GUIShade.m_GfxTransList.SetCurSel(ThisGfx.TransMode);
		GUIShade.EnableCallback();
}

/*****************************************************************************/
void	CLayerShade::GUIChanged(CCore *Core)
{
int		i,ListSize;
int		LastGfx=CurrentGfx;
// Shade
		for (i=0; i<LAYER_SHADE_MAX; i++) 
		{
			GUIShade.GetShadeRGB(i,ShadeRGB[i].R,ShadeRGB[i].G,ShadeRGB[i].B);
		}
		GUIShade.GetVal(GUIShade.m_ShadeCount,ShadeCount,2,4);
// Gfx
		ListSize=GfxList.size();
		GUIShade.GetVal(GUIShade.m_GfxCurrent,ListSize,0,ListSize);
		if (!ListSize || CurrentGfx==-1) return;

sLayerShadeGfx	&ThisGfx=GfxList[CurrentGfx];
		
		GUIShade.GetVal(GUIShade.m_GfxPosX,ThisGfx.Pos.x);
		GUIShade.GetVal(GUIShade.m_GfxPosY,ThisGfx.Pos.y);
		ThisGfx.Gfx=GUIShade.m_GfxBankList.GetCurSel();
		
		for ( i=0;i<4; i++)
		{
			GUIShade.GetGfxRGB(i,ThisGfx.RGB[i].R,ThisGfx.RGB[i].G,ThisGfx.RGB[i].B);
		}
		GUIShade.GetVal(GUIShade.m_Gfxx0,ThisGfx.Ofs[0].x); GUIShade.GetVal(GUIShade.m_Gfxy0,ThisGfx.Ofs[0].y);
		GUIShade.GetVal(GUIShade.m_Gfxx1,ThisGfx.Ofs[1].x); GUIShade.GetVal(GUIShade.m_Gfxy1,ThisGfx.Ofs[1].y);
		GUIShade.GetVal(GUIShade.m_Gfxx2,ThisGfx.Ofs[2].x); GUIShade.GetVal(GUIShade.m_Gfxy2,ThisGfx.Ofs[2].y);
		GUIShade.GetVal(GUIShade.m_Gfxx3,ThisGfx.Ofs[3].x); GUIShade.GetVal(GUIShade.m_Gfxy3,ThisGfx.Ofs[3].y);
		ThisGfx.TransMode=GUIShade.m_GfxTransList.GetCurSel();

		if (CurrentGfx!=LastGfx)
		{
			GUIUpdate(Core);
		}
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/

bool	CLayerShade::LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
		if (DownFlag) AddGfx(Core);
		return(true);
}

/*****************************************************************************/
bool	CLayerShade::RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
		if (DownFlag) Cursor.Gfx=-1;
		return(true);
}

/*****************************************************************************/
bool	CLayerShade::MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)
{
		return(true);
}

/*****************************************************************************/
bool	CLayerShade::Command(int CmdMsg,CCore *Core,int Param0,int Param1)
{
bool	Ret=false;

		switch(CmdMsg)
		{
		case CmdMsg_ShadeGfxNew:
			Cursor.Gfx=Param0;
			GUIShade.m_DefList.SetCurSel(-1);
			break;
		case CmdMsg_ShadeGfxSelect:
			GUIShade.GetVal(GUIShade.m_GfxCurrent,CurrentGfx);
			if (CurrentGfx>GfxList.size()-1) CurrentGfx=GfxList.size()-1;
			GUIUpdate(Core);
			break;
		case CmdMsg_ShadeGfxGoto:
			GotoGfx(Core);
			break;
		case CmdMsg_ShadeGfxDelete:
			DeleteGfx(Core);
			break;
		default:
			break;
		}

		return(Ret);

}

/*****************************************************************************/
void	CLayerShade::AddGfx(CCore *Core)
{
	if (Cursor.Gfx==-1) return;

	CurrentGfx=GfxList.size();
	GfxList.push_back(Cursor);

	Cursor.Gfx=-1;
	GUIUpdate(Core);
}

/*****************************************************************************/
void	CLayerShade::GotoGfx(CCore *Core)
{
		if (CurrentGfx==-1) return;
sLayerShadeGfx	&ThisGfx=GfxList[CurrentGfx];
CPoint		Pos;
			Pos.x=ThisGfx.Pos.x;
			Pos.y=ThisGfx.Pos.y;
			Core->SetCamPos(Pos);
}

/*****************************************************************************/
void	CLayerShade::DeleteGfx(CCore *Core)
{
		if (!GfxList.size()) return;

		GfxList.erase(CurrentGfx);
		if (CurrentGfx) CurrentGfx--;
		GUIUpdate(Core);

}

/*****************************************************************************/
void	CLayerShade::Export(CCore *Core,CExport &Exp)
{
int		i,ListSize;

		Exp.ExportLayerHeader(LayerDef);//LAYER_TYPE_SHADE,LayerDef.SubType,LayerDef.Width,LayerDef.Height);
		Exp.Write(&ShadeCount,sizeof(int));
		for (i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			Exp.Write(&ShadeRGB[i],sizeof(sRGBCol));
		}

// Write Gfx List
		ListSize=GfxList.size();
		Exp.Write(&ListSize,sizeof(int));

		for (i=0; i<ListSize; i++)
		{
			sLayerShadeGfx	&ThisGfx=GfxList[i];
			Exp.Write(&ThisGfx,sizeof(sLayerShadeGfx));
		}
// Write Gfx Names
		ListSize=BankList.size();
		Exp.Write(&ListSize,sizeof(int));
		for (i=0; i<ListSize; i++)
		{
			char	Txt[256];
			sprintf(Txt,BankList[i].Name);
			Exp.Write(Txt,strlen(Txt)+1);
		}
}
