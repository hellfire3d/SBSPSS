/**********************/
/*** Export Structs ***/
/**********************/

#ifndef	__EXPORT_STRUCTS_HEADER__
#define	__EXPORT_STRUCTS_HEADER__

#include	<Vector>

#include	<List.h>

/*****************************************************************************/
#define	EXPORT_LAYER_COUNT	8
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
		int		LayerOfs[EXPORT_LAYER_COUNT];
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
struct	sExpColTile
{
		u8		Tile;
		u8		Flags;

bool	operator==(sExpColTile const &v1)
		{
		return(Tile==v1.Tile && Flags==v1.Flags);
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
/*** Things ******************************************************************/
/*****************************************************************************/
struct	sExpLayerThing
{
	int						Health;
	int						AttackStrength;
	int						Speed;
	int						TurnRate;
	bool					CollisionFlag;
	bool					PlayerFlag;
	int						Spare[8];
};
#endif

