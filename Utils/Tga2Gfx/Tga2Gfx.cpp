/***************************/
/*** TGA 2 Gfx Convertor ***/
/***************************/

//#include <windows.h>
#include <stdlib.h>
#include <stdarg.h>
//#include <string.h>
#include <stdio.h>
//#include <time.h>
//#include <math.h>
#include <io.h>
#include <fstream.h>
#include <conio.h>



//***************************************************************************
struct	sTgaHdr
{
	char	id;			   // 0
	char	colmaptype;	   // 1
	char	imagetype;	   // 2
	char	fei[2];		   // 3
	char	cml[2];		   // 5
	char	cmes;		   // 7
	short	xorig;		   // 8
	short	yorig;		   // 10
	short	width;		   // 12
	short	height;		   // 14
	char	depth;		   // 16
	char	imagedesc;	   // 17
};


//***************************************************************************
sTgaHdr	*TGA;
short	*Buffer;
char	*InFile;
char	*OutFile;

//***************************************************************************
void	Load()
{
FILE			*File;
int				Size;

		File=fopen(InFile,"rb");
		fseek(File,0,SEEK_END);
		Size=ftell(File);
		fseek(File,0,SEEK_SET);

		if( (TGA=(sTgaHdr*)malloc(Size))==NULL ) {printf("Out of memory.\n");exit(123);}
   
		fread(TGA, 1,Size,File);
		fclose(File);

		if( (Buffer=(short*)malloc(TGA->width*TGA->height*sizeof(short)))==NULL ) {printf("Out of memory.\n");exit(123);}
}

//***************************************************************************
void	Process()
{
unsigned char	*InSrc=(unsigned char*)TGA+sizeof(sTgaHdr);
short	*Dst=Buffer;
/*
		printf("width %i\n",TGA->width);
		printf("height %i\n",TGA->height);
		printf("depth %i\n",TGA->depth);
		printf("imagedesc %i\n",TGA->imagedesc);
*/
		for (int Y=0; Y<TGA->height; Y++)
		{
			unsigned char	*Src=&InSrc[(TGA->height-Y-1)*(TGA->width*3)];	// Flip Image
			for (int X=0; X<TGA->width; X++)
			{
				int		B=*Src++;
				int		G=*Src++;
				int		R=*Src++;

				if (TGA->depth==24)
				{
					R>>=(8-5);
					G>>=(8-5);
					B>>=(8-5);
				}
				*Dst=0;
				*Dst|=R<<0;
				*Dst|=G<<5;
				*Dst|=B<<10;
				Dst++;
			}
		}
}

//***************************************************************************
void	Write()
{
FILE			*File;

		File=fopen(OutFile,"wb");

		fwrite(Buffer,TGA->width*TGA->height*sizeof(short),1,File);  
		fclose(File);

		free(TGA);
		free(Buffer);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************

void Usage(char *ErrStr)
{
	printf("\nTga2Gfx: by Dave\n");
	printf("Usage: Tga2Gfx <file> [ <file>.. ] [ switches.. ]\n");
	printf("Switches:\n");
	printf("   -o:[FILE]       Set output File\n");
	printf("\nERROR: %s\n",ErrStr);
	exit(-1);
}

//***************************************************************************
int		main(int argc, char *argv[])
{
		if (argc!=3) Usage("Incorrect Params");

		InFile=argv[1];
		OutFile=argv[2];


		Load();
		Process();
		Write();
		return(0);
}
