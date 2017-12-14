#ifndef SAPROJECTMANAGER_H
#define SAPROJECTMANAGER_H
#include <QObject>
#include <QMap>
#include <QSet>
#include "SACommonUIGlobal.h"
#include <functional>
class QDomDocument;
class SAValueManager;
class SAProjectManagerPrivate;
class SA_COMMON_UI_EXPORT SAProjectManager : public QObject
{
    Q_OBJECT
private:
    explicit SAProjectManager();
    Q_DISABLE_COPY(SAProjectManager)
public:
    typedef std::function<void(SAProjectManager*)> FunAction;
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
    bool isDirty() const;
    //设置当前工程已经变更
    void setDirty(bool isDirty);
    //工程名设置
    QString getProjectName() const;
    void setProjectName(const QString &projectName);
    //工程描述设置
    QString getProjectDescribe() const;
    void setProjectDescribe(const QString &projectDescribe);
    //获取工程的xml描述文件
    static QString getProjectDescribeFilePath(const QString &projectFolder);
    //获取工程的数据文件目录
    static QString getProjectDataFolderPath(const QString &projectFolder, bool autoMakePath= true);
    QString getProjectDataFolderPath(bool autoMakePath= true);
    //获取工程的数据文件目录
    static QString getProjectSubWindowFolderPath(const QString &projectFolder, bool autoMakePath= true);
    QString getProjectSubWindowFolderPath(bool autoMakePath= true);
    //获取每个数据对应的文件路径
    QString getDataFilePath(const SAAbstractDatas* dataPtr) const;
    //添加保存时的额外动作
    void addSaveFunctionList(FunAction fun);
    const QList<SAProjectManager::FunAction>& getSaveFunctionList() const;
    void removeSaveFunctionList(FunAction fun);
    //添加加载时的额外动作
    void addLoadFunctionList(FunAction fun);
    const QList<SAProjectManager::FunAction>& getLoadFunctionList() const;
    void removeLoadFunctionList(FunAction fun);
private:
    //设置项目的路径
    void setProjectFullPath(const QString &projectFullPath);
    //保存项目的描述信息
    void saveProjectInfo(const QString &projectFullPath);
    //保存项目信息
    bool loadProjectInfo(const QString &projectFullPath);
    //加载变量
    void loadValues(const QString &projectFullPath);
    //保存变量
    bool saveValues(const QString &projectFullPath);
    //移除记录的要删除的数据
    void removeWillDeletedFiles();
signals:
    ///
    /// \brief 信息，对于一些操作的错误等内容，通过message信号发射，信息的类型通过type进行筛选
    /// \param des 信息的内容
    /// \param type 信息的类型
    ///
    void messageInformation(const QString& des,SA::MeaasgeType type);
    ///
    /// \brief 变更状态改变发射的信号
    /// \param isdirty 变更状态true时表示需要重新保存
    ///
    void dirtyStateChanged(bool isdirty);
private slots:
    //数据管理器删除数据发射信号的绑定
    void onDataRemoved(const QList<SAAbstractDatas*>& dataBeDeletedPtr);
    //数据管理器清除数据发射信号的绑定
    void onDataClear();
    //数据管理器变量名字变更的绑定
    void onDataNameChanged(SAAbstractDatas* data,const QString& oldName);
private:
    SA_IMPL(SAProjectManager)
private:
    static SAProjectManager* s_instance;
};

#ifndef saProjectManager
#define saProjectManager SAProjectManager::getInstance()
#endif
#endif // SAPROJECTMANAGER_H
