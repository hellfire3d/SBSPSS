/*********************************/
/*** SpongeBob 3d Anim Creator ***/
/*********************************/

#include <Vector3.h>
#include <List.h>


//***************************************************************************
struct	sBoneAnim
{
		vector<sQuat>		Quat;
		vector<int>			Idx;
};

struct	sAnim
{
		int					FrameCount;
		vector<sBoneAnim>	BoneAnim;
		int					FileOfs;
};

//***************************************************************************
class	CMkAnim3d
{
public:
		CMkAnim3d()
		{
			BoneCount=-1;
		}

		void			Add(GString const &Filename);
		void			Write(GString &Filename);

private:
		int				ProcessSkel(CScene &Scene,sAnim &ThisAnim,int Idx);
		int				WriteAnim(sAnim const &ThisAnim);
		int				WriteQuatTable();

		int				BoneCount;
		vector<sAnim>	AnimList;
		CList<sQuat>	QuatList;
		FILE			*File;

};

//***************************************************************************
