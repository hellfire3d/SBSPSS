/**********************/
/*** Export Structs ***/
/**********************/

#ifndef	__EXPORT_STRUCTS_HEADER__
#define	__EXPORT_STRUCTS_HEADER__

#include	<Vector>

#include	<List.h>

/*****************************************************************************/
struct	sRGBCol
{
		u8	R,G,B,P;
bool	operator ==(sRGBCol const &v1)	{return(R==v1.R && G==v1.G && B==v1.B);}
};

struct	sXY
{
		int	x,y;
};

/*****************************************************************************/
#define	EXPORT_LAYER_COUNT	16
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
		int			Flags;
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
//bool	operator==(sExpLayerTile const &v1)	{return (Tile==v1.Tile);}
bool	operator ==(sExpLayerTile const &v1)	{return(Tile==v1.Tile && Flags==v1.Flags);}

};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
struct	sLayerShadeGfx
{
	sRGBCol		RGB[4];
	int			Gfx;
	sXY			Pos;
	sXY			Ofs[4];
	int			TransMode;
};

/*****************************************************************************/
/*** Things ******************************************************************/
/*****************************************************************************/
struct	sLayerThingDef
{
	int		Store[32];
};

struct	sLayerThingActor
{
	int						ActorSpeed;
	int						ActorTurnRate;
	int						ActorHealth;
	int						ActorAttackStrength;
	bool					ActorCollisionFlag;
};

struct	sLayerThingItem
{
};

struct	sLayerThingPlatform
{
	int						PlatformSpeed;
	int						PlatformTurnRate;
	int						PlatformCollisionFlag;
	int						PlatformType;
	int						PlatformMoveType;
	int						PlatformTriStart;
	int						PlatformTriCount;
	int						PlatformParam0;
};

struct	sLayerThingTrigger
{
	int						TriggerWidth;
	int						TriggerHeight;
	int						TriggerTargetX;
	int						TriggerTargetY;
	int						TriggerTargetW;
	int						TriggerTargetH;
	int						TriggerVal0;
	int						TriggerVal1;
};

struct	sLayerThingFX
{
	int						FXSpeed;
	int						FXWidth;
	int						FXHeight;
};

struct	sLayerThingHazard
{
	int						HazardSpeed;
	int						HazardTurnRate;
	int						HazardHealth;
	int						HazardAttackStrength;
	int						HazardRespawn;
	bool					HazardCollisionFlag;
	int						HazardTriStart;
	int						HazardTriCount;

};

struct	sLayerThingData
{
	int						WaypointCount;
	int						Angle;
	int						Flip;

	union
	{
		sLayerThingDef		RESERVE;
		sLayerThingActor	Actor;
		sLayerThingItem		Item;
		sLayerThingPlatform	Platform;
		sLayerThingTrigger	Trigger;
		sLayerThingFX		FX;
		sLayerThingHazard	Hazard;
	};
};

struct	sLayerThingDataV9
{
	int						WaypointCount;

	union
	{
		sLayerThingDef		RESERVE;
		sLayerThingActor	Actor;
		sLayerThingItem		Item;
		sLayerThingPlatform	Platform;
		sLayerThingTrigger	Trigger;
		sLayerThingFX		FX;
		sLayerThingHazard	Hazard;
	};
};

struct	sLayerThingDataOLD
{
	int						WaypointCount;

	int						Speed;
	int						TurnRate;
	int						Health;
	int						AttackStrength;
	bool					CollisionFlag;
	bool					PlayerFlag;

// Platform
	int						MoveType;
	int						PlatformType;
// Boxes
	int						Width,Height;
	int						TriStart,TriCount;
	int						TargetX,TargetY;
// NO SPARE!!
};


#endif

