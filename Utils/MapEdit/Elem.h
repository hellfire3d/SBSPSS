/******************/
/*** Elem Stuph ***/
/******************/

#ifndef	__ELEM_HEADER__
#define	__ELEM_HEADER__

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<Vector>
#include	"TexCache.h"
#include	"GinTex.h"

struct	sTriFace
{
	int				Mat;
	Vector3			vtx[3];
	int				pts[3];
	sUV				uvs[3];
};


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class	CCore;
class	CElem
{
public:
		enum ElemType
		{
			ElemType2d=0,
			ElemType3d,
		};
		enum	ElemEnums
		{
			PCTexWidth	=16,
			PCTexHeight	=PCTexWidth,
		};

//		CElem();
		CElem(int Blank,int Width=PCTexWidth,int Height=PCTexHeight	);						// Blank (2d)
		CElem(CCore *Core,const char *Filename,int TexId,int XOfs,int YOfs);				// 2d elem
		CElem(CCore *Core,const char *Filename,CScene &ThisScene,int Node);					// 3d elem

		void	Purge();
		void	Render(int Flags,bool Render3d);

		int		GetPCTexW()					{return(PCTexWidth);}
		int		GetPCTexH()					{return(PCTexHeight);}
		u8		*GetPCTexRGB()				{return(PCTexRGB);}

		bool	IsValid()					{return(ValidFlag);}

		bool	IsElem3d()					{return(Type==ElemType3d);}
		int		GetTexXOfs()				{return(TexXOfs);}
		int		GetTexYOfs()				{return(TexYOfs);}
		int		GetTexID()					{return(Tex2dID);}

		std::vector<sTriFace> 	&GetTriList() {return(TriList);}

protected:
		void	Build3dElem(CTexCache &TexCache,CScene &ThisScene,int Node);
		void	Build2dElem(CCore *Core,const char *Filename,int TexId);
//		void	FixUp();
		void	Build3dDrawList(CTexCache &TexCache,GLint &List);
		void	Build2dDrawList(CTexCache &TexCache,GLint &List);

		void	RenderElem4Texture(sRGBData &RGBData);
		void	Create2dTexture(CTexCache &TexCache,const char *Filename,int ID);
		bool	CheckHasData(sRGBData &RGBData);

		GString					SetPath;

		std::vector<sTriFace>	TriList;
		

		Vector3					Ofs;
		int						Width,Height;

		GLint					DrawList[2];
		ElemType				Type;
		bool					ValidFlag;
		int						TexXOfs,TexYOfs;
		int						Tex2dID;
		
//		char					PCTexName[256];
		int						PCTexID;
		u8						*PCTexRGB;

};

/*****************************************************************************/
#endif