/**********************/
/*** Layer Platform ***/
/**********************/

#include	<Davelib.h>
#include	<List2d.h>

//#include	"MkLevel.h"
#include	"MkLevelLayerPlatform.h"


/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerPlatform::PreProcess(CMkLevel *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerPlatform::Process(CMkLevel *Core)
{
		ProcessList(Core);
		printf("%i Platforms\n",ThingList.size());
}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerPlatform::Write(FILE *File,const char *LayerName,const char *MapName)
{
int			ThisPos=ftell(File);
sThingHdr	Hdr;
int			i,ListSize=ThingList.size();
		
		Hdr.Count=ListSize;
		fwrite(&Hdr,sizeof(sThingHdr),1,File);

		for (i=0; i<ListSize; i++)
		{
			sMkLevelLayerThing	&ThisThing=ThingList[i];
			int					p,PointCount=ThisThing.XY.size();
			sThingPlatform			OutThing;

			OutThing.Type=ThisThing.Type;
			OutThing.Speed=ThisThing.Data.Speed;
			OutThing.TurnRate=ThisThing.Data.TurnRate;
			OutThing.Flags=ThisThing.Data.CollisionFlag;
			OutThing.PointCount=PointCount;
			fwrite(&OutThing,sizeof(sThingPlatform),1,File);

			for (p=0;p<PointCount;p++)
			{
				sThingPoint	Pnt;
				Pnt.X=ThisThing.XY[p].x;
				Pnt.Y=ThisThing.XY[p].y;
				fwrite(&Pnt,sizeof(sThingPoint),1,File);
			}
		}

		return(ThisPos);
}

/*****************************************************************************/
