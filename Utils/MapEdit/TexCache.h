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

struct	sTex
{
	char			Name[256];
	char			Path[256];
	GLuint			TexID;
	int				Flags;
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class	CTexCache
{
public:
	
		int		ProcessTexture(char *TexName,char *Path,int Flags);
		sTex	&GetTex(int Id)						{return(TexList[Id]);}
		GLuint	GetTexGLId(int Id)					{return(TexList[Id].TexID);}


		std::vector<sTex>		TexList;

};

/*****************************************************************************/
#endif