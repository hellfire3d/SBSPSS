#ifndef __SYSTEM_VSPRINTF_H__
#define	__SYSTEM_VSPRINTF_H__


// stdarg defs from MSVC
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define __va_start(ap,v)  ( ap = (__va_list)&v + _INTSIZEOF(v) )
#define __va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define __va_end(ap)      ( ap = (__va_list)0 )
typedef char *__va_list;


extern int __vsprintf(char *buf, const char *fmt, __va_list args);

#endif	/* __SYSTEM_VSPRINTF_H__ */