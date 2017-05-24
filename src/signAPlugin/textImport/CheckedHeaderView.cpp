#include "CheckedHeaderView.h"

CheckedHeaderView::CheckedHeaderView(Qt::Orientation orientation, QWidget *parent)
    : QHeaderView(orientation, parent)
    , m_isEnableCheckBox(false)
{
    setSectionsClickable (true);
}

void CheckedHeaderView::setHeaderChecked(bool checked)
{
    m_isEnableCheckBox = checked;
    if (isEnabled() && m_isEnableCheckBox)
    {
        for(int i=0;i<count ();++i)
        {
            updateSection(i);
            QAbstractItemModel* m = model();
            if(!m)
                return;
            QVariant var = m->headerData (i,orientation (),Qt::CheckStateRole);
            if(!var.isValid ())
                return;
            if(!var.canConvert <bool>())
                return;
            bool check = var.toBool ();
            emit toggled(i,check);
        }
    }
    else
    {
        for(int i=0;i<count ();++i)
        {
            updateSection(i);
        }
    }
}

void CheckedHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();
    if(!m_isEnableCheckBox)
        return;
    QAbstractItemModel* m = model();
    if(!m)
        return;
    //复选框

    QVariant var = m->headerData (logicalIndex,orientation (),Qt::CheckStateRole);
    if(!var.isValid ())
        return;
    QStyleOptionButton optionCheckBox;
    if (isEnabled())
        optionCheckBox.state |= QStyle::State_Enabled;
    optionCheckBox.rect = checkBoxRect(rect);
    if (var.toBool ())
        optionCheckBox.state |= QStyle::State_On;
    else
        optionCheckBox.state |= QStyle::State_Off;


    style()->drawControl(QStyle::CE_CheckBox, &optionCheckBox, painter);

    //原来的控件内容
//    QStyleOptionHeader opt;
//    initStyleOption(&opt);
//    opt.rect = rect;
//    opt.section = logicalIndex;
//    if (isSortIndicatorShown() && sortIndicatorSection() == logicalIndex)
//            opt.sortIndicator = (sortIndicatorOrder() == Qt::AscendingOrder)
//                                ? QStyleOptionHeader::SortDown : QStyleOptionHeader::SortUp;
//    QVariant textAlignment = m->headerData(logicalIndex, orientation(),
//                                                      Qt::TextAlignmentRole);
//    opt.textAlignment = Qt::Alignment(textAlignment.isValid()
//                                          ? Qt::Alignment(textAlignment.toInt())
//                                          : defaultAlignment());
//    opt.iconAlignment = Qt::AlignVCenter;
//    opt.text = m->headerData(logicalIndex, orientation(),
//                                        Qt::DisplayRole).toString();
//    if (textElideMode() != Qt::ElideNone)
//        opt.text = opt.fontMetrics.elidedText(opt.text, textElideMode() , rect.width() - 4 - optionCheckBox.rect.width ());
//    opt.rect.setX (opt.rect.x () + 4 + optionCheckBox.rect.width ());
//    opt.rect.setWidth (opt.rect.width () - 4 - optionCheckBox.rect.width ());
//    style()->drawControl(QStyle::CE_Header, &opt, painter, this);
}

void CheckedHeaderView::mousePressEvent(QMouseEvent *event)
{    
    if (isEnabled())
    {
        if(m_isEnableCheckBox)
        {
            int index = logicalIndexAt(event->pos());
            if(index<count())
            {
                QAbstractItemModel* m = model();
                if(!m)
                    return;
                QVariant var = m->headerData (index,orientation (),Qt::CheckStateRole);
                if(!var.isValid ())
                    return;
                if(!var.canConvert <bool>())
                    return;
                bool check = !var.toBool ();
                m->setHeaderData (index,orientation (),check,Qt::CheckStateRole);
                updateSection(index);
                emit toggled(index,check);
            }
        }
        QHeaderView::mousePressEvent(event);
    }
    else
        QHeaderView::mousePressEvent(event);
}



QRect CheckedHeaderView::checkBoxRect(const QRect &sourceRect) const
{
    QStyleOptionButton checkBoxStyleOption;
    QRect checkBoxRect = style()->subElementRect(QStyle::SE_CheckBoxIndicator,
                                                 &checkBoxStyleOption);
//    QPoint checkBoxPoint(sourceRect.x() +
//                         sourceRect.width() / 2 -
//                         checkBoxRect.width() / 2,
//                         sourceRect.y() +
//                         sourceRect.height() / 2 -
//                         checkBoxRect.height() / 2);
    QPoint checkBoxPoint(sourceRect.x() + 2
                         ,
                         sourceRect.y() +
                         sourceRect.height() / 2 -
                         checkBoxRect.height() / 2);
    return QRect(checkBoxPoint, checkBoxRect.size());
}
