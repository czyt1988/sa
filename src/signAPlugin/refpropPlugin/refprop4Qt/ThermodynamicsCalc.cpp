#include "ThermodynamicsCalc.h"
#include <vector>
#include <QDebug>
ThermodynamicsCalc::ThermodynamicsCalc(EasyREFPROP *property):m_refProp(property)
{


}

void ThermodynamicsCalc::setRefProp(EasyREFPROP* refProp)
{
    m_refProp = refProp;
}
///
/// \brief 根据压力和熵值计算温度
/// \param p 压力
/// \param s 熵
/// \param uP 压力单位
/// \param uEntropy 熵的单位
/// \param uT 计算的温度单位
/// \return 返回计算的温度
///
double ThermodynamicsCalc::temperaturePS(double p, double s
                                         , UnitP uP
                                         , UnitS uEntropy
                                         , UnitT uT)
{
    double isot,t1,t2,t3,t4,t5,t6,t7,t8,t9;
    std::vector<double> tempv;
    m_refProp->property_PS(castCurUnit2CalcUnit(p,uP),castCurUnit2CalcUnit(s,uEntropy)
                           ,isot,t1,t2,t3,tempv,tempv,t4,t5,t6,t7,t8,t9);
    return castCalcUnit2CurUnit(isot,uT);
}
///
/// \brief 计算焓值
/// \param t 温度
/// \param p 压力
/// \param uT 温度单位
/// \param uP 压力单位
/// \param uEnthalpy 焓单位
/// \return
///
double ThermodynamicsCalc::enthalpy(double t, double p
                                    , UnitT uT, UnitP uP
                                    , UnitH uEnthalpy)
{
    double h = m_refProp->enthalpy(castCurUnit2CalcUnit(t,uT),castCurUnit2CalcUnit(p,uP));
    return castCalcUnit2CurUnit(h,uEnthalpy);
}
///
/// \brief 计算熵值
/// \param t 温度
/// \param p 压力
/// \param uT 温度单位
/// \param uP 压力单位
/// \param uEntropy 熵单位
/// \return
///
double ThermodynamicsCalc::entropy(double t, double p
                                   , UnitT uT
                                   , UnitP uP
                                   , UnitS uEntropy)
{
    double s = m_refProp->entropy(castCurUnit2CalcUnit(t,uT),castCurUnit2CalcUnit(p,uP));
    return castCalcUnit2CurUnit(s,uEntropy);
}
///
/// \brief 计算两个状态点的焓差
/// \param t1 状态点1 的温度
/// \param p1 状态点1 的压力
/// \param t2 状态点2 的温度
/// \param p2 状态点2 的压力
/// \param uT 温度单位
/// \param uP 压力单位
/// \param uEnthalpy 计算的焓值单位
/// \return 计算的焓差值
///
double ThermodynamicsCalc::enthalpyDiff(double t1, double p1
                                        , double t2, double p2
                                        , UnitT uT
                                        , UnitP uP
                                        , UnitH uEnthalpy)
{
    double h1 = enthalpy(t1,p1,uT,uP,uEnthalpy);
    double h2 = enthalpy(t2,p2,uT,uP,uEnthalpy);
    return h2-h1;
}
///
/// \brief 计算两个状态点的熵差
/// \param t1 状态点1 的温度
/// \param p1 状态点1 的压力
/// \param t2 状态点2 的温度
/// \param p2 状态点2 的压力
/// \param uT 温度单位
/// \param uP 压力单位
/// \param uEntropy 熵的单位
/// \return 熵差
///
double ThermodynamicsCalc::entropyDiff(double t1, double p1
                                       , double t2, double p2
                                       , UnitT uT
                                       , UnitP uP
                                       , UnitS uEntropy)
{
    double s1 = entropy(t1,p1,uT,uP,uEntropy);
    double s2 = entropy(t2,p2,uT,uP,uEntropy);
    return s2-s1;
}
///
/// \brief 计算某状态的等熵温度，第一个状态的压力温度，和第二个点的压力，求第二个点的等熵（绝热）温度
/// \param t1
/// \param p1
/// \param p2
/// \param uT
/// \param uP
/// \return 等熵温度
///
double ThermodynamicsCalc::isentropyTemp(double t1, double p1
                                         , double p2
                                         , UnitT uT
                                         , UnitP uP)
{
    double s1 = entropy(t1,p1,uT,uP,UnitConversion_CalorificValue::unit_jPerMol);
    return temperaturePS(p2,s1,uP,UnitConversion_CalorificValue::unit_jPerMol,uT);
}
///
/// \brief 根据温度压力，计算熵和焓
/// \param t 温度
/// \param p 压力
/// \param s 计算得到的熵
/// \param h 计算得到的焓
/// \param uT 温度单位
/// \param uP 压力单位
/// \param uEntropy 熵单位
/// \param uEnthalpy 焓单位
///
void ThermodynamicsCalc::propertyTP(double t, double p, double &s, double &h
                                    , UnitT uT
                                    , UnitP uP
                                    , UnitS uEntropy
                                    , UnitH uEnthalpy)
{
    double t1,t2,t3,t4,t5,t6,t7,t8;
    std::vector<double> tv1,tv2;
    m_refProp->property_TP(
                castCurUnit2CalcUnit(t,uT)
                ,castCurUnit2CalcUnit(p,uP)
                ,t1,t2,t3,tv1,tv2,t4,t5,h,s,t6,t7,t8);
    h = castCalcUnit2CurUnit(h,uEnthalpy);
    s = castCalcUnit2CurUnit(s,uEntropy);
}
///
/// \brief 根据温度压力，计算熵和焓
/// \param t 温度
/// \param p 压力
/// \param s 计算得到的熵
/// \param h 计算得到的焓
/// \param density 计算得到的密度
/// \param cp cp
/// \param cv cv
/// \param z 压缩指数
/// \param uT 温度单位
/// \param uP 压力单位
/// \param uEntropy 熵单位
/// \param uEnthalpy 焓单位
/// \param uDensity 密度单位
///
void ThermodynamicsCalc::propertyTP(double t, double p, double& s, double& h, double& density
                                    ,double& cp,double& cv,double& z
                                    , UnitT uT
                                    , UnitP uP
                                    , UnitS uEntropy
                                    , UnitH uEnthalpy
                                    , UnitD uDensity)
{
    double t2,t3,t4,t5,t8;
    std::vector<double> tv1,tv2;
    p = castCurUnit2CalcUnit(p,uP);
    t = castCurUnit2CalcUnit(t,uT);
    m_refProp->property_TP(t,p
                ,density,t2,t3,tv1,tv2,t4,t5,h,s,cv,cp,t8);
    z = m_refProp->getZ(p,density,t);
    h = castCalcUnit2CurUnit(h,uEnthalpy);
    s = castCalcUnit2CurUnit(s,uEntropy);
    density = castCalcUnit2CurUnit(density,uDensity);
}

void ThermodynamicsCalc::propertyTD(double t, double density
                                    , double &s, double &h, double &p, double &cp, double &cv, double &z
                                    , UnitT uT
                                    , UnitD uDensity
                                    , UnitS uEntropy
                                    , UnitH uEnthalpy
                                    , UnitP uP)
{
    double t1,t2,t3,t4,t5;
    std::vector<double> tv1,tv2;
    t = castCurUnit2CalcUnit(t,uT);
    density = castCurUnit2CalcUnit(density,uDensity);
    m_refProp->property_TD(t,density,p,t1,t2,tv1,tv2,t3,t4,h,s,cv,cp,t5);
    z = m_refProp->getZ(p,density,t);
    s = castCalcUnit2CurUnit(s,uEntropy);
    h = castCalcUnit2CurUnit(h,uEnthalpy);
    p = castCalcUnit2CurUnit(p,uP);
}

void ThermodynamicsCalc::propertyTS(double t, double s
                                    , double &p, double &density, double &h, double &cp, double &cv, double &z
                                    , UnitT uT
                                    , UnitS uEntropy
                                    , UnitP uP
                                    , UnitD uDensity
                                    , UnitH uEnthalpy)
{
    double t1,t2,t3,t4,t5;
    std::vector<double> tv1,tv2;
    t = castCurUnit2CalcUnit(t,uT);
    s = castCurUnit2CalcUnit(s,uEntropy);
    m_refProp->property_TS(t,s,p,density,t1,t2,tv1,tv2,t3,t4,h,cv,cp,t5);
    z = m_refProp->getZ(p,density,t);
    p = castCalcUnit2CurUnit(p,uP);
    h = castCalcUnit2CurUnit(h,uEnthalpy);
    density = castCalcUnit2CurUnit(density,uDensity);
}

void ThermodynamicsCalc::propertyTH(double t, double h
                                    , double &p, double &density, double &s, double &cp, double &cv, double &z
                                    , UnitT uT
                                    , UnitH uEnthalpy
                                    , UnitP uP
                                    , UnitD uDensity
                                    , UnitS uEntropy)
{
    double t1,t2,t3,t4,t5;
    std::vector<double> tv1,tv2;
    t = castCurUnit2CalcUnit(t,uT);
    h = castCurUnit2CalcUnit(h,uEnthalpy);
    m_refProp->property_TH(t,h,p,density,t1,t2,tv1,tv2,t3,t4,s,cv,cp,t5);
    z = m_refProp->getZ(p,density,t);
    p = castCalcUnit2CurUnit(p,uP);
    density = castCalcUnit2CurUnit(density,uDensity);
    s = castCalcUnit2CurUnit(s,uEntropy);
}

void ThermodynamicsCalc::propertyPS(double p, double s, double& t, double& h, double& density
                                    ,double& cp,double& cv,double& z
                                    , UnitP uP
                                    , UnitS uEntropy
                                    , UnitT uT
                                    , UnitH uEnthalpy
                                    , UnitD uDensity)
{
    double t2,t3,t4,t5,t1;
    std::vector<double> tv1,tv2;
    p = castCurUnit2CalcUnit(p,uP);
    s = castCurUnit2CalcUnit(s,uEntropy);
    m_refProp->property_PS(p,s
                ,t,density,t1,t2,tv1,tv2,t3,t4,h,cv,cp,t5);
    z = m_refProp->getZ(p,density,t);
    h = castCalcUnit2CurUnit(h,uEnthalpy);
    t = castCalcUnit2CurUnit(t,uT);
    density = castCalcUnit2CurUnit(density,uDensity);
}

void ThermodynamicsCalc::propertyPH(double p, double h
                                    , double &t, double &s, double &density, double &cp, double &cv, double &z
                                    , UnitP uP
                                    , UnitH uEnthalpy
                                    , UnitT uT
                                    , UnitS uEntropy
                                    , UnitD uDensity)
{
    double t2,t3,t4,t5,t1;
    std::vector<double> tv1,tv2;
    p = castCurUnit2CalcUnit(p,uP);
    h = castCurUnit2CalcUnit(h,uEnthalpy);
    m_refProp->property_PH(p,h,t,density,t1,t2,tv1,tv2,t3,t4,s,cv,cp,t5);
    z = m_refProp->getZ(p,density,t);
    t = castCalcUnit2CurUnit(t,uT);
    s = castCalcUnit2CurUnit(s,uEntropy);
    density = castCalcUnit2CurUnit(density,uDensity);
}

void ThermodynamicsCalc::propertyDS(double density, double s
                                    , double &p, double &t, double &h, double &cp, double &cv, double &z
                                    , UnitD uDensity
                                    , UnitS uEntropy
                                    , UnitP uP
                                    , UnitT uT
                                    , UnitH uEnthalpy)
{
    double t2,t3,t4,t5,t1;
    std::vector<double> tv1,tv2;
    density = castCurUnit2CalcUnit(density,uDensity);
    s = castCurUnit2CalcUnit(s,uEntropy);
    m_refProp->property_DS(density,s,t,p,t1,t2,tv1,tv2,t3,t4,h,cv,cp,t5);
    z = m_refProp->getZ(p,density,t);
    t = castCalcUnit2CurUnit(t,uT);
    p = castCalcUnit2CurUnit(p,uP);
    h = castCalcUnit2CurUnit(h,uEnthalpy);
}

void ThermodynamicsCalc::propertyDH(double density, double h, double &p, double &t, double &s, double &cp, double &cv, double &z
                                    , UnitD uDensity
                                    , UnitH uEnthalpy
                                    , UnitP uP
                                    , UnitT uT
                                    , UnitS uEntropy)
{
    double t2,t3,t4,t5,t1;
    std::vector<double> tv1,tv2;
    density = castCurUnit2CalcUnit(density,uDensity);
    h = castCurUnit2CalcUnit(h,uEnthalpy);
    m_refProp->property_DH(density,h,t,p,t1,t2,tv1,tv2,t3,t4,s,cv,cp,t5);
    z = m_refProp->getZ(p,density,t);
    t = castCalcUnit2CurUnit(t,uT);
    p = castCalcUnit2CurUnit(p,uP);
    s = castCalcUnit2CurUnit(s,uEntropy);
}

void ThermodynamicsCalc::propertyHS(double h, double s
                                    , double &p, double &t, double &density, double &cp, double &cv, double &z
                                    , UnitH uEnthalpy
                                    , UnitS uEntropy
                                    , UnitP uP
                                    , UnitT uT
                                    , UnitD uDensity)
{
    double t2,t3,t4,t5,t1;
    std::vector<double> tv1,tv2;
    h = castCurUnit2CalcUnit(h,uEnthalpy);
    s = castCurUnit2CalcUnit(s,uEntropy);
    m_refProp->property_HS(h,s,t,p,density,t1,t2,tv1,tv2,t3,t4,cv,cp,t5);
    z = m_refProp->getZ(p,density,t);
    t = castCalcUnit2CurUnit(t,uT);
    p = castCalcUnit2CurUnit(p,uP);
    density = castCalcUnit2CurUnit(density,uDensity);
}

void ThermodynamicsCalc::propertyPD(double p, double density
                                    , double &t, double &s, double &h, double &cp, double &cv, double &z
                                    , UnitP uP
                                    , UnitD uDensity
                                    , UnitT uT
                                    , UnitS uEntropy
                                    , UnitH uEnthalpy)
{
    double t2,t3,t4,t5,t1;
    std::vector<double> tv1,tv2;
    p = castCurUnit2CalcUnit(p,uP);
    density = castCurUnit2CalcUnit(density,uDensity);
    m_refProp->property_PD(p,density,t,t1,t2,tv1,tv2,t3,t4,h,s,cv,cp,t5);
    z = m_refProp->getZ(p,density,t);
    t = castCalcUnit2CurUnit(t,uT);
    s = castCalcUnit2CurUnit(s,uEntropy);
    h = castCalcUnit2CurUnit(h,uEnthalpy);
}
///
/// \brief 根据压力温度计算密度
/// \param t 温度
/// \param p 压力
/// \param uT 温度单位
/// \param uP 压力单位
/// \param uD 密度单位
///
double ThermodynamicsCalc::density(double t, double p
                                   , UnitT uT
                                   , UnitP uP
                                   , UnitD uD)
{
    double d = m_refProp->density(castCurUnit2CalcUnit(t,uT),castCurUnit2CalcUnit(p,uP));
    return castCalcUnit2CurUnit(d,uD);
}
///
/// \brief 把计算得到的变量的值转换为设定输出单位的值，就是计算单位转换为设定单位CurUnit代表当前单位，就是设定单位
/// \param t 温度
/// \param uT 温度单位
/// \return 计算得到的值转换为用户设定单位对应的值
///
double ThermodynamicsCalc::castCalcUnit2CurUnit(double t,UnitT uT)
{
    return UnitConversion_Temperature::conversion(t,UnitConversion_Temperature::unit_dk,uT);
}
double ThermodynamicsCalc::castCalcUnit2CurUnit(double p,UnitP uP)
{
    return UnitConversion_Pressure::conversion(p,UnitConversion_Pressure::unit_kPa,uP);
}
double ThermodynamicsCalc::castCalcUnit2CurUnit(double d, UnitD uD, double molWeight)
{
    return UnitConversion_Density::conversion(d,UnitConversion_Density::unit_molPerL,uD,molWeight);
}
double ThermodynamicsCalc::castCalcUnit2CurUnit(double d,UnitD uD)
{
    return castCalcUnit2CurUnit(d,uD,m_refProp->getMolecularWeight());
}
double ThermodynamicsCalc::castCalcUnit2CurUnit(double hs,UnitConversion_CalorificValue::UNIT uhs,double molWeight)
{
    return UnitConversion_CalorificValue::conversion(hs,UnitConversion_CalorificValue::unit_jPerMol,uhs,molWeight);
}
double ThermodynamicsCalc::castCalcUnit2CurUnit(double hs,UnitConversion_CalorificValue::UNIT uhs)
{
    return castCalcUnit2CurUnit(hs,uhs,m_refProp->getMolecularWeight());
}
///
/// \brief 把当前的变量的值换为计算需要单位的值，和castCalcUnit2CurUnit功能相反
/// \param t 温度
/// \param uT 温度单位
/// \return 计算时对应单位的值
///
double ThermodynamicsCalc::castCurUnit2CalcUnit(double t, UnitT uT)
{
    return UnitConversion_Temperature::conversion(t,uT,UnitConversion_Temperature::unit_dk);
}
double ThermodynamicsCalc::castCurUnit2CalcUnit(double p,UnitP uP)
{
    return UnitConversion_Pressure::conversion(p,uP,UnitConversion_Pressure::unit_kPa);
}
double ThermodynamicsCalc::castCurUnit2CalcUnit(double d,UnitD uD,double molWeight)
{
    return UnitConversion_Density::conversion(d,uD,UnitConversion_Density::unit_molPerL,molWeight);
}
double ThermodynamicsCalc::castCurUnit2CalcUnit(double hs,UnitConversion_CalorificValue::UNIT uHS,double molWeight)
{
    return UnitConversion_CalorificValue::conversion(hs,uHS,UnitConversion_CalorificValue::unit_jPerMol,molWeight);
}
double ThermodynamicsCalc::castCurUnit2CalcUnit(double d,UnitD uD)
{
    return castCurUnit2CalcUnit(d,uD,m_refProp->getMolecularWeight());
}

double ThermodynamicsCalc::castCurUnit2CalcUnit(double hs, UnitConversion_CalorificValue::UNIT uHS)
{
    return castCurUnit2CalcUnit(hs,uHS,m_refProp->getMolecularWeight());
}
