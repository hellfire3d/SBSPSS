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
static const u8		s_collisionTileRemapTable[17]=
{
	0,	
	1,
	1,
	1,
	1,
	2,
	3,
	0,
	0,
	4,
	5,
	0,
	0,
	6,
	7,
	0,
	0,
};

int		CMkLevelLayerCollision::Write(CMkLevel *Core,FILE *File,const char *LayerName)
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

				OutElem=s_collisionTileRemapTable[ThisElem.Tile];
				OutElem|=ThisElem.Flags<<COLLISION_TYPE_FLAG_SHIFT;
				fwrite(&OutElem,sizeof(u8),1,File);
			}
		}
		PadFile(File);

		Size=ftell(File)-ThisPos;
		return(ThisPos);
}

/*****************************************************************************/
