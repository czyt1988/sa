#ifndef SAVALUEMANAGERTREEVIEW_H
#define SAVALUEMANAGERTREEVIEW_H
#include <QTreeView>
#include "SACommonUIGlobal.h"
class SAValueManagerModel;
class SAAbstractDatas;
class SA_COMMON_UI_EXPORT SAValueManagerTreeView : public QTreeView
{
    Q_OBJECT
public:
    SAValueManagerTreeView(QWidget* parent = nullptr);
    SAValueManagerModel* getValueManagerModel() const;
    QList<SAAbstractDatas*> getSeletedDatas() const;
    SAAbstractDatas* getSeletedData() const;
protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void startDrag(Qt::DropActions supportedActions) Q_DECL_OVERRIDE;
private:
    SAValueManagerModel* m_modelValueMgr;
};

#endif // SAVALUEMANAGERTREEVIEW_H
