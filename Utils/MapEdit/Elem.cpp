/*********************/
/*** Tile Stuph ***/
/*********************/

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<Vector>
#include	<GFName.hpp>

#include	"Core.h"
#include	"TexCache.h"
//#include	"TileSet.h"
#include	"Tile.h"
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
/*
CTile::CTile()
{
}
*/
/*****************************************************************************/
CTile::CTile(int Blank,int _Width,int _Height)
{
		Width=_Width;
		Height=_Height;
		PCTexID=-1;
		PCTexRGB=(u8*)malloc(Width*Height*3);
		memset(PCTexRGB,0,Width*Height*3);
		Tex2dID=-1;
		Type=TileType2d;
}

/*****************************************************************************/
// 3d tile (From Gin File)
CTile::CTile(CCore *Core,const char *Filename,CScene &ThisScene,int Node)
{
CNode		&ThisNode=ThisScene.GetNode(Node);
CTexCache	&TexCache=Core->GetTexCache();
GFName		Path=Filename;

			SetPath=Path.Drive();
			SetPath+=Path.Dir();
			SetPath.Append('\\');

			Type=TileType3d;
			TexXOfs=-1;
			TexYOfs=-1;
			Build3dTile(TexCache,ThisScene,Node);
			Ofs.x=+0.5f; Ofs.y=0; Ofs.z=+4.0f;
			Build3dDrawList(TexCache,DrawList[TileType3d]);
			Create2dTexture(TexCache,Path.File(),Node);
			Build2dDrawList(TexCache,DrawList[TileType2d]);
}

/*****************************************************************************/
// 2d tile (From Bmp File)
CTile::CTile(CCore *Core,const char *Filename,int TexID,int XOfs,int YOfs)
{
CTexCache	&TexCache=Core->GetTexCache();
GFName		Path=Filename;

			Type=TileType2d;
			TexXOfs=XOfs;
			TexYOfs=YOfs;

			Build2dTile(Core,Path.File(),TexID);
			Ofs.Zero();
			Build3dDrawList(TexCache,DrawList[TileType3d]);
			Create2dTexture(TexCache,Path.File(),TexID);
			Build2dDrawList(TexCache,DrawList[TileType2d]);
}

/*****************************************************************************/
void	CTile::Build2dTile(CCore *Core,const char *Filename,int TexID)
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
		
			Tex2dID=TexID;
			u0=(TexXOfs)*ThisTex.dW;
			u1=u0+ThisTex.dW;

			v0=(((ThisTex.TexHeight/16)-(TexYOfs+1))*ThisTex.dH);
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
void	CTile::Build3dTile(CTexCache &TexCache,CScene &ThisScene,int Node)
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
// Sort Rest
				for (int p=0; p<3; p++)
				{
					Tri.vtx[p]=NodeVtxList[ThisTri.p[p]];
					Tri.uvs[p].u=ThisUV.p[p].u;
					Tri.uvs[p].v=ThisUV.p[p].v;
					Tri.Mat=TexID;
				}
			}

		for (int Child=0; Child<ChildCount; Child++) Build3dTile(TexCache,ThisScene,ThisNode.PruneChildList[Child]);

}

/*****************************************************************************/
void	CTile::Build2dDrawList(CTexCache &TexCache,GLint &List)
{
			List=glGenLists(1);
			glNewList(List,GL_COMPILE);

			glBindTexture(GL_TEXTURE_2D, TexCache.GetTexGLId(PCTexID));
			glBegin (GL_QUADS);
				glTexCoord2f(0.0f,0.0f);
				glVertex3f( 0.0f,0.0f,0.0f);

				glTexCoord2f(1.0f,0.0f);
				glVertex3f( 1.0f,0.0f,0.0f);

				glTexCoord2f(1.0f,1.0f);
				glVertex3f( 1.0f,1.0f,0.0f);

				glTexCoord2f(0.0f,1.0f);
				glVertex3f( 0.0f,1.0f,0.0f);

			glEnd();
			glEndList();

}

/*****************************************************************************/
// Build OpenGL display lists, sorted by material (luckily by gin!!)
void	CTile::Build3dDrawList(CTexCache &TexCache,GLint &List)
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
void	CTile::Render(int Flags,bool Render3d)
{
		if (!ValidFlag) return;
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
		glCallList(DrawList[Render3d]);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
}

/*****************************************************************************/
void	CTile::Purge()
{
		glDeleteLists(DrawList[TileType3d],1);
		glDeleteLists(DrawList[TileType2d],1);
		TriList.clear();
}

/*****************************************************************************/
void	CTile::RenderTile4Texture(sRGBData &RGBData)
{

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
		glCallList(DrawList[TileType3d]);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
 		glPopMatrix();
		glPopAttrib();

		glReadPixels(0,0,RGBData.Width,RGBData.Height,GL_RGB, GL_UNSIGNED_BYTE,RGBData.RGB);
}

/*****************************************************************************/
// ID used to create unique textures within one set :o)
void	CTile::Create2dTexture(CTexCache &TexCache,const char *Filename,int ID)
{
sRGBData	RGBData;

// PC
		glClearColor(1,0,1,1 );
		RGBData.Width=PCTexWidth;
		RGBData.Height=PCTexHeight;
		RGBData.RGB=PCTexRGB;
		RenderTile4Texture(RGBData);
		PCTexID=TexCache.ProcessTexture("",0,&RGBData);
		ValidFlag=CheckHasData(RGBData);
		glClearColor(0,0,0,1 );

#ifdef _DEBUG
		if (0)
		{
char	Filename[256];
char	TexName[256];
static	asd=0;
		sprintf(TexName,"_2dPc_%s_%03d",Filename,asd++);
		sprintf(Filename,"/x/%s.Tga",TexName);
		SaveTGA(Filename,PCTexWidth,PCTexHeight,PCTexRGB);
		}

#endif

}

/*****************************************************************************/
bool	CTile::CheckHasData(sRGBData &RGBData)
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



