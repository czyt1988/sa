#ifndef PROGRESSSTATEWIDGET_H
#define PROGRESSSTATEWIDGET_H

#include <QWidget>

namespace Ui {
class progressStateWidget;
}
class QProgressBar;
class progressStateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit progressStateWidget(QWidget *parent = 0);
    ~progressStateWidget();
    QProgressBar* getProgressBar();
public slots:
    void setProgress(int present,const QString& text);
    void setPresent(int present);
    void setText(const QString& text);
private:
    Ui::progressStateWidget *ui;
};

#endif // PROGRESSSTATEWIDGET_H
