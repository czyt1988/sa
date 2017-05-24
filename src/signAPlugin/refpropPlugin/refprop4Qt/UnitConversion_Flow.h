#ifndef UNITCONVERSION_FLOW_H
#define UNITCONVERSION_FLOW_H
//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2014, 中科院工程热物理研究所  （版权声明）
///    All rights reserved.
///
/// \file UnitConversion_Flow.h
/// \brief 实现常见流量单位的转换所需要头文件
///
/// 实现常见流量单位的转换，必须使用EasyREFPROP.h，物质性质先在EasyREFPROP中设定好
///
/// \version 1.0
/// \author 陈宗衍
/// \date 2014-05-7
///
//////////////////////////////////////////////////////////////////////////

#include <UnitConversion.h>
#include <EasyREFPROP.h>
class UnitConversion_Flow:public UnitConversionBase{
public:
    UnitConversion_Flow():m_refProp(0),m_den(-1),m_t_k(0.0),m_p_kPa(0.0)
    {}
    UnitConversion_Flow(double kgPerS):m_refProp(0),m_den(-1),m_t_k(0.0),m_p_kPa(0.0)
    {
        this->m_data = kgPerS;

    }
    UnitConversion_Flow(EasyREFPROP* prop,double t_k,double p_kPa,EasyREFPROP::Phase p)
        :m_refProp(prop),m_t_k(t_k),m_p_kPa(p_kPa)
    {
        this->m_data = 0.0;
        this->m_den = -1;
        setStandardState(t_k,p_kPa,p);
    }
    UnitConversion_Flow(EasyREFPROP* prop)
        :m_refProp(prop)
    {
        this->m_data = 0.0;
        this->m_den = -1;
        m_t_k = 273.15;
        m_p_kPa=101.325;
    }
    enum UNIT{
        unit_kgPerS///< kg/s
        ,unit_kgPerMin///< kg/min
        ,unit_kgPerHour///< kg/h
        ,unit_m3PerH
        ,unit_m3PerMin
        ,unit_m3PerS
    };
    ///
    /// \brief 设置一个标准状态，用于转换成体积流量
    /// \param t_k
    /// \param p_pa
    ///
    void setStandardState(double t_k,double p_kPa,EasyREFPROP::Phase p = EasyREFPROP::Vapor)
    {
        this->m_t_k = t_k;
        this->m_p_kPa = p_kPa;
        if(m_refProp)
        {
            m_den = m_refProp->density(t_k,p_kPa,p);
            m_den = UnitConversion_Density::conversion(m_den
                                                       ,UnitConversion_Density::unit_molPerL
                                                       ,UnitConversion_Density::unit_kgPerM3
                                                       ,m_refProp->getMolecularWeight());
        }
    }
    ///
    /// \brief 设置物性操作操作内容
    /// \param prop
    ///
    void setRefProp(EasyREFPROP* prop){m_refProp = prop;}
    ///
    /// \brief kg/s
    /// \param d
    ///
    void setkgPerS(double d){m_data = d;}
    double tokgPerS(){return m_data;}
    ///
    /// \brief kg/min
    /// \param d
    ///
    void setkgPerMin(double d){m_data = d/60.0;}
    double tokgPerMin(){return m_data*60.0;}
    ///
    /// \brief 公斤每小时
    /// \param d
    ///
    void setkgPerHour(double d){m_data = d/3600.0;}
    double tokgPerHour(){return m_data*3600.0;}
    ///
    /// \brief setM3PerS
    /// \param d
    ///
    void setM3PerS(double d){
        m_data = d * m_den;
    }
    double toM3PerS(){
        return m_data/m_den;
    }
    ///
    /// \brief m^3/min
    /// \param d
    ///
    void setM3PerMin(double d){
        d /= 60;
        m_data = d * m_den;
    }
    double toM3PerMin(){
        return (m_data/m_den)*60;
    }

    void setM3PerHour(double d){
        d /= 3600;
        m_data = d * m_den;
    }
    double toM3PerHour(){
        return (m_data/m_den)*3600;
    }
    ///
    /// \brief 计算标准状态的密度
    /// \param p
    ///
    void calcDensity(EasyREFPROP::Phase p = EasyREFPROP::Vapor){
        double den = m_refProp->density(m_t_k,m_p_kPa,p);
        den = UnitConversion_Density::conversion(den
                                                 ,UnitConversion_Density::unit_molPerL
                                                 ,UnitConversion_Density::unit_kgPerM3
                                                 ,m_refProp->getMolecularWeight());
    }
    ///
    /// \brief 设置流量
    /// \param data 数据
    /// \param unit 单位标志
    ///
    void setFlow(double data,UNIT unit){
        switch(unit){
        case unit_kgPerS:setkgPerS(data);break;
        case unit_kgPerMin:setkgPerMin(data);break;
        case unit_kgPerHour:setkgPerHour(data);break;
        case unit_m3PerH:setM3PerHour(data);break;
        case unit_m3PerMin:setM3PerMin(data);break;
        case unit_m3PerS:setM3PerS(data);break;
        }
    }
    double toFlow(UNIT unit){
        switch(unit){
        case unit_kgPerS:return tokgPerS();
        case unit_kgPerMin:return tokgPerMin();
        case unit_kgPerHour:return tokgPerHour();
        case unit_m3PerH:return toM3PerHour();
        case unit_m3PerMin:return toM3PerMin();
        case unit_m3PerS:return toM3PerS();
        }
        return 0;
    }

    static double conversion(double inData,UNIT inUnit,UNIT outUnit,EasyREFPROP* refProp
                             ,double standard_t_k,double standard_p_kPa,EasyREFPROP::Phase p = EasyREFPROP::Vapor)
    {
        UnitConversion_Flow flow(refProp);
        flow.setStandardState(standard_t_k,standard_p_kPa,p);
        flow.setFlow(inData,inUnit);
        return flow.toFlow(outUnit);
    }

private:
    double data(){
        return m_data;
    }
    double m_t_k;
    double m_p_kPa;
    EasyREFPROP* m_refProp;
    double m_den;
};

#endif // UNITCONVERSION_FLOW_H
