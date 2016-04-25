#ifndef SBOX4X4HANDLER_GLOBAL_H
#define SBOX4X4HANDLER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SBOX4X4HANDLER_LIBRARY)
#  define SBOX4X4SHARED_EXPORT Q_DECL_EXPORT
#else
#  define SBOX4X4SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SBOX4X4HANDLER_GLOBAL_H
