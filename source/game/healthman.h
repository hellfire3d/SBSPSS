/******************************/
/*** Health throw out stuff ***/
/******************************/

#ifndef	__GAME_HEALTH_MANAGER_H__
#define	__GAME_HEALTH_MANAGER_H__

/*****************************************************************************/
class	CPlayer;
class CHealthManager
{
public:
		enum
		{
			ITEM_MAX	=	160,
		};

		struct	sItemTable
		{
			u16			Count;
			u16			Life;
			u8			R,G,B;
		};

		struct	sItem
		{
			VECTOR	Pos;
			VECTOR	Vel;
			DVECTOR	ScrPos;
			u16		Life;
			u16		Count;
			TSPRT	Sprite;
		};

		void		init();
		void		shutdown();

		void		drop(DVECTOR const &Pos,int Amount,int Vel);

		void		checkPlayerCol(CPlayer *Thing);
		void		think(int frames);
		void		render();

protected:
		void		addItem(DVECTOR const &Pos,int TableIdx,int Angle,int Vel);

		sItem		ItemList[ITEM_MAX];
		sFrameHdr	*FrameHdr;

static	sItemTable	ItemTable[];
static	const int	ItemTableSize;
};

#endif	
