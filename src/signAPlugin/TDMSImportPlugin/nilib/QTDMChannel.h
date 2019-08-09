#ifndef QTDMCHANNEL_H
#define QTDMCHANNEL_H
#include "nilibddc.h"
#include <QString>
#include <iterator>
#include <QVariant>
#include "SALibGlobal.h"

class QDateTime;
class SALIB_EXPORT QTDMChannel
{
public:
    QTDMChannel();
    QTDMChannel(DDCChannelHandle handle);

    DDCChannelHandle getHandle() const;
    void setHandle(DDCChannelHandle handle);

    DDCDataType getDataType() const;
    ///{@ 数据属性操作
    QString getName() const;
    QString getDescription() const;
    QString getUnit() const;
    QString getDataTypeString() const;
    ///}@

    /// {@ 获取数据操作
    unsigned __int64 getDataNums() const;
    int getDoubleData(double* datas,size_t firstIndex,size_t length) const;
    int getFloatData(float* datas,size_t firstIndex,size_t length) const;
    int getStringData(QString* datas,size_t firstIndex,size_t length) const;
    int getLongData(long* datas, size_t firstIndex, size_t length) const;
    int getShortData(short* datas, size_t firstIndex, size_t length) const;
    int getUCharData(unsigned char* datas, size_t firstIndex, size_t length) const;
    int getDateTimeData(QDateTime* datas, size_t firstIndex, size_t length) const;
    int getVariantData(QVariant* datas, size_t firstIndex, size_t length) const;

    QVariant getVariantData(size_t index) const;
    /// }@
    static QString dataTypeToString(const DDCDataType dataType);


    ///
    /// \brief 把序列转换为ARRAY<QVariant>类型的序列，可以是QVector<QVariant>,也可以是,前提是类型可转换
    ///
    /// 如果类型是自定义类型，需要用宏Q_DECLARE_METATYPE告知QMetaType
    ///
    /// \code{.cpp}
    ///struct MyStructZ
    ///{
    ///   int i;
    ///   ...
    ///};
    ///Q_DECLARE_METATYPE(MyStruct)
    /// \endcode
    /// \param in_begin 需要装换数据开始迭代器
    /// \param in_end 需要装换数据结束迭代器
    /// \param out_begin 结果数据开始迭代器
    /// \param out_end 结果数据结束迭代器
    ///
    template<typename IT_in,typename IT_QVariant>
    static void array_to_qvariantarray(const IT_in in_begin,const IT_in in_end
                            ,IT_QVariant out_begin)
    {
        IT_in in_ite;IT_QVariant out_ite;
        for(in_ite = in_begin,out_ite = out_begin
            ;(in_ite != in_end)
            ;++in_ite,++out_ite)
        {
            *out_ite = QVariant::fromValue(*in_ite);
        }
    }

private:
    QString getStringProperty(const char* propertyName) const;
private:
    DDCChannelHandle m_channel;
};

#endif // QTDMCHANNEL_H
