/*********************/
/*** List 2d Class ***/
/*********************/


#pragma warning( disable : 4786 )

#ifndef	__LIST_2D_HEADER__
#define	__LIST_2D_HEADER__

#include	<Vector>

/*****************************************************************************/
template <class T> class CList2d
{
public:
inline	int			GetWidth()
{
	return(List.size());
}

inline	int			GetHeight()
{
		if (GetWidth())
		{
			return(List[0].size());
		}
		return(0);
}

inline	void		SetSize(int Width,int Height)
{
		List.resize(Width);
		for (int i=0;i<Width;i++)
		{
			List[i].resize(Height);
		}
}

inline	void		Resize(int Width,int Height)
{
CList2d<T>	Old=*this;

int		OldWidth=Old.GetWidth();
int		OldHeight=Old.GetHeight();
int		MinW=min(Width,OldWidth);
int		MinH=min(Height,OldHeight);

		Delete();
		SetSize(Width,Height,TRUE);
		for (int Y=0; Y<MinH; Y++)
		{
			for (int X=0; X<MinW; X++)
			{
				Set(X,Y,Old.Get(X,Y));
			}
		}
}

inline	void		Delete()
{
int		Width=GetWidth();

		for (int i=0;i<Width;i++)
		{
			List[i].clear();
		}
		List.clear();
}

inline	T			&Get(int X,int Y)
{
		return(List[X][Y]);
}

inline	void		Set(int X,int Y,T &Src)
{
int		Width=GetWidth();
int		Height=GetHeight();

// Make sure within List
		if ((X>=0 && X<Width) && (Y>=0 && Y<Height))
		{
			List[X][Y]=Src;
		}
}

inline	void		Set(int X,int Y,CList2d &Src)
{
int		Width=Src.GetWidth();
int		Height=Src.GetHeight();

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				Set(StartX+X,StartY+Y,Src.Get(X,Y));
			}
		}
}

inline	bool		DoesContainElem(T &Tile)
{
int		Width=GetWidth();
int		Height=GetHeight();

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				T	&ThisElem=Get(X,Y);
					if (ThisElem==Elem) return(TRUE);
			}
		}
		return(FALSE);
}


inline	void	operator=(T &Src)	
{
int		Width=Src.GetWidth();
int		Height=Src.GetHeight();

		Delete();
		SetSize(Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				Set(X,Y,Src.Get(StartX+X,StartY+Y),Force);
			}
		}

}

protected:
		std::vector< std::vector<T> > List;
};


/*****************************************************************************/
#endif
