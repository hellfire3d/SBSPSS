/*********************/
/*** TileSet Stuph ***/
/*********************/

#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<Vector>

#include	"TileSet.h"
#include	"GinTex.h"
#include	"utils.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CTileSet::CTileSet(char *_Filename,CCore *Core)
{
	strcpy(Filename,_Filename);
	Load(Core);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CTileSet::~CTileSet()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CTileSet::Load(CCore *Core)
{
CScene	Scene;

		Scene.Load(Filename);

CNode	&ThisNode=Scene.GetSceneNode(0);
int		ChildCount=ThisNode.GetPruneChildCount();
		
		for (int Child=0; Child<ChildCount; Child++) 
		{
			CTile	NewTile;
			NewTile.Load(Core,Scene,ThisNode.PruneChildList[Child]);
			Tile.push_back(NewTile);
//			AddTileToSet(Scene,ThisNode.PruneChildList[Child]);
		}

/*
std::vector<GString> const &Tex=Scene.GetTexNames();
	
	for (i=0;i<Tex.size();i++)
	{
		TRACE1("%s\n",Tex[i]);
	}
*/

	return(1);
}

