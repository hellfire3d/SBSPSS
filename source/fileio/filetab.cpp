/******************/
/*** File Table ***/
/******************/

/*******************************************************************************/
char	*FilenameList[FILEPOS_MAX]=
{
"BIGLUMP.BIN",
"BIGLUMP.BIN",
"THQ.STR",
"INTRO.STR",
//"CLIMAX.STR",
//"LEVEL1.STR",
//"LEVEL2.STR",
//"LEVEL3.STR",
//"LEVEL4.STR",
//"LEVEL5.STR",
//"LEVEL6.STR",
//"LEVEL7.STR",
//"ZORD1.STR",
//"ZORD2.STR",
/*
"DEMO2.STR",
"DEMO3.STR",
"DEMO4.STR",
"DEMO5.STR",
"DEMO6.STR"
*/
};

/*******************************************************************************/
void	CalcFilePos(int *Dst)
{
		for (int Loop=0;Loop<FILEPOS_MAX;Loop++)	
			{
			CdlFILE	RetFile;
			int		Pos;
			char	Name[32];
			sprintf(Name,"\\%s;1",FilenameList[Loop]);
			while ( !CdSearchFile(&RetFile,Name) );
			Dst[Loop]=CdPosToInt(&RetFile.pos);
			}
}


