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

struct	sRGBData
{
	int		Width;
	int		Height;
	u8		*RGB;
};

struct	sTex
{
	char			Name[256];
	char			Path[256];
	GLuint			TexID;
	int				Flags;
	int				Width,Height;
};

const RGBQUAD	BlankRGB={255,0,255};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class	CTexCache
{
public:
	
		int		GetTexIdx(char *Name,int Flags);

		int		ProcessTexture(char *TexName,char *Path,int Flags,sRGBData *RGBData=0);
		void	Purge();

		void	LoadBMP(char *Filename,sRGBData &RGBData);
		void	FreeBMP(sRGBData &RGBData);

		void	LoadTex(sTex &ThisTex,sRGBData *TexData);

		sTex	&GetTex(int Id)						{return(TexList[Id]);}
		GLuint	GetTexGLId(int Id)					{return(TexList[Id].TexID);}

		std::vector<sTex>		TexList;

};

/*****************************************************************************/
#endif