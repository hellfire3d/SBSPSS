/*=========================================================================

	VIMAGE.CPP	

	Author:  Gary Liddon @ Climax
	Created:
	Project: TPW Parkgrab
	Purpose: An object that represents a bit of VRAM

	Copyright (c) 1998 G R Liddon

===========================================================================*/

#ifndef __VIMAGE_H__
#define __VIMAGE_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	Glib
	---- */
#include <gobject.hpp>
#include <fstream>

/*	Local
	----- */
#include "sprset.h"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class VRAMImage : protected GObject
{
public:

	VRAMImage(int NewWidthInTpages,int NewHeightInPixels);
	~VRAMImage(void);

	void WriteInTpageChunks(std::ofstream & Str);
	void Write(std::ofstream & Str);
	void PlotPal(SprPal const & PalToPlot);
	void PlotFrame(SprFrame const & FrameToPlot);
	void SaveAs16ColLbm(const char * Name);

	void	setDoCompress(bool newVal)
		{m_doCompress=newVal;}

	bool	getDoCompress(void) const
		{return(m_doCompress);}

	void getTpData(unsigned int tp,std::vector<u8> & dest) const;
			
protected:
	void	PlotFrame4(SprFrame const & Fr);
	void	PlotFrame8(SprFrame const & Fr);
	u16		GetPsxCol(Colour const & Col) const;

	int		WidthInBytes;
	int		WidthInTpages;
	int		HeightInTPages;
	int		HeightInPixels;
	int		aTPageHeight;
	int		TPageSizeInBytes;
	int		VramAreaBytes;
	bool	m_doCompress;

	u8 *	lbmData;
	u8 *	VRAMData;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */

/*---------------------------------------------------------------------- */

#else	/* __VIMAGE_H__ */

class VRAMImage;

#endif

/*===========================================================================
 end */
