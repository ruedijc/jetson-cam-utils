#ifndef XICOREGUI_GLOBAL_H
#define XICOREGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XICOREGUI_LIBRARY)
#  define XICOREGUI_API Q_DECL_EXPORT
#else
#  define XICOREGUI_API Q_DECL_IMPORT
#endif

#endif // XICOREGUI_GLOBAL_H
