/******************/
/*** Map Stuph ***/
/*****************/

#ifndef	__MAP_HEADER__
#define	__MAP_HEADER__

#include	<Vector>

struct	sMapElem
{
	int		Bank;
	int		Tile;
	int		Flags;
};

/*****************************************************************************/
class	CMap
{
public:
		CMap(){};
		~CMap(){};

		int		GetWidth();
		int		GetHeight();

		void	SetSize(int Width,int Height);
		void	SetWidth(int Width);
		void	SetHeight(int Height);

protected:
		std::vector< std::vector<sMapElem> > Map;
};


/*****************************************************************************/
#endif
