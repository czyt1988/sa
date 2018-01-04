#ifndef SACOMBOXPROPERTYITEM_H
#define SACOMBOXPROPERTYITEM_H
#include "SAPropertyItemContainer.h"
#include <QVariant>
class QComboBox;
class SAComboBoxPropertyItem : public SAPropertyItemContainer
{
    Q_OBJECT
public:
    explicit SAComboBoxPropertyItem(QWidget *parent = nullptr);
    ~SAComboBoxPropertyItem();
    QComboBox* getComboBox() const;

    void addItem(const QString &text, const QVariant &userData = QVariant());
    void addItem(const QIcon &icon, const QString &text, const QVariant &userData = QVariant());
    void addItems(const QStringList &texts);
    void setEditable(bool editable);

    QString currentText() const;
    QVariant currentData(int role = Qt::UserRole) const;
    int currentIndex() const;

    void clearAllItem();
public slots:

    void setCurrentIndex(int index);
    void setCurrentText(const QString &text);
    void setEditText(const QString &text);


Q_SIGNALS:
    void currentIndexChanged(int index);
    void currentIndexChanged(const QString &text);
    void currentTextChanged(const QString &text);
    void editTextChanged(const QString &text);
private:
    class UI;
    SAComboBoxPropertyItem::UI *ui;
};

#endif // SACOMBOXPROPERTYITEM_H
