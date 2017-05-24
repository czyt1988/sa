#ifndef QTDMFILE_H
#define QTDMFILE_H
#include "nilibddc.h"
#include <QString>
#include <QList>
#include <iterator>
#include <memory>
#include "QTDMGroup.h"
#include "SALibGlobal.h"
class SALIB_EXPORT QTDMFile
{
public:
    QTDMFile();
    QTDMFile(DDCFileHandle file);
    QTDMFile(QString strFile, bool isRedOnly);
    ~QTDMFile();
    void setHandle(DDCFileHandle file);
    DDCFileHandle getHandle() const;

    unsigned int getGroupNums() const;
    QList<DDCChannelGroupHandle> getGroupHandles() const;
    QList<QTDMGroup*> getGroups() const;

    QString getName() const;
    QString getDescription() const;
    QString getTitle() const;
    QString getAuthor() const;

    bool openFile(QString strFile,bool readOnly);
    void closeFile();
    bool isFileOpen() const;

    bool isAutoCloseGroup() const;
    void setAutoCloseGroup(bool enable);
private:
    typedef std::shared_ptr<QTDMGroup> QTDMGroupPtr;
    void getGroupHandlesInFile();
    QString getStringProperty(const char* propertyName) const;
    QTDMGroupPtr makeGroupPtr(DDCChannelGroupHandle handle) const;
    bool openTdms(QString strFile,bool readOnly);
    bool openTdm(QString strFile,bool readOnly);
    void closeGroup();
private:
    int m_lastErrCode;
    DDCFileHandle m_file;
    QList<DDCChannelGroupHandle> m_groupHandles;
    QList<QTDMGroupPtr> m_groups;
    bool m_isAutoCloseGroup;///< 在析构时自动关闭打开的组句柄
};

#endif // QTDMFILE_H
