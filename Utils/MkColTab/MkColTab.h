/*********************************/
/*** Make Collision Table Util ***/
/*********************************/

#ifndef	__MKCOLTAB_HEADER__
#define	__MKCOLTAB_HEADER__

using namespace std;

struct	sColInfo
{
		vector<s8>		Ofs;
};

//***************************************************************************
class	CMkColTab
{
public:
		enum
		{
			BLK_SIZE=16,
		};
		void	Load(const char *Filename);
		void	Process();
		void	Write(const char *Filename);

protected:
		void	BuildTable(Frame &ThisFrame,sColInfo &ThisTile);

		Frame				InFrame;
		vector<sColInfo>	Tiles;
		FILE				*File;
};

#endif
