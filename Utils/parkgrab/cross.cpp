/*=========================================================================

	CROSS.CPP	

	Author:  Gary Liddon @ Climax
	Created:
	Project: Diablo Playstation Conversion
	Purpose: Find A Crosshair on a frame

	Copyright (c) 1996 Director's Cut Ltd.

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	STL
	--- */

/*	Glib
	---- */

/*	Local
	----- */
#include "cross.h"

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
using namespace std;

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Classes
	------- */


/*----------------------------------------------------------------------
	Description:
		Find a cross hair in this frame for animation offset and store it
		into member vars x and y.

	Returns:
		True if a cross hair is found
  ---------------------------------------------------------------------- */
bool CROSS_RES::FindCrossHair(
							  Frame const & Fr,	/* Frame we're looking for a cross-hair in */
							  u8 CrossCol		/* Pixel value of cross hair we're looking for */
							  )
{
	u8 const *	FrmData;
	int *		CountCol=NULL;
	int *		CountRow=NULL;

	int			Width;
	int			Height;
	bool		RetVal;

	RetVal=false;

	Width=Fr.GetWidth();
	Height=Fr.GetHeight();

	if (Width && Height)
		{
		int Col;
		int Row;

		if (!(CountCol=new int[Width]))
			{
			cout<<"Width is "<<Width<<endl;
			GObject::Error(ERM_OUTOFMEM);
			}

		if (!(CountRow=new int[Height]))
			{
			cout<<"Height is "<<Width<<endl;
			GObject::Error(ERM_OUTOFMEM);
			}	

		memset(CountRow,0,sizeof(int)*Height);
		memset(CountCol,0,sizeof(int)*Width);

		FrmData=Fr.SeeData();

		for (int yy=0;yy<Height;yy++)
			{
			u8 const * ThisRow=&FrmData[yy*Width];

			for (int xx=0;xx<Width;xx++)
				{
				if (ThisRow[xx] == CrossCol)
					{
					CountRow[yy]++;
					CountCol[xx]++;
					}
				}
			}

		Col=FindLargestIndex(CountCol,Width);
		Row=FindLargestIndex(CountRow,Height);

		if ((Col != -1) && (Row != -1))
			{
			x=Col;
			y=Row;
			RetVal=true;
			}
		}

	if (CountCol)
		delete CountCol;

	if (CountRow)
		delete CountRow;

	return(RetVal);
}

/*----------------------------------------------------------------------
	Description:
	Returns:
  ---------------------------------------------------------------------- */
int CROSS_RES::FindLargestIndex(int * CountCol,int Width)
{
	int		LargestIndex=-1;
	int		Largest=0;

	for (int f=0;f<Width;f++)
		{
		if (CountCol[f] > Largest)
			{
			Largest=CountCol[f];
			LargestIndex=f;
			}
		}

	return(LargestIndex);
}

/*===========================================================================
 end */

