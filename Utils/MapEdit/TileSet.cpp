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
char	Drive[_MAX_DRIVE];
char	Dir[_MAX_DIR];
char	Fname[_MAX_FNAME];
char	Ext[_MAX_EXT];

	_splitpath(_Filename,Drive,Dir,Fname,Ext);
	sprintf(Path,"%s%s",Drive,Dir);
	sprintf(Filename,"%s%s",Fname,Ext);

	Load(Core,_Filename);
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
int		CTileSet::Load(CCore *Core,char *_Filename)
{
		CScene	Scene;

		Scene.Load(_Filename);

CNode	&ThisNode=Scene.GetSceneNode(0);
int		ChildCount=ThisNode.GetPruneChildCount();
		
		for (int Child=0; Child<ChildCount; Child++) 
		{
			CTile	NewTile;
			NewTile.Load(Core,this,Scene,ThisNode.PruneChildList[Child]);
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

