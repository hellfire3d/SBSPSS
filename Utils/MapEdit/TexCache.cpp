/*********************/
/*** Texture Cache ***/
/*********************/

#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<Vector>

#include	"TexCache.h"
#include	"utils.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int			CTexCache::ProcessTexture(char *TexName)
{
//		TRACE3("%i %s\t%i Tris\n",Id,ThisNode.GetName(),TriCount);
int		ListSize=TexList.size();
		
// Check if Tex exists		
		for (int Count=0;Count<ListSize;Count++)
		{
			if (strcmp(TexName,TexList[Count].Name))
			{
				return(TexList[Count].TexID);
			}
		}
sTex	NewTex;
		strcpy(NewTex.Name,TexName);
		LoadGLTexture(TexName,NewTex.TexID);
		TexList.push_back(NewTex);
				
		return(NewTex.TexID);
//			
}

