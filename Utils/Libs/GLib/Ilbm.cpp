/*=========================================================================

	ILBM.HPP

	Author:  Gary Liddon @ Watford
	Created: 30/03/92
	Purpose: Crappy ilbm reader

	Copyright (c) 1992 - 1997 Gary Liddon

===========================================================================*/

/* -----------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <string.h>

/*	Glib
	---- */
#include "gutils.h"

/*	Local
	----- */
#include "ilbm.hpp"
#include "iff.hpp"

/*	Graphics
	-------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
nilbm::nilbm(char const *name) : niff(name,ILBM)
{
	if (err_no==NOT_FORM)
		{
		if (open(name,PBM))
			{
			err_no=NO_ERROR;
			}
		}

	if (!err_no)
		starta();
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
nilbm::nilbm(FILE *fp) : niff(fp,ILBM)
{
	if (err_no==PASSED_ERR)
		if (mount_form(PBM))
			err_no=NO_ERROR;

	starta();
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
nilbm::~nilbm()
{
	DiscardCmap();
	DiscardBmap();
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void nilbm::starta()
{
	cmap=NULL;
	Bmap=NULL;
	
	if (err_no)
		return;

	if ((!file_opened) && (!form_mounted))
		return;

	GetBmHeadFromDisk();


	if (goto_hunk(CMAP))
		cmap=get_hunk();

	Bmap=GetBmapFromDisk();
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
U8 *nilbm::TakeBmap()
{
	U8* Retp=Bmap;
	Bmap=NULL;
	return Retp;	
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
U8 *nilbm::TakeCmap()
{
	U8* Retp=cmap;
	cmap=NULL;
	return Retp;	
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void nilbm::DiscardBmap()
{
	if (Bmap)
		delete Bmap;
		
	Bmap=NULL;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void nilbm::DiscardCmap()
{
	if (cmap)
		delete cmap;
		
	cmap=NULL;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void nilbm::GetBmHeadFromDisk()
{
	if (goto_hunk(BMHD))
		{
		GetIntelLong();
		GetIntelLong();

		w=GetIntelWord();
		h=GetIntelWord();

		GetIntelWord();
		GetIntelWord();

		planes=fgetc(fp);

		fgetc(fp);
		comp=fgetc(fp);

		rh = h;

		}
}



/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
U8 *nilbm::GetBmapFromDisk()
{
	U8 *buffa=NULL;
	U8 *Ptr;
	
	if (file_opened)
		{
		if (goto_hunk(BODY))
			{
			long temp;

			fread(&temp,1,sizeof(ULONG),fp);
			fread(&temp,1,sizeof(ULONG),fp);
			
			if (!(buffa=new U8[w*h]))
				Error(ERR_FATAL,"Allocating ILBM body (%ld)",(long)w*(long)h);
			
			U8 *line_buffer;

			int NormWidth=((w+7)&(0xfff8));

			int bwidth=NormWidth/8;

			if (!(line_buffer=new U8[NormWidth*4]))				//MA increased for safety incase of bad encoding
				return NULL;
			else
				{
				int z,dest;										// Init source count

				for (int line=0;line<(h);line++)
					{
					dest=0;												//	Destination count

					memset(line_buffer,0,NormWidth);					// Clear out buffer

					if (form_name==PBM)
						{
						if (comp)
							{
							dest=0;

							while (dest < w)
								{
								s8 val = fgetc(fp);

								if (val<0)
									{
									val=(0-val)+1;
									U8 ch=fgetc(fp);

									while (val--)
										{
										line_buffer[dest]=ch;
										dest++;
										}
									}
								else if (val>=0)
									{
									val++;

									while (val--)
										{
										line_buffer[dest]=fgetc(fp);
										dest++;
										}
									}
								}
							}
						else
							{
							int WidthToRead = GU_AlignVal(w, 2);
							fread(line_buffer,WidthToRead,1,fp);
							}
						}
					else
						{
						for (int p=0;p<planes;p++)
							{
							dest=0;

							if (comp)
								{
								while (dest < bwidth)
									{
									S8 val = fgetc(fp);
									Ptr=&line_buffer[dest*8];

									if (val<0)
										{
										val=(0-val)+1;
										U8 ch=fgetc(fp);

										while (val--)
											{

											for (z=7;z>=0;z--)
												*Ptr++ |= ((ch>>(z))&1)<<p;

											dest++;
											}
										}
									else if (val>=0)
										{
										val++;

										while (val--)
											{
											U8 ch=fgetc(fp);
							
											for (z=7;z>=0;z--)
												*Ptr++ |=  ((ch>>(z))&1)<<p;

											dest++;
											}
										}
									}
								}
							else
								{
								for (int x=0;x<bwidth;x++)
									{
									Ptr=&line_buffer[dest*8];

									U8 ch=fgetc(fp);
									for (z=7;z>=0;z--)
										*Ptr++ |=  ((ch>>(z))&1)<<p;
									dest++;
									}
								}
							}
						}

					memcpy(&buffa[line*w],line_buffer,w);
					}
				}
			delete line_buffer;
			}
		}
	return buffa;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void nilbm::SavePbm(char *Name,U8 *Palette,U8 *Body,int W,int H)
{
	if (Palette && Body)
		{
		IFF_FILE outfile;

		if (outfile.write(Name) != 0)
			{
			int i, j;

			outfile.form("PBM ");

			outfile.chunk("BMHD");
			outfile.writeword(W);
			outfile.writeword(H);
			outfile.writeword(0);  // xpos
			outfile.writeword(0);  // ypos
			outfile.writechar(8); // num-planes
			outfile.writechar(mskNone); // masking
			outfile.writechar(cmpNone); // compression
			outfile.writechar(0); // Pad byte
			outfile.writeword(0); // Transparent colour
			outfile.writechar(1);  // xAspect
			outfile.writechar(1);  // yAspect
			outfile.writeword(320);
			outfile.writeword(200);
			outfile.endchunk();

			if (256)
				{
				outfile.chunk("CMAP");
				for (i = 0; i < 3*256; i++)
				outfile.writechar(Palette[i]);

				outfile.endchunk();
				}

			U8 *ptr;

			outfile.chunk("BODY");

			for (i = 0; i < H; i++)
				{
				// Save out a planar scanline
				ptr = &Body[(long) i * (long) W];

				j = W;
				while (j--)
					outfile.writechar(*ptr++);

				if (W&1)
					{
					int l = W&1;
					for (int k=0; k<l; k++)
						outfile.writechar(0xfe);
					}
				}

			outfile.endchunk();

			outfile.endform();
			outfile.close();
			}
		else
			Error(ERR_FATAL,"Error trying to write %s",Name);
		}
	else
		Error(ERR_FATAL,"Image not defined so can't write %s",Name);

}

/*===========================================================================
 end */




