/*=========================================================================

	FILENAME.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 G R Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	Glib
	---- */
#include <frame.hpp>
#include <misc.hpp>

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */
using namespace std;

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */
struct RGB
{
	u8	r,g,b;
};

class BITMAP
{
public:
	BITMAP(void)
		{
		m_width=0;
		m_height=0;
		}

	void setSize(int width,int height)
		{
		bitMap.resize(width*height);

		m_width=width;
		m_height=height;
		}

	void clear(void)
		{
		if (m_width && m_height)
			memset(&bitMap[0],0,m_width*m_height);
		}

	void line(unsigned int y,unsigned int x,u8 pix)
	{
		if (x >= m_width)
		{
			GObject::Error(ERR_WARNING,"Out of X boundary - %d %d\n", x, m_width);
			x = m_width-1;
		}
		if (y >= m_height)
		{
			GObject::Error(ERR_WARNING,"Out of Y boundary - %d %d\n", y, m_height);
			y = m_height-1;
		}
		bitMap[y*m_width+x]=pix;

	}

	u8 const * getBitMap(void) const
		{return(&bitMap[0]);}

protected:
	int				m_width;
	int				m_height;
	vector<u8>		bitMap;
};

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>


#define BI_RGB          0
#define BI_RLE8         1
#define BI_RLE4         2
#define BI_BITFIELDS    3

#define OS2INFOHEADERSIZE  12
#define WININFOHEADERSIZE  40


typedef struct BITMAPFILEHEADER
{
   unsigned long  bfType;
   unsigned long  bfSize;
   unsigned short bfReserved1;
   unsigned short bfReserved2;
   unsigned long  bfOffBits;
} BITMAPFILEHEADER;


/* Used for both OS/2 and Windows BMP. 
 * Contains only the parameters needed to load the image 
 */
typedef struct BITMAPINFOHEADER
{
   unsigned long  biWidth;
   unsigned long  biHeight;
   unsigned short biBitCount;
   unsigned long  biCompression;
} BITMAPINFOHEADER;


typedef struct WINBMPINFOHEADER  /* size: 40 */
{
   unsigned long  biWidth;
   unsigned long  biHeight;
   unsigned short biPlanes;
   unsigned short biBitCount;
   unsigned long  biCompression;
   unsigned long  biSizeImage;
   unsigned long  biXPelsPerMeter;
   unsigned long  biYPelsPerMeter;
   unsigned long  biClrUsed;
   unsigned long  biClrImportant;
} WINBMPINFOHEADER;


typedef struct OS2BMPINFOHEADER  /* size: 12 */
{
   unsigned short biWidth;
   unsigned short biHeight;
   unsigned short biPlanes;
   unsigned short biBitCount;
} OS2BMPINFOHEADER;



/* read_bmfileheader:
 *  Reads a BMP file header and check that it has the BMP magic number.
 */
static int read_bmfileheader(Gifstream & f, BITMAPFILEHEADER *fileheader)
{
   fileheader->bfType = f.Get16();
   fileheader->bfSize= f.Get32();
   fileheader->bfReserved1= f.Get16();
   fileheader->bfReserved2= f.Get16();
   fileheader->bfOffBits= f.Get32();

   if (fileheader->bfType != 19778)
      return -1;

   return 0;
}



/* read_win_bminfoheader:
 *  Reads information from a BMP file header.
 */
static int read_win_bminfoheader(Gifstream & f, BITMAPINFOHEADER *infoheader)
{
   WINBMPINFOHEADER win_infoheader;

   win_infoheader.biWidth = f.Get32();
   win_infoheader.biHeight = f.Get32();
   win_infoheader.biPlanes = f.Get16();
   win_infoheader.biBitCount = f.Get16();
   win_infoheader.biCompression = f.Get32();
   win_infoheader.biSizeImage = f.Get32();
   win_infoheader.biXPelsPerMeter = f.Get32();
   win_infoheader.biYPelsPerMeter = f.Get32();
   win_infoheader.biClrUsed = f.Get32();
   win_infoheader.biClrImportant = f.Get32();

   infoheader->biWidth = win_infoheader.biWidth;
   infoheader->biHeight = win_infoheader.biHeight;
   infoheader->biBitCount = win_infoheader.biBitCount;
   infoheader->biCompression = win_infoheader.biCompression;

   return 0;
}



/* read_os2_bminfoheader:
 *  Reads information from an OS/2 format BMP file header.
 */
static int read_os2_bminfoheader(Gifstream & f, BITMAPINFOHEADER *infoheader)
{
   OS2BMPINFOHEADER os2_infoheader;

   os2_infoheader.biWidth = f.Get16();
   os2_infoheader.biHeight = f.Get32();
   os2_infoheader.biPlanes = f.Get32();
   os2_infoheader.biBitCount = f.Get32();

   infoheader->biWidth = os2_infoheader.biWidth;
   infoheader->biHeight = os2_infoheader.biHeight;
   infoheader->biBitCount = os2_infoheader.biBitCount;
   infoheader->biCompression = 0;

   return 0;
}


/* read_bmicolors:
 *  Loads the color pallete for 1,4,8 bit formats.
 */
static void read_bmicolors(int ncols, RGB *pal, Gifstream & f,int win_flag)
{
	int i;

	for (i=0; i<ncols; i++)
		{
		pal[i].b = f.get();
		pal[i].g = f.get();
		pal[i].r = f.get();

		if (win_flag)
			f.get();
		}
}



/* read_1bit_line:
 *  Support function for reading the 1 bit bitmap file format.
 */
static void read_1bit_line(int length, Gifstream & f, BITMAP *bmp, int line)
{
   unsigned char b[32];
   unsigned long n;

   int i, j, k;
   int pix;

	for (i=0; i<length; i++)
		{
		j = i % 32;
		if (j == 0)
			{
			n = f.Get32();

			for (k=0; k<32; k++)
				{
				b[31-k] = n & 1;
				n = n >> 1;
				}
	      }

		pix = b[j];
		bmp->line(line,i,pix);
		}
}



/* read_4bit_line:
 *  Support function for reading the 4 bit bitmap file format.
 */
static void read_4bit_line(int length, Gifstream & f, BITMAP *bmp, int line)
{
   unsigned char b[8];
   unsigned long n;
   int i, j, k;
   int temp;
   int pix;

	for (i=0; i<length; i++)
		{
		j = i % 8;
		if (j == 0)
			{
			n = f.Get32();

			for (k=0; k<4; k++)
				{
				temp = n & 255;
				b[k*2+1] = temp & 15;
				temp = temp >> 4;
				b[k*2] = temp & 15;
				n = n >> 8;
				}
			}

		pix = b[j];
		bmp->line(line,i,pix);
		}
}



/* read_8bit_line:
 *  Support function for reading the 8 bit bitmap file format.
 */
static void read_8bit_line(int length, Gifstream & f, BITMAP *bmp, int line)
{
   unsigned char b[4];
   unsigned long n;
   int i, j, k;
   int pix;

	for (i=0; i<length; i++)
		{
		j = i % 4;
		if (j == 0)
			{
			n = f.Get32();
			for (k=0; k<4; k++)
				{
				b[k] = n & 255;
				n = n >> 8;
				}
			}
		pix = b[j];
		bmp->line(line,i,pix);
		}
}


/* read_24bit_line:
 *  Support function for reading the 24 bit bitmap file format, doing
 *  our best to convert it down to a 256 color pallete.
 */
static void read_24bit_line(int length, Gifstream & f, BITMAP *bmp, int line)
{
#if 0
   int i, nbytes;
   RGB c;

   nbytes=0;

   for (i=0; i<length; i++) {
      c.b = f.get();
      c.g = f.get();
      c.r = f.get();
      bmp->line[line][i*3+_rgb_r_shift_24/8] = c.r;
      bmp->line[line][i*3+_rgb_g_shift_24/8] = c.g;
      bmp->line[line][i*3+_rgb_b_shift_24/8] = c.b;
      nbytes += 3;
   }

   nbytes = nbytes % 4;
   if (nbytes != 0)
      for (i=nbytes; i<4; i++)
	 f.get();;
#endif
}



/* read_image:
 *  For reading the noncompressed BMP image format.
 */
static void read_image(Gifstream & f, BITMAP *bmp, BITMAPINFOHEADER *infoheader)
{
   int i, line;

   for (i=0; i<(int)infoheader->biHeight; i++) {
      line = i;

      switch (infoheader->biBitCount) {

	 case 1:
	    read_1bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;

	 case 4:
	    read_4bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;

	 case 8:
	    read_8bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;

	 case 24:
	    read_24bit_line(infoheader->biWidth, f, bmp, infoheader->biHeight-i-1);
	    break;
      }
   }
}



/* read_RLE8_compressed_image:
 *  For reading the 8 bit RLE compressed BMP image format.
 */
static void read_RLE8_compressed_image(Gifstream & f, BITMAP *bmp, BITMAPINFOHEADER *infoheader)
{
   unsigned char count, val, val0;
   int j, pos, line;
   int eolflag, eopicflag;

   eopicflag = 0;
   line = infoheader->biHeight - 1;

   while (eopicflag == 0) {
      pos = 0;                               /* x position in bitmap */
      eolflag = 0;                           /* end of line flag */

      while ((eolflag == 0) && (eopicflag == 0)) {
	 count = f.get();
	 val = f.get();

	 if (count > 0) {                    /* repeat pixel count times */
	    for (j=0;j<count;j++) {
	       bmp->line(line,pos,val);
	       pos++;
	    }
	 }
	 else {
	    switch (val) {

	       case 0:                       /* end of line flag */
		  eolflag=1;
		  break;

	       case 1:                       /* end of picture flag */
		  eopicflag=1;
		  break;

	       case 2:                       /* displace picture */
		  count = f.get();
		  val = f.get();
		  pos += count;
		  line -= val;
		  break;

	       default:                      /* read in absolute mode */
		  for (j=0; j<val; j++) {
		     val0 = f.get();
		     bmp->line(line,pos,val0);
		     pos++;
		  }

		  if (j%2 == 1)
		     val0 = f.get();    /* align on word boundary */
		  break;

	    }
	 }

	 if (pos > (int)infoheader->biWidth)
	    eolflag=1;
      }

      line--;
      if (line < 0)
	 eopicflag = 1;
   }
}



/* read_RLE4_compressed_image:
 *  For reading the 4 bit RLE compressed BMP image format.
 */
static void read_RLE4_compressed_image(Gifstream & f, BITMAP *bmp, BITMAPINFOHEADER *infoheader)
{
   unsigned char b[8];
   unsigned char count;
   unsigned short val0, val;
   int j, k, pos, line;
   int eolflag, eopicflag;

   eopicflag = 0;                            /* end of picture flag */
   line = infoheader->biHeight - 1;

   while (eopicflag == 0) {
      pos = 0;
      eolflag = 0;                           /* end of line flag */

      while ((eolflag == 0) && (eopicflag == 0)) {
	 count = f.get();
	 val = f.get();

	 if (count > 0) {                    /* repeat pixels count times */
	    b[1] = val & 15;
	    b[0] = (val >> 4) & 15;
	    for (j=0; j<count; j++) {
	       bmp->line(line,pos,b[j%2]);
	       pos++;
	    }
	 }
	 else {
	    switch (val) {

	       case 0:                       /* end of line */
		  eolflag=1;
		  break;

	       case 1:                       /* end of picture */
		  eopicflag=1;
		  break;

	       case 2:                       /* displace image */
		  count = f.get();
		  val = f.get();
		  pos += count;
		  line -= val;
		  break;

	       default:                      /* read in absolute mode */
		  for (j=0; j<val; j++) {
		     if ((j%4) == 0) {
			val0 = f.Get16();
			for (k=0; k<2; k++) {
			   b[2*k+1] = val0 & 15;
			   val0 = val0 >> 4;
			   b[2*k] = val0 & 15;
			   val0 = val0 >> 4;
			}
		     }
		     bmp->line(line,pos,b[j%4]);
		     pos++;
		  }
		  break;
	    }
	 }

	 if (pos > (int)infoheader->biWidth)
	    eolflag=1;
      }

      line--;
      if (line < 0)
	 eopicflag = 1;
   }
}



/* load_bmp:
 *  Loads a Windows BMP file, returning a bitmap structure and storing
 *  the pallete data in the specified pallete (this should be an array of
 *  at least 256 RGB structures).
 *
 *  Thanks to Seymour Shlien for contributing this function.
 */

void load_bmp(Frame & frm,char const *filename)
{
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	RGB		pal[256];
	BITMAP	bmp;

	Gifstream	f(Gifstream::LITTLE_ENDIAN);

	int ncol;
	unsigned long biSize;

	f.open(filename,ios::in|ios::binary);

	if (!f)
		GObject::Error(ERR_FATAL,"couldn't open file %s",filename);

	if (read_bmfileheader(f, &fileheader) != 0)
		{
		GObject::Error(ERR_FATAL,"error reading bmp hdr for %s",filename);
		}

   biSize = f.Get32();

	if (biSize == WININFOHEADERSIZE)
		{
		if (read_win_bminfoheader(f, &infoheader) != 0)
			GObject::Error(ERR_FATAL,"error reading windows bmp info hdr for %s",filename);

		/* compute number of colors recorded */
		ncol = (fileheader.bfOffBits - 54) / 4;
		read_bmicolors(ncol, pal, f, 1);
		}
	else if (biSize == OS2INFOHEADERSIZE)
		{
		if (read_os2_bminfoheader(f, &infoheader) != 0)
			GObject::Error(ERR_FATAL,"error reading os2 bmp info hdr for %s",filename);
		
		/* compute number of colors recorded */
		ncol = (fileheader.bfOffBits - 26) / 3;
		read_bmicolors(ncol, pal, f, 0);
		}
	else
		{
		GObject::Error(ERR_FATAL,"error finding correct hdr for bmp %s",filename);
		}

	if (infoheader.biBitCount != 4 && infoheader.biBitCount != 8)
		GObject::Error(ERR_FATAL,"only handles 4 && 8 bit bmps not %d : %s",infoheader.biBitCount,filename);


	bmp.setSize(infoheader.biWidth, infoheader.biHeight);

	bmp.clear();

	switch (infoheader.biCompression)
		{

		case BI_RGB:
			read_image(f, &bmp, &infoheader);
			break;

		case BI_RLE8:
			read_RLE8_compressed_image(f, &bmp, &infoheader);
			break;

		case BI_RLE4:
			read_RLE4_compressed_image(f, &bmp, &infoheader);
			break;

		default:
			GObject::Error(ERR_FATAL,"unknown compression foramt for %s",filename);
			break;
		}

	f.close();


		{
		Palette	palObj;



		for (int f=0;f<ncol;f++)
			{
			Colour & col = palObj[f];
			col.SetRGB(pal[f].r,pal[f].g,pal[f].b);
			}

		frm.SetFrame(bmp.getBitMap(),infoheader.biWidth,infoheader.biHeight,palObj);
		}

}

/*===========================================================================
 end */
