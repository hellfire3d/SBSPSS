/****************/
/*** Layer FX ***/
/****************/

#include	<Davelib.h>
#include	<List2d.h>

#include	"..\MkLevel.h"
#include	"MkLevelLayerFX.h"


/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerFX::PreProcess(CMkLevel *Core)
{
		//printf("Pre-Process FX Layer ()\n");
}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerFX::Process(CMkLevel *Core)
{
		ProcessList(Core);
		printf("%i FX\n",ThingList.size());
}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerFX::Write(CMkLevel *Core,FILE *File,const char *LayerName)
{
int			ThisPos=ftell(File);
sThingHdr	Hdr;
int			i,ListSize=ThingList.size();
		
		Hdr.Count=ListSize;
		fwrite(&Hdr,sizeof(sThingHdr),1,File);

		for (i=0; i<ListSize; i++)
		{
			sMkLevelLayerThing	&ThisThing=ThingList[i];
			sThingFX			OutThing;

			OutThing.Type=ThisThing.Type;
			OutThing.Speed=ThisThing.Data.FX.FXSpeed;
			OutThing.Pos.X=ThisThing.XY[0].x;
			OutThing.Pos.Y=ThisThing.XY[0].y;
			OutThing.Size.X=ThisThing.Data.FX.FXWidth;
			OutThing.Size.Y=ThisThing.Data.FX.FXHeight;
			fwrite(&OutThing,sizeof(sThingFX),1,File);

		}

		Size=ftell(File)-ThisPos;
		return(ThisPos);
}

/*****************************************************************************/
