#include "SAPenSetWidget.h"
#include <QApplication>
#include "ui_SAPenSetWidget.h"
#define TR(str) QApplication::translate("SAPenSetWidget", str, 0)
class SAPenSetWidget::UI
{
private:
    QHBoxLayout *horizontalLayoutMain;
    QVBoxLayout *verticalLayoutHorizontal;
    QHBoxLayout *horizontalLayoutHorizontalColor;
    //
    QVBoxLayout *verticalLayoutMain;
    QHBoxLayout *horizontalLayoutVerticalColor;
    QHBoxLayout *horizontalLayoutVerticalWidth;
    QHBoxLayout *horizontalLayoutVerticalStyle;
public:
    QLabel *labelColor;
    SAColorPickerButton *pushButtonColor;
    QSlider *horizontalSliderColorAlpha;
    QLabel *labelWidth;
    QSpinBox *spinBoxPenWidth;
    QLabel *labelStyle;
    SAPenStyleComboBox *comboBoxPenStyle;


    void setupUI(SAPenSetWidget*parent ,Qt::Orientation orient)
    {
        if (parent->objectName().isEmpty())
            parent->setObjectName(QStringLiteral("SAPenSetWidget"));
        if(Qt::Horizontal == orient)
        {
            parent->resize(247, 48);
            horizontalLayoutMain = new QHBoxLayout(parent);
            horizontalLayoutMain->setSpacing(0);
            horizontalLayoutMain->setObjectName(QStringLiteral("horizontalLayout_2"));
            horizontalLayoutMain->setContentsMargins(0, 0, 0, 0);
            verticalLayoutHorizontal = new QVBoxLayout();
            verticalLayoutHorizontal->setSpacing(0);
            verticalLayoutHorizontal->setObjectName(QStringLiteral("verticalLayout"));
            horizontalLayoutHorizontalColor = new QHBoxLayout();
            horizontalLayoutHorizontalColor->setObjectName(QStringLiteral("horizontalLayout"));
            labelColor = new QLabel(parent);
            labelColor->setObjectName(QStringLiteral("labelColor"));
            labelColor->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

            horizontalLayoutHorizontalColor->addWidget(labelColor);

            pushButtonColor = new SAColorPickerButton(parent);
            pushButtonColor->setObjectName(QStringLiteral("pushButtonColor"));
            QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(pushButtonColor->sizePolicy().hasHeightForWidth());
            pushButtonColor->setSizePolicy(sizePolicy);

            horizontalLayoutHorizontalColor->addWidget(pushButtonColor);


            verticalLayoutHorizontal->addLayout(horizontalLayoutHorizontalColor);

            horizontalSliderColorAlpha = new QSlider(parent);
            horizontalSliderColorAlpha->setObjectName(QStringLiteral("horizontalSliderColorAlpha"));
            QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
            sizePolicy1.setHorizontalStretch(0);
            sizePolicy1.setVerticalStretch(0);
            sizePolicy1.setHeightForWidth(horizontalSliderColorAlpha->sizePolicy().hasHeightForWidth());
            horizontalSliderColorAlpha->setSizePolicy(sizePolicy1);
            horizontalSliderColorAlpha->setMinimumSize(QSize(8, 0));
            horizontalSliderColorAlpha->setOrientation(Qt::Horizontal);

            verticalLayoutHorizontal->addWidget(horizontalSliderColorAlpha);


            horizontalLayoutMain->addLayout(verticalLayoutHorizontal);

            labelWidth = new QLabel(parent);
            labelWidth->setObjectName(QStringLiteral("labelWidth"));
            labelWidth->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

            horizontalLayoutMain->addWidget(labelWidth);

            spinBoxPenWidth = new QSpinBox(parent);
            spinBoxPenWidth->setObjectName(QStringLiteral("spinBoxPenWidth"));
            spinBoxPenWidth->setMaximum(100);

            horizontalLayoutMain->addWidget(spinBoxPenWidth);

            labelStyle = new QLabel(parent);
            labelStyle->setObjectName(QStringLiteral("labelStyle"));
            labelStyle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

            horizontalLayoutMain->addWidget(labelStyle);

            comboBoxPenStyle = new SAPenStyleComboBox(parent);
            comboBoxPenStyle->setObjectName(QStringLiteral("comboBoxPenStyle"));

            horizontalLayoutMain->addWidget(comboBoxPenStyle);
        }
        else
        {
            parent->resize(109, 79);
            verticalLayoutMain = new QVBoxLayout(parent);
            verticalLayoutMain->setSpacing(4);
            verticalLayoutMain->setObjectName(QStringLiteral("verticalLayout"));
            verticalLayoutMain->setContentsMargins(1, 1, 1, 1);
            horizontalLayoutVerticalColor = new QHBoxLayout();
            horizontalLayoutVerticalColor->setSpacing(4);
            horizontalLayoutVerticalColor->setObjectName(QStringLiteral("horizontalLayout"));
            labelColor = new QLabel(parent);
            labelColor->setObjectName(QStringLiteral("labelColor"));
            labelColor->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

            horizontalLayoutVerticalColor->addWidget(labelColor);

            pushButtonColor = new SAColorPickerButton(parent);
            pushButtonColor->setObjectName(QStringLiteral("pushButtonColor"));
            QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(pushButtonColor->sizePolicy().hasHeightForWidth());
            pushButtonColor->setSizePolicy(sizePolicy);

            horizontalLayoutVerticalColor->addWidget(pushButtonColor);

            horizontalSliderColorAlpha = new QSlider(parent);
            horizontalSliderColorAlpha->setObjectName(QStringLiteral("horizontalSliderColorAlpha"));
            QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
            sizePolicy1.setHorizontalStretch(0);
            sizePolicy1.setVerticalStretch(0);
            sizePolicy1.setHeightForWidth(horizontalSliderColorAlpha->sizePolicy().hasHeightForWidth());
            horizontalSliderColorAlpha->setSizePolicy(sizePolicy1);
            horizontalSliderColorAlpha->setMinimumSize(QSize(30, 0));
            horizontalSliderColorAlpha->setOrientation(Qt::Horizontal);


            horizontalLayoutVerticalColor->addWidget(horizontalSliderColorAlpha);


            verticalLayoutMain->addLayout(horizontalLayoutVerticalColor);

            horizontalLayoutVerticalWidth = new QHBoxLayout();
            horizontalLayoutVerticalWidth->setObjectName(QStringLiteral("horizontalLayout_2"));
            labelWidth = new QLabel(parent);
            labelWidth->setObjectName(QStringLiteral("labelWidth"));
            labelWidth->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

            horizontalLayoutVerticalWidth->addWidget(labelWidth);

            spinBoxPenWidth = new QSpinBox(parent);
            spinBoxPenWidth->setObjectName(QStringLiteral("spinBoxPenWidth"));
            spinBoxPenWidth->setMaximum(100);

            horizontalLayoutVerticalWidth->addWidget(spinBoxPenWidth);


            verticalLayoutMain->addLayout(horizontalLayoutVerticalWidth);

            horizontalLayoutVerticalStyle = new QHBoxLayout();
            horizontalLayoutVerticalStyle->setObjectName(QStringLiteral("horizontalLayout_3"));
            labelStyle = new QLabel(parent);
            labelStyle->setObjectName(QStringLiteral("labelStyle"));
            labelStyle->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

            horizontalLayoutVerticalStyle->addWidget(labelStyle);

            comboBoxPenStyle = new SAPenStyleComboBox(parent);
            comboBoxPenStyle->setObjectName(QStringLiteral("comboBoxPenStyle"));

            horizontalLayoutVerticalStyle->addWidget(comboBoxPenStyle);


            verticalLayoutMain->addLayout(horizontalLayoutVerticalStyle);


        }
        pushButtonColor->setStandardColors();
        horizontalSliderColorAlpha->setRange(0,255);
        retranslateUi(parent);
    }

    void retranslateUi(SAPenSetWidget *par)
    {
        par->setWindowTitle(QApplication::translate("SAPenSetWidget", "Form", 0));
        labelColor->setText(QApplication::translate("SAPenSetWidget", "Color", 0));
#ifndef QT_NO_TOOLTIP
        pushButtonColor->setToolTip(QApplication::translate("SAPenSetWidget", "<html><head/><body><p>set curve line color</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        pushButtonColor->setText(QString());
        labelWidth->setText(QApplication::translate("SAPenSetWidget", "Width", 0));
#ifndef QT_NO_TOOLTIP
        spinBoxPenWidth->setToolTip(QApplication::translate("SAPenSetWidget", "<html><head/><body><p>set curve line width</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        labelStyle->setText(QApplication::translate("SAPenSetWidget", "Style", 0));
#ifndef QT_NO_TOOLTIP
        comboBoxPenStyle->setToolTip(QApplication::translate("SAPenSetWidget", "<html><head/><body><p>set curve line style</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
    } // retranslateUi
};

SAPenSetWidget::SAPenSetWidget(QWidget *parent, Qt::Orientation orient) :
    QWidget(parent),
    ui(new SAPenSetWidget::UI)
  ,m_enableEmit(true)
{
    ui->setupUI(this,orient);
    m_enableEmit = false;
    connect(ui->pushButtonColor,&SAColorPickerButton::colorChanged
            ,this,&SAPenSetWidget::onCurrentColorChanged);
    connect(ui->horizontalSliderColorAlpha,&QSlider::valueChanged
            ,this,&SAPenSetWidget::onCurrentPenColorAlphaChanged);
    connect(ui->spinBoxPenWidth,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged)
            ,this,&SAPenSetWidget::onCurrentPenWidthChanged);
    connect(ui->comboBoxPenStyle,&SAPenStyleComboBox::penStyleChanged
            ,this,&SAPenSetWidget::onCurrentPenStyleChanged);
    m_enableEmit = true;
}

SAPenSetWidget::~SAPenSetWidget()
{
    delete ui;
}

QPen SAPenSetWidget::getPen() const
{
    return m_curPen;
}

void SAPenSetWidget::setPen(const QPen &pen)
{
    m_curPen = pen;
    m_enableEmit = false;
    ui->pushButtonColor->setCurrentColor(pen.color());
    ui->comboBoxPenStyle->setStyle(pen.style());
    ui->spinBoxPenWidth->setValue(pen.width());
    ui->horizontalSliderColorAlpha->setValue(pen.color().alpha());
    m_enableEmit = true;
}

void SAPenSetWidget::onCurrentColorChanged(const QColor &clr)
{
    m_curPen.setColor(clr);
    if(m_enableEmit)
        emit penChanged(m_curPen);
}

void SAPenSetWidget::onCurrentPenWidthChanged(int v)
{
    m_curPen.setWidth(v);
    if(m_enableEmit)
        emit penChanged(m_curPen);
}

void SAPenSetWidget::onCurrentPenStyleChanged(Qt::PenStyle style)
{
    m_curPen.setStyle(style);
    if(m_enableEmit)
        emit penChanged(m_curPen);
}

void SAPenSetWidget::onCurrentPenColorAlphaChanged(int v)
{
    QColor c = m_curPen.color();
    c.setAlpha(v);
    m_curPen.setColor(c);
    if(m_enableEmit)
        emit penChanged(m_curPen);
}

