/******************/
/*** Map Stuph ***/
/*****************/

#ifndef	__MAP_HEADER__
#define	__MAP_HEADER__

#include	<Vector>

struct	sMapElem
{
	int		Set;
	int		Tile;
	int		Flags;
};

enum
{
	MapElemFlagMirrorX=	1<<0,
	MapElemFlagMirrorY=	1<<1,
};

/*****************************************************************************/
class	CMap
{
public:
		CMap(){};
		~CMap(){};

		int			GetWidth()			{return(Map.size());}
		int			GetHeight()			{return(Map[0].size());}

		void		SetSize(int Width,int Height,BOOL Clear=FALSE);
		void		SetWidth(int Width);
		void		SetHeight(int Height);
		void		Clear();

		sMapElem	&GetTile(int X,int Y);
		void		SetTile(int X,int Y,sMapElem &New);

protected:
		std::vector< std::vector<sMapElem> > Map;
};


/*****************************************************************************/
#endif
