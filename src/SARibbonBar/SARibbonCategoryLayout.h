#ifndef SARIBBONCATEGORYLAYOUT_H
#define SARIBBONCATEGORYLAYOUT_H
#include <QLayout>
#include <QList>
#include <QMap>
#include "SARibbonCategory.h"
class SARibbonReduceActionInfo;
class SARibbonPannel;
class SARibbonCategoryLayout : public QLayout 
{
public:
    SARibbonCategoryLayout(SARibbonCategory *parent);
    SARibbonCategoryLayout();
    ~SARibbonCategoryLayout();

    SARibbonCategory* ribbonCategory();

    virtual void addItem(QLayoutItem *item);
    virtual QLayoutItem *itemAt(int index) const;
    virtual QLayoutItem *takeAt(int index);
    virtual int count() const;
    
    void setGeometry(const QRect &rect);
    QSize sizeHint() const;
    QSize minimumSize();
    Qt::Orientations expandingDirections() const;
protected:
    int buildReduceModePannel(SARibbonPannel* realPannel, int x, int y);
    static QPoint calcPopupPannelPosition(SARibbonCategory* category,SARibbonPannel *pannel, int x);
private:
    QList<QLayoutItem *> itemList;
    QMap<SARibbonPannel*,SARibbonReduceActionInfo> m_pannelReduceInfo;
};

#endif // SARIBBONCATEGORYLAYOUT_H
