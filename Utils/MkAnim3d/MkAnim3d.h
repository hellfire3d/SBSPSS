/*********************************/
/*** SpongeBob 3d Anim Creator ***/
/*********************************/

#include <Vector3.h>
#include <List.h>


//***************************************************************************
struct	sBoneAnim
{
//		vector<sQuat>		Quat;
		vector<int>			Idx;
};

struct	sAnim
{
		int					FrameCount;
		vector<sBoneAnim>	BoneAnim;
		vector<s32>			Move;
		int					AnimOfs;
		int					MoveOfs;
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
		int				ProcessSkelMove(CScene &Scene,sAnim &ThisAnim,int Idx);
		void			ProcessSkelAnim(CScene &Scene,sAnim &ThisAnim,int Idx);
		int				WriteMove(sAnim const &ThisAnim);
		int				WriteAnim(sAnim const &ThisAnim);
		int				WriteQuatTable();

		int				BoneCount;
		vector<sAnim>	AnimList;
		CList<sQuat>	QuatList;
		FILE			*File;

};

//***************************************************************************
