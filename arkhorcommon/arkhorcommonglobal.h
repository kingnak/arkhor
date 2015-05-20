#ifndef ARKHORCOMMONGLOBAL_H
#define ARKHORCOMMONGLOBAL_H

#include <QtGlobal>

#undef ARKHOR_COMMON_EXPORTING
#undef ARKHOR_COMMON_IMPORTING

#ifdef Q_OS_WIN32
#   ifdef ARKHOR_COMMON_DLL
#       define ARKHOR_COMMON_EXPORTS __declspec(dllexport)
#       define ARKHOR_COMMON_EXPORTING
#   else
#       define ARKHOR_COMMON_EXPORTS __declspec(dllimport)
#       define ARKHOR_COMMON_IMPORTING
#   endif
#else
#   define ARKHOR_COMMON_EXPORTS
#endif

#endif // ARKHORCOMMONGLOBAL_H
