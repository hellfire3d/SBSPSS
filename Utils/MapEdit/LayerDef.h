/*** Layer Defs ***/
/******************/

#ifndef	__LAYER_DEFS_HEADER__
#define	__LAYER_DEFS_HEADER__

/*****************************************************************************/
enum LAYER_TYPE
{
	LAYER_TYPE_TILE=0,
	LAYER_TYPE_MAX
};

enum LAYER_SUBTYPE
{
	LAYERTILE_BACK=0,
	LAYERTILE_MID,
	LAYERTILE_ACTION,
	LAYERTILE_FORE,
};

enum	TILE_FLAG
{
	TILE_FLAG_MIRROR_X=1<<0,
	TILE_FLAG_MIRROR_Y=1<<1,
	TILE_FLAG_MIRROR_XY=TILE_FLAG_MIRROR_X|TILE_FLAG_MIRROR_Y,
};


/*****************************************************************************/
#endif