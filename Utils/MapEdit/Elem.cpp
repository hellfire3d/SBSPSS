/*********************/
/*** Elem Stuph ***/
/*********************/

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<Vector>
#include	<GFName.hpp>

#include	"Core.h"
#include	"TexCache.h"
#include	"Elem.h"
#include	"GinTex.h"
#include	"utils.h"



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
		ElemRGB=(u8*)malloc(ElemWidth*ElemHeight*3);
		memset(ElemRGB,0,ElemWidth*ElemHeight*3);

		Type=ElemType2d;
		TexXOfs=0;
		TexYOfs=0;
		CreateBlankTileGfx();
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
			Build3dElem(TexCache,ThisScene,Node);
			Calc3dSize();
			Ofs.x=+0.5f; Ofs.y=0; Ofs.z=+4.0f;
			Build3dDrawList(TexCache,DrawList[ElemType3d]);
			Create2dTexture(TexCache,Path.File(),Node);
			Build2dDrawList(TexCache,DrawList[ElemType2d]);
			if (!ValidFlag)	CreateInvalidTileGfx();

}

/*****************************************************************************/
// 2d Elem (From Bmp File)
CElem::CElem(CCore *Core,const char *Filename,int TexID,int XOfs,int YOfs,int Width,int Height)
{
CTexCache	&TexCache=Core->GetTexCache();
GFName		Path=Filename;

			ElemWidth=Width;
			ElemHeight=Height;
			UnitWidth=ElemWidth/UnitSize;
			UnitHeight=ElemHeight/UnitSize;

			Type=ElemType2d;
			TexXOfs=XOfs;
			TexYOfs=YOfs;
			Build2dElem(Core,Path.File(),TexID);
			Ofs.Zero();
			Build3dDrawList(TexCache,DrawList[ElemType3d]);
			Create2dTexture(TexCache,Path.File(),TexID);
			Build2dDrawList(TexCache,DrawList[ElemType2d]);
			if (!ValidFlag)	CreateInvalidTileGfx();
}

/*****************************************************************************/
void	CElem::CleanUp()
{
		free(ElemRGB);
}

/*****************************************************************************/
void	CElem::Build2dElem(CCore *Core,const char *Filename,int TexID)
{
const Vector3	P0(0.0f,0.0f,0.0f);
const Vector3	P1(1.0f,0.0f,0.0f);
const Vector3	P2(0.0f,1.0f,0.0f);
const Vector3	P3(1.0f,1.0f,0.0f);

int			ListSize=TriList.size();
			TriList.resize(ListSize+2);
sTriFace	&Tri0=TriList[ListSize+0];
sTriFace	&Tri1=TriList[ListSize+1];

CTexCache	&TexCache=Core->GetTexCache();
sTex		&ThisTex=TexCache.GetTex(TexID);

float		u0,u1,v0,v1;
		
			u0=(TexXOfs)*ThisTex.dW;
			u1=u0+ThisTex.dW;

			v0=(((ThisTex.TexHeight/UnitSize)-(TexYOfs+1))*ThisTex.dH);
			v1=v0+ThisTex.dH;

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
int							ChildCount=ThisNode.GetChildCount();
std::vector<sGinTri> const	&NodeTriList=ThisNode.GetTris();
std::vector<Vector3>const	&NodeVtxList=ThisNode.GetRelPts();
std::vector<sUVTri>	const	&NodeUVList=ThisNode.GetUVTris();
std::vector<int>	const	&NodeTriMat=ThisNode.GetTriMaterial();
std::vector<GString> const	&TexList= ThisScene.GetTexNames();//List();
std::vector<Material> const &MatList= ThisScene.GetMaterials();

int			TriCount=NodeTriList.size();
int			ListSize=TriList.size();

			TriList.resize(ListSize+TriCount);
			for (int T=0; T<TriCount; T++)
			{
				sGinTri	const	&ThisTri=NodeTriList[T];
				sUVTri	const	&ThisUV=NodeUVList[T];
				sTriFace		&Tri=TriList[ListSize+T];
				int				ThisMat=NodeTriMat[T];
				int				TexID=1;
				

// Sort Textures - Only add the ones that are used :o)
				TexID=MatList[ThisMat].TexId;
				if (TexID!=-1)
					{
					GString	ThisName;

					ThisName=SetPath+TexList[TexID];
					TexID=TexCache.ProcessTexture(ThisName,MatList[ThisMat].Flags);
					}
// Sort Rest of Tri info
				for (int p=0; p<3; p++)
				{
					Tri.vtx[p]=NodeVtxList[ThisTri.p[p]];
					Tri.uvs[p].u=ThisUV.p[p].u;
					Tri.uvs[p].v=ThisUV.p[p].v;
					Tri.Mat=TexID;
				}
			}

		for (int Child=0; Child<ChildCount; Child++) Build3dElem(TexCache,ThisScene,ThisNode.PruneChildList[Child]);

}

/*****************************************************************************/
void	CElem::Calc3dSize()
{
int			ListSize=TriList.size();
float		XMin=+9999;
float		XMax=-9999;
float		YMin=+9999;
float		YMax=-9999;

		for (int i=0; i<ListSize; i++)
		{
			sTriFace		&Tri=TriList[i];
			for (int p=0; p<3; p++)
			{
				if (XMin>Tri.vtx[p].x) XMin=Tri.vtx[p].x;
				if (XMax<Tri.vtx[p].x) XMax=Tri.vtx[p].x;
				if (YMin>Tri.vtx[p].y) YMin=Tri.vtx[p].y;
				if (YMax<Tri.vtx[p].y) YMax=Tri.vtx[p].y;
			}
		}
		UnitWidth=round(XMax-XMin);
		UnitHeight=round(YMax-YMin);
		if (UnitWidth<1) UnitWidth=1;
		if (UnitHeight<1) UnitHeight=1;
		ElemWidth=UnitWidth*UnitSize;
		ElemHeight=UnitHeight*UnitSize;

}

/*****************************************************************************/
void	CElem::Build2dDrawList(CTexCache &TexCache,GLint &List)
{
			List=glGenLists(1);
			glNewList(List,GL_COMPILE);

			glBindTexture(GL_TEXTURE_2D, TexCache.GetTexGLId(ElemID));
			glBegin (GL_QUADS);
				glTexCoord2f(0.0f,0.0f);
				glVertex3f( 0.0f,0.0f,0.0f);

				glTexCoord2f(1.0f,0.0f);
				glVertex3f( UnitWidth,0.0f,0.0f);

				glTexCoord2f(1.0f,1.0f);
				glVertex3f( UnitWidth,UnitHeight,0.0f);

				glTexCoord2f(0.0f,1.0f);
				glVertex3f( 0.0f,UnitHeight,0.0f);

			glEnd();
			glEndList();

}

/*****************************************************************************/
// Build OpenGL display lists, sorted by material (luckily by gin!!)
void	CElem::Build3dDrawList(CTexCache &TexCache,GLint &List)
{
int			TriCount=TriList.size();
int			LastMat=-1,ThisMat;

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
					}
				for (int p=0; p<3; p++)
				{
					Vector3	&ThisVtx=ThisTri.vtx[p];
					glTexCoord2f(ThisTri.uvs[p].u, ThisTri.uvs[p].v); 

					glVertex3f( (ThisVtx.x+Ofs.x), (ThisVtx.y+Ofs.y), -(ThisVtx.z+Ofs.z));		// Neg Z (cos openGL)

				}
			}

			glEnd();
			glEndList();

}

/*****************************************************************************/
void	CElem::Render(int Flags,bool Render3d)
{
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
			switch (ColFlags)
			{
			case PC_TILE_COLLISION_NORMAL:		glColor4f(1,1,1,0.5); break;
			case PC_TILE_COLLISION_DAMAGE:		glColor4f(1,0,0,0.5); break;
			case PC_TILE_COLLISION_SLIPPERY:	glColor4f(0,1,0,0.5); break;
			case PC_TILE_COLLISION_ELECTRIC:	glColor4f(1,0,1,0.5); break;
			case PC_TILE_COLLISION_STICKY:		glColor4f(1,1,0,0.5); break;
			case PC_TILE_COLLISION_WATER:		glColor4f(0,0,1,0.5); break;
			}
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
void	CElem::RenderInvalid()
{
float	X0=0;
float	X1=UnitWidth;
float	Y0=0;
float	Y1=UnitHeight;

		glBindTexture(GL_TEXTURE_2D, 0);
		glBegin(GL_LINES); 
			glColor4f(1,1,1,1);
			
			glVertex3f( X0,Y0,0);
			glVertex3f( X1,Y1,0);

			glVertex3f( X1,Y0,0);
			glVertex3f( X0,Y1,0);
		glEnd();
}

/*****************************************************************************/
void	CElem::Purge()
{
		for (int i=0; i<ElemTypeMax; i++)
			glDeleteLists(DrawList[i],1);
		TriList.clear();
}

/*****************************************************************************/
// Only created for the tile browser, should NEVER appear in main view
void	CElem::CreateBlankTileGfx()
{
float	X0=0;
float	X1=UnitWidth;
float	Y0=0;
float	Y1=UnitHeight;

		Purge();
		for (int i=0; i<ElemTypeMax; i++)
		{
			DrawList[i]=glGenLists(1);
			glNewList(DrawList[i],GL_COMPILE);
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
}

/*****************************************************************************/
void	CElem::CreateInvalidTileGfx()
{
float	X0=0;
float	X1=UnitWidth;
float	Y0=0;
float	Y1=UnitHeight;

		Purge();
		for (int i=0; i<ElemTypeMax; i++)
		{
			DrawList[i]=glGenLists(1);
			glNewList(DrawList[i],GL_COMPILE);
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

		glClearColor(1,0,1,1 );
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,RGBData.Width,RGBData.Height);

		glMatrixMode(GL_PROJECTION);
 		glPushMatrix();

		glLoadIdentity();
		glOrtho(-0.5f, +0.5f, -0.5f, +0.5f, -16.0f, +16.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(-0.5f,-0.5f,0);

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

		glReadPixels(0,0,RGBData.Width,RGBData.Height,GL_RGB, GL_UNSIGNED_BYTE,RGBData.RGB);
		glClearColor(0,0,0,1 );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen
}

/*****************************************************************************/
// ID used to create unique textures within one set :o)
void	CElem::Create2dTexture(CTexCache &TexCache,const char *Filename,int ID)
{
sRGBData	RGBData;

		ElemRGB=(u8*)malloc(ElemWidth*ElemHeight*3);

		RGBData.Width=ElemWidth;
		RGBData.Height=ElemHeight;
		RGBData.RGB=ElemRGB;
		RenderElem4Texture(RGBData);
		ElemID=TexCache.ProcessTexture("",0,&RGBData);
		ValidFlag=CheckHasData(RGBData);

#ifdef _DEBUG
		if (0)
		{
char	Filename[256];
char	TexName[256];
static	asd=0;
		sprintf(TexName,"_2dPc_%s_%03d",Filename,asd++);
		sprintf(Filename,"/x/%s.Tga",TexName);
		SaveTGA(Filename,ElemWidth,ElemHeight,ElemRGB);
		}

#endif

}

/*****************************************************************************/
bool	CElem::CheckHasData(sRGBData &RGBData)
{
u8		*Src=RGBData.RGB;
int		Size=RGBData.Width*RGBData.Height;

		for (int i=0; i<Size; i++)
		{
			u8	R=*Src++;
			u8	G=*Src++;
			u8	B=*Src++;
			if (R!=255 || G!=0 || B!=255) return(TRUE);

		}
	return(FALSE);
}

/*****************************************************************************/
/*****************************************************************************/
/*** Elem Set ****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CElemSet::CElemSet(const char *_Filename,int Idx,int Width,int Height)
{
GFName	FName=_Filename;

		Filename=_Filename;
		Name=FName.File();
		
		DefWidth=Width;
		DefHeight=Height;
		Loaded=FALSE;
		SetNumber=Idx;
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
int			TexID=TexCache.ProcessTexture(Filename,0);
sTex		&ThisTex=TexCache.GetTex(TexID);
int			Width=ThisTex.TexWidth/DefWidth;
int			Height=ThisTex.TexHeight/DefHeight;

		ElemList.push_back(CElem(DefWidth,DefHeight));	// Insert Blank		
		
		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				ElemList.push_back(CElem(Core,Filename,TexID,X,Y,DefWidth,DefHeight));
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
		
		ElemList.push_back(CElem(DefWidth,DefHeight));	// Insert Blank		
		
		for (int Child=0; Child<ChildCount; Child++) 
		{
			ElemList.push_back(CElem(Core,Filename,Scene,ThisNode.PruneChildList[Child]));
		}
		ElemBrowserWidth=DEF_ELEMBROWSWEWIDTH;
}

/*****************************************************************************/
void	CElemSet::Purge()
{
int	ListSize=ElemList.size();
	
		for (int i=0; i<ListSize; i++)
		{
			ElemList[i].Purge();
		}
		ElemList.clear();
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
		if (No>ListSize) return(false);
		return(ElemList[No].IsValid());
}

/*****************************************************************************/
/*****************************************************************************/
/*** Elem Bank ***************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CElemBank::CElemBank()
{
		LoadFlag=FALSE;
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

		File->Write(&ListSize,sizeof(int));

		for (int i=0; i<ListSize; i++)
		{
			char		Filename[256+64];
			RootPath.makerelative(SavePath,SetList[i].GetFilename(),Filename);	
			File->Write(Filename,strlen(Filename)+1);		
		}

}

/*****************************************************************************/
void	CElemBank::AddSet(const char *Filename)
{
int			ListSize=SetList.size();
CElemSet	NewSet(Filename,ListSize);

			SetList.Add(NewSet);
			if (SetList.size()!=ListSize) LoadFlag=TRUE;
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
CElem	&CElemBank::GetElem(int Set,int Elem)
{
		ASSERT(Set>=0 && Elem>=0);
		return(SetList[Set].GetElem(Elem));
}

/*****************************************************************************/
bool	CElemBank::IsValid(int Set,int Elem)
{
 		if (Set<0 || Elem<0) return(false);
		if (Elem==0) return(true);
		ASSERT(Set<SetList.size());
		return(SetList[Set].IsValid(Elem));
}

