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

BOOL	operator==(sMapElem const &v1)	// Doesnt Check flags
		{
		return(Set==v1.Set && Tile==v1.Tile);
		}
};

/*****************************************************************************/
class	CFile;
class	CPoint;
class	CMap
{
public:
		CMap(){};
		~CMap(){};

		int			GetWidth();
		int			GetHeight();
		BOOL		IsValid()			{return(GetHeight()!=0);}

		void		SetSize(int Width,int Height,BOOL Clear=FALSE);
		void		SetWidth(int Width);
		void		SetHeight(int Height);
		void		Clear();
		void		Delete();
		void		MirrorX(int Flag,CRect *R=NULL);
		void		MirrorY(int Flag,CRect *R=NULL);
		void		SetFlags(int Flags,int Mask,CRect *R=NULL);

		sMapElem	&Get(int X,int Y);
		void		Set(int X,int Y,sMapElem &Blk,BOOL Force=FALSE);
		void		Set(int X,int Y,CMap &Blk,BOOL Force=FALSE);
		void		Set(CMap &Src,int StartX,int StartY,int Width,int Height,BOOL Force=FALSE);
		void		Set(CMap &Src,CRect &Rect,BOOL Force=FALSE);
		void		Paste(CMap &Src,CRect *R);

		void		Resize(int Width,int Height);

		BOOL		DoesContainTile(sMapElem &Tile);

		void		Load(CFile *File,int Version);
		void		Save(CFile *File);

		void		RemoveSet(int Set);
		void		RemapSet(int Old,int New);



inline	void	operator=(CMap &Src)	
{
int		Width=Src.GetWidth();
int		Height=Src.GetHeight();
		Delete();
		SetSize(Width,Height);
		Set(Src,0,0,Width,Height);
}

protected:
		std::vector< std::vector<sMapElem> > Map;
};


/*****************************************************************************/
#endif
