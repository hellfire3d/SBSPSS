/*
 * File:strings.h
 * string functions pseudo definition header 
 */
/*
 * $PSLibId: Run-time Library Release 4.7$
 */

#ifndef	_STRINGS_H
#define	_STRINGS_H

#define LMAX 256

#ifndef NULL
#define NULL 0		       /* null pointer constant */
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;  /* result type of the sizeof operator (ANSI) */
#endif

#include <memory.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif
extern char *strcat (char *, const char *);
extern char *strncat(char *, const char *, int);
extern int   strcmp (/* char *, char * */);	/* To avoid conflicting */
extern int   strncmp(const char *,const char *, int);
extern char *strcpy (/* char *, char * */);	/* To avoid conflicting */
extern char *strncpy(char *, const char *, int);
extern int   strlen (/* char * */);		/* To avoid conflicting */
extern char *index  (const char *, char);
extern char *rindex (const char *, char);

extern char *strchr (const char *, char);
extern char *strrchr(const char *, char);
extern char *strpbrk(const char *, const char *);
extern int   strspn (const char *, const char *);
extern int   strcspn(const char *, const char *);
extern char *strtok (char *, const char *);
extern char *strstr (const char *, const char *);
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#define strdup(p)	( strcpy(malloc(strlen(p)+1),p); )

#endif	/* _STRINGS_H */

