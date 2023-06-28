#pragma once

#include <QtCore/qglobal.h>
#include <QDebug>
#ifndef BUILD_STATIC
# if defined(XCHATYUTILTY_LIB)
#  define XCHATYUTILTY_EXPORT Q_DECL_EXPORT
# else
#  define XCHATYUTILTY_EXPORT Q_DECL_IMPORT
# endif
#else
# define XCHATYUTILTY_EXPORT
#endif

#define XLOG(...) qDebug() << __FILE__ << ":" << __LINE__ << " : " << __VA_ARGS__
#define XWAR(...) qWarning() << __FILE__ << ":" << __LINE__ << " : " << __VA_ARGS__
#define XERR(...) qCritical() << __FILE__ << ":" << __LINE__ << " : " << __VA_ARGS__
