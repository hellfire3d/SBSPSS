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
int			CTexCache::ProcessTexture(char *TexName,char *Path,int Flags)
{
int		ListSize=TexList.size();
		
// Check if Tex exists		
		for (int Count=0;Count<ListSize;Count++)
		{
			if (strcmp(TexName,TexList[Count].Name)==0 && TexList[Count].Flags==Flags)
			{
				return(Count);
			}
		}
sTex	NewTex;
char	Filename[256];
		strcpy(NewTex.Name,TexName);
		strcpy(NewTex.Path,Path);
		sprintf(Filename,"%s%s",Path,TexName);
		TRACE1("Loading Texture %s\n",Filename);
		LoadGLTexture(Filename,NewTex.TexID);
		NewTex.Flags=Flags;
		TexList.push_back(NewTex);
				
		return(Count);
}

