/*********************************/
/*** SpongeBob 3d Anim Creator ***/
/*********************************/

#include <Vector3.h>
#include <List.h>


//***************************************************************************
struct	sBoneAnim
{
		vector<int>			Idx;
};

//***************************************************************************
struct	sAnim
{
		GString				Name;
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

/*Anim*/void			Add(GString const &Filename);
/*Anim*/void			Write(GString &Filename);
/*Anim*/void			WriteInclude(GString const &IncludeFile);

private:
/*Anim*/int				ProcessSkelMove(CScene &Scene,sAnim &ThisAnim,int Idx);
/*Anim*/void			ProcessSkelAnim(CScene &Scene,sAnim &ThisAnim,int Idx);
/*Anim*/int				WriteMove(sAnim const &ThisAnim);
/*Anim*/int				WriteAnim(sAnim const &ThisAnim);
/*Anim*/int				WriteQuatTable();

/*Anim*/int				BoneCount;
/*Anim*/vector<sAnim>	AnimList;
/*Anim*/CList<sQuat>	QuatList;
		FILE			*File;

};

//***************************************************************************
