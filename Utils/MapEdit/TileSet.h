/*********************/
/*** TileSet Stuph ***/
/*********************/

#ifndef	__TILESET_HEADER__
#define	__TILESET_HEADER__

#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<Vector>

#include	"TexCache.h"
#include	"Tile.h"


/*****************************************************************************/
class	CCore;
class	CTile;

/*****************************************************************************/
class	CTileBank
{
public:
	CTileBank();
	~CTileBank();

	void	AddTileSet(char *Filename);
	int		NeedLoad()					{return(LoadFlag);}
	void	LoadTileSets(CCore *Core);
	CTile	&GetTile(int Bank,int Tile);

private:
		std::vector<CTileSet>	TileSet;
		BOOL					LoadFlag;

};

/*****************************************************************************/
class	CTileSet
{
public:
		CTileSet(char *_Filename);
		~CTileSet();
		
		int		IsLoaded()			{return(Loaded);}
		void	Load(CCore *Core);
		char	*GetPath()			{return(Path);}
		char	*GetName()			{return(Name);}
		CTile	&GetTile(int No)	{return(Tile[No]);}


private:
		char				Path[256],Name[256];
		std::vector<CTile>	Tile;
		BOOL				Loaded;

};

/*****************************************************************************/
#endif