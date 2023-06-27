#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(XCHATYUTILTY_LIB)
#  define XCHATYUTILTY_EXPORT Q_DECL_EXPORT
# else
#  define XCHATYUTILTY_EXPORT Q_DECL_IMPORT
# endif
#else
# define XCHATYUTILTY_EXPORT
#endif
