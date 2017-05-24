#ifndef MATERIALPROP_H
#define MATERIALPROP_H
#include <ThermodynamicsCalc.h>
#include <QDataStream>

class UnitData
{
public:
    UnitData(){}
    typedef UnitConversion_Pressure::UNIT UnitP;
    typedef UnitConversion_Temperature::UNIT UnitT;
    typedef UnitConversion_CalorificValue::UNIT UnitH;
    typedef UnitConversion_CalorificValue::UNIT UnitS;
    typedef UnitConversion_Density::UNIT UnitD;
    UnitT uT;
    UnitP uP;
    UnitD uD;
    UnitS uS;
    UnitH uH;

    ///
    /// \brief 设置为常用，既是压力MPa,温度℃，密度kg/m3，焓熵kJ/kg
    ///
    void setUnitUseful()
    {
        uT = UnitConversion_Temperature::unit_dc;
        uP = UnitConversion_Pressure::unit_MPa;
        uD = UnitConversion_Density::unit_kgPerM3;
        uH = UnitConversion_CalorificValue::unit_kJPerKg;
        uS = UnitConversion_CalorificValue::unit_kJPerKg;
    }
    void serializeIn(QDataStream& os)
    {
        int it,ip,id,is,ih;
        os>>it>>ip>>id>>is>>ih;
        uT = (UnitConversion_Temperature::UNIT)it;
        uP = (UnitConversion_Pressure::UNIT)ip;
        uD = (UnitConversion_Density::UNIT)id;
        uS = (UnitConversion_CalorificValue::UNIT)is;
        uH = (UnitConversion_CalorificValue::UNIT)ih;
    }

    void serializeOut(QDataStream& os) const
    {
        os<<int(uT)<<int(uP)<<int(uD)<<int(uS)<<int(uH);
    }
    double calcT2CurT(double t) const
    {
        return UnitConversion_Temperature::conversion(t,UnitConversion_Temperature::unit_dk,uT);
    }
    double calcP2CurP(double p) const
    {
        return UnitConversion_Pressure::conversion(p,UnitConversion_Pressure::unit_kPa,uP);
    }
    double toCalcT(double t) const
    {
        return UnitConversion_Temperature::conversion(t,uT,UnitConversion_Temperature::unit_dk);
    }
    double toCalcP(double p) const
    {
        return UnitConversion_Pressure::conversion(p,uP,UnitConversion_Pressure::unit_kPa);
    }
};
///
/// \brief 物性
///
class materialProp
{
public:
    static const double NanValue;
    static bool isNanValue(const double& d);
    materialProp(EasyREFPROP* prop);
    void setRefProp(EasyREFPROP* prop);
    EasyREFPROP* getRefProp() const;

    void reset();
    bool setT(double data);
    bool setP(double data);
    bool setD(double data);
    bool setH(double data);
    bool setS(double data);
    void setPT(double p,double t);
    void setPS(double p,double s);

    void setUnitT(UnitConversion_Temperature::UNIT u){m_unit.uT = u;}
    void setUnitP(UnitConversion_Pressure::UNIT u){m_unit.uP = u;}
    void setUnitS(UnitConversion_CalorificValue::UNIT u){m_unit.uS = u;}
    void setUnitH(UnitConversion_CalorificValue::UNIT u){m_unit.uH = u;}
    void setUnitD(UnitConversion_Density::UNIT u){m_unit.uD = u;}
    void setUnit(const UnitData& unit);
    const UnitData& getUnit() const{return m_unit;}

    double getP() const {return m_p;}
    double getT() const {return m_t;}
    double getS() const {return m_s;}
    double getH() const {return m_h;}
    ///
    /// \brief getDensity获取密度
    /// \return 密度
    ///
    double getDensity() const {return m_d;}
    double getCp() const {return m_cp;}
    double getCv() const {return m_cv;}
    ///
    /// \brief 获取绝热指数K
    ///
    /// 计算公式如下：
    /// \image html k.gif
    /// \return
    ///
    ///
    double getK() const{return m_cp/m_cv;}
    double getZ() const{return m_z;}
	double getViscosity() const{return m_viscosity_upas;}
	double getThermalConductivity() const{return m_thermalConductivity_WperMK;}

    void serializeIn(QDataStream& os);
    void serializeOut(QDataStream& os) const;
    ///
    /// \brief 判断是否经过计算
    /// \return
    ///
    bool isCalc() const {return m_isCalc;}

	/// 
	/// \brief 单位的转换
	/// \param unit 要转换的单位
	/// \return 转换好的值
	/// 
	double to(UnitConversion_Density::UNIT unit);
private:
    static int totalOne(int x);
    int markTotalOne() const;
    void clearValue();
    void clearValueAndMark();
    void calcPT();
    void calcTD();
    void calcTS();
    void calcTH();
    void calcPD();
    void calcPS();
    void calcPH();
    void calcDS();
    void calcDH();
    void calcHS();
	void calcViscosityAndThermalConductivity();
private:
    double m_p;
    double m_t;
    double m_d;
    double m_s;
    double m_h;
    double m_cp;
    double m_cv;
    double m_z;
	double m_viscosity_upas;
	double m_thermalConductivity_WperMK;

    bool m_isCalc;
    UnitData m_unit;
    ThermodynamicsCalc m_thCalc;
    int m_mark;///< 用于记录参数的计算情况
};

#endif // MATERIALPROP_H
