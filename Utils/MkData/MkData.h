/************************/
/*** DataFile Creator ***/
/************************/

//***********************************************************************
//***********************************************************************
//***********************************************************************
#define	CHAR_COMMENT			'#'
#define	CHAR_TAB				0x09
#define	CHAR_EOL				0x0d
#define	CHAR_SKIP				'-'
#define	CHAR_STARTBANK			'{'
#define	CHAR_ENDBANK			'}'

//***********************************************************************
//***********************************************************************
//***********************************************************************
typedef	signed char		s8;
typedef	signed short	s16;
typedef	signed long		s32;

//***********************************************************************
//***********************************************************************
//***********************************************************************
struct	sFAT
{
		s32				FilePos;
		s32				FileSize;
};

struct	sBankEntry
{
		int				ListIdx;
		int				Pos;
};

struct	sBank
{
	char					Name[256];
	int						Align;
	std::vector<sBankEntry>	FileList;
};

struct	sFile
{
	char				Filename[256];
	char				ShortName[256];
	int					Size;
	char				*Data;
	int					BankIdx;
};

//***********************************************************************
class	CFileList
{
public:
	CFileList(){};
	~CFileList();
	int			FindFileIdx(char *Filename);
	int			AddFile(char *Filename,int BankIdx=0);
	int			GetCount()			{return(FileList.size());}
	sFile		&GetFile(int Idx)	{return(FileList[Idx]);}

private:

	std::vector<sFile>		FileList;
};

//***********************************************************************
class	CBank
{
public:
	void		Start(char *Name,int Align=0);
	void		End();
	int			FindEntry(int Idx);
	void		Add(char *Filename);
	int			GetCount()			{return(BankList.size());}
	sBank		&GetBank(int Idx)	{return(BankList[Idx]);}
	int			GetCurrentBankNo()	{return(CurrentBank);}

private:
	int						CurrentBank;
	std::vector<sBank>		BankList;
};

//***********************************************************************
