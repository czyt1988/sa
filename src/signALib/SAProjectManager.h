#ifndef SAPROJECTMANAGER_H
#define SAPROJECTMANAGER_H
#include <QObject>
#include "SAGlobals.h"
#include "SALibGlobal.h"
class QDomDocument;
class SAValueManager;
class SALIB_EXPORT SAProjectManager : public QObject
{
    Q_OBJECT
private:
    explicit SAProjectManager();
    Q_DISABLE_COPY(SAProjectManager)
public:
    ~SAProjectManager();
    //获取对象
    static SAProjectManager *getInstance();
    //获取当前项目的全称路径
    QString getProjectFullPath() const;
    //保存工程
    bool save();
    //另存工程
    bool saveAs(const QString &savePath);
    //加载工程
    bool load(const QString &projectedPath);
    //当前工程是否已变更
    bool isdirty() const;
    //设置当前工程已经变更
    void setDirty(bool isdirty);

    QString getProjectName() const;
    void setProjectName(const QString &projectName);

    QString getProjectDescribe() const;
    void setProjectDescribe(const QString &projectDescribe);
    //获取工程的xml描述文件
    static QString getProjectDescribeFilePath(const QString &projectFolder);
    //获取工程的数据文件目录
    static QString getProjectDataFolderPath(const QString &projectFolder);
private:
    //设置项目的路径
    void setProjectFullPath(const QString &projectFullPath);
    //保存项目的描述信息
    void saveProjectInfo(const QString &projectFullPath);
    //保存项目信息
    bool loadProjectInfo(const QString &projectFullPath);
signals:
    ///
    /// \brief 信息，对于一些操作的错误等内容，通过message信号发射，信息的类型通过type进行筛选
    /// \param des 信息的内容
    /// \param type 信息的类型
    ///
    void messageInformation(const QString& des,SA::MeaasgeType type);
private:
    QString m_projectFullPath;///< 项目对应路径
    QString m_projectName;///< 项目名
    QString m_projectDescribe;///< 项目描述
    bool m_isdirty;///< 工程变更标记
    static SAProjectManager* s_instance;
};

#ifndef saProjectManager
#define saProjectManager SAProjectManager::getInstance()
#endif
#endif // SAPROJECTMANAGER_H
