/**********************/
/*** Layer Hazard ***/
/**********************/

#include	<Davelib.h>
#include	<List2d.h>

//#include	"MkLevel.h"
#include	"MkLevelLayerHazard.h"


/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerHazard::PreProcess(CMkLevel *Core)
{
int		i,ListSize;
		ProcessList(Core);

		ListSize=ThingList.size();
		RemapTable.resize(ListSize);
		for (i=0; i<ListSize; i++)
		{
			sMkLevelLayerThing	&ThisThing=ThingList[i];
			RemapTable[i]=Core->AddModel(ThisThing.Name,ThisThing.Data.Hazard.HazardTriStart,ThisThing.Data.Hazard.HazardTriCount);

		}

		printf("%i Hazards\n",ThingList.size());
}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerHazard::Process(CMkLevel *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerHazard::Write(FILE *File,const char *LayerName,const char *MapName)
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
			sThingHazard			OutThing;

			OutThing.Type=ThisThing.Type;
			OutThing.Speed=ThisThing.Data.Hazard.HazardSpeed;
			OutThing.TurnRate=ThisThing.Data.Hazard.HazardTurnRate;
			OutThing.Flags=ThisThing.Data.Hazard.HazardCollisionFlag;
			OutThing.PointCount=PointCount;
			OutThing.Respawn=ThisThing.Data.Hazard.HazardRespawn;
			OutThing.Gfx=RemapTable[i];
			fwrite(&OutThing,sizeof(sThingHazard),1,File);

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
