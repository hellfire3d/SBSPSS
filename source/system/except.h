#ifndef __SYSTEM_EXCEPT_H__
#define __SYSTEM_EXCEPT_H__

extern void installExceptionHandler();


extern "C" void exc_c(void);
extern "C" void install_exc(void);
extern "C" void uninstall_exc(void);

extern int *reg_lst;
extern int dev_kit;

#endif


