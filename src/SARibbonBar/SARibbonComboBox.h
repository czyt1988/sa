#ifndef SARIBBONCOMBOBOX_H
#define SARIBBONCOMBOBOX_H
#include <QComboBox>
#include <QScopedPointer>
class SARibbonComboBoxPrivate;
///
/// \brief QComboBox的Ribbon显示，可以显示QIcon和windowTitle在左侧
///
class SARibbonComboBox : public QWidget
{
public:
    SARibbonComboBox(QWidget *parent = Q_NULLPTR);
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QComboBox* comboBox();
protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e ) Q_DECL_OVERRIDE;
    void initStyleOption(QStyleOption* opt);
private:
    QScopedPointer<SARibbonComboBoxPrivate>  m_d;
};

#endif // SARIBBONCOMBOBOX_H
