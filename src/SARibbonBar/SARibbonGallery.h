#ifndef SARIBBONGALLERY_H
#define SARIBBONGALLERY_H
#include "SARibbonGlobal.h"
#include <QFrame>
#include "SARibbonGalleryGroup.h"
class QVBoxLayout;
class SARibbonGalleryPrivate;
class SA_RIBBON_EXPORT SARibbonGallery : public QFrame
{
    Q_OBJECT
public:
    SARibbonGallery(QWidget* parent = 0);
    virtual ~SARibbonGallery();
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    virtual SARibbonGalleryGroup* addGalleryGroup();
protected slots:
    void onPageDown();
    void onPageUp();
    void onShowMoreDetail();
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    friend class SARibbonGalleryPrivate;
    SARibbonGalleryPrivate* m_d;
};

class RibbonGalleryViewport : public QWidget
{
    Q_OBJECT
public:
    RibbonGalleryViewport(QWidget* parent);
    void addWidget(QWidget* w);
private:
    QVBoxLayout* m_layout;
};

#endif // SARIBBONGALLERY_H
