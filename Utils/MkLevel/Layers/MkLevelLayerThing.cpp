/*******************/
/*** Layer Thing ***/
/*******************/

#include	<DaveLib.h>
//#include	<List2d.h>

#include	"..\MkLevel.h"
#include	"MkLevelLayer.h"
#include	"MkLevelLayerThing.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CMkLevelLayerThing::CMkLevelLayerThing(sExpLayerHdr *LayerHdr)
{
u8		*Ptr=(u8*)LayerHdr;

		Type=LayerHdr->Type;
		SubType=LayerHdr->SubType;
		Width=LayerHdr->Width;
		Height=LayerHdr->Height;

int		*iPtr=(int*)(Ptr+sizeof(sExpLayerHdr));

int		i,ListSize=*iPtr++;
		ThingList.resize(ListSize);
		Ptr=(u8*)iPtr;
		for (i=0; i<ListSize; i++)
		{
			Ptr=LoadThing(ThingList[i],Ptr);
		}
		LoadThingNames((char*)Ptr);

}

/*****************************************************************************/
u8		*CMkLevelLayerThing::LoadThing(sMkLevelLayerThing &ThisThing,u8 *Ptr)
{
int		i,ListSize;

sLayerThingData	*DPtr=(sLayerThingData*)Ptr;

		ThisThing.Data=*DPtr++;
int		*iPtr=(int*)DPtr;

		ListSize=ThisThing.Data.WaypointCount;
		ThisThing.XY.resize(ListSize);
		for (i=0; i<ListSize; i++)
		{
			ThisThing.XY[i].x=*iPtr++;
			ThisThing.XY[i].y=*iPtr++;
		}
		
		return((u8*)iPtr);
}

/*****************************************************************************/
void	CMkLevelLayerThing::LoadThingNames(char *Ptr)
{
int		i,ListSize=ThingList.size();

		for (i=0; i<ListSize; i++)
		{
			sMkLevelLayerThing	&ThisThing=ThingList[i];

			ThisThing.Name=Ptr;
			Ptr+=strlen(Ptr)+1;
		}

}

/*****************************************************************************/
void	CMkLevelLayerThing::ProcessList(CMkLevel *Core)
{
int		i,ListSize=ThingList.size();
CIni	&Config=Core->GetConfig();

		for (i=0; i<ListSize; i++)
		{
			sMkLevelLayerThing	&ThisThing=ThingList[i];
			bool	Found=Config.GetInt(GetTypeName(),ThisThing.Name,ThisThing.Type);
			if (!Found)
				GObject::Error(ERR_FATAL,"%s not found in list\n",ThisThing.Name);
		}

}
