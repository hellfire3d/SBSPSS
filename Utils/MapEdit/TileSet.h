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


/*****************************************************************************/
class	CScene;
class	CNode;
class	CTileSet
{
public:
		CTileSet(char *Filename);
		~CTileSet();

		int		Load();
		void	AddTileToSet(CScene &ThisScene,int Id);

private:
		char	Filename[256];
		std::vector<GLint>	Tile;

};

/*****************************************************************************/
#endif