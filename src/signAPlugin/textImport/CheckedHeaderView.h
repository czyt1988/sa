#ifndef CHECKEDHEADERVIEW_H
#define CHECKEDHEADERVIEW_H
#include <QHeaderView>
#include <QPainter>
#include <QMouseEvent>
#include <QList>

class CheckedHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit CheckedHeaderView(Qt::Orientation orientation, QWidget *parent = 0);

    void setHeaderChecked(bool checked);

signals:
    void toggled(int section,bool checked);

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    void mousePressEvent(QMouseEvent *event);
private:
    QRect checkBoxRect(const QRect &sourceRect) const;

    bool m_isEnableCheckBox;
};


#endif // CHECKEDHEADER_H
