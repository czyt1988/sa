#ifndef SASELECTREGIONEDITOR_H
#define SASELECTREGIONEDITOR_H
#include "SACommonUIGlobal.h"
#include "SAAbstractPlotEditor.h"
class SASelectRegionEditorPrivate;
class SAChart2D;
///
/// \brief 用于控制选区数据的编辑器
///
class SA_COMMON_UI_EXPORT SASelectRegionEditor : public SAAbstractPlotEditor
{
    Q_OBJECT
    SA_IMPL(SASelectRegionEditor)
public:

    SASelectRegionEditor(SAChart2D *parent);
    ~SASelectRegionEditor();
    enum RTTI
    {
        RTTISelectRegionEditor = (SAAbstractPlotEditor::RTTIUserDefine + 13)
    };
    //是否启用编辑器
    virtual int rtti() const;
    //
    const SAChart2D* chart2D() const;
    SAChart2D* chart2D();
private slots:
    void onSelectionRegionChanged(const QPainterPath& shape);
    void onCurrentSelectItemsChanged(const QList<QwtPlotItem*>& items);
private:
    //更新选中的索引
    void updateRegionIndex();
protected:
    virtual bool eventFilter(QObject *object, QEvent *event);
    virtual bool mousePressEvent(const QMouseEvent *e);
    virtual bool mouseMovedEvent( const QMouseEvent *e);
    virtual bool mouseReleasedEvent( const QMouseEvent *e);
    virtual bool keyPressEvent(const QKeyEvent *e);
    virtual bool keyReleaseEvent(const QKeyEvent *e);

};

#endif // SASELECTREGIONEDITOR_H
