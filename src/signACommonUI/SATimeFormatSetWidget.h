#ifndef SATIMEFORMATSETWIDGET_H
#define SATIMEFORMATSETWIDGET_H
#include "SACommonUIGlobal.h"
#include <QWidget>
#include <QDateTime>
class QComboBox;
namespace Ui {
class SATimeFormatSetWidget;
}

///
/// \brief 时间设置窗口 用于设置时间的格式
///
class SA_COMMON_UI_EXPORT SATimeFormatSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SATimeFormatSetWidget(QWidget *parent = 0);
    ~SATimeFormatSetWidget();
    QString getTimeFormat() const;
    void setTimeFormatText(const QString& str);
private slots:
    void onCurrentTextChanged(const QString& text);
private:
    void setComboxTimeFormat(QComboBox* comb);
private:
    Ui::SATimeFormatSetWidget *ui;
    QDateTime m_currentDateTime;
};

#endif // SATIMEFORMATSETWIDGET_H
