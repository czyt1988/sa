#ifndef SADATAIMPORTDELEGATE_H
#define SADATAIMPORTDELEGATE_H
#include <SAMainWindowDelegate.h>

class MainWindow;
class OpenFileManager;
class SADataImportDelegate : public SAMainWindowDelegate
{
public:
    SADataImportDelegate(MainWindow* wnd);
    virtual ~SADataImportDelegate();
    bool textImport(const QString& path);
    bool csvImport(const QString& path);
    bool csvImportAsTable(const QString& path);
   //bool cfxCsv2DImport();
private:
};

#endif // SADATAIMPORTDELEGATE_H
