/*********************/
/*** Layer Trigger ***/
/*********************/

#include	<Davelib.h>
#include	<List2d.h>

//#include	"MkLevel.h"
#include	"MkLevelLayerTrigger.h"


/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerTrigger::PreProcess(CMkLevel *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerTrigger::Process(CMkLevel *Core)
{
		ProcessList(Core);
		printf("%i Trigger\n",ThingList.size());

}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerTrigger::Write(CMkLevel *Core,FILE *File,const char *LayerName)
{
int			ThisPos=ftell(File);
sThingHdr	Hdr;
int			i,ListSize=ThingList.size();
		
		Hdr.Count=ListSize;
		fwrite(&Hdr,sizeof(sThingHdr),1,File);

		for (i=0; i<ListSize; i++)
		{
			sMkLevelLayerThing	&ThisThing=ThingList[i];
			sThingTrigger		OutThing;

			OutThing.Type=ThisThing.Type;
			OutThing.Pos.X=ThisThing.XY[0].x;
			OutThing.Pos.Y=ThisThing.XY[0].y;
			OutThing.Width=ThisThing.Data.Trigger.TriggerWidth;
			OutThing.Height=ThisThing.Data.Trigger.TriggerHeight;
			OutThing.TargetPos.X=ThisThing.Data.Trigger.TriggerTargetX;
			OutThing.TargetPos.Y=ThisThing.Data.Trigger.TriggerTargetY;
			OutThing.TargetSize.X=ThisThing.Data.Trigger.TriggerTargetW;
			OutThing.TargetSize.Y=ThisThing.Data.Trigger.TriggerTargetH;
			OutThing.Val0=ThisThing.Data.Trigger.TriggerVal0;
			OutThing.Val1=ThisThing.Data.Trigger.TriggerVal1;
			fwrite(&OutThing,sizeof(sThingTrigger),1,File);
		}

		Size=ftell(File)-ThisPos;
		return(ThisPos);
}

/*****************************************************************************/
