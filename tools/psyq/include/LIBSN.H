/*
 * $PSLibId: Run-time Library Release 4.7$
 */
#ifndef _LIBSN_H_
#define _LIBSN_H_

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif


/*
** LIBSN.H	declare libary functions provided by LIBSN.LIB
**
** 05/02/94 ADB
** 21/03/94 ADB added user notes as comments
** 18/09/94 ADB	added PCcreat() - it was missing before
** 31/05/95 ADB	added PSYQpause() for new debug stub 4.04
** 20/09/95 ADB added SNFlushCache - but removed after email from Tom Boyd
** 27/03/97 GJ  v2.00
                C++ linkage
                Call to global destructors in snmain.s        
** 12.2.98  GJ  v2.01
                C++ structor functions added in ctors.c
                snmain.s shuffled - __do_global_dtors removed
                Compiler support functions moved to libsngcc.lib
** 18.2.98	GJ  v2.02
                C++ structor functions trimmed down because of assumptions about group names
** 24.3.98      GJ v2.03
                Initialise GP from __SN_GP_BASE linker symbol
*/

#define	pollhost()	__asm__ ("break 1024")	/* inline to keep variable scope */
#define	PSYQpause()	__asm__  ("break 1031")	/* inline to keep variable scope */

/*
** C++ static class object functions
** These are here for when you can't use __SN_ENTRY_POINT (eg a final build)
** or when you need to instantiate static class objects from an overlay.
** Static class objects are those declared with non-function scope which
** have constructors. How do these get called? In the startup code, normally!
** But in final builds you can't link with our startup code, and the Sony
** startup objects don't have hooks for C++ initialisation. So:
** Call __sn_cpp_structors to initialise your standard static class
** objects, or to destroy them. Memory has already
** been allocated for these objects in the image; these functions just call
** the constructors to populate that memory appropriately.
** Call __sn_cpp_structors(section_obj, section_objend),
** supplying the obj and objend variables for a ctors section, to initialise
** the class objects referenced in that section. Or supply the obj and
** objend variables for a dtors section to call the corresponding
** destructors.
*/

extern void __sn_cpp_structors (long, long);


/*
** FILESERVER FUNCTIONS:
**
** NOTE: For PCread and PCwrite do not load files by passing extreme
** values for count as you might on UNIX as this will cause the full
** amount specified to be transferred - the file will be padded to
** that length with zeroes which may over-write memory beyond the
** end of the file.
**
** If you are unsure of the length of a file which you are about
** to read into memory then perform a
** 	len = PClseek( fd, 0, 2);
** This will set len to the length of the file which you can then
** pass to a PCread() function call.
**
*/

/*
** re-initialise PC filing system, close open files etc
**
** passed: void
**
** return: error code (0 if no error)
*/
int	PCinit (void);

/*
** open a file on PC host
**
** passed:	PC file pathname, open mode, permission flags
**
** return:	file-handle or -1 if error
**
** note: perms should be zero (it is ignored)
**
** open mode:	0 => read only
** 				1 => write only
**					2 => read/write
*/
int	PCopen (char *name, int flags, int perms);

/*
** create (and open) a file on PC host
**
** passed:	PC file pathname, open mode, permission flags
**
** return:	file-handle or -1 if error
**
** note: perms should be zero (it is ignored)
*/
int	PCcreat (char *name, int perms);

/*
** seek file pointer to new position in file
**
** passed: file-handle, seek offset, seek mode
**
** return: absolute value of new file pointer position
**
** (mode 0 = rel to start, mode 1 = rel to current fp, mode 2 = rel to end)
*/
int	PClseek (int fd, int offset, int mode);

/*
** read bytes from file on PC
**
** passed: file-handle, buffer address, count
**
** return: count of number of bytes actually read
**
** note: unlike assembler function this provides for full 32 bit count
*/
int	PCread (int fd, char *buff, int len);

/*
** write bytes to file on PC
**
** passed: file-handle, buffer address, count
**
** return: count of number of bytes actually written
**
** note: unlike assembler function this provides for full 32 bit count
*/
int	PCwrite (int fd, char *buff, int len);

/*
** close an open file on PC
**
** passed: file-handle
**
** return: negative if error
**
*/
int	PCclose (int fd);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* _LIBSN_H_ */
