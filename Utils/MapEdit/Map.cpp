/******************/
/*** Map Stuph ***/
/*****************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>

#include	"Map.h"


/*****************************************************************************/
void	CMap::SetSize(int Width,int Height,BOOL Clear)
{
	Map.resize(Width);
	for (int i=0;i<Width;i++)
	{
		Map[i].resize(Height);
	}

	if (Clear)
	{
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
void	CMap::Clear()
{
int	Width=GetWidth();

	for (int i=0;i<Width;i++)
	{
		Map[i].clear();
	}
	Map.clear();

}

/*****************************************************************************/
sMapElem	&CMap::GetTile(int X,int Y)	
{
	return(Map[X][Y]);
}

/*****************************************************************************/
void	CMap::SetTile(int X,int Y,sMapElem &New)
{
		Map[X][Y]=New;
}

