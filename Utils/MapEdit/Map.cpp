/*****************/
/*** Map Stuph ***/
/*****************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>

#include	"Map.h"


/*****************************************************************************/
void	CMap::SetSize(int Width,int Height,BOOL ClearFlag)
{
	Map.resize(Width);
	for (int i=0;i<Width;i++)
	{
		Map[i].resize(Height);
	}

	if (ClearFlag)	Clear();
}

/*****************************************************************************/
void	CMap::Clear()
{
int	Width=GetWidth();
int	Height=GetHeight();

		for (int Y=0;Y<Height;Y++)
		{
			for (int X=0;X<Width;X++)
			{
				Map[X][Y].Set=0;
				Map[X][Y].Flags=0;
				Map[X][Y].Tile=0;
			}
		}
}

/*****************************************************************************/
void	CMap::SetWidth(int Width)
{
}

/*****************************************************************************/
void	CMap::SetHeight(int Height)
{
}
/*****************************************************************************/
int		CMap::GetWidth()			
{
	return(Map.size());
}

/*****************************************************************************/
int		CMap::GetHeight()
{
		if (GetWidth())
		{
			return(Map[0].size());
		}
		return(0);
}

/*****************************************************************************/
void	CMap::Delete()
{
int		Width=GetWidth();

		for (int i=0;i<Width;i++)
		{
			Map[i].clear();
		}
		Map.clear();
}

/*****************************************************************************/
sMapElem	&CMap::Get(int X,int Y)	
{
		return(Map[X][Y]);
}

/*****************************************************************************/
void	CMap::Set(int X,int Y,sMapElem &Blk)
{
int		Width=GetWidth();
int		Height=GetHeight();

// Make sure within map
		if ((X>=0 && X<Width) && (Y>=0 && Y<Height))
		{
			Map[X][Y]=Blk;
		}
}

/*****************************************************************************/
void	CMap::Set(int StartX,int StartY,CMap &Blk)
{
int		Width=Blk.GetWidth();
int		Height=Blk.GetHeight();

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				Set(StartX+X,StartY+Y,Blk.Get(X,Y));
			}
		}
}

/*****************************************************************************/
void	CMap::Set(CMap &Src,int StartX,int StartY,int Width,int Height)
{
		Delete();
		SetSize(Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				Set(X,Y,Src.Get(StartX+X,StartY+Y));
			}
		}

}

/*****************************************************************************/
void	CMap::MirrorX(int Flag)
{
CMap	Old=*this;

int		Width=GetWidth();
int		Height=GetHeight();

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem	&ThisElem=Old.Get(X,Y);
				ThisElem.Flags^=Flag;
				Set((Width-1)-X,Y,ThisElem);
			}
		}

}

/*****************************************************************************/
void	CMap::MirrorY(int Flag)
{
CMap	Old=*this;

int		Width=GetWidth();
int		Height=GetHeight();

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem	&ThisElem=Old.Get(X,Y);
				ThisElem.Flags^=Flag;
				Set(X,(Height-1)-Y,ThisElem);
			}
		}

}

/*****************************************************************************/
BOOL	CMap::DoesContainTile(sMapElem &Tile)
{
int		Width=GetWidth();
int		Height=GetHeight();

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem	&ThisElem=Get(X,Y);
				if (ThisElem.Set==Tile.Set && ThisElem.Tile==Tile.Tile) return(TRUE);
			}
		}
		return(FALSE);
}

/*****************************************************************************/
void	CMap::Resize(int Width,int Height)
{
CMap	Old=*this;

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
