/*****************/
/*** Map Stuph ***/
/*****************/


#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>

#include	"Map.h"


/*****************************************************************************/
void	CMap::Load(CFile *File,int Version)
{
// Version 1
int	Width;
int	Height;
int	VFix=0;	// Fix for New tileset shash

		if (Version<2) VFix=1;

		File->Read(&Width,sizeof(int));
		File->Read(&Height,sizeof(int));

		Delete();
		SetSize(Width,Height);
		for (int Y=0;Y<Height;Y++)
		{
			for (int X=0;X<Width;X++)
			{
				sMapElem	ThisElem;
				File->Read(&ThisElem,sizeof(sMapElem));
				ThisElem.Set+=VFix;
				if (Version==3) 
				{
					ThisElem.Set--;
					if (ThisElem.Set<0)
					{
						ThisElem.Set=0;
					}
				}

				Set(X,Y,ThisElem,TRUE);
			}
		}
}

/*****************************************************************************/
void	CMap::Save(CFile *File)
{
int	Width=GetWidth();
int	Height=GetHeight();

		File->Write(&Width,sizeof(int));
		File->Write(&Height,sizeof(int));

		for (int Y=0;Y<Height;Y++)
		{
			for (int X=0;X<Width;X++)
			{
				sMapElem	&ThisElem=Get(X,Y);
				File->Write(&ThisElem,sizeof(sMapElem));
			}
		}
}

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
int		Width=GetWidth();
int		Height=GetHeight();

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
void	CMap::Set(int X,int Y,sMapElem &Blk,BOOL Force)
{
int		Width=GetWidth();
int		Height=GetHeight();

		if (!Force) // if bank or tile invalid
			if (Blk.Set==-1 || Blk.Tile==-1) return;

// Make sure within map
		if ((X>=0 && X<Width) && (Y>=0 && Y<Height))
		{
			Map[X][Y]=Blk;
			if (Blk.Tile==0)
			{ // Only use one zero tile
				Map[X][Y].Set=0;
			}
		}
}

/*****************************************************************************/
void	CMap::Set(int StartX,int StartY,CMap &Blk,BOOL Force)
{
int		Width=Blk.GetWidth();
int		Height=Blk.GetHeight();

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				Set(StartX+X,StartY+Y,Blk.Get(X,Y),Force);
			}
		}
}

/*****************************************************************************/
void	CMap::Set(CMap &Src,int StartX,int StartY,int Width,int Height,BOOL Force)
{
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

/*****************************************************************************/
void	CMap::Set(CMap &Src,CRect &Rect,BOOL Force)
{
	Set(Src,Rect.left,Rect.top,Rect.Width(),Rect.Height(),Force);
}

/*****************************************************************************/
void	CMap::MirrorX(int Flag,CRect *R)
{
CMap	Old=*this;

		if (!R)
		{ // No rect, use full
			R=new CRect(0,0,GetWidth(),GetHeight());
		}

int		Ofs=(R->right+R->left)-1;
		for (int Y=R->top; Y<R->bottom; Y++)
		{
			for (int X=R->left; X<R->right; X++)
			{
				sMapElem	&ThisElem=Old.Get(X,Y);
				ThisElem.Flags^=Flag;
				Set(Ofs-X,Y,ThisElem,true);
			}
		}
}

/*****************************************************************************/
void	CMap::MirrorY(int Flag,CRect *R)
{
CMap	Old=*this;

		if (!R)
		{ // No rect, use full
			R=new CRect(0,0,GetWidth(),GetHeight());
		}

int		Ofs=(R->bottom+R->top)-1;
		for (int Y=R->top; Y<R->bottom; Y++)
		{
			for (int X=R->left; X<R->right; X++)
			{
				sMapElem	&ThisElem=Old.Get(X,Y);
				ThisElem.Flags^=Flag;
				Set(X,Ofs-Y,ThisElem,true);
			}
		}
}

/*****************************************************************************/
void	CMap::SetFlags(int Flags,int Mask,CRect *R)
{
		if (!R)
		{ // No rect, use full
			R=new CRect(0,0,GetWidth(),GetHeight());
		}

		for (int Y=R->top; Y<R->bottom; Y++)
		{
			for (int X=R->left; X<R->right; X++)
			{
				sMapElem	&ThisElem=Get(X,Y);
				ThisElem.Flags&=Mask;
				ThisElem.Flags|=Flags;
			}
		}

}

/*****************************************************************************/
void	CMap::Paste(CMap &Src,CRect *R)
{
		if (!R)
		{ // No rect, use full
			R=new CRect(0,0,GetWidth(),GetHeight());
		}

		for (int Y=0; Y<R->Height(); Y++)
		{
			for (int X=0; X<R->Width(); X++)
			{
				int	SrcX=X % Src.GetWidth();
				int	SrcY=Y % Src.GetHeight();
				sMapElem	&ThisElem=Src.Get(SrcX,SrcY);
				
				Set(X+R->left,Y+R->top,ThisElem,true);
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
				if (ThisElem.Tile==Tile.Tile)
					if (ThisElem.Set==Tile.Set || Tile.Tile==0) return(TRUE);
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

/*****************************************************************************/
void	CMap::RemoveSet(int Set)
{
int		Width=GetWidth();
int		Height=GetHeight();

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem	&ThisElem=Get(X,Y);
				if (ThisElem.Set==Set)
				{
					ThisElem.Set=0;
					ThisElem.Tile=0;
					ThisElem.Flags=0;
				}
			}
		}
}

/*****************************************************************************/
void	CMap::RemapSet(int Old,int New)
{
int		Width=GetWidth();
int		Height=GetHeight();

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem	&ThisElem=Get(X,Y);
				if (ThisElem.Set==Old)
				{
					ThisElem.Set=New;
				}
			}
		}
}