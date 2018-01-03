#ifndef FEE_GLOBAL_H
#define FEE_GLOBAL_H

#if defined(FEE_LIBRARY)
#  define FEESHARED_EXPORT Q_DECL_EXPORT
#else
#  define FEESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FEE_GLOBAL_H
