#ifndef SARIBBONGALLERY_H
#define SARIBBONGALLERY_H
#include <QFrame>
#include "SARibbonGalleryGroup.h"
class SARibbonGalleryPrivate;
class SARibbonGallery : public QFrame
{
    Q_OBJECT
public:
    SARibbonGallery(QWidget* parent = 0);
    virtual ~SARibbonGallery();
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    virtual void addGalleryGroup(SARibbonGalleryGroup* group);
protected slots:
    void onPageDown();
    void onPageUp();
    void onShowMoreDetail();
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    SARibbonGalleryPrivate* m_d;
};

#endif // SARIBBONGALLERY_H
