/*=========================================================================

	ILBM.HPP

	Author:  Gary Liddon @ Watford
	Created: 30/03/92
	Purpose: Crappy ilbm reader

	Copyright (c) 1992 - 1997 Gary Liddon

===========================================================================*/

#ifndef __PC_GLIB_ILBM_HPP__
#define __PC_GLIB_ILBM_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	Glib
	---- */
#include "gtypes.h"

/*	Local
	----- */
#include "niff.hpp"
#include "gobject.hpp"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Class defintions
	---------------- */
class GLIB_API nilbm : public niff, public GObject
{
	public:
		 nilbm(char const *name);
		 nilbm(FILE *fp);
		 ~nilbm();
		int		GetWidth(void)	{ return w; };
		int		GetHeight(void)	{ return h; };
		int		GetRH(void)		{ return rh; };
		int		GetPlanes(void)	{ return planes;}
		U8 *	GetPalAddr()	{ return cmap;}
		                    
		void	DiscardBmap(void);
		U8 *	TakeBmap(void);
		
		void DiscardCmap();
		U8 *TakeCmap();

		u8 const * SeeBmap(){return(Bmap);}
		u8 const * SeeCmap(){return(cmap);}

		static void SavePbm(char *Name,U8 *Palette,U8 *Body,int W,int H);

	protected:
		U8 *GetBmapFromDisk();
		void GetBmHeadFromDisk();

		void starta();
		int w, h, rh;
		int planes;
		int comp;
		U8 *cmap;
		U8 *Bmap;
        
		struct bmhd								// Bit map header for ilbm
		{
			U16 w,h;
			S16	x,y;
			U8 planes;
			U8 masking;
			U8 compression;
			U8 pad;
			U16 trans_col;
			U8 x_ratio, y_ratio;
			U16 page_width,page_height;
		};
};


/*---------------------------------------------------------------------- */

#endif	/* __PC_GLIB_ILBM_HPP__ */

/*===========================================================================
 end */

