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

	sprintf(FullName,"%s",_Filename);
	_splitpath(_Filename,Drive,Dir,Fname,Ext);
	sprintf(Path,"%s%s",Drive,Dir);
	sprintf(Filename,"%s%s",Fname,Ext);
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

		Scene.Load(FullName);

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
