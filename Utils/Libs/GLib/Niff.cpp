/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³                                                 ³
   ³   IA - Deluxe Paint 3 Animation player          ³
   ³                                                 ³
   ³   NIFF.CPP                                      ³
   ³                                                 ³
   ³   Written  by Gary Liddon 27 May 1991           ³
   ³   Modified by Carl Muller 30 Oct 1991           ³
   ³                                                 ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */


#include "stdio.h"
#include "conio.h"

#include "niff.hpp"




/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   IFF error messages                            ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */

char *niff_error_text[] =
{
	NULL,
	"Error opening IFF file",
	"Not an IFF file",
	"Can't find FORM",
	"Can't find HUNK",
	"Corrupted bit map header in picture file",
	"Can't find BODY",
	"All out of memory",
	"Mangled IFF file",
	"Can't find CMAP",
	"No ILBM's yet",
	"Error with passed IFF file",
	"Error with ILBM in ANIM",
	"Not aksed for FORM type",

};




/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Create an IFF descriptor from a filename      ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
niff::niff(char const *name, U32 form)
{
	form_mounted = FALSE;
	err_no = NO_ERROR;

	file_opened = FALSE;
	fp = NULL;
	passed_file = FALSE;

	if (name)
		open(name,form);

}


/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Create an IFF descriptor from a file pointer  ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
niff::niff(FILE * new_file, U32 form)
{
	form_mounted = FALSE;
	err_no = NO_ERROR;
	file_opened = TRUE;							//	File pre-opened

	fp = new_file;
	iff_base = ftell(fp);						//	Get base pos
	passed_file = TRUE;							//	This is a passed file

	if (!mount_form(form))
		err_no = PASSED_ERR;
}


/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Open an IFF file for reading                  ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
BOOL niff::open(char const *name, U32 form)
{
	if (passed_file)								// Passed files can't be re-opened
	{
		err_no = OPEN_ERROR;
		return FALSE;
	}

	if (file_opened)								// Already open so don't bother
	{
		err_no = OPEN_ERROR;
		return FALSE;
	}

	if (!(fp = fopen(name,"rb")))
		{
		err_no = OPEN_ERROR;

		return FALSE;
		}
	
	if (!mount_form(form))						// Mount this form
		{
		fclose(fp);
		if (!err_no)
			err_no = NOT_FORM;
		return FALSE;
		}

	iff_base = 0L;									//	Set position to zero
	file_opened = TRUE;
	return TRUE;
}


/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Read an IFF hunk into memory                  ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
U8 *niff::get_hunk()
{
	if ((!file_opened) && (!form_mounted))
		return NULL;

	U8 *hunk=NULL;
	long name,len,temp;

	temp = ftell(fp);

	fread(&name, 1, sizeof(U32), fp);
	fread(&len, 1, sizeof(U32), fp);

	len = rev_long(len);


	hunk = new U8[len];

	splen = len;

	if (hunk)
		fread(hunk,1,len,fp);

	fseek(fp,temp,SEEK_SET);

	return hunk;
}




/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Reset to beginning of form                    ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
void niff::goto_form_start()
{
	fseek(fp, form_start, SEEK_SET);
}


/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Mount form at filepos                         ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
BOOL niff::mount_form(U32 form)
{
	form_mounted=FALSE;
	U32 temp,temp_name;						//	Check to see if we've an iff
	
	if (fread(&temp,sizeof(U32),1,fp) != 1)
		{
		err_no=	MANGLED_IFF;
		return(false);
		}

	if (temp != FORM)								// We haven't.
		return FALSE;

	fread(&temp,1,sizeof(U32),fp);
	form_end = full_len(temp)+ftell(fp);

	fread (&temp_name,1,sizeof(U32),fp);

	if (form)										//	Are we checking for a specific
	{													// form?
		if (temp_name != form)
			return FALSE;
	}

	form_name=temp_name;
	form_start=ftell(fp);
	form_mounted=TRUE;
	return TRUE;
}


/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Goto a hunk                                   ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
BOOL niff::goto_hunk(U32 form)
{
	U32 len, namer;

	if ((!form_mounted) || (!file_opened))
		return FALSE;

	goto_form_start();
	fread(&namer, 1, sizeof(U32), fp);
	fread(&len, 1, sizeof(U32), fp);

	goto_form_start();

	if (namer == form)
		return TRUE;
	
	return next_hunk(form);
}


//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Get the number of hunks of a certain name in this file
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int niff::GetNumOfHunks(U32 FormName)
{
	int	NumOfHunks=0;

	if (goto_hunk(FormName))
		{
		do
			{
			NumOfHunks++;
			}
		while (next_hunk(FormName));
		}

	return NumOfHunks;
}

/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Goto next hunk in current form                ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
BOOL niff::next_hunk(U32 form)
{
	if ((!form_mounted) || (!file_opened))
		return FALSE;

	U32 temp = ftell(fp);
	long len;
	U32 name = 0L;
	BOOL found = FALSE;

	fread(&name, 1, sizeof(U32), fp);
	fread(&len, 1, sizeof(U32), fp);
	
	while ((ftell(fp) < form_end) && (!found))
	{
		fseek(fp, full_len(len), SEEK_CUR);
		fread(&name, 1, sizeof(U32), fp);
		fread(&len, 1, sizeof(U32), fp);

		if (form)
		{
			if (form == name)
				found = TRUE;
		}
		else
		{
			if (form != FORM)
				found = TRUE;
		}
	}

	if (found && (ftell(fp) < form_end))
	{
		fseek(fp, -8L, SEEK_CUR);
		return TRUE;
	}

	fseek(fp, temp, SEEK_SET);
	return FALSE;
}



/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Goto Next form in current form                ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
BOOL niff::next_form(U32 form)
{
	if ((!form_mounted) || (!file_opened))
		return FALSE;

	U32 len;
	U32 name = 0L;
	U32 fname = 0L;
	BOOL found=FALSE;

	fread(&name, 1, sizeof(U32), fp);
	fread(&len, 1, sizeof(U32), fp);
	
	do	{
		fseek(fp, full_len(len), SEEK_CUR);

		fread(&name, 1, sizeof(U32), fp);
		fread(&len, 1, sizeof(U32), fp);

		if (name == FORM)
		{
			fread(&fname, 1, sizeof(U32), fp);
			if (!form)
				found = TRUE;
			else if (form == fname)
				found = TRUE;
			len -= 4L;
		}
	} while (!found && (ftell(fp) < form_end));

	if (found)												// If we got what we wanted
	{															// then seek back to start of
		fseek(fp, -12L, SEEK_CUR);						// header and flag success
		return TRUE;
	}

	goto_form_start();									// Else back to start of form
	return FALSE;
}



/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Close an IFF file                             ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
BOOL niff::close()
{
	if (passed_file || (!file_opened))		// Passed or unopened files
		return FALSE;								// can't be closed
	else if (fclose(fp) == 0)					// If close succesful
	{
		file_opened=FALSE;						// Flag file as closed
		return TRUE;								// Return Success
	}
	else
		return FALSE;								// Else an error
}


//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Do a Iff file seek
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int niff::seek(long offset,int orig)
{
	return fseek(fp,offset,orig);
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Do a Iff file seek
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
long niff::tell()
{
	return ftell(fp);
}
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Read Data from current pos
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void niff::Read(U8 *Buf,long bytes)
{
	fread(Buf,bytes,1,fp);
}


//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Return an Intel Word from current file pos
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
U16 niff::GetIntelWord()
{
	U16 Word=0;

	if (file_opened)
		{
		fread((U8 *) &Word,sizeof(U16),1,fp);
		return rev_word(Word);
		}
	else
		return 0;

}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Return an Intel U32 from current file pos
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
U32 niff::GetIntelLong()
{
	U32 Long=0;

	if (file_opened)
		{
		fread((U8 *) &Long,sizeof(U32),1,fp);
		return rev_long(Long);
		}
	else
		return 0;
}


//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//	Return the error string
//	ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
char *niff::error()
{
	return niff_error_text[err_no];
}



/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Destroy an IFF descriptor                     ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
niff::~niff()
{
	close();
}



/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Return full chunk length                      ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
U32 niff::full_len(U32 val)
{
	return (rev_long(val)+1)&0xfffffffeL;
}



/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Reverse a long: Motorola <-> Intel            ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
U32 niff::rev_long(U32 val)
{
	return ((val >> 24) & 0x000000ff) | ((val >> 8) & 0x0000ff00) |
			 ((val << 24) & 0xff000000) | ((val << 8) & 0x00ff0000);
}

/* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³   Reverse a long: Motorola <-> Intel            ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
U16 niff::rev_word(U16 val)
{
	return ((val >> 8) & 0x000000ff) | ((val << 8) & 0x0000ff00);
}
