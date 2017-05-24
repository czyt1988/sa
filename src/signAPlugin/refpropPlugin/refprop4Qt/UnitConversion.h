#ifndef UNITCONVERSION_H
#define UNITCONVERSION_H
//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2014, 中科院工程热物理研究所  （版权声明）
///    All rights reserved.
///
/// \file UnitConversion.h
/// \brief 实现常见单位的转换所需要头文件
///
///实现常见单位的转换
/// 2014.05.09加入能量单位焦耳的转换
///
/// \version 1.2
/// \author 陈宗衍
/// \date 2014-03-29
///
//////////////////////////////////////////////////////////////////////////


///
/// \brief 单位转换用的抽象基类
///
class UnitConversionBase
{
public:
    UnitConversionBase(){}
    ~UnitConversionBase(){}
 //   virtual ~UnitConversionBase(){}
    virtual double data() = 0;
    double m_data;
};

///
/// \brief 用于温度转换
///
class UnitConversion_Temperature : protected UnitConversionBase
{
public:
    UnitConversion_Temperature():absolute0degrees(-273.15){}
    UnitConversion_Temperature(double Kelvin):absolute0degrees(-273.15){m_data = Kelvin;}
    ///
    /// \brief 用于标明单位类型的枚举
    ///
    enum UNIT{
        unit_dc///< 摄氏度
        ,unit_dk///< 开尔文温度
        ,unit_df///< 华氏温度
    };



    ///
    /// \brief 设置温度为摄氏度
    /// \param dc 摄氏度值
    ///
    void setDegreeCentigrade(double dc){ m_data = dc - absolute0degrees;}
    ///
    /// \brief 转换为摄氏度
    /// \return 
    ///
    double toDegreeCentigrade(){return m_data + absolute0degrees; }
    ///
    /// \brief 设置为开尔文温度
    /// \param dk
    ///
    void setDegreeKelvin(double dk){ m_data = dk;}
    ///
    /// \brief 转换为开尔文温度
    /// \return 
    ///
    double toDegreeKelvin(){return m_data;}   
    ///
    /// \brief 设置为华氏温度
    /// \param df
    ///
    void setDegreeFahrenheit(double df){m_data = (df - 32.0) * 5.0 / 9.0 - absolute0degrees;}
    ///
    /// \brief 转换为华氏温度
    /// \return 
    ///
    double toDegreeFahrenheit(){return (m_data+absolute0degrees)*9.0/5.0+32.0;}
    ///
    /// \brief 设置温度，根据枚举变量UNIT
    /// \param data 值
    /// \param unit 标定输入的温度的单位
    /// \see UNIT
    void setTemperature(double data,UNIT unit){
        switch(unit){
        case unit_dc:setDegreeCentigrade(data);break;
        case unit_dk:setDegreeKelvin(data);break;
        case unit_df:setDegreeFahrenheit(data);break;
        }
    }
    ///
    /// \brief 转换温度，根据枚举变量UNIT
    ///
    /// 使用这个函数之前先要使用setTemperature设置温度
    /// \param unit 需要输出温度的单位
    /// \return 转换的温度
    ///
    double toTemperature(UNIT unit){
        switch(unit){
        case unit_dc:return toDegreeCentigrade();
        case unit_dk:return toDegreeKelvin();
        case unit_df:return toDegreeFahrenheit();
        }
        return 0;
    }
    ///
    /// \brief 转换静态函数
    /// \param inData 输入数据
    /// \param inUnit 输入数据的单位
    /// \param outUnit 输出数据的单位
    /// \return 转换后的结果
    ///
    static double conversion(double inData,UNIT inUnit,UNIT outUnit)
    {
        UnitConversion_Temperature tc;
        tc.setTemperature(inData,inUnit);
        return tc.toTemperature(outUnit);
    }

private:
    double data(){
        return m_data;
    }
    const double absolute0degrees;//-273.15
};

///
/// \brief 用于压力单位转换
///
class UnitConversion_Pressure : protected UnitConversionBase
{
public:
    UnitConversion_Pressure():standard_atmosphere_pressure(101325.0),psi_factor(6895.0),mmHg_factor(133.322368421){}
    UnitConversion_Pressure(double pa):standard_atmosphere_pressure(101325.0),psi_factor(6895.0),mmHg_factor(133.322368421){m_data = pa;}
    ///
    /// \brief 用于标明单位类型的枚举
    ///
    enum UNIT{
        unit_Pa///< 帕斯卡
        ,unit_kPa///< 千帕
        ,unit_MPa///< 兆帕
        ,unit_psi///< psi
        ,unit_atm///< atm
        ,unit_bar///< bar
        ,unit_mmHg///< 毫米汞柱
    };
    ///
    /// \brief 设置帕（Pa）
    /// \param kPa
    ///
    void setPa(double Pa){ m_data = Pa;}
    double toPa(){return (m_data);}
    ///
    /// \brief 设置千帕（kPa）
    /// \param kPa
    ///
    void setkPa(double kPa){ m_data = kPa * 1000.0;}
    double tokPa(){return (m_data / 1000.0);}
    ///
    /// \brief 设置兆帕（MPa）
    /// \param mpa
    ///
    void setMPa(double mpa){m_data = mpa * 1000000.0;}
    double toMPa(){return m_data / 1000000.0;}
    ///
    /// \brief 设置为Bar
    /// \param bar
    ///
    void setBar(double bar){m_data = bar * 100000.0;}
    double toBar(){ return m_data / 100000.0;}
    
    ///
    /// \brief 设置为标准大气压
    /// \param atm
    ///
    void setAtm(double atm){m_data = atm * standard_atmosphere_pressure;}
    double toAtm(){ return m_data / standard_atmosphere_pressure;}
    
    ///
    /// \brief 设置为psi 磅每平方英寸
    /// \param psi
    ///
    void setPSI(double psi){m_data = psi * psi_factor;}
    double toPsi(){return m_data / psi_factor;}

    ///
    /// \brief 设置毫米汞柱
    /// \param mmHg
    ///
    void setmmHg(double mmHg){m_data = mmHg*mmHg_factor;}
    double tommHg(){return m_data / mmHg_factor;}
    ///
    /// \brief 设置压力，根据枚举变量UNIT
    /// \param data 值
    /// \param unit 标定输入的压力的单位
    /// \see UNIT
    void setPressure(double data,UNIT unit){
        switch(unit){
        case unit_Pa:setPa(data);break;
        case unit_kPa:setkPa(data);break;
        case unit_MPa:setMPa(data);break;
        case unit_psi:setPSI(data);break;
        case unit_atm:setAtm(data);break;
        case unit_bar:setBar(data);break;
        case unit_mmHg:setmmHg(data);break;
        }
    }
    ///
    /// \brief 转换压力，根据枚举变量UNIT
    ///
    /// 使用这个函数之前先要使用setPressure设置压力
    /// \param unit 需要输出压力的单位
    /// \return 转换的压力
    /// \see setPressure
    ///
    double toPressure(UNIT unit){
        switch(unit){
        case unit_Pa:return toPa();
        case unit_kPa:return tokPa();
        case unit_MPa:return toMPa();
        case unit_psi:return toPsi();
        case unit_atm:return toAtm();
        case unit_bar:return toBar();
        case unit_mmHg:return tommHg();
        }
        return 0;
    }

    ///
    /// \brief 转换静态函数
    /// \param inData 输入数据
    /// \param inUnit 输入数据的单位
    /// \param outUnit 输出数据的单位
    /// \return 转换后的结果
    ///
    static double conversion(double inData,UNIT inUnit,UNIT outUnit)
    {
        UnitConversion_Pressure c;
        c.setPressure(inData,inUnit);
        return c.toPressure(outUnit);
    }
private:
    double data(){
        return m_data;
    }
    const double standard_atmosphere_pressure;
    const double psi_factor;
    const double mmHg_factor;///< 1mmHg对应的Pa
};

class UnitConversion_Density : protected UnitConversionBase
{
public:
    UnitConversion_Density(){}
    UnitConversion_Density(double KgPerm3){m_data = KgPerm3;}
    ///
    /// \brief 用于标明单位类型的枚举
    ///
    enum UNIT{
        unit_kgPerM3///<公斤每立方米
        ,unit_gPerCm3///< 克每立方厘米
        ,unit_molPerL///< 摩尔每升
    };
    ///
    /// \brief 设置相对分子质量，这样才能计算mol有关的密度
    /// \param 分子质量
    ///
    void setMolecularMass(double d_GPerMol){molecular_mass = d_GPerMol;}
    double getMolecularMass(){return molecular_mass;}
    ///
    /// \brief SI kg/m^3
    /// \param kg/m^3
    ///
    void setKgPerM3(double d){m_data = d;}
    double toKgPerM3(){return m_data;}
    ///
    /// \brief g/cm^3
    /// \param g/cm^3
    ///
    void setGPerCm3(double d){m_data = d*1000.0;}
    double toGPerCm3(){return m_data/1000.0;}
    ///
    /// \brief setMolPerL
    /// \param d
    ///
    void setMolPerL(double d){m_data = d*molecular_mass;}
    double toMolPerL(){return m_data/molecular_mass;}

    ///
    /// \brief 设置压力，根据枚举变量UNIT
    /// \param data 值
    /// \param unit 标定输入的压力的单位
    /// \see UNIT
    void setDensity(double data,UNIT unit){
        switch(unit){
        case unit_kgPerM3:setKgPerM3(data);break;
        case unit_gPerCm3:setGPerCm3(data);break;
        case unit_molPerL:setMolPerL(data);break;
        }
    }
    double toDensity(UNIT unit){
        switch(unit){
        case unit_kgPerM3:return toKgPerM3();
        case unit_gPerCm3:return toGPerCm3();
        case unit_molPerL:return toMolPerL();
        }
        return 0;
    }

    ///
    /// \brief 转换静态函数
    /// \param inData 输入数据
    /// \param inUnit 输入数据的单位
    /// \param outUnit 输出数据的单位
    /// \param molecular_mass 当使用摩尔相关密度时需要设置molecular_mass的值
    /// \return 转换后的结果
    ///
    static double conversion(double inData,UNIT inUnit,UNIT outUnit,double molecular_mass_gPerMol = -1)
    {
        UnitConversion_Density c;
        c.setMolecularMass(molecular_mass_gPerMol);
        c.setDensity(inData,inUnit);
        return c.toDensity(outUnit);
    }
private:
    double data(){
        return m_data;
    }
    double molecular_mass;///< 分子量
};

///
/// \brief 用于能量单位转换
///
class UnitConversion_Power : protected UnitConversionBase
{
public:
    UnitConversion_Power(){}
    enum UNIT{
        unit_kj///<千焦
        ,unit_j///< 焦
        ,unit_Mj///<兆焦
    };
    void setJ(double j){
        m_data = j;
    }
    double toJ(){return m_data;}

    void setKj(double kj){
        m_data = kj*1000.0;
    }
    double toKj(){return m_data/1000.0;}

    void setMj(double m){
        m_data = m*1000000.0;
    }
    double toMj(){
        return m_data/1000000.0;
    }
    void setPower(double data,UNIT unit){
        switch(unit){
        case unit_j:setJ(data);break;
        case unit_kj:setJ(data);break;
        case unit_Mj:setMj(data);break;
        }
    }
    double toPower(UNIT unit){
        switch(unit){
        case unit_j:return toJ();
        case unit_kj:return toKj();
        case unit_Mj:return toMj();
        }
        return 0;
    }
    static double conversion(double inData,UNIT inUnit,UNIT outUnit)
    {
        UnitConversion_Power c;
        c.setPower(inData,inUnit);
        return c.toPower(outUnit);
    }
private:
    double data(){
        return m_data;
    }
};

///
/// \brief 热值单位，用于焓、内能等
///
class UnitConversion_CalorificValue : protected UnitConversionBase
{
public:
    UnitConversion_CalorificValue(){}
     UnitConversion_CalorificValue(double jPerKg){m_data = jPerKg;}
    enum UNIT{
        unit_kjPerMol///<千焦每摩尔
        ,unit_jPerMol///< 焦每摩尔
        ,unit_kJPerKg///< 千焦每公斤
        ,unit_kjPerG///< 千焦每克
        ,unit_jPerKg///< 焦每公斤
        ,unit_jPerG///< 焦每克
    };
    ///
    /// \brief 设置相对分子质量，这样才能计算mol有关的密度
    /// \param 分子质量
    ///
    void setMolecularMass(double d_GPerMol){molecular_mass = d_GPerMol;}
    double getMolecularMass(){return molecular_mass;}
    ///
    /// \brief 设置为焦每公斤
    /// \param j_per_kg焦每公斤
    ///
    void setJPerKg(double j_per_kg){m_data = j_per_kg;}
    double toJPerKg(){return m_data;}
    ///
    /// \brief 千焦每公斤 unit_kjPerKg
    /// \param kj_per_kg
    ///
    void setKjPerKg(double kj_per_kg){m_data = kj_per_kg*1000.0;}
    double toKjPerKg(){return m_data/1000.0;}
    ///
    /// \brief 焦每克
    /// \param j_per_g
    ///
    void setJPerG(double j_per_g){m_data = j_per_g*1000.0;}
    double toJPerG(){return m_data/1000.0;}
    ///
    /// \brief 千焦每克
    /// \param kj_per_g
    ///
    void setKjPerG(double kj_per_g){m_data = kj_per_g*1000000.0;}
    double toKjPerG(){return m_data/1000000.0;}
    ///
    /// \brief setJPerMol 焦每摩尔
    /// \param j_per_mol
    ///
    void setJPerMol(double j_per_mol){m_data = (j_per_mol/molecular_mass)*1000.0;}
    double toJPerMol(){return m_data*molecular_mass/1000.0;}
    ///
    /// \brief 千焦每摩尔
    /// \param kj_per_mol
    ///
    void setKjPerMol(double kj_per_mol){setJPerMol(kj_per_mol*1000.0);}
    double toKjPerMol(){return toJPerMol()/1000.0;}

    ///
    /// \brief 设置压力，根据枚举变量UNIT
    /// \param data 值
    /// \param unit 标定输入的压力的单位
    /// \see UNIT
    void setCalorificValue(double data,UNIT unit){
        switch(unit){
        case unit_kjPerMol:setKjPerMol(data);break;
        case unit_jPerMol:setJPerMol(data);break;
        case unit_kJPerKg:setKjPerKg(data);break;
        case unit_kjPerG:setKjPerG(data);break;
        case unit_jPerKg:setJPerKg(data);break;
        case unit_jPerG:setJPerG(data);break;
        }
    }
    double toCalorificValue(UNIT unit){
        switch(unit){
        case unit_kjPerMol:return toKjPerMol();
        case unit_jPerMol:return toJPerMol();
        case unit_kJPerKg:return toKjPerKg();
        case unit_kjPerG:return toKjPerG();
        case unit_jPerKg:return toJPerKg();
        case unit_jPerG:return toJPerG();
        }
        return 0;
    }
    ///
    /// \brief 转换静态函数
    /// \param inData 输入数据
    /// \param inUnit 输入数据的单位
    /// \param outUnit 输出数据的单位
    /// \param molecular_mass 当使用摩尔相关密度时需要设置molecular_mass的值
    /// \return 转换后的结果
    ///
    static double conversion(double inData,UNIT inUnit,UNIT outUnit,double molecular_mass_gPerMol = -1)
    {
        UnitConversion_CalorificValue c;
        c.setMolecularMass(molecular_mass_gPerMol);
        c.setCalorificValue(inData,inUnit);
        return c.toCalorificValue(outUnit);
    }
private:
    double data(){
        return m_data;
    }
    double molecular_mass;///< 分子量
};

#endif // UNITCONVERSION_H
