#ifndef SACELLINPUTWIDGET_H
#define SACELLINPUTWIDGET_H

#include <QWidget>
#include <QList>
#include <QPair>
#include "SAPopupWidget.h"
class QLabel;
class QLineEdit;
namespace Ui {
class SACellInputWidget;
}
///
/// \brief cell表格编辑器
///
class SACellInputWidget : public SAPopupWidget
{
    Q_OBJECT

public:
    explicit SACellInputWidget(QWidget *parent = 0);
    ~SACellInputWidget();
    //cell的数量
    int cellsCount() const;
    //清空
    void clearCells();
    //重置数量
    void resizeCells(int size);
    //插入一个cell
    void appendCell();
    //移除最右边的cell
    void removeBackCell();
    //设置标题
    void setCellTitleText(int index,const QString& getCellTitleText);
    QString getCellTitleText(int index) const;
    //设置编辑内容
    void setCellEditText(int index,const QString& getCellTitleText);
    QString getCellEditText(int index) const;
    QStringList getCellEditTexts() const;
    //输入是否有效
    bool isAcceptInput() const;
    //返回按钮区的大小
    int getButtonAreaWidth() const;
    //设置单元格的宽度
    void setCellWidth(int index,int w);
    int getCellWidth(int index) const;
private slots:
    void onButtonOkPressed();
    void onButtonCancelClicked();
private:
    Ui::SACellInputWidget *ui;
    QList<QPair<QLabel*,QLineEdit*> > m_cellInfos;
    bool m_isAccept;
};

#endif // SACELLINPUTWIDGET_H
