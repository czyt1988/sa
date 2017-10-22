#ifndef SARIBBONELEMENTCREATEDELEGATE_H
#define SARIBBONELEMENTCREATEDELEGATE_H
class QWidget;
class SARibbonTabBar;
class SARibbonApplicationButton;
class SARibbonCategory;
class SARibbonContextCategory;
class SARibbonPannel;
class SARibbonSeparatorWidget;
class SARibbonGallery;
class SARibbonGalleryGroup;
class SARibbonToolButton;
///
/// \brief SARibbon的子元素创建的代理，SARibbon内部创建子元素都通过SARibbonElementCreateDelegate来创建
/// 如果有些子元素重载，如SARibbonCategory，可以重载此类的createRibbonCategory,返回重载的类来进行重载
///
class SARibbonElementCreateDelegate
{
public:
    SARibbonElementCreateDelegate();
    virtual ~SARibbonElementCreateDelegate();
    virtual SARibbonTabBar* createRibbonTabBar(QWidget* parent);
    virtual SARibbonApplicationButton* createRibbonApplicationButton(QWidget* parent);
    virtual SARibbonCategory* createRibbonCategory(QWidget* parent);
    virtual SARibbonContextCategory* createRibbonContextCategory(QWidget* parent);
    virtual SARibbonPannel* createRibbonPannel(QWidget* parent);
    virtual SARibbonSeparatorWidget* createRibbonSeparatorWidget(int value,QWidget* parent);
    virtual SARibbonGallery* createRibbonGallery(QWidget* parent);
    virtual SARibbonGalleryGroup* createRibbonGalleryGroup(QWidget* parent);
    virtual SARibbonToolButton* createRibbonToolButton(QWidget* parent);
};

#endif // SARIBBONELEMENTCREATEDELEGATE_H
