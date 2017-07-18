#ifndef CTKWIDGETSEXPORT_H
#define CTKWIDGETSEXPORT_H
#include <QtGlobal>

#if defined(Q_OS_WIN) || defined(Q_OS_SYMBIAN)
#  if defined(MAKE_CTK_LIB)
#    define CTK_WIDGETS_EXPORT Q_DECL_EXPORT
#  endif
#endif

#if !defined(CTK_WIDGETS_EXPORT)
#  if defined(CTK_SHARED)
#    define CTK_WIDGETS_EXPORT Q_DECL_EXPORT
#  else
#    define CTK_WIDGETS_EXPORT
#  endif
#endif

#endif // CTKWIDGETSEXPORT_H
