#ifndef SACRASHSTACK_H
#define SACRASHSTACK_H
/**
  * @file 用于生成dump文件
  *
  * 原文链接：https://blog.csdn.net/wangshubo1989/article/details/52586516
  *
 */

//#ifdef Q_OS_WIN
#include <windows.h>
#include <QString>
int generate_mini_dump(PEXCEPTION_POINTERS pExceptionPointers);
LONG WINAPI unhandled_exception_filter(LPEXCEPTION_POINTERS lpExceptionInfo);

#ifndef SA_MINIDUMP_RECORD
#define SA_MINIDUMP_RECORD \
    SetUnhandledExceptionFilter(&unhandled_exception_filter);
#endif
//#endif //Q_OS_WIN
#endif // SACRASHSTACK_H
