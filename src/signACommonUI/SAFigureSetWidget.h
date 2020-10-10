#ifndef SAFIGURESETWIDGET_H
#define SAFIGURESETWIDGET_H

#include <QWidget>
#include "SACommonUIGlobal.h"
class SAFigureWindow;

/**
 * @brief The SAFigureSetWidget class
 */
class SA_COMMON_UI_EXPORT SAFigureSetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SAFigureSetWidget(QWidget *parent = nullptr);
    ~SAFigureSetWidget();
    //设置图表
    void setFigureWidget(SAFigureWindow *fig);
    SAFigureWindow *figure() const;

    //清空内容
    void clear();

protected:
    // 处理语言切换事件
    void changeEvent(QEvent *e) override;

private slots:
    void onFigureDestroy(QObject *obj);
    void onFigureBackgroundColorChanged(const QColor& clr);

private:
    class UI;
    SAFigureSetWidget::UI *ui;
};

#endif // SAFIGURESETWIDGET_H
