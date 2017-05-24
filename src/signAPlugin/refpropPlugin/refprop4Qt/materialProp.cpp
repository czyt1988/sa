#include "materialProp.h"
#include <numeric>
//啥也没设置
#define SET_NONE 0x0
//设置了P
#define SET_P 0x1 
//设置了T
#define SET_T 0x2
//设置了Density
#define SET_D 0x4
//设置了S
#define SET_S 0x8
//设置了H
#define SET_H 0x10
////设置了TP
//#define SET_TP 0x3
////设置了PD
//#define SET_PD 0x5
////设置了PS
//#define SET_PS 0x9
////设置了PH
//#define SET_PH 0x11
////设置了TD
//#define SET_TD 0x6
////设置了TS
//#define SET_TS 0xA
////设置了TH
//#define SET_TS 0x12
////设置了DS
//#define SET_DS 0xC
////设置了DH
//#define SET_DH 0x14
////设置了SH
//#define SET_DH 0x18

const double materialProp::NanValue = std::numeric_limits<double>::max();
bool materialProp::isNanValue(const double& d)
{
    return (d == NanValue);
}

materialProp::materialProp(EasyREFPROP* prop):m_thCalc(prop)
  ,m_isCalc(false)
  ,m_mark(SET_NONE)

{
    m_unit.setUnitUseful();
    clearValueAndMark();
}

void materialProp::setRefProp(EasyREFPROP* prop)
{
    m_thCalc.setRefProp(prop);
    //clearValueAndMark();
}



void materialProp::setUnit(const UnitData& unit)
{
    m_unit = unit;
}


void materialProp::serializeIn(QDataStream& os)
{
//为了兼容低版本的变化，每个序列化都添加版本号
    double ver;
    os>>ver;
    os>>m_p>>m_t>>m_d>>m_s>>m_h>>m_cp>>m_cv>>m_z;
	os>>m_viscosity_upas>>m_thermalConductivity_WperMK;
    os>>m_mark;
	m_unit.serializeIn(os);  
}

void materialProp::serializeOut(QDataStream& os) const
{
    double ver = 1.0;
    os<<ver;
    os<<m_p<<m_t<<m_d<<m_s<<m_h<<m_cp<<m_cv<<m_z;
	os<<m_viscosity_upas<<m_thermalConductivity_WperMK;
    os<<m_mark;
    m_unit.serializeOut(os);
}

///
/// \brief 获取物性指针
/// \return
///
EasyREFPROP*materialProp::getRefProp() const
{
    return m_thCalc.getRefProp();
}

void materialProp::reset()
{
    clearValueAndMark();
}
///
/// \brief 设置温度
///
/// 设置温度后，不一定进行计算，如果参数满足的话会进行计算，参数满足的条件是设置了两种参数，当设置了两种参数后函数自动计算，并把参数设置
/// 标记设置为SET_NONE
/// \param data 温度值
/// \return 成功设置了返回true（无论有没有进行计算），否则返回false
///
bool materialProp::setT(double data)
{
    int setCount = markTotalOne();
    m_t = data;
    if(0 == setCount)
    {
        if(isCalc())
        {//如果前面进行了计算的话，需要清空数值和计算标记位
            clearValue();
            m_t = data;
        }
        m_mark = SET_T;
        return true;
    }
    else if(1 == setCount)
    {//说明已经设置了一个参数了，这时需要进行计算
        switch (m_mark) {
        case SET_T://如果参数是重新设置，就直接返回,但不对mark进行修改
            return true;
        case SET_P:
            calcPT();
            break;
        case SET_D:
            calcTD();
            break;
        case SET_S:
            calcTS();
            break;
        case SET_H:
            calcTH();
            break;
        default://不明情况，先设置回只设置了温度
            m_mark = SET_T;
            return false;
        }
        m_mark = SET_NONE;//经过计算后，标记为置回SET_NONE
        return true;
    }
    else
    {//不明情况
        m_mark = SET_NONE;
    }
    return false;
}
///
/// \brief 设置压力参数
///
/// 设置压力后，不一定进行计算，如果参数满足的话会进行计算，参数满足的条件是设置了两种参数，当设置了两种参数后函数自动计算，并把参数设置
/// 标记设置为SET_NONE
/// \param data
/// \return 成功设置了返回true（无论有没有进行计算），否则返回false
///
bool materialProp::setP(double data)
{
    int setCount = markTotalOne();
    m_p = data;
    if(0 == setCount)
    {
        if(isCalc())
        {
            clearValue();//如果前面进行了计算的话，需要清空
            m_p = data;
        }
        m_mark = SET_P;
        return true;
    }
    else if(1 == setCount)
    {
        switch (m_mark) {
        case SET_T:
            calcPT();
            break;
        case SET_P://如果参数是重新设置，就直接返回,但不对mark进行修改
            return true;
        case SET_D:
            calcPD();
            break;
        case SET_S:
            calcPS();
            break;
        case SET_H:
            calcPH();
            break;
        default:
            m_mark = SET_P;
            return false;
        }
        m_mark = SET_NONE;
        return true;
    }
    else
    {//不明情况
        m_mark = SET_NONE;
    }
    return false;
}
///
/// \brief 设置密度
///
/// 设置密度后，不一定进行计算，但是，如果参数满足的话会进行计算，参数满足的条件是设置了两种参数，当设置了两种参数后函数自动计算
/// \return 成功设置了返回true（无论有没有进行计算），否则返回false
///
bool materialProp::setD(double data)
{
    int setCount = markTotalOne();
    m_d = data;
    if(0 == setCount)
    {
        if(isCalc())
        {
            clearValue();//如果前面进行了计算的话，需要清空
            m_d = data;
        }
        m_mark = SET_D;
        return true;
    }
    else if(1 == setCount)
    {
        switch (m_mark) {
        case SET_T:
            calcTD();
            break;
        case SET_P:
            calcPD();
            break;
        case SET_D://如果参数是重新设置，就直接返回,但不对mark进行修改
            return true;
        case SET_S:
            calcDS();
            break;
        case SET_H:
            calcDH();
            break;
        default:
            m_mark = SET_D;
            return false;
        }
        m_mark = SET_NONE;
        return true;
    }
    else
    {//不明情况
        m_mark = SET_NONE;
    }
    return false;
}
///
/// \brief 设置比焓
///
/// 设置比焓后，不一定进行计算，但是，如果参数满足的话会进行计算，参数满足的条件是设置了两种参数，当设置了两种参数后函数自动计算
/// \param data
/// \return 成功设置了返回true（无论有没有进行计算），否则返回false
///
bool materialProp::setH(double data)
{
    int setCount = markTotalOne();
    m_h = data;
    if(0 == setCount)
    {
        if(isCalc())
        {
            clearValue();//如果前面进行了计算的话，需要清空
            m_h = data;
        }
        m_mark = SET_H;
        return true;
    }
    else if(1 == setCount)
    {
        switch (m_mark) {
        case SET_T:
            calcTH();
            break;
        case SET_P:
            calcPH();
            break;
        case SET_D:
            calcDH();
            break;
        case SET_H://如果参数是重新设置，就直接返回,但不对mark进行修改
            return true;
        case SET_S:
            calcHS();
            break;
        default:
            m_mark = SET_H;
            return false;
        }
        m_mark = SET_NONE;
        return true;
    }
    else
    {//不明情况
        m_mark = SET_NONE;
    }
    return false;
}
///
/// \brief 设置比熵
///
/// 设置比熵后，不一定进行计算，但是，如果参数满足的话会进行计算，参数满足的条件是设置了两种参数，当设置了两种参数后函数自动计算
/// \param data
/// \return 成功设置了返回true（无论有没有进行计算），否则返回false
///
bool materialProp::setS(double data)
{
    int setCount = markTotalOne();
    m_s = data;
    if(0 == setCount)
    {
        if(isCalc())
        {
            clearValue();//如果前面进行了计算的话，需要清空
            m_s = data;
        }
        m_mark = SET_S;
        return true;
    }
    else if(1 == setCount)
    {
        switch (m_mark) {
        case SET_T:
            calcTS();
            break;
        case SET_P:
            calcPS();
            break;
        case SET_D:
            calcDS();
            break;
        case SET_H://如果参数是重新设置，就直接返回,但不对mark进行修改
            calcHS();
            break;
        case SET_S:
            return true;
        default:
            m_mark = SET_S;
            return false;
        }
        m_mark = SET_NONE;
        return true;
    }
    else
    {//不明情况
        m_mark = SET_NONE;
    }
    return false;
}


void materialProp::setPT(double p, double t)
{
    m_p = p;
    m_t = t;
    calcPT();
}

void materialProp::setPS(double p, double s)
{
    m_p = p;
    m_s = s;
    calcPS();
}
///
/// \brief 检验一个int数据里的二进制有多少个1
/// \param x
/// \return
///
int materialProp::totalOne(int x)
{
    int count = 0;
    while(x)
    {
     x = x & ( x - 1 );
     count++; 
    }
    return count;
}

int materialProp::markTotalOne() const
{
    return totalOne(m_mark);
}
///
/// \brief 清空数值和计算标记位，所有数值等于NanValue，isCalc()==false
///
void materialProp::clearValue()
{
    m_p = m_t = m_d = m_s = m_h = m_cp = m_cv = m_z = 
		m_viscosity_upas = m_thermalConductivity_WperMK = NanValue;
    m_isCalc = false;
}

void materialProp::clearValueAndMark()
{
    clearValue();
    m_mark = SET_NONE;
}

double materialProp::to(UnitConversion_Density::UNIT unit)
{
	return UnitConversion_Density::conversion(m_d,m_unit.uD,unit,getRefProp()->getMolecularWeight());
}

void materialProp::calcTD()
{
    m_thCalc.propertyTD(m_t,m_d,m_s,m_h,m_p,m_cp,m_cv,m_z
                        ,m_unit.uT,m_unit.uD,m_unit.uS,m_unit.uH,m_unit.uP);
	calcViscosityAndThermalConductivity();
    m_isCalc = true;
    m_mark = SET_NONE;
}

void materialProp::calcTS()
{
    m_thCalc.propertyTS(m_t,m_s,m_p,m_d,m_h,m_cp,m_cv,m_z,
                        m_unit.uT,m_unit.uS,m_unit.uP,m_unit.uD,m_unit.uH);
	calcViscosityAndThermalConductivity();
    m_isCalc = true;
    m_mark = SET_NONE;
}

void materialProp::calcTH()
{
    m_thCalc.propertyTH(m_t,m_h,m_p,m_d,m_s,m_cp,m_cv,m_z
                        ,m_unit.uT,m_unit.uH,m_unit.uP,m_unit.uD,m_unit.uS);
	calcViscosityAndThermalConductivity();
    m_isCalc = true;
    m_mark = SET_NONE;
}

void materialProp::calcPT()
{
    m_thCalc.propertyTP(m_t,m_p,m_s,m_h,m_d,m_cp,m_cv,m_z
                        ,m_unit.uT,m_unit.uP,m_unit.uS,m_unit.uH,m_unit.uD);
	calcViscosityAndThermalConductivity();
    m_isCalc = true;
    m_mark = SET_NONE;
}

void materialProp::calcPD()
{
    m_thCalc.propertyPD(m_p,m_d,m_t,m_s,m_h,m_cp,m_cv,m_z
                        ,m_unit.uP,m_unit.uD,m_unit.uT,m_unit.uS,m_unit.uH);
	calcViscosityAndThermalConductivity();
    m_isCalc = true;
    m_mark = SET_NONE;
}

void materialProp::calcPS()
{
    m_thCalc.propertyPS(m_p,m_s,m_t,m_h,m_d,m_cp,m_cv,m_z
                        ,m_unit.uP,m_unit.uS,m_unit.uT,m_unit.uH,m_unit.uD);
	calcViscosityAndThermalConductivity();
    m_isCalc = true;
    m_mark = SET_NONE;
}


void materialProp::calcPH()
{
    m_thCalc.propertyPH(m_p,m_h,m_t,m_s,m_d,m_cp,m_cv,m_z
                        ,m_unit.uP,m_unit.uH,m_unit.uT,m_unit.uS,m_unit.uD);
	calcViscosityAndThermalConductivity();
    m_isCalc = true;
    m_mark = SET_NONE;
}

void materialProp::calcDS()
{
    m_thCalc.propertyDS(m_d,m_s,m_p,m_t,m_h,m_cp,m_cv,m_z
                        ,m_unit.uD,m_unit.uS,m_unit.uP,m_unit.uT,m_unit.uH);
	calcViscosityAndThermalConductivity();
    m_isCalc = true;
    m_mark = SET_NONE;
}

void materialProp::calcDH()
{
    m_thCalc.propertyDH(m_d,m_h,m_p,m_t,m_s,m_cp,m_cv,m_z
                        ,m_unit.uD,m_unit.uH,m_unit.uP,m_unit.uT,m_unit.uS);
	calcViscosityAndThermalConductivity();
    m_isCalc = true;
    m_mark = SET_NONE;
}

void materialProp::calcHS()
{
    m_thCalc.propertyHS(m_h,m_s,m_p,m_t,m_d,m_cp,m_cv,m_z
                        ,m_unit.uH,m_unit.uS,m_unit.uP,m_unit.uT,m_unit.uD);
    m_isCalc = true;
    m_mark = SET_NONE;
}

void materialProp::calcViscosityAndThermalConductivity()
{
	getRefProp()->viscosity_and_thermalConductivity(
		m_unit.toCalcT(m_t)
		,to(UnitConversion_Density::unit_molPerL)
		,m_viscosity_upas
		,m_thermalConductivity_WperMK
		);
}

