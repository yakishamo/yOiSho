#ifndef STDARG_H
#define STDARG_H

#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)

typedef __builtin_va_list va_list;

#endif /* STDARG_H */
