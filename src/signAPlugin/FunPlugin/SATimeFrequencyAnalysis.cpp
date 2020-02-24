#include "SATimeFrequencyAnalysis.h"
#include "ui_SATimeFrequencyAnalysis.h"
#include <algorithm>
#include <QButtonGroup>
#include <czyMath_DSP.h>
#include <qwt_plot_zoneitem.h>
#include "SAUIReflection.h"
#include "SAUIInterface.h"
#include <iterator>
#include "SADataConver.h"
SATimeFrequencyAnalysis::SATimeFrequencyAnalysis(QWidget *parent) :
    QWidget(parent)
  ,ui(new Ui::SATimeFrequencyAnalysis)
  ,m_interval(0.001)
  ,m_wave(new QwtPlotCurve)
  ,m_spectrum(new QwtPlotCurve)
  ,m_xIntervalStart(0),m_xIntervalEnd(0)
  ,m_xIntervalStartIndex(0),m_xIntervalEndIndex(0)
  ,m_rangLength(256)

{
    ui->setupUi(this);
    initUI();
    initChart ();
    updateSliderRang();
}

SATimeFrequencyAnalysis::~SATimeFrequencyAnalysis()
{
    //m_wave和m_spectrum会自动析构，不需要chart管理内存
    m_wave.get()->detach();
    m_spectrum.get()->detach();
    delete ui;
}
///
/// \brief 设置波形数据
///
/// 设置的数据会直接存在m_wave中，通过getWaveData获取
/// \param data 数据
/// \return 若成功则会返回true否则返回false并触发postInfoMessage抛出错误的信息
///
bool SATimeFrequencyAnalysis::setWaveData(SAAbstractDatas* data)
{
    QVector<double> wave;
    if(SADataConver::converToDoubleVector(data,wave))
    {
        if(!checkSampleInterval())
            return false;
        QVector<double> x;
        x.reserve(wave.size());
        for(int i=0;i<wave.size();++i)
        {
            x.push_back(0+m_interval*double(i));
        }
        m_wave->setSamples(x,wave);
        updateData();
        return true;
    }
    QVector<QPointF> points;
    if(SADataConver::converToPointFVector(data,points))
    {
        m_interval = points[1].x () - points[0].x ();//获取x轴的间隔
        if(!checkSampleInterval())
            return false;
        m_wave->setSamples(points);
        updateSampleInterval(false);
        updateData();
        return true;
    }
    return false;
}
///
/// \brief 滚动条改变
///
/// 会调用updateTextInfo刷新信息，updateSpectrumChart刷新频谱
/// \param value 改变的值
/// \see updateSpectrumChart updateTextInfo
///
void SATimeFrequencyAnalysis::onSliderMoved(int value)
{
    Q_UNUSED(value);
    updateSlider();
    updateTextInfo();
    updateSpectrumChart();
}
///
/// \brief 导入数据
///
void SATimeFrequencyAnalysis::onPushButtonImport()
{
#if 0
    SAAbstractDatas* data = m_mainWnd->getSeletedData ();
    if(nullptr == data)
    {
        //m_mainWnd->showElapesdMessageInfo(QStringLiteral("请选择有效的波形数据,数据可以为线性数组或线性点序列！"),SA::ErrorMessage);
        saUI->showWarningMessageInfo(tr("请选择有效的波形数据,数据可以为线性数组或线性点序列！"));
        return;
    }
    if(!setWaveData(data))
    {
        //m_mainWnd->showElapesdMessageInfo(QStringLiteral("请选择有效的波形数据,数据可以为线性数组或线性点序列！"),SA::ErrorMessage);
    }
#else
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    if(!setWaveData(data))
    {
        saUI->showWarningMessageInfo(tr("please choose valid wave data!"));
    }
#endif
}
///
/// \brief 更新图表
///
//void SATimeFrequencyAnalysis::onPushbuttonUpdata()
//{
//    updateSampleInterval (true);
//    QVector<QPointF> series;

//    size_t size = m_wave->dataSize();
//    for(int i=0;i<size;++i)
//    {
//        series.push_back(QPointF(0+m_interval*double(i)
//                                 ,m_wave->sample(i).y()));

//    }
//    m_wave->setSamples(series);
//    updateData();
//}

void SATimeFrequencyAnalysis::onLineEditIntervalFinished()
{
    int oldInterval = m_interval;
    updateSampleInterval (true);
    if(m_interval == oldInterval)
        return;
    if(m_wave->dataSize()<=0)
        return;
    if(m_interval > m_wave->dataSize())
        m_interval = m_wave->dataSize();
    QVector<QPointF> series;
    size_t size = m_wave->dataSize();
    for(int i=0;i<size;++i)
    {
        series.push_back(QPointF(0+m_interval*double(i)
                                 ,m_wave->sample(i).y()));

    }
    m_wave->setSamples(series);
    updateData ();
}
///
/// \brief 导出数据
///
void SATimeFrequencyAnalysis::onPushbuttonExportdata()
{

}
///
/// \brief 按钮-左移
///
void SATimeFrequencyAnalysis::onPushButtonLeftMove()
{
    size_t size = m_wave->dataSize();
    if(size <= 0)
        return;
    qDebug()<<"m_xIntervalStartIndex - m_rangLength:"<<m_xIntervalStartIndex - m_rangLength;
    setStartIndex (m_xIntervalStartIndex - m_rangLength);//此函数会更改m_xIntervalStartIndex，m_xIntervalEndIndex
    qDebug()<<m_xIntervalStartIndex;
    ui->horizontalSlider_windowRange->setValue(calcSliderValueByIndex(m_xIntervalStartIndex,size,m_rangLength));

    updateTextInfo();
    updateSpectrumChart();
}
///
/// \brief 按钮-右移
///
void SATimeFrequencyAnalysis::onPushButtonRightMove()
{
    size_t size = m_wave->dataSize();
    if(size <= 0)
        return;
    setStartIndex (m_xIntervalStartIndex + m_rangLength);//此函数会更改m_xIntervalStartIndex，m_xIntervalEndIndex
    ui->horizontalSlider_windowRange->setValue(calcSliderValueByIndex(m_xIntervalStartIndex,size,m_rangLength));
    updateTextInfo();
    updateSpectrumChart();
}

void SATimeFrequencyAnalysis::onPushButtonLocate()
{
    bool isVisible = ui->widget_localtionPane->isVisible ();
    ui->widget_localtionPane->setVisible (!isVisible);
    if(isVisible)
        ui->pushButton_locate->setIcon (QIcon(":/image/res_image/expand_down.png"));
    else
        ui->pushButton_locate->setIcon (QIcon(":/image/res_image/expand_up.png"));
}
///
/// \brief 幅值设置被选中
/// \param id id对应czy::Math::DSP::SpectrumType
/// \param checked
///
void SATimeFrequencyAnalysis::onButtonGroupMagSetToggled(int id, bool checked)
{
    if(checked)
    {
        m_magType = static_cast<SA::Math::DSP::SpectrumType>(id);
        updateSpectrumChart();
    }
}
///
/// \brief 功率谱估计方法
/// \param id
/// \param checked
///
void SATimeFrequencyAnalysis::onButtonGroupPSDSetToggled(int id, bool checked)
{
    if(checked)
    {
        m_psdType = static_cast<SA::Math::DSP::PowerDensityWay>(id);
        updateSpectrumChart();
    }
}
///
/// \brief 傅里叶分析
///
void SATimeFrequencyAnalysis::onRadioButtonFFT_Toggled(bool toggled)
{
    if(toggled)
    {
        m_spectrumType = CalcType::FFT;
        updateSpectrumChart();
    }
}
///
/// \brief 功率谱分析
///
void SATimeFrequencyAnalysis::onRadioButtonPSD_Toggled(bool toggled)
{
    if(toggled)
    {
        m_spectrumType = CalcType::PSD;
        updateSpectrumChart();
    }
}
///
/// \brief 截断长度的改变
/// \param 长度
///
void SATimeFrequencyAnalysis::onSpinBoxIntervalLengthEditingFinished()
{
    updateSliderRang();
}

void SATimeFrequencyAnalysis::onSpinBoxStartIndexEditingFinished()
{
    if(m_wave->dataSize()<=0)
    {
        ui->spinBox_startIndex->setValue (0);
        return;
    }
    int value = ui->spinBox_startIndex->value ();
    setStartIndex (value);
    if(value != m_xIntervalStartIndex)
    {
        //说明数值进行了调整
        ui->spinBox_startIndex->setValue (m_xIntervalStartIndex);
    }
    ui->spinBox_endIndex->setValue (m_xIntervalEndIndex);
}

void SATimeFrequencyAnalysis::onSpinBoxEndIndexEditingFinished()
{
    if(m_wave->dataSize()<=0)
    {
        ui->spinBox_endIndex->setValue (0);
        return;
    }
    int value = ui->spinBox_endIndex->value ();
    setEndIndex (value);
    if(value != m_xIntervalEndIndex)
    {
        //说明数值进行了调整
        ui->spinBox_endIndex->setValue (m_xIntervalEndIndex);
    }
    ui->spinBox_startIndex->setValue (m_xIntervalStartIndex);
}

void SATimeFrequencyAnalysis::initUI()
{
    //radio button fft psd
    m_groupAnasysSet = new QButtonGroup(this);
    ui->radioButton_fft->setChecked(true);
    m_spectrumType = CalcType::FFT;
    m_groupAnasysSet->addButton(ui->radioButton_fft,CalcType::FFT);
    m_groupAnasysSet->addButton(ui->radioButton_psd,CalcType::PSD);
    connect(ui->radioButton_fft,&QAbstractButton::toggled,this,&SATimeFrequencyAnalysis::onRadioButtonFFT_Toggled);
    connect(ui->radioButton_psd,&QAbstractButton::toggled,this,&SATimeFrequencyAnalysis::onRadioButtonPSD_Toggled);

    //幅值选择
    ui->radioButton_amp->setChecked (true);//默认选中，避免触发
    m_groupMagSet = new QButtonGroup(this);
    m_groupMagSet->addButton (ui->radioButton_mag,SA::Math::DSP::Magnitude);
    m_groupMagSet->addButton (ui->radioButton_magDB,SA::Math::DSP::MagnitudeDB);
    m_groupMagSet->addButton (ui->radioButton_amp,SA::Math::DSP::Amplitude);
    m_groupMagSet->addButton(ui->radioButton_ampDB,SA::Math::DSP::AmplitudeDB);
    m_magType = SA::Math::DSP::Amplitude;
    connect (m_groupMagSet,static_cast<void (QButtonGroup::*)(int,bool)>(&QButtonGroup::buttonToggled)
             ,this,&SATimeFrequencyAnalysis::onButtonGroupMagSetToggled);

    //psd估计方式
    ui->radioButton_msa->setChecked (true);//默认选中，避免触发
    m_groupPSDSet = new QButtonGroup(this);
    m_groupPSDSet->addButton (ui->radioButton_msa,SA::Math::DSP::MSA);
    m_groupPSDSet->addButton (ui->radioButton_ssa,SA::Math::DSP::SSA);
    m_groupPSDSet->addButton (ui->radioButton_tisa,SA::Math::DSP::TISA);
    m_psdType = SA::Math::DSP::MSA;
    connect (m_groupPSDSet,static_cast<void (QButtonGroup::*)(int,bool)>(&QButtonGroup::buttonToggled)
             ,this,&SATimeFrequencyAnalysis::onButtonGroupPSDSetToggled);

    //采样间隔
    ui->lineEdit_interval->setText (QString::number (m_interval*1000.0));

    //窗口长度
    connect(ui->spinBox_intervalLength,&QAbstractSpinBox::editingFinished
            ,this,&SATimeFrequencyAnalysis::onSpinBoxIntervalLengthEditingFinished);
    //窗函数复选框
    connect(ui->comboBox_window,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
            ,this,[&](int index){
        m_signalWindow = static_cast<SA::Math::DSP::WindowType>(ui->comboBox_window->itemData (index).toInt ());
        updateSpectrumChart ();
    });
    ui->comboBox_window->addItem (tr("Rect"),int(SA::Math::DSP::WindowRect));
    ui->comboBox_window->addItem (tr("Hanning"),int(SA::Math::DSP::WindowHanning));
    ui->comboBox_window->addItem (tr("Hamming"),int(SA::Math::DSP::WindowHamming));
    ui->comboBox_window->addItem (tr("Blackman"),int(SA::Math::DSP::WindowBlackman));//巴克曼窗
    ui->comboBox_window->addItem (tr("Bartlett"),int(SA::Math::DSP::WindowBartlett));//巴特利窗
    ui->comboBox_window->setCurrentIndex (0);
    m_signalWindow = SA::Math::DSP::WindowRect;
    //按钮
    connect (ui->pushButton_import,&QAbstractButton::clicked,this,&SATimeFrequencyAnalysis::onPushButtonImport);
    //connect (ui->pushButton_upData,&QAbstractButton::clicked,this,&SATimeFrequencyAnalysis::onPushbuttonUpdata);
    connect (ui->pushButton_exportData,&QAbstractButton::clicked,this,&SATimeFrequencyAnalysis::onPushbuttonExportdata);
    connect (ui->pushButton_leftMove,&QAbstractButton::clicked,this,&SATimeFrequencyAnalysis::onPushButtonLeftMove);
    connect (ui->pushButton_rightMove,&QAbstractButton::clicked,this,&SATimeFrequencyAnalysis::onPushButtonRightMove);
    connect (ui->pushButton_locate,&QAbstractButton::clicked,this,&SATimeFrequencyAnalysis::onPushButtonLocate);
    ui->pushButton_locate->setIcon (QIcon(":/image/res_image/expand_down.png"));
    ui->widget_localtionPane->setVisible (false);
    //去均值
    ui->checkBox_detrend->setChecked (true);
    m_isDetrend = true;
    connect(ui->checkBox_detrend,&QAbstractButton::toggled,this,[&](bool isCheck){m_isDetrend = isCheck;});
    //采样间隔刷新
    connect(ui->lineEdit_interval,&QLineEdit::editingFinished,this,&SATimeFrequencyAnalysis::onLineEditIntervalFinished);
    //索引调整
    connect(ui->spinBox_startIndex,&QAbstractSpinBox::editingFinished
            ,this,&SATimeFrequencyAnalysis::onSpinBoxStartIndexEditingFinished);
    connect(ui->spinBox_endIndex,&QAbstractSpinBox::editingFinished
            ,this,&SATimeFrequencyAnalysis::onSpinBoxEndIndexEditingFinished);
    //滚动条
    connect (ui->horizontalSlider_windowRange,&QAbstractSlider::sliderMoved,this,&SATimeFrequencyAnalysis::onSliderMoved);
    ui->horizontalSlider_windowRange->setRange(0,100);//滚动条分为100等分
    updateSampleInterval(false);

}
///
/// \brief 初始化图表
///
void SATimeFrequencyAnalysis::initChart()
{

    m_wave->setPen(Qt::blue);
    m_spectrum->setPen(Qt::red);
    ui->waveChart->setAxisTitle (QwtPlot::xBottom,tr("time(s)"));
    ui->spectrogramChart->setAxisTitle (QwtPlot::yLeft,tr("amplitude"));
    ui->spectrogramChart->setAxisTitle (QwtPlot::xBottom,tr("frequency(Hz)"));
    ui->waveChart->addItem(m_wave.get(),tr("add wave curve"));
    ui->spectrogramChart->addItem(m_spectrum.get(),tr("add spectrum"));
    m_plotZone = new QwtPlotZoneItem();
    m_plotZone->setPen( Qt::darkGray );
    m_plotZone->setBrush( QColor(247,82,134,80) );
    m_plotZone->setOrientation( Qt::Vertical );
    m_plotZone->setInterval( m_xIntervalStart,m_xIntervalEnd);
    m_plotZone->attach( ui->waveChart );
}
///
/// \brief 刷新采样间隔
/// \param load true为获取界面数据，false为上传内存数据到界面
///
void SATimeFrequencyAnalysis::updateSampleInterval(bool load)
{
    if(load)
    {
        bool isOkConver;
        double temp = ui->lineEdit_interval->text ().toDouble(&isOkConver);
        if(isOkConver)
        {
            m_interval = temp/1000.0;
        }
    }
    else
    {
        ui->lineEdit_interval->setText(QString::number(m_interval*1000));
    }
}
///
/// \brief 采样间隔的检查，会获取用户界面到内存
/// \return 用户输入没有问题返回true，否则抛出postInfoMessage信号，内容为出错信息
///
bool SATimeFrequencyAnalysis::checkSampleInterval()
{
    updateSampleInterval(true);//获取采样间隔
    if(m_interval<=0)
    {
        saUI->showErrorMessageInfo(tr("Sampling interval must be positive"));//采样间隔需为正数
        return false;
    }
    return true;
}

///
/// \brief 刷新所有内容
///
void SATimeFrequencyAnalysis::updateData()
{
    //获取时间间隔
    updateSampleInterval(true);
    updateSliderRang();
    updateSlider();//包括截断长度的获取
    updateTextInfo();
    updateSpectrumChart();
}
///
/// \brief 获取截断长度，把用户界面数据捕获到内存
///
/// 如果用户输入小于0的数，会自动设置为之前的数据，若超过信号长度，会设置为当前信号长度，并抛出警告消息
///
void SATimeFrequencyAnalysis::updateIntervalLength()
{
    size_t size = m_wave->dataSize();
    int nTemp = m_rangLength;
    m_rangLength = ui->spinBox_intervalLength->value();
    if(m_rangLength > size)
    {
        m_rangLength = size;
        //截断长度不应该大于波形长度
        saUI->showWarningMessageInfo(tr("Truncation length should not be greater than the wave length"));
    }
    if(m_rangLength<=0)//如果长度小于0，返回原来结果
    {
        ui->spinBox_intervalLength->setValue(nTemp);
        m_rangLength = nTemp;
        //截断长度no能小于0
        saUI->showWarningMessageInfo(tr("Cutting length can not less than zero"));
    }
}
///
/// \brief 根据滑块位置，刷新内存，若设置参数超出预期，会自动调整滑块位置
///
void SATimeFrequencyAnalysis::updateSlider()
{
    updateIntervalLength();
    size_t dataSize = m_wave->dataSize();

    if(dataSize <= 0)//说明还没有数据
    {
        m_xIntervalEndIndex = 0;
        m_xIntervalStart = 0;
        m_xIntervalEnd = 0;
    }
    else//说明有数据
    {
        const int value = ui->horizontalSlider_windowRange->value();
        int startIndex = calcIndexBySliderValue(value,dataSize,m_rangLength);
        //开始索引的最大值
        setStartIndex (startIndex);
    }
}
///
/// \brief 刷新滚动条范围
///
void SATimeFrequencyAnalysis::updateSliderRang()
{
    updateIntervalLength ();
    size_t size = m_wave->dataSize();
    if(size > 0)
    {
//        int maxRang = size - m_rangLength - 1;
//        if(maxRang > ui->horizontalSlider_windowRange->width())
//            maxRang = ui->horizontalSlider_windowRange->width();
        ui->horizontalSlider_windowRange->setRange(0,100);
    }
    else
    {
        ui->horizontalSlider_windowRange->setRange(0,0);
    }
    updateSlider();
    updateTextInfo();
    updateSpectrumChart ();
}
///
/// \brief 更新显示信息
///
void SATimeFrequencyAnalysis::updateTextInfo()
{
    QString str = QStringLiteral("x:(%1-%2)")
            .arg(m_xIntervalStart).arg(m_xIntervalEnd);
    ui->label_info->setText(str);
    ui->spinBox_startIndex->setValue (m_xIntervalStartIndex);
    ui->spinBox_endIndex->setValue (m_xIntervalEndIndex);
}
///
/// \brief 根据设置刷新频谱
///
void SATimeFrequencyAnalysis::updateSpectrumChart()
{
    switch (m_spectrumType) {
    case CalcType::FFT:
        dealFFT();
        break;
    case CalcType::PSD:
        dealPSD();
        break;
    default:
        break;
    }
    ui->spectrogramChart->update();
}

int SATimeFrequencyAnalysis::calcSliderValueByIndex(int index, int dataSize, int intervalSize) const
{
    const int sliderRang = ui->horizontalSlider_windowRange->maximum() - ui->horizontalSlider_windowRange->minimum();
    int diff = dataSize - intervalSize;
    if(diff<=0)
        diff = 1;
    return double(index)/double(dataSize)*sliderRang + ui->horizontalSlider_windowRange->minimum();
}

int SATimeFrequencyAnalysis::calcIndexBySliderValue(int sliderValue, int dataSize, int intervalSize) const
{
    const int sliderRang = ui->horizontalSlider_windowRange->maximum() - ui->horizontalSlider_windowRange->minimum();
    const int diff = dataSize - intervalSize;
    return (double(sliderValue)/double(sliderRang))*diff;
}


void SATimeFrequencyAnalysis::setStartIndex(int startIndex)
{
    int dataSize = m_wave->dataSize();
    if(dataSize<=0)
        return;
    if(startIndex > dataSize - m_rangLength -1)
    {
        startIndex = dataSize - m_rangLength -1;
    }
    if(startIndex<0)
        startIndex = 0;
    int endIndex = startIndex + m_rangLength - 1;//计算结束索引
    //结束索引判断
    if(endIndex > (dataSize - 1))//保证索引的正确性
    {
        endIndex = dataSize - 1;
        startIndex = endIndex - m_rangLength;//重新计算开始索引
        if(startIndex<0)
            startIndex = 0;
        //ui->horizontalSlider_windowRange->setValue(calcSliderValueByIndex(m_xIntervalStart,dataSize));
    }
    m_xIntervalStartIndex = startIndex;
    m_xIntervalEndIndex = endIndex;
    m_xIntervalStart = m_wave->sample(startIndex).x();
    m_xIntervalEnd = m_wave->sample(endIndex).x();
    m_plotZone->setInterval(m_xIntervalStart,m_xIntervalEnd);
}

void SATimeFrequencyAnalysis::setEndIndex(int endIndex)
{
    int dataSize = m_wave->dataSize();
    if(dataSize<=0)
        return;
    if(endIndex > dataSize - 1)
    {
        endIndex = dataSize -1;
    }
    int startIndex = endIndex - m_rangLength;//计算开始索引
    //开始索引判断
    if(startIndex < 0)//保证索引的正确性
    {
        startIndex = 0;
        //这时要重新复算结束索引，保证截断长度的不变
        endIndex = startIndex + m_rangLength;//前提m_rangLength不能大于datasize
        if(endIndex > dataSize - 1)
        {
            endIndex = dataSize -1;
        }
    }
    m_xIntervalStartIndex = startIndex;
    m_xIntervalEndIndex = endIndex;
    m_xIntervalStart = m_wave->sample(startIndex).x();
    m_xIntervalEnd = m_wave->sample(endIndex).x();
    m_plotZone->setInterval(m_xIntervalStart,m_xIntervalEnd);
}

void SATimeFrequencyAnalysis::dealFFT()
{
    QVector<double> wave;
    QVector<double> x,y;
    double samRate = getWaveData(wave);
    if(samRate <= 0)
        return;
    SA::Math::DSP::windowed (wave.begin (),wave.end (),m_signalWindow);
    if(m_isDetrend)
    {
        SA::Math::DSP::detrend(wave.begin(),wave.end());
    }
    SA::Math::DSP::spectrum(wave.begin(),wave.end()
                             ,std::back_inserter(x)
                             ,std::back_inserter(y)
                             ,samRate
                             ,0
                             ,m_magType);
    if(x.size () <= 0 || y.size() <= 0)
    {
        return;
    }
    m_spectrum->setSamples(x,y);
}

void SATimeFrequencyAnalysis::dealPSD()
{
    QVector<double> wave;
    QVector<double> x,y;
    double samRate = getWaveData(wave);
    if(samRate <= 0)
        return;
    SA::Math::DSP::windowed (wave.begin (),wave.end (),m_signalWindow);
    if(m_isDetrend)
    {
        SA::Math::DSP::detrend(wave.begin(),wave.end());
    }
    SA::Math::DSP::powerSpectrum(wave.begin(),wave.end()
                             ,std::back_inserter(x)
                             ,std::back_inserter(y)
                             ,samRate
                             ,0
                             ,m_psdType
                             ,m_interval);
    if(x.size () <= 0 || y.size() <= 0)
    {
        return;
    }
    m_spectrum->setSamples(x,y);
}

double SATimeFrequencyAnalysis::getWaveData(QVector<double> &wave)
{
    size_t size = m_wave->dataSize();
    if(size <= 1)
        return -1;
    wave.clear();
    wave.reserve(size);
    for(int i=m_xIntervalStart;i<m_rangLength;++i)
    {
        wave.push_back(m_wave->sample(i).y());
    }
    return 1.0/(m_wave->sample(1).x() - m_wave->sample(0).x());
}
