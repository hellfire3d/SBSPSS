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
			
			SPAT_CLUMP		=   5,
			SPAT_MAX		=	(100/SPAT_CLUMP)+(SPAT_CLUMP-1),
			SPAT_LIFE		=	256,

		};

		struct	sItemTable
		{
			u8			Count;
			u8			R,G,B;
		};

		struct	sItem
		{
			VECTOR	Pos;
			VECTOR	Vel;
			DVECTOR	ScrPos;
			u16		Life;
			u16		Count;
			TSPRT	Sprite[2];
		};

		void		init();
		void		shutdown();

		void		drop(DVECTOR const &Pos,int Amount,int Vel);

		void		checkPlayerCol(CPlayer *Thing);
		void		think(int frames);
		void		render();

protected:
		void		addItem(DVECTOR const &Pos,int TableIdx,int Angle,int Vel);

		sItem		ItemList[SPAT_MAX];
		sFrameHdr	*FrameHdr;
		int			Frame;

static	sItemTable	ItemTable[];
static	const int	ItemTableSize;
};

#endif	
