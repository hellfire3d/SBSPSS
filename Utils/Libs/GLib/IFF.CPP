/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³                                               ³
	³   FILE: IFF.CPP                               ³
   ³                                               ³
   ³   Written  by Carl Muller 25 Sep 1990         ³
	³   Modified by Carl Muller 13 Sep 1991         ³
   ³                                               ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */



#include "iff.hpp"


/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Start writing an IFF file                   ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
FILE *IFF_FILE::write(char const *filename)
{
	stackptr = 0;
	file = fopen(filename, "wb");
	return file;
}



/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Close an IFF file                           ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
void IFF_FILE::close()
{
	fclose(file);
}



/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Start an IFF form                           ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
void IFF_FILE::form(char name[5])
{
	fputc('F', file);
	fputc('O', file);
	fputc('R', file);
	fputc('M', file);
	push(ftell(file));
	fputc(0, file);
	fputc(0, file);
	fputc(0, file);
	fputc(0, file);
	fputc(name[0], file);
	fputc(name[1], file);
	fputc(name[2], file);
	fputc(name[3], file);
}



/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Finish an IFF form                          ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
void IFF_FILE::endform()
{
	long pos1, pos2, len;

	pos2 = ftell(file);
	if (pos2 & 1)
		{
		fputc(0, file);
		pos2++;
		}
		
	pos1 = pop();
	len = pos2 - pos1 - 4L;

	fseek(file, pos1, SEEK_SET);
	fputc((UCHAR)(len>>24), file);
	fputc((UCHAR)(len>>16) & 255, file);
	fputc((UCHAR)(len>>8) & 255, file);
	fputc((UCHAR)(len&0xff), file);

	fseek(file,pos2,SEEK_SET);
}



/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Start an IFF chunk                          ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
void IFF_FILE::chunk(char name[5])
{
	fputc(name[0], file);
	fputc(name[1], file);
	fputc(name[2], file);
	fputc(name[3], file);
	push(ftell(file));
	fputc(0, file);
	fputc(0, file);
	fputc(0, file);
	fputc(0, file);
}



/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Finish an IFF chunk                         ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
void IFF_FILE::endchunk()
{
	long pos1, pos2, len;

	pos2 = ftell(file);
	if (pos2 & 1)
	{
		fputc(0, file);
		pos2++;
	}
	pos1 = pop();
	len = pos2 - pos1 - 4L;
	fseek(file, pos1, SEEK_SET);
	fputc((UCHAR)(len>>24), file);
	fputc((UCHAR)(len>>16) & 255, file);
	fputc((UCHAR)(len>>8) & 255, file);
	fputc((UCHAR)(len&255), file);
	fseek(file, pos2, SEEK_SET);
}

long IFF_FILE::tell()
{
	return ftell(file);
}
