/*********************/
/*** Texture Cache ***/
/*********************/

#ifndef	__TEXCACHE_HEADER__
#define	__TEXCACHE_HEADER__

#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<Vector>
#include	"Utils.h"
#include	"List.h"

struct	sRGBData
{
	int		Width;
	int		Height;
	u8		*RGB;
};

struct	sTex
{
	char			Filename[_MAX_PATH];
	GLuint			TexID;
	int				Flags;
	int				TexWidth,TexHeight;
	float			dW,dH;
	BOOL			Loaded;

	BOOL			operator==(sTex const &v1)		{return (!strcmp(Filename,v1.Filename) && Flags==v1.Flags);}
};

const RGBQUAD	BlankRGB={255,0,255};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class	CTexCache
{
public:
	
		int		GetTexIdx(sTex &Tex)						{return(TexList.Find(Tex));}
		int		GetTexIdx(char *Filename,int Flags);

		int		ProcessTexture(char *Path,int Flags,sRGBData *RGBData=0);
		void	Purge();

		void	LoadBMP(char *Filename,sRGBData &RGBData);
		void	FreeBMP(sRGBData &RGBData);
		void	FixBMP(sRGBData &RGBData);
		BOOL	IsSizeOk(int Size);
		int		AlignSize(int Size);

		void	LoadTex(sTex &ThisTex,sRGBData *TexData);
		

		sTex	&GetTex(int Id)						{return(TexList[Id]);}
		GLuint	GetTexGLId(int Id)					{return(TexList[Id].TexID);}


		CList<sTex>				TexList;

};

/*****************************************************************************/
#endif