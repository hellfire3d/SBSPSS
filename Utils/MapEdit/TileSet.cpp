/*********************/
/*** TileSet Stuph ***/
/*********************/

#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<Vector>

#include	"Core.h"
#include	"TileSet.h"
#include	"GinTex.h"
#include	"utils.h"


/*****************************************************************************/
/*****************************************************************************/
/*** TileBank ****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CTileBank::CTileBank()
{
	LoadFlag=FALSE;
}

/*****************************************************************************/
CTileBank::~CTileBank()
{
}

/*****************************************************************************/
void	CTileBank::AddTileSet(char *Filename)
{
	TileSet.push_back(CTileSet(Filename));
	LoadFlag=TRUE;
}

/*****************************************************************************/
void	CTileBank::LoadTileSets(CCore *Core)
{
int	ListSize=TileSet.size();
	
	for (int i=0;i<ListSize;i++)
	{
		CTileSet	&ThisSet=TileSet[i];

		if (!ThisSet.IsLoaded()) ThisSet.Load(Core);
	}
	LoadFlag=FALSE;
}

/*****************************************************************************/
CTile	&CTileBank::GetTile(int Bank,int Tile)
{
	return(TileSet[Bank].GetTile(Tile));
}

/*****************************************************************************/
/*****************************************************************************/
/*** TileSet *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CTileSet::CTileSet(char *_Filename)
{
char	Drive[_MAX_DRIVE];
char	Dir[_MAX_DIR];
char	Fname[_MAX_FNAME];
char	Ext[_MAX_EXT];

	_splitpath(_Filename,Drive,Dir,Fname,Ext);
	sprintf(Path,"%s%s",Drive,Dir);
	sprintf(Name,"%s",Fname);
	Loaded=FALSE;
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
void	CTileSet::Load(CCore *Core)
{
CScene	Scene;
char	Filename[256+64];

		sprintf(Filename,"%s%s.%s",Path,Name,"Gin");
		Scene.Load(Filename);

CNode	&ThisNode=Scene.GetSceneNode(0);
int		ChildCount=ThisNode.GetPruneChildCount();
		
		for (int Child=0; Child<ChildCount; Child++) 
		{
			CTile	NewTile;
			NewTile.Load(Core,this,Scene,ThisNode.PruneChildList[Child]);
			Tile.push_back(NewTile);
		}

	Loaded=TRUE;
}

/*****************************************************************************/
