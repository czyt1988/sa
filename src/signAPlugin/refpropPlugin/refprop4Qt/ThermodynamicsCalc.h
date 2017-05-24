#ifndef THERMODYNAMICSCALC_H
#define THERMODYNAMICSCALC_H
#include <EasyREFPROP.h>
#include <UnitConversion.h>
#ifndef DEFAULT_UNIT_T
#define DEFAULT_UNIT_T UnitConversion_Temperature::unit_dk
#endif
#ifndef DEFAULT_UNIT_P
#define DEFAULT_UNIT_P UnitConversion_Pressure::unit_MPa
#endif
#ifndef DEFAULT_UNIT_D
#define DEFAULT_UNIT_D UnitConversion_Density::unit_kgPerM3
#endif
#ifndef DEFAULT_UNIT_S
#define DEFAULT_UNIT_S UnitConversion_CalorificValue::unit_jPerKg
#endif
#ifndef DEFAULT_UNIT_H
#define DEFAULT_UNIT_H UnitConversion_CalorificValue::unit_jPerKg
#endif
class ThermodynamicsCalc
{
public:
    typedef UnitConversion_Pressure::UNIT UnitP;
    typedef UnitConversion_Temperature::UNIT UnitT;
    typedef UnitConversion_CalorificValue::UNIT UnitH;
    typedef UnitConversion_CalorificValue::UNIT UnitS;
    typedef UnitConversion_Density::UNIT UnitD;
    ThermodynamicsCalc(EasyREFPROP* property);
    EasyREFPROP* getRefProp() const{return m_refProp;}
    void setRefProp(EasyREFPROP*);
    //根据压力熵计算温度
    double temperaturePS(double p,double s,UnitP uP,UnitS uEntropy
                         ,UnitT uT = DEFAULT_UNIT_T);
    //计算焓值
    double enthalpy(double t,double p,UnitT uT,UnitP uP
                    ,UnitH uEnthalpy = DEFAULT_UNIT_H);

    //计算熵值
    double entropy(double t,double p,UnitT u,UnitP uP,UnitS uEntropy = DEFAULT_UNIT_S);
    //两状态点焓差计算
    double enthalpyDiff(double t1,double p1,double t2,double p2
                        ,UnitT uT,UnitP uP,UnitH uEnthalpy = DEFAULT_UNIT_H);
    //计算两状态点熵差
    double entropyDiff(double t1,double p1,double t2,double p2
                       ,UnitT uT,UnitP uP,UnitS uEntropy = DEFAULT_UNIT_S);
    //计算某状态的等熵温度，第一个状态的压力温度，和第二个点的压力，求第二个点的等熵（绝热）温度
    double isentropyTemp(double t1,double p1,double p2
                         ,UnitT uT,UnitP uP);
    //根据温度压力，计算其他物性
    void propertyTP(double t,double p,double& s,double& h
                    ,UnitT uT,UnitP uP,UnitS uEntropy = DEFAULT_UNIT_S,UnitH uEnthalpy = DEFAULT_UNIT_H);
    void propertyTP(double t ,double p
                    ,double& s,double& h,double& density,double& cp,double& cv,double& z
                    ,UnitT uT,UnitP uP
                    ,UnitS uEntropy = DEFAULT_UNIT_S
            ,UnitH uEnthalpy = DEFAULT_UNIT_H
            ,UnitD uDensity = DEFAULT_UNIT_D);
    void propertyTD(double t ,double density
                    ,double& s,double& h,double& p,double& cp,double& cv,double& z
                    ,UnitT uT,UnitD uDensity
                    ,UnitS uEntropy = DEFAULT_UNIT_S
            ,UnitH uEnthalpy = DEFAULT_UNIT_H
            ,UnitP uP = DEFAULT_UNIT_P);
    void propertyTS(double t ,double s
                    ,double& p,double& density,double& h,double& cp,double& cv,double& z
                    ,UnitT uT,UnitS uEntropy
                    ,UnitP uP = DEFAULT_UNIT_P
            ,UnitD uDensity = DEFAULT_UNIT_D
            ,UnitH uEnthalpy = DEFAULT_UNIT_H);
    void propertyTH(double t ,double h
                    ,double& p,double& density,double& s,double& cp,double& cv,double& z
                    ,UnitT uT,UnitH uEnthalpy
                    ,UnitP uP = DEFAULT_UNIT_P
            ,UnitD uDensity = DEFAULT_UNIT_D
            ,UnitS uEntropy = DEFAULT_UNIT_S);
    //压力-密度求物性
    void propertyPD(double p ,double density
                    ,double& t,double& s,double& h,double& cp,double& cv,double& z
                    ,UnitP uP,UnitD uDensity
                    ,UnitT uT = DEFAULT_UNIT_T
            ,UnitS uEntropy = DEFAULT_UNIT_S
            ,UnitH uEnthalpy = DEFAULT_UNIT_H);
    //熵-压力，计算其他物性
    void propertyPS(double p ,double s
                    ,double& t,double& h,double& density,double& cp,double& cv,double& z
                    ,UnitP uP,UnitS uEntropy
                    ,UnitT uT = DEFAULT_UNIT_T
            ,UnitH uEnthalpy = DEFAULT_UNIT_H
            ,UnitD uDensity = DEFAULT_UNIT_D);
    void propertyPH(double p ,double h
                    ,double& t,double& s,double& density,double& cp,double& cv,double& z
                    ,UnitP uP,UnitH uEnthalpy
                    ,UnitT uT = DEFAULT_UNIT_T
            ,UnitS uEntropy = DEFAULT_UNIT_S
            ,UnitD uDensity = DEFAULT_UNIT_D);
    void propertyDS(double density,double s
                    ,double& p ,double& t,double& h,double& cp,double& cv,double& z
                    ,UnitD uDensity
                    ,UnitS uEntropy
                    ,UnitP uP = DEFAULT_UNIT_P
                    ,UnitT uT = DEFAULT_UNIT_T,UnitH uEnthalpy=DEFAULT_UNIT_H);
    void propertyDH(double density,double h
                    ,double& p ,double& t,double& s,double& cp,double& cv,double& z
                    ,UnitD uDensity
                    ,UnitH uEnthalpy
                    ,UnitP uP = DEFAULT_UNIT_P
                    ,UnitT uT = DEFAULT_UNIT_T,UnitS uEntropy =DEFAULT_UNIT_H);
    void propertyHS(double h,double s
                    ,double&p,double& t,double &density,double& cp,double& cv,double& z
                    ,UnitH uEnthalpy
                    ,UnitS uEntropy
                    ,UnitP uP = DEFAULT_UNIT_P
                    ,UnitT uT = DEFAULT_UNIT_T
            ,UnitD uDensity = DEFAULT_UNIT_D
            );
    //根据压力温度计算密度
    double density(double t,double p,UnitT uT,UnitP uP,UnitD uD = DEFAULT_UNIT_D);

    static double castCalcUnit2CurUnit(double t,UnitT uT);
    static double castCalcUnit2CurUnit(double p,UnitP uP);
    static double castCalcUnit2CurUnit(double d,UnitD uD,double molWeight);
    static double castCalcUnit2CurUnit(double hs,UnitConversion_CalorificValue::UNIT uhs,double molWeight);
    double castCalcUnit2CurUnit(double d,UnitD uD);
    double castCalcUnit2CurUnit(double hs,UnitConversion_CalorificValue::UNIT uhs);

    static double castCurUnit2CalcUnit(double t,UnitT uT);
    static double castCurUnit2CalcUnit(double p,UnitP uP);
    static double castCurUnit2CalcUnit(double d,UnitD uD,double molWeight);
    static double castCurUnit2CalcUnit(double hs,UnitConversion_CalorificValue::UNIT uHS,double molWeight);
    double castCurUnit2CalcUnit(double d, UnitD uD);
    double castCurUnit2CalcUnit(double hs,UnitConversion_CalorificValue::UNIT uHS);
private:
    EasyREFPROP* m_refProp;
};

#endif // THERMODYNAMICSCALC_H
