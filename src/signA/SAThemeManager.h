#ifndef SATHEMEMANAGER_H
#define SATHEMEMANAGER_H
#include <QMainWindow>
///
/// \brief 用于管理主题
///
class SAThemeManager
{
public:
    SAThemeManager(QWidget* wind);
    void setTableViewLayout(QWidget* wind,const QString& qss=QString());
private:
    void setDefault();
    QString getDefaultTableViewLayoutQss();
private:
    QWidget* m_mainWindow;
};

#endif // SATHEMEMANAGER_H
