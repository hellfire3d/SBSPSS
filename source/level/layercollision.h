/******************************/
/*** Collision  Layer Class ***/
/******************************/

#ifndef __LAYER_COLLISION_H__
#define __LAYER_COLLISION_H__

#include	<dstructs.h>


#if	!defined(__USER_CDBUILD__)
#define __SHOW_COLLISION__
#endif


/*****************************************************************************/
class CLayerCollision
{
public:
		enum
		{
			COL_Y_OFS	=8,
		};

		CLayerCollision(sLayerHdr *Hdr);
virtual	~CLayerCollision();

virtual	void			shutdown();

		u16				GetYPos(int Y)						{return(ColYTable[Y+COL_Y_OFS]);}

		int				getHeightFromGround(int _x,int _y,int _maxHeight=32);
		int				getHeightFromGroundExcluding(int _x,int _y,int _exclusion,int _maxHeight=32);
		int				getHeightFromGroundNonSB(int _x,int _y,int _maxHeight=32)	{return( CLayerCollision::getHeightFromGroundExcluding( _x, _y, COLLISION_TYPE_FLAG_SB_NOMOVE, _maxHeight ) );}
		int				getHeightFromGroundCart(int _x,int _y,int _maxHeight=32);
		int				getHeightFromGroundAmmo(int _x,int _y,int _maxHeight=32);

inline	u8				*getMapPtr(int _x,int _y)			
		{
			int		Ofs=(_x>>4)+GetYPos(_y>>4);
			return(&Map[Ofs]);
//			return(&Map[(_x>>4)+((_y>>4)*MapWidth)]);
		}

inline	int				getCollisionBlock(int _x,int _y)	
		{
			u8		Col=*getMapPtr(_x,_y);
			return	(Col);
//			return Map[(_x>>4)+((_y>>4)*MapWidth)];
		}

inline	u8				Get(int X,int Y)					
		{
			return(getCollisionBlock(X,Y) & COLLISION_TILE_MASK);
//			return(Map[X+(Y*MapWidth)]&COLLISION_TILE_MASK);
		}


#ifdef __SHOW_COLLISION__
		void			render(DVECTOR &MapPos);
#endif

		int				getMapWidth()	{return(MapWidth);}
		int				getMapHeight()	{return(MapHeight);}

protected:
		sLayerHdr		*LayerHdr;
		int				MapWidth,MapHeight;


		u8				*Map;

		static u8		s_collisionTable[];
		u16				*ColYTable;
};



/*****************************************************************************/

#endif