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
	GLuint			TexID;
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class	CTexCache
{
public:
	
		int		ProcessTexture(char *TexName);
		int		AddTexture(char *TexName);


		std::vector<sTex>		TexList;

};

/*****************************************************************************/
#endif