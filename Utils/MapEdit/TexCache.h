/*********************/
/*** Texture Cache ***/
/*********************/

#ifndef	__TEXCACHE_HEADER__
#define	__TEXCACHE_HEADER__

#include	"stdafx.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<Vector>
#include	"Utils.h"
#include	"List.h"
#include	<misc.hpp>

struct	sRGBData
{
	int		Width;
	int		Height;
	u8		*RGB;
};

struct	sTex
{
	GString			Filename;
	GString			Name;
	GLuint			TexID;
	int				Flags;
	int				TexWidth,TexHeight;
	float			dW,dH;
	bool			Loaded;

	bool			operator==(sTex const &v1)		{return (Filename==v1.Filename && Flags==v1.Flags);}
};

const RGBQUAD	BlankRGB={255,0,255};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class	CTexCache
{
public:
	
		int		GetTexIdx(sTex &Tex)						{return(TexList.Find(Tex));}

		int		ProcessTexture(const char *Name,int Flags,sRGBData *RGBData=0);
		void	Purge();

		bool	LoadBMP(const char *Filename,sRGBData &RGBData);
		void	FreeBMP(sRGBData &RGBData);
		void	FixBMP(sRGBData &RGBData);
		bool	IsSizeOk(int Size);
		int		AlignSize(int Size);

		void	LoadTex(sTex &ThisTex,sRGBData *TexData);
		
		sTex	&GetTex(int Id)						{return(TexList[Id]);}
		GString	&GetTexFilename(int Id)				{return(TexList[Id].Filename);}
		GString	&GetTexName(int Id)					{return(TexList[Id].Name);}
		GLuint	GetTexGLId(int Id)					{return(TexList[Id].TexID);}


		CList<sTex>				TexList;

};

/*****************************************************************************/
#endif