/*************************/
/*** Actor Sprite Tool ***/
/*************************/

#include <Vector3.h>
#include <List.h>

//***************************************************************************
struct	sFrame
{
		GString				Filename;
		int					FrameIdx;
};

//***************************************************************************
struct	sAnim
{
		GString				Name;
		vector<sFrame>		Frames;
		bool				VRamFlag;
};

//***************************************************************************
struct	sBmp
{
		Frame		Frm;
		int			OrigW,OrigH;
		int			ChkR,ChkG,ChkB;
		u8			*RGB;
		u8			*Psx;
		u8			*Pak;
		int			PsxSize;
		int			PakSize;
		bool		VRamFlag;
		int			CrossHairX,CrossHairY;
};

//***************************************************************************
class	CMkActor
{
public:
		CMkActor(GString &ActorName,GString &ActorPath,GString &SpritePath);

static	void				SetTPData(const char *Name,int TPBase,int TPW,int TPH);
		
		void				Load();
		void				Process();
		void				Write();
		void				CleanUp();

private:
		void				ReadScript(const char *Filename,vector<GString>	&List);
		void				BuildFrameList();
		void				FindFrames(sAnim &ThisAnim);
		int					FindDup(sBmp &Frm);

		void				LoadFrameList();
		void				LoadFrame(sFrame &ThisFrame,bool VRamFlag);
		int					LoadBmp(GString	&Name,bool VRamFlag);
		void				CheckAndShrinkFrame(sBmp &Bmp);
		bool				IsImageSame(sBmp &Bmp0,sBmp &Bmp1);
		void				MakePsxGfx(sBmp &Bmp);
		void				ProcessFrames();

		int					WritePalette();
		int					WriteAnimList();
		int					WriteFrameList();

		void				WriteInclude();
		GString				Name,ActorDir,SpriteDir;
		GString				OutFile,IncFile;

		FILE				*File;
		sSpriteAnimBank		FileHdr;

		vector<GString>		InAnimList;
		vector<sAnim>		AnimList;
		vector<sBmp>		BmpList;
		CTexGrab			TexGrab;
		int					DupCount;
		int					MaxW,MaxH;
		int					TotalIn,TotalOut;
};

//***************************************************************************
