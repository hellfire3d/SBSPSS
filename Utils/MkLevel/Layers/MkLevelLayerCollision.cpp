/***********************/
/*** Layer Collision ***/
/***********************/

#include	<DaveLib.h>
#include	<List2d.h>

#include	"MkLevelLayer.h"
#include	"MkLevelLayerCollision.h"

/*
0	- Blank
1	- Solid block (no flips)
2	- slope0
3	- slope0 flip x
4	- slope1
5	- slope1 flip x
6	- slope2
7	- slope2 flip x
*/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CMkLevelLayerCollision::CMkLevelLayerCollision(sExpLayerHdr *LayerHdr)
{
		Type=LayerHdr->Type;
		SubType=LayerHdr->SubType;
		Width=LayerHdr->Width;
		Height=LayerHdr->Height;

sExpColTile	*MapPtr=(sExpColTile*)((int)LayerHdr+sizeof(sExpLayerHdr));

		Map.SetSize(Width,Height);
		
		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				Map.Set(X,Y,*MapPtr++);
			}
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerCollision::PreProcess(CMkLevel *Core)
{
		//printf("PreProcess Collision Layer\n");
}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerCollision::Process(CMkLevel *Core)
{
		//printf("Process Collision Layer\n");
}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerCollision::Write(FILE *File,const char *LayerName,const char *MapName)
{
sLayerHdr	Hdr;
int			ThisPos=ftell(File);
int			Width=Map.GetWidth();
int			Height=Map.GetHeight();

		Hdr.Type=Type;
		Hdr.SubType=SubType;
		Hdr.Width=Width;
		Hdr.Height=Height;
		fwrite(&Hdr,sizeof(sLayerHdr),1,File);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sExpColTile	&ThisElem=Map.Get(X,Y);	
				u8		OutElem;

				OutElem=ThisElem.Tile;
				OutElem|=ThisElem.Flags<<COLLISION_TYPE_FLAG_SHIFT;
/*
				OutElem=0;
				if (ThisElem.Tile || ThisElem.Flags)
				{
					int	FF=ThisElem.Tile & 1;
					int	T=(ThisElem.Tile>>1)+1;
					OutElem=((T-1)*4)+1;
					OutElem+=FF;
					OutElem|=ThisElem.Flags<<COLLISION_TYPE_FLAG_SHIFT;

				}

*/
/*				if (ThisElem.Tile>(u16)COLLISION_MASK)
				{
					printf("COLLISION OVERFLOW %s: %i,%i=(%i,%i)!!\n",MapName,X,Y,ThisElem.Tile,ThisElem.Flags);
				}
*/
				fwrite(&OutElem,sizeof(u8),1,File);
			}
		}
		PadFile(File);

		return(ThisPos);
}

/*****************************************************************************/
