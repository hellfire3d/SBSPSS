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


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerShade::CLayerShade(sLayerDef &Def)
{
		InitLayer(Def);
// Load script (messy)
GString	ExecPath;
GString	ScriptName;

		GetExecPath(ExecPath);
		ScriptName=ExecPath+theApp.GetConfigStr("LayerScript","BackGfxScript");
		Script.LoadAndImport(ScriptName);

int		i,ListSize=Script.GetGroupCount();
		BackGfx.resize(ListSize);

		for (i=0; i<ListSize; i++)
		{
			BackGfx[i]=Script.GetGroupName(i);
			TRACE1("%s\n",BackGfx[i]);
		}
		Back0=Back1=-1;
		TransMode0=TransMode1=0;
		Flags0=Flags1=-1;
}


/*****************************************************************************/
CLayerShade::~CLayerShade()
{
}

/*****************************************************************************/
void	CLayerShade::InitLayer(sLayerDef &Def)
{
		CLayer::InitLayer(Def);
		LayerDef.Width=TileLayerMinWidth+(Def.Width-TileLayerMinWidth)/GetScaleFactor();
		LayerDef.Height=TileLayerMinHeight+(Def.Height-TileLayerMinHeight)/GetScaleFactor();

		RGB[0].rgbRed=255;	RGB[0].rgbGreen=255;	RGB[0].rgbBlue=255;
		RGB[1].rgbRed=255;	RGB[1].rgbGreen=0;		RGB[1].rgbBlue=0;
		RGB[2].rgbRed=0;	RGB[2].rgbGreen=255;	RGB[2].rgbBlue=0;
		RGB[3].rgbRed=0;	RGB[3].rgbGreen=0;		RGB[3].rgbBlue=255;
		Count=2;

}
/*****************************************************************************/
void	CLayerShade::Load(CFile *File,int Version)
{
		if (Version<=5)
		{
			BOOL	DB;
			float	DF;
			LayerDef.Type=LAYER_TYPE_SHADE;
			File->Read(&DB,sizeof(BOOL));
			File->Read(&DF,sizeof(float));
			File->Read(&DB,sizeof(BOOL));
			File->Read(&LayerDef.VisibleFlag,sizeof(BOOL));
			File->Read(&LayerDef.SubType,sizeof(int));
			File->Read(&LayerDef.Width,sizeof(int));
			File->Read(&LayerDef.Height,sizeof(int));
		}
		InitLayer(LayerDef);
		File->Read(&Count,sizeof(int));
		for (int i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			File->Read(&Pos[i],sizeof(int));
			File->Read(&RGB[i],sizeof(RGBQUAD));
		}
		if (Version>=7)
		{
			File->Read(&Back0,sizeof(int));
			File->Read(&Back1,sizeof(int));
		}
		if (Version>=8)
		{
			File->Read(&TransMode0,sizeof(int));
			File->Read(&Flags0,sizeof(int));
			File->Read(&TransMode1,sizeof(int));
			File->Read(&Flags1,sizeof(int));
		}

}

/*****************************************************************************/
void	CLayerShade::Save(CFile *File)
{
// Always Save current version
		File->Write(&Count,sizeof(int));
		for (int i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			File->Write(&Pos[i],sizeof(int));
			File->Write(&RGB[i],sizeof(RGBQUAD));
		}
		File->Write(&Back0,sizeof(int));
		File->Write(&Back1,sizeof(int));
		File->Write(&TransMode0,sizeof(int));
		File->Write(&Flags0,sizeof(int));
		File->Write(&TransMode1,sizeof(int));
		File->Write(&Flags1,sizeof(int));
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
int			ThisCount=Count-1;
float		X0=0;
float		X1=LayerDef.Width;
float		Y=(0+1);

float		YInc=(float)LayerDef.Height/(float)ThisCount;

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glScalef(Scale.x,Scale.y,Scale.z);
			glTranslatef(-ThisCam.x,ThisCam.y,0);		// Set scroll offset
			glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

			glBegin (GL_QUADS);
			for (int i=0; i<ThisCount; i++)
			{
					glColor3ub(RGB[i+0].rgbRed,RGB[i+0].rgbGreen,RGB[i+0].rgbBlue);
					glVertex3f( X0,Y,0.0f);
					glVertex3f( X1,Y,0.0f);
					Y-=YInc;
					glColor3ub(RGB[i+1].rgbRed,RGB[i+1].rgbGreen,RGB[i+1].rgbBlue);
					glVertex3f( X1,Y,0.0f);
					glVertex3f( X0,Y,0.0f);
			}
			glEnd();
		glPopMatrix();
}


/*****************************************************************************/
bool	CLayerShade::Resize(int Width,int Height)
{
		LayerDef.Width=TileLayerMinWidth+(Width-TileLayerMinWidth)/GetScaleFactor();
		LayerDef.Height=TileLayerMinHeight+(Height-TileLayerMinHeight)/GetScaleFactor();

		return(true);
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerShade::GUIInit(CCore *Core)
{
		Core->GUIAdd(GUI,IDD_LAYER_SHADE);
		for (int i=0; i<LAYER_SHADE_MAX; i++) 
		{
			GUI.SetRGB(RGB[i],i);
		}
		GUI.SetCount(Count);
		InitGfxList();

		if (Flags0 & SpinFlag)	GUI.m_Spin0.SetCheck(true);
		if (Flags0 & ScaleFlag)	GUI.m_Scale0.SetCheck(true);
		if (Flags0 & MoveFlag)	GUI.m_Move0.SetCheck(true);
		if (Flags0 & ColorFlag)	GUI.m_Color0.SetCheck(true);

		if (Flags1 & SpinFlag)	GUI.m_Spin1.SetCheck(true);
		if (Flags1 & ScaleFlag)	GUI.m_Scale1.SetCheck(true);
		if (Flags1 & MoveFlag)	GUI.m_Move1.SetCheck(true);
		if (Flags1 & ColorFlag)	GUI.m_Color1.SetCheck(true);

		GUI.m_Trans0.ResetContent();
		GUI.m_Trans0.AddString("Normal");
		GUI.m_Trans0.AddString("50%");
		GUI.m_Trans0.AddString("Subtractive");
		GUI.m_Trans0.AddString("Another one");
		GUI.m_Trans0.SetCurSel(TransMode0);

		GUI.m_Trans1.ResetContent();
		GUI.m_Trans1.AddString("Normal");
		GUI.m_Trans1.AddString("50%");
		GUI.m_Trans1.AddString("Subtractive");
		GUI.m_Trans1.AddString("Another one");
		GUI.m_Trans1.SetCurSel(TransMode1);

}

/*****************************************************************************/
void	CLayerShade::InitGfxList()
{
int		i,ListSize=BackGfx.size();
CComboBox	&List0=GUI.m_Gfx0;
CComboBox	&List1=GUI.m_Gfx1;

		List0.ResetContent();
		List1.ResetContent();
		for (i=0; i<ListSize; i++)
		{
			List0.AddString(BackGfx[i]);
			List1.AddString(BackGfx[i]);
		}
		List0.SetCurSel(Back0);
		List1.SetCurSel(Back1);
}

/*****************************************************************************/
void	CLayerShade::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUI,IDD_LAYER_SHADE);
}

/*****************************************************************************/
void	CLayerShade::GUIUpdate(CCore *Core)
{
		GUI.CheckData();
}

/*****************************************************************************/
void	CLayerShade::GUIChanged(CCore *Core)
{
		for (int i=0; i<LAYER_SHADE_MAX; i++) 
		{
			GUI.GetRGB(RGB[i],i);
		}
		GUI.GetCount(Count);
		Back0=GUI.m_Gfx0.GetCurSel();
		Back1=GUI.m_Gfx1.GetCurSel();

		Flags0=0;
		if (GUI.m_Spin0.GetCheck())		Flags0|=SpinFlag;
		if (GUI.m_Scale0.GetCheck())	Flags0|=ScaleFlag;
		if (GUI.m_Move0.GetCheck())		Flags0|=MoveFlag;
		if (GUI.m_Color0.GetCheck())	Flags0|=ColorFlag;
		TransMode0=GUI.m_Trans0.GetCurSel();

		Flags1=0;
		if (GUI.m_Spin1.GetCheck())		Flags1|=SpinFlag;
		if (GUI.m_Scale1.GetCheck())	Flags1|=ScaleFlag;
		if (GUI.m_Move1.GetCheck())		Flags1|=MoveFlag;
		if (GUI.m_Color1.GetCheck())	Flags1|=ColorFlag;
		TransMode1=GUI.m_Trans1.GetCurSel();

}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
void	CLayerShade::Export(CCore *Core,CExport &Exp)
{
		Exp.ExportLayerHeader(LayerDef);//LAYER_TYPE_SHADE,LayerDef.SubType,LayerDef.Width,LayerDef.Height);
		Exp.Write(&Count,sizeof(int));
		for (int i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			Exp.Write(&Pos[i],sizeof(int));
			Exp.Write(&RGB[i].rgbRed,sizeof(u8));
			Exp.Write(&RGB[i].rgbGreen,sizeof(u8));
			Exp.Write(&RGB[i].rgbBlue,sizeof(u8));
			Exp.Write(&RGB[i].rgbRed,sizeof(u8));	// Pad
		}

		Exp.Write(&TransMode0,sizeof(int));
		Exp.Write(&Flags0,sizeof(int));
		Exp.Write(&TransMode1,sizeof(int));
		Exp.Write(&Flags1,sizeof(int));

// Back Gfx
char	Txt[256];

		Txt[0]=0; 
		if (Back0!=-1) sprintf(Txt,BackGfx[Back0]);
		Exp.Write(Txt,strlen(Txt)+1);		
		
		Txt[0]=0; 
		if (Back1!=-1) sprintf(Txt,BackGfx[Back1]);
		Exp.Write(Txt,strlen(Txt)+1);		

}
