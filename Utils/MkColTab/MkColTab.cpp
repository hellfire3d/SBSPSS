/*********************************/
/*** Make Collision Table Util ***/
/*********************************/

#include	"stdio.h"
#include	<misc.hpp>
#include	<vector>
#include	<DaveLib.h>

#include	"MkColTab.h"

//***************************************************************************
char * CycleCommands(char *String,int Num)
{
		if (String[0]=='-' || String[0]=='/')
		{
// Check Format
			switch (String[1])
			{// Switches 
			case 'o':
				OutStr = CheckFileString(String);
				break;
			case 'd':
				DebugOn=true;
				break;
			default:
				GObject::Error(ERR_FATAL,"Unknown switch %s",String);
				break;
			}
		} 
		else
		{
			GString	UpperName(String);
			UpperName.Upper();
			MyFiles.AddFile(UpperName);
		}

		return(String);
}

//***************************************************************************

void Usage(char *ErrStr)
{
	printf("\nMkColTab: by Dave\n");
	printf("Usage: nMkColTab <Col.Bmp> [ <file>.. ] [ switches.. ]\n");
	printf("Switches:\n");
	printf("   -o:[FILE]       Set output File\n");
	printf("   -d              Set Debug On\n");
	GObject::Error(ERR_FATAL,ErrStr);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
void	CMkColTab::Load(const char*Name)
{
		InFrame.LoadBMP(Name);
}

//***************************************************************************
void	CMkColTab::Process()
{
int			BlkW=InFrame.GetWidth()/BLK_SIZE;
int			BlkH=InFrame.GetHeight()/BLK_SIZE;
Rect		R;
sColInfo	ThisTile;

		R.W=BLK_SIZE;
		R.H=BLK_SIZE;

		ThisTile.Ofs.resize(BLK_SIZE);
// Add Blank
		Tiles.push_back(ThisTile);

		for (int BlkY=0; BlkY<BlkH; BlkY++)
		{
			for (int BlkX=0; BlkX<BlkW; BlkX++)
			{
				R.X=BlkX*BLK_SIZE;
				R.Y=BlkY*BLK_SIZE;
				
				for (int F=0; F<4; F++)
				{
					Frame		ThisFrame;
// Cut and Flip Tile
					ThisFrame.Grab(InFrame,R);

					if (F&1) ThisFrame.FlipX();
					if (!(F&2)) ThisFrame.FlipY();	// Loaded upside down!

					if (DebugOn) printf("%i %i %i\n",BlkX,BlkY,F);
					BuildTable(ThisFrame,ThisTile);
					Tiles.push_back(ThisTile);
				}
			}
		}


}

//***************************************************************************
void	CMkColTab::BuildTable(Frame &ThisFrame,sColInfo &ThisTile)
{
int		X,Y;
u8		c;

		for (X=0; X<BLK_SIZE; X++)
		{
			c=-1;
			ThisTile.Ofs[X]=-1;
			for (Y=0; Y<BLK_SIZE && c; Y++)
			{
				c=ThisFrame.GetPixel(X,Y);
				if (c) ThisTile.Ofs[X]++;
				if (DebugOn) printf("%i",c);
			}
			if (DebugOn) printf("\t%i \n",ThisTile.Ofs[X]);
		}
		if (DebugOn) printf("\n");

}

//***************************************************************************
void	CMkColTab::Write(const char *Filename)
{
int		ListSize=Tiles.size();

		if (DebugOn) printf("%i Tiles\n",ListSize);
		File=fopen(Filename,"wb");

		for (int i=0; i<ListSize; i++)
		{
			sColInfo	&ThisTile=Tiles[i];
			for (int X=0; X<BLK_SIZE; X++)
			{
				u8	c=ThisTile.Ofs[X];
				fwrite(&c,1,1,File);
			}
		}
		fclose(File);

}

//***************************************************************************
int		main(int argc, char *argv[])
{
CMkColTab	ColTab;
		CommandLine(argc,argv,CycleCommands);

std::vector<GString> const &Files = MyFiles.GetFileInfoVector();

		if (!Files.size()) Usage("No Levels specified\n");
		if (Files.size()>1) Usage("Too many inputs\n");
		if (OutStr.Empty()) Usage("No Output specified\n");


		ColTab.Load(Files[0]);
		ColTab.Process();
		ColTab.Write(OutStr);

		printf("Made %s\n",OutStr);
		return 0;
}



