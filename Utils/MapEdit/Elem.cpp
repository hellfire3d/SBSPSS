/*********************/
/*** Elem Stuph ***/
/*********************/

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"
#include	<Vector>
#include	<GFName.hpp>

#include	"Core.h"
#include	"TexCache.h"
#include	"Elem.h"
#include	"GinTex.h"
#include	"utils.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"

#include	"GUITileBank.h"


const Vector3	DefOfs(+0.5f,0,+4.0f);
int		CElem::BlankID=-1;
int		CElem::InvalidID=-1;
bool	CElem::DefTexFlag=false;

/*****************************************************************************/
// All Elems MUST run from 0,0 ->

/*****************************************************************************/
const float	ElemBrowserGap=0.2f;
const float	ElemBrowserX0=0-ElemBrowserGap/2;
const float	ElemBrowserX1=1+ElemBrowserGap/2;
const float	ElemBrowserY0=0-ElemBrowserGap/2;
const float	ElemBrowserY1=1+ElemBrowserGap/2;

/*****************************************************************************/
const float	XFlipMtx[]=
{
-1, 0, 0, 0,
 0, 1, 0, 0,
 0, 0, 1, 0,
 0, 0, 0, 1,
};

const float	YFlipMtx[]=
{
 1, 0, 0, 0,
 0,-1, 0, 0,
 0, 0, 1, 0,
 0, 0, 0, 1,
};

/*****************************************************************************/
CElem::CElem(int Width,int Height)
{
		ElemWidth=Width;
		ElemHeight=Height;
		UnitWidth=ElemWidth/UnitSize;
		UnitHeight=ElemHeight/UnitSize;
		ElemID=-1;
int		AW=AlignSize(ElemWidth);
int		AH=AlignSize(ElemHeight);
		ElemRGB=0;
		ElemRGB=(u8*)MemAlloc(AW*AH*3);
		memset(ElemRGB,0,AW*AH*3);
		Type=ElemType2d;
		TexXOfs=0;
		TexYOfs=0;
		BlankFlag=true;
		ElemName="BLANK";

}

/*****************************************************************************/
// 3d Elem (From Gin File)
CElem::CElem(CCore *Core,const char *Filename,CScene &ThisScene,int Node)
{
CNode		&ThisNode=ThisScene.GetNode(Node);
CTexCache	&TexCache=Core->GetTexCache();
GFName		Path=Filename;

			SetPath=Path.Drive();
			SetPath+=Path.Dir();
			SetPath.Append('\\');

			Type=ElemType3d;
			TexXOfs=-1;
			TexYOfs=-1;
			ElemRGB=0;
			Ofs.Zero();
			Build3dElem(TexCache,ThisScene,Node);
			Calc3dSize();
			Build3dDrawList(TexCache,DrawList[ElemType3d]);
			Ofs.Zero();
			Create2dTexture(TexCache,Path.File(),Node);
			Build2dDrawList(TexCache,DrawList[ElemType2d]);
			BlankFlag=false;
//			if (!ValidFlag)	SetInvalid();
			ElemName=ThisScene.GetNode(Node).Name;
}

/*****************************************************************************/
// 2d Elem (From Bmp File)
// 0,0 1,0 1,1 0,1
CElem::CElem(CCore *Core,const char *Filename,int TexID,int XOfs,int YOfs,int Width,int Height,int CentreMode)
{
CTexCache	&TexCache=Core->GetTexCache();
GFName		Path=Filename;

			ElemWidth=Width;
			ElemHeight=Height;
			UnitWidth=ElemWidth/UnitSize;
			UnitHeight=ElemHeight/UnitSize;
			ElemRGB=0;

			Type=ElemType2d;
			TexXOfs=XOfs;
			TexYOfs=YOfs;
			Ofs.Zero();

			if (CentreMode & CentreModeL)
			{
				// Nothing to do, already there
			}
			if (CentreMode & CentreModeR)
			{
				Ofs.x-=UnitWidth;
			}

			if (CentreMode & CentreModeT)
			{
				Ofs.y-=UnitHeight;
			}
			if (CentreMode & CentreModeB)
			{
				// Nothing to do, already there
			}

			if (CentreMode & CentreModeLR)
			{
				Ofs.x-=(UnitWidth-1.0f)/2;
			}
			if (CentreMode & CentreModeTB)
			{
				Ofs.y-=(UnitHeight-1.0f)/2;
			}
			
			Build2dElem(Core,Path.File(),TexID);
			Build3dDrawList(TexCache,DrawList[ElemType3d]);
			Create2dTexture(TexCache,Path.File(),TexID);
			Build2dDrawList(TexCache,DrawList[ElemType2d]);
			BlankFlag=false;
			if (!ValidFlag)	SetInvalid();
}

/*****************************************************************************/
void	CElem::CleanUp()
{
		if (ElemRGB) MemFree(ElemRGB);
		ElemRGB=0;
}

/*****************************************************************************/
void	CElem::SetBlank()
{
		for (int i=0; i<ElemTypeMax; i++) DrawList[i]=BlankID;
}

/*****************************************************************************/
void	CElem::SetInvalid()
{
		Purge();	// Clear whatever is already there
		for (int i=0; i<ElemTypeMax; i++) DrawList[i]=InvalidID;
}

/*****************************************************************************/
void	CElem::Build2dElem(CCore *Core,const char *Filename,int TexID)
{
float		X0=0;
float		X1=+UnitWidth;
float		Y0=0;
float		Y1=+UnitHeight;

Vector3		P0(X0,Y0,0);
Vector3		P1(X1,Y0,0);
Vector3		P2(X0,Y1,0);
Vector3		P3(X1,Y1,0);

int			ListSize=TriList.size();
			TriList.resize(ListSize+2);
sTriFace	&Tri0=TriList[ListSize+0];
sTriFace	&Tri1=TriList[ListSize+1];

CTexCache	&TexCache=Core->GetTexCache();
sTex		&ThisTex=TexCache.GetTex(TexID);

int			CountW=ThisTex.OldW/ElemWidth;
int			CountH=ThisTex.OldH/ElemHeight;
float		dU=ThisTex.ScaleU/(float)CountW;
float		dV=ThisTex.ScaleV/(float)CountH;

			dU=(1.0f/CountW);
			dV=(1.0f/CountH);

//			dU=ThisTex.ScaleU;
//			dV=ThisTex.ScaleV;

float		u0=(TexXOfs*dU);
float		u1=u0+dU;

float		v1=1.0-(TexYOfs*dV);
float		v0=v1-dV;

			Tri0.Mat=TexID;

			Tri0.vtx[0]=P0;
			Tri0.vtx[1]=P1;
			Tri0.vtx[2]=P2;
			Tri0.uvs[0].u=u0; Tri0.uvs[0].v=v0;
			Tri0.uvs[1].u=u1; Tri0.uvs[1].v=v0;
			Tri0.uvs[2].u=u0; Tri0.uvs[2].v=v1;

			Tri1.Mat=TexID;
			Tri1.vtx[0]=P1;
			Tri1.vtx[1]=P2;
			Tri1.vtx[2]=P3;
			Tri1.uvs[0].u=u1; Tri1.uvs[0].v=v0;
			Tri1.uvs[1].u=u0; Tri1.uvs[1].v=v1;
			Tri1.uvs[2].u=u1; Tri1.uvs[2].v=v1;
}

/*****************************************************************************/
void	CElem::Build3dElem(CTexCache &TexCache,CScene &ThisScene,int Node)
{
CNode						&ThisNode=ThisScene.GetNode(Node);
CNode						&ParentNode=ThisScene.GetNode(ThisNode.ParentIdx);
int							ChildCount=ThisNode.GetPruneChildCount();
std::vector<sGinTri> const	&NodeTriList=ThisNode.GetTris();
std::vector<Vector3>const	&NodeVtxList=ThisNode.GetPts();
std::vector<sUVTri>	const	&NodeUVList=ThisNode.GetUVTris();
std::vector<int>	const	&NodeTriMat=ThisNode.GetTriMaterial();

std::vector<GString> const	&SceneTexList=ThisScene.GetTexList();
std::vector<int> const		&SceneUsedMatList=ThisScene.GetUsedMaterialIdx();
std::vector<Material> const &SceneMatList=ThisScene.GetMaterials();
int							TexCount=SceneTexList.size();

int			TriCount=NodeTriList.size();
int			ListSize=TriList.size();

			TriList.resize(ListSize+TriCount);
			for (int T=0; T<TriCount; T++)
			{
				sGinTri	const	&ThisTri=NodeTriList[T];
				sUVTri	const	&ThisUV=NodeUVList[T];
				sTriFace		&Tri=TriList[ListSize+T];
				int				ThisMat=NodeTriMat[T];
				int				TexID,TexFlags;
				

// Sort Textures - Only add the ones that are used :o)
				TexID=SceneUsedMatList[ThisMat];
				if (TexID<0 || TexID>=TexCount)
				{
					CString mexstr;
					mexstr.Format("Invalid TexId\n Wanted %i only have %i Id's for %s\nThis is gonna hurt!\n Dont ask me about this error, ask Kev.\n This MUST be fixed.",TexID,TexCount-1,ThisNode.Name);
					AfxMessageBox(mexstr,MB_OK | MB_ICONEXCLAMATION);
					TexID=0;
				}
//				else
				{
				TexFlags=SceneMatList[TexID].Flags;
				GString	ThisName;
				GString	TexName=SceneTexList[TexID];
				
				ThisName=SetPath+TexName;
//				TRACE2("%i !%s!\n",TexID,ThisName);
				TexID=TexCache.ProcessTexture(ThisName);
				}
// Sort Rest of Tri info
Matrix4x4		TransMtx;
				TransMtx.Identity();
				
				if (ParentNode.GetTris().size() || !ThisNode.ParentIdx)
				{
					TransMtx=ThisNode.Mtx;
					TransMtx.Invert();
				}

				for (int p=0; p<3; p++)
				{
					Tri.vtx[p]=TransMtx*NodeVtxList[ThisTri.p[p]];
					Tri.uvs[p].u=ThisUV.p[p].u;
					Tri.uvs[p].v=ThisUV.p[p].v;
					Tri.Mat=TexID;
					Tri.Flags=TexFlags;
				}
			}
		for (int Child=0; Child<ChildCount; Child++) Build3dElem(TexCache,ThisScene,ThisNode.PruneChildList[Child]);

}

/*****************************************************************************/
void	CElem::Calc3dSize()
{
int			i,ListSize=TriList.size();
Vector3		Min(+32000,+32000,+32000);
Vector3		Max(-32000,-32000,-32000);
Vector3		Mid;

		for (i=0; i<ListSize; i++)
		{
			sTriFace		&Tri=TriList[i];
			for (int p=0; p<3; p++)
			{
				if (Min.x>Tri.vtx[p].x) Min.x=Tri.vtx[p].x;
				if (Min.y>Tri.vtx[p].y) Min.y=Tri.vtx[p].y;
				if (Min.z>Tri.vtx[p].z) Min.z=Tri.vtx[p].z;
				if (Max.x<Tri.vtx[p].x) Max.x=Tri.vtx[p].x;
				if (Max.y<Tri.vtx[p].y) Max.y=Tri.vtx[p].y;
				if (Max.z<Tri.vtx[p].z) Max.z=Tri.vtx[p].z;
			}
		}

		UnitWidth=round(Max.x-Min.x);
		UnitHeight=round(Max.y-Min.y);
		if (UnitWidth<1) UnitWidth=1;
		if (UnitHeight<1) UnitHeight=1;
		ElemWidth=UnitWidth*UnitSize;
		ElemHeight=UnitHeight*UnitSize;
		Min.z=0;
		
		Ofs=DefOfs;
//		Ofs.x=+0.5f; Ofs.y=0; Ofs.z=+4.0f;
}

/*****************************************************************************/
void	CElem::Build2dDrawList(CTexCache &TexCache,GLint &List)
{
float	X0=Ofs.x;
float	X1=Ofs.x+UnitWidth;
float	Y0=Ofs.y;
float	Y1=Ofs.y+UnitHeight;
float	Z=Ofs.z;
sTex	&Tex=TexCache.GetTex(ElemID);
float	u0=0;
float	u1=1.0f;//Tex.ScaleU;
float	v0=0;
float	v1=1.0f;//Tex.ScaleU;

			List=glGenLists(1);
			glNewList(List,GL_COMPILE);

			TexID=Tex.TexID;
			glBindTexture(GL_TEXTURE_2D, Tex.TexID);
			glBegin (GL_QUADS);
				glTexCoord2f(u0,v0);
				glVertex3f(X0,Y0,Z);

				glTexCoord2f(u1,v0);
				glVertex3f(X1,Y0,Z);

				glTexCoord2f(u1,v1);
				glVertex3f(X1,Y1,Z);

				glTexCoord2f(u0,v1);
				glVertex3f(X0,Y1,Z);
			glEnd();
			glEndList();

}

/*****************************************************************************/
// Build OpenGL display lists, sorted by material (luckily by gin!!)
void	CElem::Build3dDrawList(CTexCache &TexCache,GLint &List)
{
int			TriCount=TriList.size();
int			LastMat=-1,ThisMat;
float		ScaleU,ScaleV;

			List=glGenLists(1);
			glNewList(List,GL_COMPILE);

			for (int T=0; T<TriCount; T++)
			{
				sTriFace	&ThisTri=TriList[T];
				ThisMat=ThisTri.Mat;
				
				if (!T || ThisMat!=LastMat)	// First Tri or new material
				{
					if (T) glEnd();	// Not first tri, so end previous mat set
					glBindTexture(GL_TEXTURE_2D, TexCache.GetTexGLId(ThisMat));
					glBegin (GL_TRIANGLES);
					LastMat=ThisMat;
					sTex &Tex=TexCache.GetTex(ThisMat);
					ScaleU=Tex.ScaleU;
					ScaleV=Tex.ScaleV;
				}
		
				for (int p=0; p<3; p++)
				{
					Vector3	&ThisVtx=ThisTri.vtx[p];
					float	u=ThisTri.uvs[p].u*ScaleU;
					float	v=ThisTri.uvs[p].v*ScaleV;
					glTexCoord2f(u,v);
					glVertex3f( (ThisVtx.x+Ofs.x), (ThisVtx.y+Ofs.y), -(ThisVtx.z+Ofs.z));		// Neg Z (cos openGL)
				}
			}

			glEnd();
			glEndList();

}

/*****************************************************************************/
struct	sColTab
{
	u8	R,G,B;
};
sColTab	ColTable[]=
{
{255,255,255},		// PC_TILE_COLLISION_NORMAL:				
{255,  0,  0},		// PC_TILE_COLLISION_DAMAGE:				
{  0,255,  0},		// PC_TILE_COLLISION_SLIPPERY:			
{255,  0,255},		// PC_TILE_COLLISION_ELECTRIC:			
{200,127,180},		// PC_TILE_COLLISION_DESTRUCTABLE_WALL:	
{  0,127,255},		// PC_TILE_COLLISION_SOAK_UP:			
{  0,255,255},		// PC_TILE_COLLISION_SOLID:
{200,  0,  0},		// PC_TILE_COLLISION_DEATH_FALL:			
{127,200,180},		// PC_TILE_COLLISION_DESTRUCTABLE_FLOOR:	
{255,127,127},		// PC_TILE_COLLISION_DEATH_INSTANT:		

{  0,127,  0},		// PC_TILE_COLLISION_MOVE_LEFT:			
{  0,127,127},		// PC_TILE_COLLISION_MOVE_RIGHT:			
{  0,  0,255},		// PC_TILE_COLLISION_DEATH_LIQUID:		

{255,127,0},		// PC_TILE_COLLISION_SB_NOMOVE

};

void	CElem::Render(int Flags,bool Render3d)
{
		if (BlankFlag)
		{
			glCallList(BlankID);
			return;
		}
		glPushMatrix();

		if (Flags & PC_TILE_FLAG_MIRROR_X)
		{
			glTranslatef(1,0,0);
			glMultMatrixf(XFlipMtx);
		}
		if (Flags & PC_TILE_FLAG_MIRROR_Y)
		{
			glTranslatef(0,1,0);
			glMultMatrixf(YFlipMtx);
		}

int		ColFlags=Flags >> PC_TILE_FLAG_COLLISION_SHIFT;

		if (ColFlags)
		{
			sColTab	&ThisCol=ColTable[ColFlags];
			glColor4ub(ThisCol.R,ThisCol.G,ThisCol.B,127);
		}
		glEnable(GL_TEXTURE_2D);
		if (Render3d) 
		{
			glEnable(GL_DEPTH_TEST);
			glCallList(DrawList[ElemType3d]);
			glDisable(GL_DEPTH_TEST);
		}
		else
		{
			glCallList(DrawList[ElemType2d]);
		}
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
}

/*****************************************************************************/
void	CElem::Purge()
{
		for (int i=0; i<ElemTypeMax; i++)
			glDeleteLists(DrawList[i],1);
		CleanUp();
//		TriList.clear();
}

/*****************************************************************************/
// Only created for the tile browser, should NEVER appear in main view
void	CElem::CreateDefaultTileGfx()
{
float	X0=0;
float	X1=1.0f;
float	Y0=0;
float	Y1=1.0f;
		DefTexFlag=true;
// Blank
		if (BlankID==-1)
		{
			BlankID=glGenLists(1);
			glNewList(BlankID,GL_COMPILE);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBegin(GL_LINES); 
				glColor4f(1,1,1,1);
			
				glVertex3f( X0,Y0,0);
				glVertex3f( X1,Y0,0);
	
				glVertex3f( X0,Y1,0);
				glVertex3f( X1,Y1,0);
	
				glVertex3f( X0,Y0,0);
				glVertex3f( X0,Y1,0);
	
				glVertex3f( X1,Y0,0);
				glVertex3f( X1,Y1,0);
			glEnd();
			glEndList();
		}
// Invalid
		if (InvalidID==-1)
		{
			InvalidID=glGenLists(1);
			glNewList(InvalidID,GL_COMPILE);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBegin(GL_LINES); 
				glColor4f(1,1,1,1);
			
				glVertex3f( X0,Y0,0);
				glVertex3f( X1,Y1,0);

				glVertex3f( X1,Y0,0);
				glVertex3f( X0,Y1,0);
			glEnd();
			glEndList();
		}
}


/*****************************************************************************/
void	CElem::RenderElem4Texture(sRGBData &RGBData)
{
float	W=UnitWidth;
float	H=UnitHeight;

		glClearColor(1,0,1,1 );
//		glClearColor(1,1,1,1 );
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,RGBData.TexW,RGBData.TexH);

		glMatrixMode(GL_PROJECTION);
 		glPushMatrix();

		glLoadIdentity();
		glOrtho(0, W, 0, H, -16.0f, +16.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(-Ofs.x,-Ofs.y,-Ofs.z);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen

		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glCallList(DrawList[ElemType3d]);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
 		glPopMatrix();
		glPopAttrib();

		glReadPixels(0,0,RGBData.TexW,RGBData.TexH,GL_RGB, GL_UNSIGNED_BYTE,RGBData.RGB);
		glClearColor(0,0,0,1 );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen
}

/*****************************************************************************/
// ID used to create unique textures within one set :o)
void	CElem::Create2dTexture(CTexCache &TexCache,const char *Filename,int ID)
{
sRGBData	RGBData;
char		TexName[256];

		sprintf(TexName,"_2dPc_%s_%03d",Filename,ID);
int		AW=AlignSize(ElemWidth);
int		AH=AlignSize(ElemHeight);
		ElemRGB=(u8*)MemAlloc(AW*AH*3);

		RGBData.TexW=AlignSize(ElemWidth);
		RGBData.TexH=AlignSize(ElemHeight);
		RGBData.ScaleU=1.0f;
		RGBData.ScaleV=1.0f;
		RGBData.RGB=ElemRGB;
		RenderElem4Texture(RGBData);
		ElemID=TexCache.ProcessTexture(TexName,&RGBData);
		ValidFlag=CheckHasData(RGBData);
		ElemName=TexName;
#ifdef _DEBUG
		if (0)
		{
char	Filename[256];
		sprintf(Filename,"/x/%s.Tga",TexName);
		SaveTGA(Filename,ElemWidth,ElemHeight,ElemRGB);
		}

#endif

}

/*****************************************************************************/
bool	CElem::CheckHasData(sRGBData &RGBData)
{
u8		*Src=RGBData.RGB;
int		Size=RGBData.TexW*RGBData.TexH;

		while (Size--)
		{
			u8	R=*Src++;
			u8	G=*Src++;
			u8	B=*Src++;
			if (R!=255 || G!=0 || B!=255) return(true);
		}

	return(false);
}

/*****************************************************************************/
void	CElem::Report(FILE	*File)
{
// Check Mid Geom
int		i,ListSize=TriList.size();
bool	MidGeom=false;
		
		if (!IsElem3d()) return;

		for (i=0; i<ListSize && !MidGeom; i++)
		{
			sTriFace	&Tri=TriList[i];
			for (int p=0; p<3; p++)
			{
//				fprintf(File,"%f\n",Tri.vtx[p].z);
				if (Tri.vtx[p].z>-1.0f && Tri.vtx[p].z<+1.0f) MidGeom=true;
			}
		}
// Basic Stats
		fprintf(File,"%s\tT:%i",ElemName,TriList.size());
		if (MidGeom)
		{
			fprintf(File,"\t*** MID GEOM ***");
		}
		fprintf(File,"\n");

}

/*****************************************************************************/
/*****************************************************************************/
/*** Elem Set ****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CElemSet::CElemSet(const char *_Filename,int Idx,int Width,int Height,bool CreateBlank,int Centre)
{
GFName	FName=_Filename;

		Filename=_Filename;
		Name=FName.File();
		
		MaxWidth=Width;
		MaxHeight=Height;
		CentreMode=Centre;
		Loaded=FALSE;
		SetNumber=Idx;

		if (CreateBlank)
		{
			ElemList.push_back(CElem(MaxWidth,MaxHeight));	// Insert Blank		
		}

}

/*****************************************************************************/
CElemSet::~CElemSet()
{
}

/*****************************************************************************/
void	CElemSet::CleanUp()
{
int	ListSize=ElemList.size();

		for (int i=0; i<ListSize; i++)
		{
			ElemList[i].CleanUp();
		}
}

/*****************************************************************************/
void	CElemSet::Load(CCore *Core)
{
GFName	FName=Filename;
GString	Ext=FName.Ext();
		Ext.Upper();
		if (!CElem::DefTexFlag) CElem::CreateDefaultTileGfx();

		if (!ElemList.size())
		{
//			ElemList[0].SetBlank();
		}

		if (Ext=="GIN")
			Load3d(Core);
		else
			Load2d(Core);
		Loaded=TRUE;
}

/*****************************************************************************/
void	CElemSet::Load2d(CCore *Core)
{
CTexCache	&TexCache=Core->GetTexCache();
int			TexID=TexCache.ProcessTexture(Filename);
sTex		&ThisTex=TexCache.GetTex(TexID);
int			Width,Height;

		if (MaxWidth==-1) MaxWidth=ThisTex.OldW;
		if (MaxHeight==-1) MaxHeight=ThisTex.OldH;

		Width=ThisTex.OldW/MaxWidth;
		Height=ThisTex.OldH/MaxHeight;

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				ElemList.push_back(CElem(Core,Filename,TexID,X,Y,MaxWidth,MaxHeight,CentreMode));
			}
		}
		ElemBrowserWidth=Width;
}

/*****************************************************************************/
void	CElemSet::Load3d(CCore *Core)
{
CScene	Scene;

		Scene.Load(Filename);

CNode	&ThisNode=Scene.GetSceneNode(0);
int		ChildCount=ThisNode.GetPruneChildCount();

		for (int Child=0; Child<ChildCount; Child++) 
		{
			ElemList.push_back(CElem(Core,Filename,Scene,ThisNode.PruneChildList[Child]));
		}
		ElemBrowserWidth=DEF_ELEMBROWSWEWIDTH;
}

/*****************************************************************************/
void	CElemSet::Purge()
{
int		i,ListSize=ElemList.size();
	
		for (i=0; i<ListSize; i++)
		{
			if (!ElemList[i].IsBlank())
			{
				ElemList[i].Purge();
			}
		}

		if (ElemList[0].IsBlank())
		{
			ElemList.erase(1,ListSize);
		}
		else
		{
			ElemList.clear();
		}
		Loaded=FALSE;
}

/*****************************************************************************/
CPoint	CElemSet::GetElemPos(int ID)
{
	if (ID==0)
		return(CPoint(-1,-1));
	else
		return(IDToPoint(ID-1,ElemBrowserWidth));
}

/*****************************************************************************/
bool	CElemSet::IsValid(int No)			
{
int		ListSize=ElemList.size();
		if (No>=ListSize) return(false);
		return(ElemList[No].IsValid());
}

/*****************************************************************************/
void	CElemSet::Report(FILE	*File)
{
int		i,ListSize=ElemList.size();

		for (i=0; i<ListSize; i++)
		{
			ElemList[i].Report(File);
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Elem Bank ***************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CElemBank::CElemBank(int W,int H,bool Blank,int Centre)
{
		MaxWidth=W; 
		MaxHeight=H; 
		BlankFlag=Blank;
		CentreMode=Centre;

		LoadFlag=false;
		CurrentSet=0;
		LayerDef.VisibleFlag=true;
}

/*****************************************************************************/
CElemBank::~CElemBank()
{
}

/*****************************************************************************/
void	CElemBank::CleanUp()
{
int	ListSize=SetList.size();

		for (int i=0; i<ListSize; i++)
		{
			SetList[i].CleanUp();
		}
}

/*****************************************************************************/
void	CElemBank::Load(CFile *File,int Version)
{
int		ListSize;
GFName	RootPath=File->GetFilePath();
GString	FilePath;
char	FixPath[1024];

		FilePath=RootPath.Drive();
		FilePath+=RootPath.Dir();
		FilePath.Append('\\');
		FilePath.Upper();
	
		if (Version>=5)
		{
			File->Read(&CurrentSet,sizeof(int));
		}
		File->Read(&ListSize,sizeof(int));

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
			AddSet(FullName);
		}
}

/*****************************************************************************/
void	CElemBank::Save(CFile *File)
{
int		ListSize=SetList.size();
GFName	RootPath=File->GetFilePath();
GString	SavePath;

		SavePath=RootPath.Drive();
		SavePath+=RootPath.Dir();
		SavePath.Append('\\');

		SavePath.Upper();

		File->Write(&CurrentSet,sizeof(int));
		File->Write(&ListSize,sizeof(int));

		for (int i=0; i<ListSize; i++)
		{
			char		Filename[256+64];
			RootPath.makerelative(SavePath,SetList[i].GetFilename(),Filename);	
			File->Write(Filename,strlen(Filename)+1);		
		}

}

/*****************************************************************************/
int			CElemBank::AddSet(const char *Filename)
{
int			ListSize=SetList.size();
CElemSet	NewSet(Filename,ListSize,MaxWidth,MaxHeight,BlankFlag,CentreMode);

int			Idx=SetList.Add(NewSet);
			if (SetList.size()!=ListSize) LoadFlag=TRUE;
			return(Idx);
}

/*****************************************************************************/
void	CElemBank::LoadAllSets(CCore *Core)
{
int		ListSize=SetList.size();
	
		if (!LoadFlag) return;

		for (int i=0;i<ListSize;i++)
		{
			CElemSet	&ThisSet=SetList[i];

			if (!ThisSet.IsLoaded()) ThisSet.Load(Core);
		}
		LoadFlag=false;
}

/*****************************************************************************/
void	CElemBank::LoadNewSet(CCore *Core)
{
char		BASED_CODE GinFilter[]= "All Tile Files (*.Gin; *.Bmp)|*.gin;*.Bmp|3d Tile Files (*.Gin)|*.Gin|2d Tile Files (*.Bmp)|*.Bmp|All Files (*.*)|*.*||";
CFileDialog	Dlg(TRUE,"Gin",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,GinFilter);

		if (Dlg.DoModal()!=IDOK) return;

char	Filename[256];
		sprintf(Filename,"%s",Dlg.GetPathName());
		AddSet(Filename);
}

/*****************************************************************************/
void	CElemBank::ReloadAllSets()
{
int		ListSize=SetList.size();

		for (int i=0; i<ListSize; i++)
		{
			SetList[i].Purge();
		}

		LoadFlag=TRUE;
}

/*****************************************************************************/
void	CElemBank::DeleteSet(int Set)
{
		SetList.erase(Set);
}

/*****************************************************************************/
bool	CElemBank::IsValid(int Set,int Elem)
{
 		if (Set<0 || Elem<0) return(false);
		if (Elem==0) return(true);
 		if (Set>=SetList.size()) return(false);
// 		if (Set>SetList.size()) return(false);
		ASSERT(Set<SetList.size());
		return(SetList[Set].IsValid(Elem));
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CPoint	CElemBank::GetElemPos(int ID,int Width)
{
	if (ID==0)
		return(CPoint(-1,-1));
	else
		return(IDToPoint(ID-1,Width));
}

/*****************************************************************************/
void	CElemBank::RenderElem(int Set,int Elem,int Flags,bool Is3d)
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
void	CElemBank::RenderGrid(CCore *Core,Vector3 &CamPos,bool Active)
{
		if (!GetSetCount()) return;
CElemSet	&ThisSet=SetList[CurrentSet];
int			ListSize=ThisSet.GetCount();
int			BrowserWidth=ThisSet.GetBrowserWidth();
int			TileID=1;	// Dont bother with blank, its sorted
float		Scale=CamPos.z/(float)BrowserWidth/2.0;
		
		if (!ListSize) return;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		while(TileID!=ListSize)
		{
			CPoint	Pos=GetElemPos(TileID,BrowserWidth);
			float	XPos=(float)Pos.x*(1+ElemBrowserGap);
			float	YPos=(float)Pos.y*(1+ElemBrowserGap);

			glLoadIdentity();
			glScalef(Scale,Scale,Scale);
			glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);
			
			glBegin(GL_LINES); 
				glColor3f(1,1,1);
			
				glVertex3f( ElemBrowserX0,ElemBrowserY0,0);
				glVertex3f( ElemBrowserX1,ElemBrowserY0,0);

				glVertex3f( ElemBrowserX0,ElemBrowserY1,0);
				glVertex3f( ElemBrowserX1,ElemBrowserY1,0);

				glVertex3f( ElemBrowserX0,ElemBrowserY0,0);
				glVertex3f( ElemBrowserX0,ElemBrowserY1,0);

				glVertex3f( ElemBrowserX1,ElemBrowserY0,0);
				glVertex3f( ElemBrowserX1,ElemBrowserY1,0);

			glEnd();

			TileID++;
		}
		glPopMatrix();
}

/*****************************************************************************/
void	CElemBank::FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos)
{
		if (!GetSetCount()) return;
CElemSet	&ThisSet=SetList[CurrentSet];
int			ListSize=ThisSet.GetCount();
int			BrowserWidth=ThisSet.GetBrowserWidth();
GLint		Viewport[4];
GLuint		SelectBuffer[SELECT_BUFFER_SIZE];
int			HitCount;
int			TileID=0;
float		Scale=CamPos.z/(float)BrowserWidth/2.0;
		
		if (!ListSize) return;
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
			CPoint	Pos=GetElemPos(TileID,BrowserWidth);
			float	XPos=(float)Pos.x*(1+ElemBrowserGap);
			float	YPos=(float)Pos.y*(1+ElemBrowserGap);

			glLoadIdentity();
			glScalef(Scale,Scale,Scale);
			glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);

			glLoadName (TileID);
			glBegin (GL_QUADS); 
				BuildGLQuad(ElemBrowserX0,ElemBrowserX1,ElemBrowserY0,ElemBrowserY1,0);
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

		CursorPos=TileID;
}

/*****************************************************************************/
void	CElemBank::Report()
{
FILE	*File=fopen("\\Report.txt","wt");

int		ListSize=SetList.size();
		for (int i=0; i<ListSize; i++)
		{
			SetList[i].Report(File);
		}

		fclose(File);
}
/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CElemBank::GUIInit(CCore *Core)
{
		Core->GUIAdd(GUIElemList,IDD_ELEMLIST);
}

/*****************************************************************************/
void	CElemBank::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUIElemList,IDD_ELEMLIST);
}

/*****************************************************************************/
void	CElemBank::GUIUpdate(CCore *Core)
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
void	CElemBank::GUIChanged(CCore *Core)
{
}

/*****************************************************************************/
