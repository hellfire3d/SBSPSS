
#ifndef __SYSTEM_LNKOPT_H__
#define __SYSTEM_LNKOPT_H__

enum FILE_SYSTEM
{
	FS_PC		=0,
	FS_CD
};


enum DEV_KIT
{
	DK_SONY_ISA		=0,
	DK_SONY_PCI,
	DK_CLIMAX,
	DK_TPWART,
};


struct LNK_OPTS
{
	u32				RamSize;
	u32				StackSize;
	void *			OrgAddress;
	void *			FreeMemAddress;
	u32				FreeMemSize;
	FILE_SYSTEM		FileSystem;
	DEV_KIT			DevKit;
	u32				extraCtorsSize;
	void *			extraCtorsAddress;
};



extern LNK_OPTS OPT_LinkerOpts;
#endif /* __SYSTEM_LNKOPT_H__ */
