#ifndef SALAYERITEMDELEGATE_H
#define SALAYERITEMDELEGATE_H

#include <QStyledItemDelegate>
///
/// \brief 图层表格的代理
///
class SALayerItemDelegate : public QStyledItemDelegate
{
public:
    SALayerItemDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

};

#endif // SALAYERITEMDELEGATE_H
