/***********************/
/*** Daves Ini Class ***/
/***********************/

#pragma warning( disable : 4786 )

#ifndef	__DAVES_INI_CLASS__
#define	__DAVES_INI_CLASS__
#include <vector>

/******************************************************************************/
#define	CIni_Max_Data_Size	128

/******************************************************************************/
struct	sIniEntry
{
		char	Key[CIni_Max_Data_Size+1];
		char	Data[CIni_Max_Data_Size+1];
};

struct	sIniGroup
{
		char	Name[CIni_Max_Data_Size+1+2];
		std::vector<sIniEntry>	Entry;
};

/******************************************************************************/
class	CIni
{
public:
			CIni (char *IniFile)	{Import(IniFile);}
			CIni ()	{}

			int		GetGroupCount()				{return(IniGroup.size());}
			char	*GetGroupName(int i)		{return(IniGroup[i].Name);}

			int		GetKeyCount(const char *Grp)
			{
				int	Group=FindGroup(Grp);
				return(IniGroup[Group].Entry.size());
			}
			char	*GetKeyName(const char *Grp,int Idx)
			{
				int	Group=FindGroup(Grp);
				return(IniGroup[Group].Entry[Idx].Key);
			}

/*----------------------------------------------------------------------------*/
// returns 1 of fail, 0 on success!! (Who thought that up!)
int	MyStrCmp(const char *Str0,const char *Str1)
{
	if (strlen(Str0)!=strlen(Str1)) return(1);
	for (int Loop=0;Loop<strlen(Str0);Loop++) 
		{
		char C0=Str0[Loop];
		char C1=Str1[Loop];
		if (C0>='a' && C0<='z') C0+='A'-'a';
		if (C1>='a' && C1<='z') C1+='A'-'a';
		if (C0!=C1) return(1);
		}
	return(0);
}

/*----------------------------------------------------------------------------*/
void	SkipLine(char **InPtr)
{
char	*Ptr=*InPtr;

	while (*Ptr!='\n' && *Ptr) Ptr++;
	*InPtr=Ptr;
}
/*----------------------------------------------------------------------------*/
void	LoadAndImport(const char *Filename)
{
FILE	*File;
int		Size;
char	*Script;
		File=fopen(Filename,"rt");
		if (!File) {printf("%s Not Found\n",Filename); return;}
		fseek(File,0,SEEK_END);
		Size=ftell(File);
		fseek(File,0,SEEK_SET);
		Script=(char*)malloc(Size+1);
		memset(Script,0,Size+1);
		// Load It
		fread(Script,Size,1,File);
		fclose(File);
		Import(Script);
		free(Script);
}

/*----------------------------------------------------------------------------*/
void	Import(char *IniFile)
{
char	GroupEntry[CIni_Max_Data_Size+1+2];
char	KeyEntry[CIni_Max_Data_Size+1+2];
	
	GroupEntry[0]=0;	
	ParseFindEntry(&IniFile);
	while (*IniFile)
		{
		if (IsComment(IniFile))
		{
			SkipLine(&IniFile);
		}
		else
		{
			if (ParseGetEntry(&IniFile,KeyEntry))
				{	// Group
				strcpy(GroupEntry,KeyEntry);	// Update Group buffer
				StripGroupName(GroupEntry);
				UpdateGroup(GroupEntry);
				}
			else
				{	// Data
				UpdateKey(GroupEntry,KeyEntry);
				}
			}
		ParseFindEntry(&IniFile);
		}
}

/*----------------------------------------------------------------------------*/
void	Export(char *OutFile)
{
	*OutFile=0;	// Clear Output
	for (int Group=0;Group<IniGroup.size();Group++)
		{
		if (Group) sprintf(OutFile,"%s\r\n",OutFile);
		if (strlen(IniGroup[Group].Name)) 
			sprintf(OutFile,"%s[%s]\r\n",OutFile,IniGroup[Group].Name);
		for (int Entry=0;Entry<IniGroup[Group].Entry.size();Entry++)
			{
			if (strlen(IniGroup[Group].Entry[Entry].Data)) 
				sprintf(OutFile,"%s%s=%s\r\n",OutFile,IniGroup[Group].Entry[Entry].Key,IniGroup[Group].Entry[Entry].Data);
			}
		}
}

/*----------------------------------------------------------------------------*/
char	*GetData(const char *GroupEntry,const char *KeyEntry)
{
int	Group=FindGroup(GroupEntry);
		if (Group==-1) return(0);
int	Entry=FindKey(GroupEntry,KeyEntry);
		if (Entry==-1) return(0);

		return(IniGroup[Group].Entry[Entry].Data);
}

/*----------------------------------------------------------------------------*/
char	*GetStr(const char *GroupEntry,const char *KeyEntry)
{
char	*Str=GetData(GroupEntry,KeyEntry);
		return(Str);
}

/*----------------------------------------------------------------------------*/
int		GetInt(const char *GroupEntry,const char *KeyEntry)
{
char	*Str=GetData(GroupEntry,KeyEntry);
int		Var=0;
		if (Str) Var=atol(Str);
		return(Var);
}

/*----------------------------------------------------------------------------*/
bool	GetInt(const char *GroupEntry,const char *KeyEntry,int &Var)
{
char	*Str=GetData(GroupEntry,KeyEntry);
		if (!Str) return(false);
		Var=atol(Str);
		return(true);
}

/*----------------------------------------------------------------------------*/
int		UpdateGroup(char *GroupEntry)
{
int		Group=FindGroup(GroupEntry);
		if (Group==-1)
			{	// Doesnt exist
			Group=IniGroup.size();
			IniGroup.resize(Group+1);
			}
		strcpy(IniGroup[Group].Name,GroupEntry);
		return(Group);
}

/*----------------------------------------------------------------------------*/
int		UpdateKey(char *GroupEntry,char *KeyEntry)
{
// Split Key&Data
char	*Ptr=KeyEntry;
		while (*Ptr!='=' && *Ptr!=0) Ptr++;
		if (!*Ptr) return(-1);	//BAD DATA
		*Ptr=0;

// If cosher, add it
int		Group=UpdateGroup(GroupEntry);
int		Entry=FindKey(GroupEntry,KeyEntry);

		if (Entry==-1)
			{	// Doesnt exist
			Entry=IniGroup[Group].Entry.size();
			IniGroup[Group].Entry.resize(Entry+1);
			}
		strcpy(IniGroup[Group].Entry[Entry].Key,KeyEntry);
		KeyEntry+=strlen(KeyEntry)+1;
		while (IsWhiteSpace(*KeyEntry)) KeyEntry++;
		strcpy(IniGroup[Group].Entry[Entry].Data,KeyEntry);
		return(Entry);
}

/*----------------------------------------------------------------------------*/
int		UpdateKey(char *GroupEntry,char *KeyEntry,char *Data)
{
int		Group=UpdateGroup(GroupEntry);
int		Entry=FindKey(GroupEntry,KeyEntry);

		if (Entry==-1)
			{	// Doesnt exist
			Entry=IniGroup[Group].Entry.size();
			IniGroup[Group].Entry.resize(Entry+1);
			}
		strcpy(IniGroup[Group].Entry[Entry].Key,KeyEntry);
		strcpy(IniGroup[Group].Entry[Entry].Data,Data);
		return(Entry);
}

/*----------------------------------------------------------------------------*/
int		UpdateKey(char *GroupEntry,char *KeyEntry,int Data)
{
char	Str[32];
		sprintf(Str,"%i",Data);
		return(UpdateKey(GroupEntry,KeyEntry,Str));
}

/*----------------------------------------------------------------------------*/
int		UpdateKey(char *GroupEntry,char *KeyEntry,float Data)
{
char	Str[32];
		sprintf(Str,"%f",Data);
		return(UpdateKey(GroupEntry,KeyEntry,Str));
}

//private:
/*----------------------------------------------------------------------------*/
bool	IsWhiteSpace(char c)	{return(c==' ' || c=='\n' || c==0xa || c=='\r');}

/*----------------------------------------------------------------------------*/
bool	IsGroup(char *Entry)	{return(*Entry=='[');}
bool	IsComment(char *Entry)	{return(*Entry==';' || *Entry=='#');}

/*----------------------------------------------------------------------------*/
void	StripGroupName(char *Name)
{
// Remove Last Bracket
	if (Name[strlen(Name)-1]==']') Name[strlen(Name)-1]=0;
// Remove First bracket
	if (*Name=='[') while (*Name) *Name++=*(Name+1);
}

/*----------------------------------------------------------------------------*/
int		FindGroup(const char *GroupEntry)
{
		for (int Loop=0;Loop<IniGroup.size();Loop++)
			{
			if (!MyStrCmp(GroupEntry,IniGroup[Loop].Name)) return(Loop);
			}
		return(-1);
}

/*----------------------------------------------------------------------------*/
int		FindKey(const char *GroupEntry,const char *KeyEntry)
{
int		Group=FindGroup(GroupEntry);;
		if (Group==-1) return(-1);

		for (int Loop=0;Loop<IniGroup[Group].Entry.size();Loop++)
			{
			if (!MyStrCmp(KeyEntry,IniGroup[Group].Entry[Loop].Key)) return(Loop);
			}
		return(-1);
}

/*----------------------------------------------------------------------------*/
void	ParseFindEntry(char **InPtr)
{
char	*Ptr=*InPtr;

		while(IsWhiteSpace(*Ptr)) Ptr++;
		*InPtr=Ptr;
}

/*----------------------------------------------------------------------------*/
// returns true if group
bool	ParseGetEntry(char **InPtr,char *OutPtr)
{
char	*Ptr=*InPtr;
bool	GroupFlag;
/*
int		EqFlag=1;
int		ValFlag=1;

		while((EqFlag || ValFlag ) && *Ptr) 
			{
			if (!IsWhiteSpace(*Ptr))
				{
				if (!EqFlag) ValFlag=0;
				if (*Ptr=='=') EqFlag=0;
				*OutPtr++=*Ptr;
				}
			else
				{
				}
			Ptr++;
			}
*/
		while (IsWhiteSpace(*Ptr) && *Ptr) Ptr++;	// leading blanks
		GroupFlag=IsGroup(Ptr);
		if (GroupFlag)
		{ // Group
			while (!IsWhiteSpace(*Ptr) && *Ptr!='=' && *Ptr) *OutPtr++=*Ptr++;
		}
		else
		{
			while (!IsWhiteSpace(*Ptr) && *Ptr!='=' && *Ptr) *OutPtr++=*Ptr++;
			while (IsWhiteSpace(*Ptr) && *Ptr) Ptr++;	// Trailing blanks
			*OutPtr++=*Ptr++;					// Copy =
			while (IsWhiteSpace(*Ptr) && *Ptr) Ptr++;	// Trailing blanks
			while (!IsWhiteSpace(*Ptr) && *Ptr) *OutPtr++=*Ptr++;

		}
		*OutPtr=0;
		*InPtr=Ptr;
		return(GroupFlag);
//		if (*(InPtr-1)==0) InPtr--;

}

/*----------------------------------------------------------------------------*/

	std::vector<sIniGroup> IniGroup;
};
#endif