/**********************/
/*** Export Structs ***/
/**********************/

#ifndef	__EXPORT_STRUCTS_HEADER__
#define	__EXPORT_STRUCTS_HEADER__

#include	<Vector>
//#include	"GinTex.h"
//#include	"Tile.h"

#include	<List.h>

/*****************************************************************************/
struct	sExpFileHdr
{
		int		TileCount;
//		int		TileW,TileH;
		int		TileOfs;
		int		TriCount;
		int		TriOfs;
		int		TexCount;
		int		TexOfs;
		int		LayerCount;
//		int		LayerOfs[n]
};

/*****************************************************************************/
struct	sExpTile			//	2d	3d
{
		int		TriStart;	//	-1	n
		int		TriCount;	//	-1	n
		int		XOfs,YOfs;	//	 n	-1
		int		TexId;		//	 n	-1
		int		Flags;
		int		W,H;
		u8		*RGB;		// Used in MkLevel
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
struct	sExpLayerHdr
{
		int		Type;
		int		SubType;
		int		Width;
		int		Height;
};

/*****************************************************************************/
struct	sExpLayerTile
{
		int		Tile;
		int		Flags;
		BOOL	operator==(sExpLayerTile const &v1)	{return (Tile==v1.Tile);}
};


/*****************************************************************************/
struct	sExpTex
{
	char			*Filename;

	BOOL			operator==(sExpTex const &v1)		{return (!strcmp(Filename,v1.Filename));}
};

struct	sExpMapElem
{
	int		Set;
	int		Tile;
	int		Flags;

BOOL	operator==(sExpMapElem const &v1)	// Does Check flags
		{
		return(Set==v1.Set && Tile==v1.Tile);
//		return(Set==v1.Set && Tile==v1.Tile && Flags==v1.Flags);
		}
};

/*****************************************************************************/
#endif

