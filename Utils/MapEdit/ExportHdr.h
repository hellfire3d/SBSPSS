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
		int		TileOfs;
		int		TileW,TileH;
		int		TriCount;
		int		TriOfs;
		int		SetNameCount;
		int		SetNameOfs;
		int		TexNameCount;
		int		TexNameOfs;
		int		LayerCount;
//		int		LayerOfs[n]
};

/*****************************************************************************/
struct	sExpTile
{
		int		Set;
		int		Tile;
		int		TriStart;
		int		TriCount;
		u16		XOfs,YOfs;
		u8		*RGB;
//		int		TexId;			//	 n	-1
//		int		Flags;			//  Flip Flags

bool	operator==(sExpTile const &v1)
		{
		return(Set==v1.Set && Tile==v1.Tile);
		}
};

/*****************************************************************************/
struct	sExpTri
{
		int			TexID;
		Vector3		vtx[3];
		float		uv[3][2];
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
		u16		Tile;
		u16		Flags;
//		bool	operator==(sExpLayerTile const &v1)	{return (Tile==v1.Tile);}
};


/*****************************************************************************/
/*
struct	sExpTex
{
	char			*Filename;

	BOOL			operator==(sExpTex const &v1)		{return (!strcmp(Filename,v1.Filename));}
};
*/
/*
struct	sExpMapElem
{
	u16		Set;
	u16		Tile;
	u16		Flags;

BOOL	operator==(sExpMapElem const &v1)
		{
		return(Set==v1.Set && Tile==v1.Tile);
		}
};
*/
/*****************************************************************************/
#endif

