#ifndef EASYREFPROP_H
#define EASYREFPROP_H
//////////////////////////////////////////////////////////////////////////
///COPYRIGHT NOTICE
///Copyright (c) 2009, 中科院工程热物理研究所储能研发中心压缩机部      （版权声明）
///All rights reserved.
///
/// \file EasyREFPROP.h
/// \brief 用于调用REFPROP物性软件的数据库
///
/// 集成Qt下用于调用REFPROP物性软件的数据库
/// 2014-07-05 添加一些函数setMaterial的重载等
/// 2014-05-11.修正const函数
///
/// \version 1.3
/// \author 陈宗衍
/// \date 2014-03-27
//////////////////////////////////////////////////////////////////////////

#include <QLibrary>
#include <QDebug>
#include <QStringList>
#include <QVector>
#include <memory>
#include <vector>
#include <QDir>
const long c_refprop_char_length=255;
const long c_file_path_length=255;
const long c_length_of_reference=3;
const long c_error_message_length=255;
const long c_nc_max=20;		// Note: ncmax is the max number of components
const long c_num_params=72;
const long c_max_coefs=50;
///
/// \brief 基于qt的简易REFPROP物性dll的封装
///
/// Units.  The subroutines use the following units for all inputs and outputs:
///temperature                     K
///pressure, fugacity              kPa
///density                         mol/L
///composition                     mole fraction
///quality                         mole basis (moles vapor/total moles)
///enthalpy, internal energy       J/mol
///Gibbs, Helmholtz free energy    J/mol
///entropy, heat capacity          J/(mol.K)
///speed of sound                  m/s
///Joule-Thompson coefficient      K/kPa
///d(p)/d(rho)                     kPa.L/mol
///d2(p)/d(rho)2                   kPa.(L/mol)^2
///viscosity                       microPa.s (10^-6 Pa.s)
///thermal conductivity            W/(m.K)
///dipole moment                   debye
///surface tension                 N/m
///
class EasyREFPROP{
public:
    ///
    /// \brief EasyREFPROP 构造函数，构造函数自动加载refprop.dll
    /// \param dllPath dll的路径，默认是在当前目录下
    /// \param fld_path fluids文件夹路径
    /// \param mix_path HMX.BNC文件路径，默认在fluids\\HMX.BNC
    ///
    EasyREFPROP(QString dllPath = QDir::currentPath() + "/refprop.dll"
            ,QString fld_path = QDir::currentPath() + "/fluids/"
            ,QString mix_path= QDir::currentPath() + "/fluids/HMX.BNC")
        :
          FLD_PATH(fld_path)
        ,MIX_PATH(mix_path)
    {
        clearMaterial();
        loadDll(dllPath);
    }

    ~EasyREFPROP()
    {
        if(m_lib.isLoaded())
            m_lib.unload();
    }

    ///
    /// \brief 判断refprop.dll是否加载成功
    /// \return
    ///
    bool isLoadDll()
    {
        return m_lib.isLoaded();
    }

    ///
    /// \brief 加载refprop.dll，如果原来已经加载dll，会把原来加载好的dll卸载
    /// \param refpropDllPath dll的路径
    ///
    bool loadDll(const QString& refpropDllPath)
    {
        if(isLoadDll())
        {
            m_lib.unload();
            qDebug()<<QStringLiteral("释放dll:")<<m_lib.fileName();
        }
        m_lib.setFileName(refpropDllPath);
        bool r = m_lib.load();
        qDebug()<< QStringLiteral("加载dll:") << refpropDllPath;
        qDebug()<< QStringLiteral("加载状态:") << r;
        if(r)
            getFunPtr();
        return r;
    }
    ///
    /// \brief 打印dll错误 注意这是dll错误而不是调用错误
    ///
    void printDllErr(){
        qDebug()<<m_lib.errorString();
    }

    ///
    /// \brief 设置物性文件数据库，数据库位于REFPROP\fluids\文件夹下，REFPROP为物性软件REFPROP的安装目录，或者放在别的路径下也可以
    /// \param FluidsPath
    ///
    void setFluidsFilePath(const QString& FluidsPath)
    {
        FLD_PATH = FluidsPath;
        qDebug()<<"setFluidsFilePath:"<<FLD_PATH;
    }
    ///
    /// \brief setMixFile 设置HMX.BNC文件路径
    /// \param mixPath
    ///
    void setMixFile(const QString& mixPath)
    {
        MIX_PATH = mixPath;
        qDebug()<<"MixFile:"<<MIX_PATH;
    }


    ///
    /// \brief 状态的enum
    ///
    enum Phase{
        Liquid = 1///< 液相
        ,Vapor = 2///< 气相
    } ;

    ///
    /// \brief 物质，可以自己添加也可以使用
    ///
    enum Material{
        air///<空气
        ,_1_butene///<CH3-CH2-CH=CH2
        ,propanone///<(CH3)2CO
        ,ammonia///<NH3
        ,argon///<Ar
        ,benzene///<C6H6
        ,n_butane///<CH3-2(CH2)-CH3
        ,dodecane///<CH3-10(CH2)-CH3
        ,methylcyclohexane///<C6H11(CH3)
        ,cis_2_butene///<CH3-CH=CH-CH3
        ,n_propylcyclohexane///<(C6H11)CH2CH2CH3
        ,decafluorobutane///<C4F10
        ,dodecafluoropentane///<C5F12
        ,trifluoroiodomethane///<CF3I
        ,carbon_monoxide///<CO
        ,carbon_dioxide///<CO2
        ,carbon_oxide_sulfide///<COS
        ,cyclohexane///<cyclo-C6H12
        ,cyclopentane///<C5H10
        ,cyclopropane///<cyclo-C3H6
        ,deuterium///<D2
        ,deuterium_oxide///<D2O
        ,octamethylcyclotetrasiloxane///<C8H24O4Si4
        ,decamethylcyclopentasiloxane///<C10H30O5Si5
        ,dodecamethylcyclohexasiloxane///<C12H36Si6O6
        ,decane///<CH3-8(CH2)-CH3
        ,dimethyl_ester_carbonic_acid///<C3H6O3
        ,methoxymethane///<(CH3)2O
        ,ethane///<CH3CH3
        ,ethyl_alcohol///<C2H6O
        ,ethene///<CH2=CH2
        ,fluorine///<F2
        ,hydrogen_sulfide///<H2S
        ,helium_4///<He
        ,heptane///<CH3-5(CH2)-CH3
        ,hexane///<CH3-4(CH2)-CH3
        ,hydrogen_normal///<H2
        ,_2_methyl_1_propene///<CH2=C(CH3)2
        ,_2_methylpentane///<(CH3)2CH(CH2)2CH3
        ,_2_methylbutane///<(CH3)2CHCH2CH3
        ,_2_methylpropane///<CH(CH3)3
        ,krypton///<Kr
        ,decamethyltetrasiloxane///<C10H30Si4O3
        ,dodecamethylpentasiloxane///<C12H36Si5O4
        ,tetradecamethylhexasiloxane///<C14H42O5Si6
        ,octamethyltrisiloxane///<C8H24O2Si3
        ,methane///<CH4
        ,methanol///<CH3OH
        ,methyl_Z_Z_9_12_octadecadienoate///<C19H34O2
        ,methyl_Z_Z_Z_9_12_15_octadecatrienoate///<C19H32O2
        ,hexamethyldisiloxane///<C6H18OSi2
        ,methyl_cis_9_octadecenoate///<C19H36O2
        ,methyl_hexadecanoate///<C17H34O2
        ,methyl_octadecanoate///<C19H38O2
        ,dinitrogen_monoxide///<N2O
        ,neon///<Ne
        ,_2_2_dimethylpropane///<C(CH3)4
        ,nitrogen_trifluoride///<NF3
        ,nitrogen///<N2
        ,nonane///<CH3-7(CH2)-CH3
        ,octane///<CH3-6(CH2)-CH3
        ,orthohydrogen///<H2
        ,oxygen///<O2
        ,parahydrogen///<H2
        ,pentane///<CH3-3(CH2)-CH3
        ,propane///<CH3CH2CH3
        ,propene///<CH2=CH-CH3
        ,propyne///<CH3CCH
        ,trichlorofluoromethane///<CCl3F
        ,_1_1_2_trichloro_1_2_2_trifluoroethane///<CCl2FCClF2
        ,_1_2_dichloro_1_1_2_2_tetrafluoroethane///<CClF2CClF2
        ,chloropentafluoroethane///<CClF2CF3
        ,hexafluoroethane///<CF3CF3
        ,dichlorodifluoromethane///<CCl2F2
        ,_2_2_dichloro_1_1_1_trifluoroethane///<CHCl2CF3
        ,_2_3_3_3_tetrafluoroprop_1_ene///<CF3CF=CH2
        ,trans_1_3_3_3_tetrafluoropropene///<CHF=CHCF3 (trans)
        ,_1_chloro_1_2_2_2_tetrafluoroethane///<CHClFCF3
        ,pentafluoroethane///<CHF2CF3
        ,chlorotrifluoromethane///<CClF3
        ,_1_1_1_2_tetrafluoroethane///<CF3CH2F
        ,tetrafluoromethane///<CF4
        ,_1_1_dichloro_1_fluoroethane///<CCl2FCH3
        ,_1_chloro_1_1_difluoroethane///<CClF2CH3
        ,_1_1_1_trifluoroethane///<CF3CH3
        ,_1_1_difluoroethane///<CHF2CH3
        ,fluoroethane///<C2H5F
        ,dichlorofluoromethane///<CHCl2F
        ,octafluoropropane///<CF3CF2CF3
        ,chlorodifluoromethane///<CHClF2
        ,_1_1_1_2_3_3_3_heptafluoropropane///<CF3CHFCF3
        ,trifluoromethane///<CHF3
        ,_1_1_1_2_3_3_hexafluoropropane///<CF3CHFCHF2
        ,_1_1_1_3_3_3_hexafluoropropane///<CF3CH2CF3
        ,_1_1_2_2_3_pentafluoropropane///<CHF2CF2CH2F
        ,_1_1_1_3_3_pentafluoropropane///<CF3CH2CHF2
        ,difluoromethane///<CH2F2
        ,_1_1_1_3_3_pentafluorobutane///<CF3CH2CF2CH3
        ,fluoromethane///<CH3F
        ,octafluorocyclobutane///<cyclo-C4F8
        ,sulfur_hexafluoride///<SF6
        ,sulfur_dioxide///<SO2
        ,trans_2_butene///<CH3-CH=CH-CH3
        ,methylbenzene///<CH3-C6H5
        ,water///<H2O
        ,xenon///<Xe
    };

    ///
    /// \brief 清空设定的物质
    ///
    void clearMaterial()
    {
        this->material_file.clear();
        this->x_present.clear();
    }

    ///
    /// \brief 添加物质
    ///
    /// addMaterial使用完成后必须使用useMaterial才能使设置的生效
    /// \param materialFld 物质文件的文件名，位于REFPROP\fluids目录下
    /// \param present 物质所占百分比
    ///
    void addMaterial(QString materialFld,double present)
    {
        this->material_file.push_back(materialFld);
        this->x_present.push_back(present);
    }

    ///
    /// \brief 设置物质
    ///
    /// 区别于addMaterial和useMaterial的组合，使用setMaterial直接设置内置的纯物质或混合物
    /// \param mat
    ///
    void setMaterial(Material mat)
    {
        clearMaterial();
        switch (mat) {
        case air://空气
        {
            addMaterial("NITROGEN.FLD",0.7812);
            addMaterial("ARGON.FLD",0.0092);
            addMaterial("OXYGEN.FLD",0.2096);
        }break;
        case _1_butene:
            addMaterial("1BUTENE.FLD",1.0);
            break;
        case propanone:
            addMaterial("ACETONE.FLD",1.0);
            break;
        case ammonia:
            addMaterial("AMMONIA.FLD",1.0);
            break;
        case argon:
            addMaterial("ARGON.FLD",1.0);
            break;
        case benzene:
            addMaterial("BENZENE.FLD",1.0);
            break;
        case n_butane:
            addMaterial("BUTANE.FLD",1.0);
            break;
        case dodecane:
            addMaterial("C12.FLD",1.0);
            break;
        case methylcyclohexane:
            addMaterial("C1CC6.FLD",1.0);
            break;
        case cis_2_butene:
            addMaterial("C2BUTENE.FLD",1.0);
            break;
        case n_propylcyclohexane:
            addMaterial("C3CC6.FLD",1.0);
            break;
        case decafluorobutane:
            addMaterial("C4F10.FLD",1.0);
            break;
        case dodecafluoropentane:
            addMaterial("C5F12.FLD",1.0);
            break;
        case trifluoroiodomethane:
            addMaterial("CF3I.FLD",1.0);
            break;
        case carbon_monoxide:
            addMaterial("CO.FLD",1.0);
            break;
        case carbon_dioxide:
            addMaterial("CO2.FLD",1.0);
            break;
        case carbon_oxide_sulfide:
            addMaterial("COS.FLD",1.0);
            break;
        case cyclohexane:
            addMaterial("CYCLOHEX.FLD",1.0);
            break;
        case cyclopentane:
            addMaterial("CYCLOPEN.FLD",1.0);
            break;
        case cyclopropane:
            addMaterial("CYCLOPRO.FLD",1.0);
            break;
        case deuterium:
            addMaterial("D2.FLD",1.0);
            break;
        case deuterium_oxide:
            addMaterial("D2O.FLD",1.0);
            break;
        case octamethylcyclotetrasiloxane:
            addMaterial("D4.FLD",1.0);
            break;
        case decamethylcyclopentasiloxane:
            addMaterial("D5.FLD",1.0);
            break;
        case dodecamethylcyclohexasiloxane:
            addMaterial("D6.FLD",1.0);
            break;
        case decane:
            addMaterial("DECANE.FLD",1.0);
            break;
        case dimethyl_ester_carbonic_acid:
            addMaterial("DMC.FLD",1.0);
            break;
        case methoxymethane:
            addMaterial("DME.FLD",1.0);
            break;
        case ethane:
            addMaterial("ETHANE.FLD",1.0);
            break;
        case ethyl_alcohol:
            addMaterial("ETHANOL.FLD",1.0);
            break;
        case ethene:
            addMaterial("ETHYLENE.FLD",1.0);
            break;
        case fluorine:
            addMaterial("FLUORINE.FLD",1.0);
            break;
        case hydrogen_sulfide:
            addMaterial("H2S.FLD",1.0);
            break;
        case helium_4:
            addMaterial("HELIUM.FLD",1.0);
            break;
        case heptane:
            addMaterial("HEPTANE.FLD",1.0);
            break;
        case hexane:
            addMaterial("HEXANE.FLD",1.0);
            break;
        case hydrogen_normal:
            addMaterial("HYDROGEN.FLD",1.0);
            break;
        case _2_methyl_1_propene:
            addMaterial("IBUTENE.FLD",1.0);
            break;
        case _2_methylpentane:
            addMaterial("IHEXANE.FLD",1.0);
            break;
        case _2_methylbutane:
            addMaterial("IPENTANE.FLD",1.0);
            break;
        case _2_methylpropane:
            addMaterial("ISOBUTAN.FLD",1.0);
            break;
        case krypton:
            addMaterial("KRYPTON.FLD",1.0);
            break;
        case decamethyltetrasiloxane:
            addMaterial("MD2M.FLD",1.0);
            break;
        case dodecamethylpentasiloxane:
            addMaterial("MD3M.FLD",1.0);
            break;
        case tetradecamethylhexasiloxane:
            addMaterial("MD4M.FLD",1.0);
            break;
        case octamethyltrisiloxane:
            addMaterial("MDM.FLD",1.0);
            break;
        case methane:
            addMaterial("METHANE.FLD",1.0);
            break;
        case methanol:
            addMaterial("METHANOL.FLD",1.0);
            break;
        case methyl_Z_Z_9_12_octadecadienoate:
            addMaterial("MLINOLEA.FLD",1.0);
            break;
        case methyl_Z_Z_Z_9_12_15_octadecatrienoate:
            addMaterial("MLINOLEN.FLD",1.0);
            break;
        case hexamethyldisiloxane:
            addMaterial("MM.FLD",1.0);
            break;
        case methyl_cis_9_octadecenoate:
            addMaterial("MOLEATE.FLD",1.0);
            break;
        case methyl_hexadecanoate:
            addMaterial("MPALMITA.FLD",1.0);
            break;
        case methyl_octadecanoate:
            addMaterial("MSTEARAT.FLD",1.0);
            break;
        case dinitrogen_monoxide:
            addMaterial("N2O.FLD",1.0);
            break;
        case neon:
            addMaterial("NEON.FLD",1.0);
            break;
        case _2_2_dimethylpropane:
            addMaterial("NEOPENTN.FLD",1.0);
            break;
        case nitrogen_trifluoride:
            addMaterial("NF3.FLD",1.0);
            break;
        case nitrogen:
            addMaterial("NITROGEN.FLD",1.0);
            break;
        case nonane:
            addMaterial("NONANE.FLD",1.0);
            break;
        case octane:
            addMaterial("OCTANE.FLD",1.0);
            break;
        case orthohydrogen:
            addMaterial("ORTHOHYD.FLD",1.0);
            break;
        case oxygen:
            addMaterial("OXYGEN.FLD",1.0);
            break;
        case parahydrogen:
            addMaterial("PARAHYD.FLD",1.0);
            break;
        case pentane:
            addMaterial("PENTANE.FLD",1.0);
            break;
        case propane:
            addMaterial("PROPANE.FLD",1.0);
            break;
        case propene:
            addMaterial("PROPYLEN.FLD",1.0);
            break;
        case propyne:
            addMaterial("PROPYNE.FLD",1.0);
            break;
        case trichlorofluoromethane:
            addMaterial("R11.FLD",1.0);
            break;
        case _1_1_2_trichloro_1_2_2_trifluoroethane:
            addMaterial("R113.FLD",1.0);
            break;
        case _1_2_dichloro_1_1_2_2_tetrafluoroethane:
            addMaterial("R114.FLD",1.0);
            break;
        case chloropentafluoroethane:
            addMaterial("R115.FLD",1.0);
            break;
        case hexafluoroethane:
            addMaterial("R116.FLD",1.0);
            break;
        case dichlorodifluoromethane:
            addMaterial("R12.FLD",1.0);
            break;
        case _2_2_dichloro_1_1_1_trifluoroethane:
            addMaterial("R123.FLD",1.0);
            break;
        case _2_3_3_3_tetrafluoroprop_1_ene:
            addMaterial("R1234YF.FLD",1.0);
            break;
        case trans_1_3_3_3_tetrafluoropropene:
            addMaterial("R1234ZE.FLD",1.0);
            break;
        case _1_chloro_1_2_2_2_tetrafluoroethane:
            addMaterial("R124.FLD",1.0);
            break;
        case pentafluoroethane:
            addMaterial("R125.FLD",1.0);
            break;
        case chlorotrifluoromethane:
            addMaterial("R13.FLD",1.0);
            break;
        case _1_1_1_2_tetrafluoroethane:
            addMaterial("R134A.FLD",1.0);
            break;
        case tetrafluoromethane:
            addMaterial("R14.FLD",1.0);
            break;
        case _1_1_dichloro_1_fluoroethane:
            addMaterial("R141B.FLD",1.0);
            break;
        case _1_chloro_1_1_difluoroethane:
            addMaterial("R142B.FLD",1.0);
            break;
        case _1_1_1_trifluoroethane:
            addMaterial("R143A.FLD",1.0);
            break;
        case _1_1_difluoroethane:
            addMaterial("R152A.FLD",1.0);
            break;
        case fluoroethane:
            addMaterial("R161.FLD",1.0);
            break;
        case dichlorofluoromethane:
            addMaterial("R21.FLD",1.0);
            break;
        case octafluoropropane:
            addMaterial("R218.FLD",1.0);
            break;
        case chlorodifluoromethane:
            addMaterial("R22.FLD",1.0);
            break;
        case _1_1_1_2_3_3_3_heptafluoropropane:
            addMaterial("R227EA.FLD",1.0);
            break;
        case trifluoromethane:
            addMaterial("R23.FLD",1.0);
            break;
        case _1_1_1_2_3_3_hexafluoropropane:
            addMaterial("R236EA.FLD",1.0);
            break;
        case _1_1_1_3_3_3_hexafluoropropane:
            addMaterial("R236FA.FLD",1.0);
            break;
        case _1_1_2_2_3_pentafluoropropane:
            addMaterial("R245CA.FLD",1.0);
            break;
        case _1_1_1_3_3_pentafluoropropane:
            addMaterial("R245FA.FLD",1.0);
            break;
        case difluoromethane:
            addMaterial("R32.FLD",1.0);
            break;
        case _1_1_1_3_3_pentafluorobutane:
            addMaterial("R365MFC.FLD",1.0);
            break;
        case fluoromethane:
            addMaterial("R41.FLD",1.0);
            break;
        case octafluorocyclobutane:
            addMaterial("RC318.FLD",1.0);
            break;
        case sulfur_hexafluoride:
            addMaterial("SF6.FLD",1.0);
            break;
        case sulfur_dioxide:
            addMaterial("SO2.FLD",1.0);
            break;
        case trans_2_butene:
            addMaterial("T2BUTENE.FLD",1.0);
            break;
        case methylbenzene:
            addMaterial("TOLUENE.FLD",1.0);
            break;
        case water:
            addMaterial("WATER.FLD",1.0);
            break;
        case xenon:
            addMaterial("XENON.FLD",1.0);
            break;
        default:
            break;
        }
        useMaterial();
    }
    void setMaterial(QStringList files,std::vector<double> present)
    {
        this->material_file = files;
        this->x_present = present;
    }
    QStringList getMaterialFiles() const
    {
        return material_file;
    }
    std::vector<double> getMaterialPresent() const
    {
        return this->x_present;
    }
    ///
    /// \brief 在设置完物质配比后让设置的物质生效
    ///
    /// 使用之前先用addMaterial设置物质
    /// \see addMaterial
    ///
    bool useMaterial()
    {
        long i = this->x_present.size(),ierr = 0;

        /*
            inputs:
            nc--number of components (1 for pure fluid) [integer]
               if called with nc=-1, the version number*100 will be returned in ierr
            hfiles--array of file names specifying fluid/mixture components
               [character*255 variable] for each of the nc components;
               e.g., :fluids:r134a.fld (Mac) or fluids\r134a.fld (DOS) or
               [full_path]/fluids/r134a.fld (UNIX)
            hfmix--mixture coefficients [character*255]
               file name containing coefficients for mixture model,
               if applicable
               e.g.,  fluids\hmx.bnc
            hrf--reference state for thermodynamic calculations [character*3]
               'DEF':  default reference state as specified in fluid file
                       is applied to each pure component
               'NBP':  h,s = 0 at pure component normal boiling point(s)
               'ASH':  h,s = 0 for sat liquid at -40 C (ASHRAE convention)
               'IIR':  h = 200, s = 1.0 for sat liq at 0 C (IIR convention)
               other choices are possible, but these require a separate
               call to SETREF
            outputs:
            ierr--error flag:  0 = successful
                          101 = error in opening file
                          102 = error in file or premature end of file
                         -103 = unknown model encountered in file
                          104 = error in setup of model
                          105 = specified model not found
                          111 = error in opening mixture file
                          112 = mixture file of wrong type
                          114 = nc<>nc from setmod
                         -117 = binary pair not found, all parameters will be estimated
                          117 = No mixture data are available, mixture is outside the
                                range of the model and calculations will not be made
            herr--error string (character*255 variable if ierr<>0)
            [fluid parameters, etc. returned via various common blocks]

            explanation of parameters (used to dimension arrays)
            ncmax:    maximum number of mixture components
            n0:    lower bound on component arrays (-ncmax to accommodate
                   multiple ECS-thermo reference fluids)
            nx:    same as ncmax
            nrf0:    lower bound on arrays associated with transport props
                   (element 0 stores reference fluid information)

         */

        strcpy_s(hf,qPrintable(getMaterialFileString()));
        strcpy_s(hfmix,qPrintable(this->MIX_PATH));
        strcpy_s(hrf,"DEF");
        strcpy_s(herr,"Ok");
        qDebug() << QString("hf:") << qPrintable(hf);
        qDebug() << QString("hfmix:") << qPrintable(hfmix);
        try
        {
            if (nullptr == SETUPdll)
            {
                return false;
            }
            SETUPdll(i
                     ,hf
                     ,hfmix
                     ,hrf
                     ,ierr
                     ,herr
                     ,c_refprop_char_length*c_nc_max
                     ,c_refprop_char_length
                     ,c_length_of_reference
                     ,c_error_message_length);
        }
        catch (...)
        {
        }

        if (ierr != 0){
            m_strErr << QString("err SETUPdll:") + qPrintable(herr);
            qDebug() << QString("err SETUPdll:") << qPrintable(herr);
            return false;
        }
        /*
            subroutine INFO (icomp,wmm,ttrp,tnbpt,tc,pc,Dc,Zc,acf,dip,Rgas)

            provides fluid constants for specified component

          input:
            icomp--component number in mixture; 1 for pure fluid
          outputs:
            wmm--molecular weight [g/mol]//分子量
            ttrp--triple point temperature [K]//三相点温度
            tnbpt--normal boiling point temperature [K]正常沸点温度
            tc--critical temperature [K]临界温度
            pc--critical pressure [kPa]
            Dc--critical density [mol/L]
            Zc--compressibility at critical point [critical_pressure/(gas_constant*critical_temperature*critical_density)]在临界点的可压缩性
            acf--acentric factor [-]偏心因子
            dip--dipole moment [debye]偶极矩
            Rgas--gas constant [J/mol-K]气体常数
        */
        long info_index=1;
        INFOdll(info_index
                ,this->molecular_weight
                ,this->triple_point_temperature
                ,this->normal_boiling_point_temperature
                ,this->critical_temperature
                ,this->critical_pressure
                ,this->critical_density
                ,this->compressibility_at_critical_point
                ,this->acentric_factor
                ,this->dipole_moment
                ,this->gas_constant);
        qDebug()<<"INFOdll";
        WMOLdll(x_present.data(),molecular_weight);//计算分子质量
        qDebug()<<QStringLiteral(" 分子量:")<<getMolecularWeight()
               <<QStringLiteral(" 偏心因子:")<<getAcentricFactor()
              <<QStringLiteral(" 偶极矩:")<<getDipoleMoment()
             <<QStringLiteral(" 三相点温度:")<<getTriplePointTemperature()
            <<QStringLiteral(" 正常沸点温度:")<<getNormalBoilingPointTemperature();
        qDebug()
                <<QStringLiteral(" 临界温度:")<<getCriticalTemperature()
               <<QStringLiteral(" 临界压力:")<<getCriticalPressure()
              <<QStringLiteral(" 临界体积:")<<getCriticalDensity()
             <<QStringLiteral(" 气体常数:")<<getGasConstant()
            <<QStringLiteral(" 在临界点的可压缩性:")<<getCompressibilityAtCriticalPoint();
        return true;
    }
public:
    ///
    /// \brief  计算密度
    /// \param tep_K 温度[K]
    /// \param press_KPa 压力[KPa]
    /// \param ph 形态 - Liquid 液态 ； Vapor - 气态
    /// \return 密度 mol/L
    ///
    double density(double tep_K,double press_KPa,Phase ph=Vapor)
    {
        long tmp_int(0),ierr(0);
        long lp = ph;//If phase is known: (j=1: Liquid, j=2: Vapor)
        double d(0);
        /*
            subroutine TPRHO (t,p,x,kph,kguess,rho,ierr,herr)

            iterate for density as a function of temperature, pressure, and
            composition for a specified phase
           在已知物相的情况下，根据温度和压力关系迭代密度的值
            ***********************************************************************
            WARNING:
            Invalid densities will be returned for T & P outside range of validity,
            i.e., pressure > melting pressure, pressure less than saturation
            pressure for kph=1, etc.

            ***********************************************************************
            inputs:
                t--temperature [K]
                p--pressure [kPa]
                x--composition [array of mol frac]
              kph--phase flag:  1 = liquid
                                2 = vapor
                         N.B.:  0 = stable phase--NOT ALLOWED (use TPFLSH)
                                    (unless an initial guess is supplied for rho)
                               -1 = force the search in the liquid phase (for metastable points)
                               -2 = force the search in the vapor phase (for metastable points)
            kguess--input flag:  1 = first guess for rho provided
                                0 = no first guess provided
              rho--first guess for molar density [mol/L], only if kguess = 1

            outputs:
              rho--molar density [mol/L]
             ierr--error flag:  0 = successful
                              200 = CRITP did not converge
                              201 = illegal input (kph <= 0)
                              202 = liquid-phase iteration did not converge
                              203 = vapor-phase iteration did not converge
             herr--error string (character*255 variable if ierr<>0)
         */
        TPRHOdll(tep_K,press_KPa,x_present.data(),lp,tmp_int,d,ierr,herr,c_error_message_length);
        if (ierr != 0)
            m_strErr << (QString("density - TPRHOdll:") + herr);
        //此时计算的密度是mol/L,需要乘以相对分子质量
        return d;
    }
    ///
    /// \brief molar密度mol/L转换为标准质量密度m3/kg
    /// \param mol_per_L
    /// \return
    ///
    double molarDensityToSIDensity(const double& mol_per_L)
    {
        return (mol_per_L*(this->molecular_weight));
    }

    ///
    /// \brief 在已知温度、压力的情况下计算其他量，包括密度、焓、熵等压比热、等容比热、声速等
    ///
    /// 输出参数中out_vaporQualityOnAMolarBasis：vapor quality on a MOLAR basis [moles vapor/total moles] \n
    /// out_vaporQualityOnAMolarBasis < 0 indicates subcooled (compressed) liquid 表示过冷液体（压缩）\n
    /// out_vaporQualityOnAMolarBasis = 0 indicates saturated liquid表示饱和液体 \n
    /// out_vaporQualityOnAMolarBasis = 1 indicates saturated vapor表示的饱和蒸气 \n
    /// out_vaporQualityOnAMolarBasis > 1 indicates superheated vapor表示过热蒸汽 \n
    /// out_vaporQualityOnAMolarBasis = 998 superheated vapor, but quality not defined (in most situations, t > Tc)过热蒸汽，但质量不确定 \n
    /// out_vaporQualityOnAMolarBasis = 999 indicates supercritical state (t > Tc) and (p > Pc)表明超临界状态 \n
    /// \param tep_K 温度
    /// \param press_KPa 压力
    /// \param out_density_molPL 密度[mol/L]
    /// \param out_densityOfLiquid_molPL 液相摩尔密度[mol/L]所占比例
    /// \param out_densityOfVapor_molPL 气相摩尔密度[mol/L]所占比例
    /// \param out_compositionOfLiquidPhase 液相所占比例
    /// \param out_compositionOfVaporPhase 气相所占比例
    /// \param out_vaporQualityOnAMolarBasis vapor quality on a MOLAR basis
    /// \param out_internalEnergy 内能[J/mol]
    /// \param out_enthalpy 焓[J/mol]
    /// \param out_entropy 熵[J/mol-K]
    /// \param out_Cv [J/mol-K]等容比热容
    /// \param out_Cp [J/mol-K]等压比热容
    /// \param out_speedOfSound [m/s]声速
    /// \return 无错误发生返回true
    ///
    bool property_TP(double tep_K
                     ,double press_KPa
                     ,double& out_density_molPL
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_internalEnergy
                     ,double& out_enthalpy
                     ,double& out_entropy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);
        /*
         subroutine TPFLSH (t,p,z,D,Dl,Dv,x,y,q,e,h,s,cv,cp,w,ierr,herr)

          flash calculation given temperature, pressure, and bulk composition

          This routine accepts both single-phase and two-phase states as the
          input; if the phase is known, the subroutine TPRHO is faster.

          inputs:
                t--temperature [K]
                p--pressure [kPa]
                z--overall (bulk) composition [array of mol frac]

          outputs:
                D--overall (bulk) molar density [mol/L]
               Dl--molar density [mol/L] of the liquid phase
               Dv--molar density [mol/L] of the vapor phase
                   if only one phase is present, Dl = Dv = D
                x--composition of liquid phase [array of mol frac]
                y--composition of vapor phase [array of mol frac]
                   if only one phase is present, x = y = z
                q--vapor quality on a MOLAR basis [moles vapor/total moles]
                   q < 0 indicates subcooled (compressed) liquid 表示过冷液体（压缩）
                   q = 0 indicates saturated liquid表示饱和液体
                   q = 1 indicates saturated vapor表示的饱和蒸气
                   q > 1 indicates superheated vapor表示过热蒸汽
                   q = 998 superheated vapor, but quality not defined (in most situations, t > Tc)过热蒸汽，但质量不确定
                   q = 999 indicates supercritical state (t > Tc) and (p > Pc)表明超临界状态
                e--overall (bulk) internal energy [J/mol]内能
                h--overall (bulk) enthalpy [J/mol]焓
                s--overall (bulk) entropy [J/mol-K]熵
               Cv--isochoric (constant V) heat capacity [J/mol-K]等容比热容
               Cp--isobaric (constant p) heat capacity [J/mol-K]等压比热容
                w--speed of sound [m/s]声速
                   Cp, w are not defined for 2-phase states
                   in such cases, a flag = -9.99998d6 is returned
             ierr--error flag:  0 = successful
                                1 = Tin < Tmin
                                4 = Pin < 0
                                5 = T and P out of range
                                8 = x out of range (component and/or sum < 0
                                    or > 1)
                                9 = x and T out of range
                               12 = x out of range and P < 0
                               13 = x and T and P out of range
                              210 = CRITP did not converge
                              211 = SATT did not converge at bubble point
                              212 = SATT did not converge at dew point
                              213 = TPRHO did not converge for liquid state
                              214 = TPRHO did not converge for vapor state
                              215 = TPRHO did not convg for supercritical state
                              216 = TPRHO did not convg for liq in 2-phase it
                              217 = TPRHO did not convg for vap in 2-phase it
                              218 = liquid frac for 2-phase it did not converge
                              219 = composition for 2-phase it did not converge
             herr--error string (character*255 variable if ierr<>0)
        */
        TPFLSHdll(tep_K
                  ,press_KPa
                  ,x_present.data()
                  ,out_density_molPL
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_internalEnergy
                  ,out_enthalpy
                  ,out_entropy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_TP:") + herr);
            return false;
        }
        return true;
    }
    bool property_TD(double tep_K
                     ,double density_molPL
                     ,double& out_press_KPa
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_internalEnergy
                     ,double& out_enthalpy
                     ,double& out_entropy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);
        TDFLSHdll(tep_K
                  ,density_molPL
                  ,x_present.data()
                  ,out_press_KPa
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_internalEnergy
                  ,out_enthalpy
                  ,out_entropy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_TD:") + herr);
            return false;
        }
        return true;
    }
    bool property_TH(double tep_K
                     ,double enthalpy
                     ,double& out_press_KPa
                     ,double& out_density_molPL
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_internalEnergy
                     ,double& out_entropy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);
        /*
c  additional input--only for THFLSH, TSFLSH, and TEFLSH
c       kr--flag specifying desired root for multi-valued inputs:
c           1 = return lower density root
c           2 = return higher density root
        */
        long kr = 1;
        THFLSHdll(tep_K
                  ,enthalpy
                  ,x_present.data()
                  ,kr
                  ,out_press_KPa
                  ,out_density_molPL
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_internalEnergy
                  ,out_entropy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_TH:") + herr);
            return false;
        }
        return true;
    }
    bool property_TS(double tep_K
                     ,double entropy
                     ,double& out_press_KPa
                     ,double& out_density_molPL
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_internalEnergy
                     ,double& out_enthalpy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);
        /*
c  additional input--only for THFLSH, TSFLSH, and TEFLSH
c       kr--flag specifying desired root for multi-valued inputs:
c           1 = return lower density root
c           2 = return higher density root
        */
        long kr = 1;
        TSFLSHdll(tep_K
                  ,entropy
                  ,x_present.data()
                  ,kr
                  ,out_press_KPa
                  ,out_density_molPL
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_internalEnergy
                  ,out_enthalpy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_TS:") + herr);
            return false;
        }
        return true;
    }
    bool property_TE(double tep_K
                     ,double internalEnergy
                     ,double& out_press_KPa
                     ,double& out_density_molPL
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_enthalpy
                     ,double& out_entropy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);
        /*
c  additional input--only for THFLSH, TSFLSH, and TEFLSH
c       kr--flag specifying desired root for multi-valued inputs:
c           1 = return lower density root
c           2 = return higher density root
        */
        long kr = 1;
        TEFLSHdll(tep_K
                  ,internalEnergy
                  ,x_present.data()
                  ,kr
                  ,out_press_KPa
                  ,out_density_molPL
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_enthalpy
                  ,out_entropy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_TE:") + herr);
            return false;
        }
        return true;
    }
    bool property_PD(double press_KPa
                     ,double density_molPL
                     ,double& out_tep_K
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_internalEnergy
                     ,double& out_enthalpy
                     ,double& out_entropy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);

        PDFLSHdll(press_KPa
                  ,density_molPL
                  ,x_present.data()
                  ,out_tep_K
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_internalEnergy
                  ,out_enthalpy
                  ,out_entropy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_PD:") + herr);
            return false;
        }
        return true;
    }
    bool property_PH(double press_KPa
                     ,double enthalpy
                     ,double& out_tep_K
                     ,double& out_density_molPL
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_internalEnergy
                     ,double& out_entropy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);

        PHFLSHdll(press_KPa
                  ,enthalpy
                  ,x_present.data()
                  ,out_tep_K
                  ,out_density_molPL
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_internalEnergy
                  ,out_entropy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_PH:") + herr);
            return false;
        }
        return true;
    }
    bool property_PS(double press_KPa
                     ,double entropy
                     ,double& out_tep_K
                     ,double& out_density_molPL
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_internalEnergy
                     ,double& out_enthalpy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);
        try
        {
            PSFLSHdll(press_KPa
                      ,entropy
                      ,x_present.data()
                      ,out_tep_K
                      ,out_density_molPL
                      ,out_densityOfLiquid_molPL
                      ,out_densityOfVapor_molPL
                      ,out_compositionOfLiquidPhase.data()
                      ,out_compositionOfVaporPhase.data()
                      ,out_vaporQualityOnAMolarBasis
                      ,out_internalEnergy
                      ,out_enthalpy
                      ,out_Cv
                      ,out_Cp
                      ,out_speedOfSound
                      ,ierr,herr,c_error_message_length);
        }
        catch(...)
        {

        }
        if (ierr != 0)
        {
            m_strErr << (QString("property_PS:") + herr);
            return false;
        }
        return true;
    }
    bool property_PE(double press_KPa
                     ,double internalEnergy
                     ,double& out_tep_K
                     ,double& out_density_molPL
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_enthalpy
                     ,double& out_entropy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);

        PEFLSHdll(press_KPa
                  ,internalEnergy
                  ,x_present.data()
                  ,out_tep_K
                  ,out_density_molPL
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_enthalpy
                  ,out_entropy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_PE:") + herr);
            return false;
        }
        return true;
    }
    bool property_HS(double enthalpy
                     ,double entropy
                     ,double& out_tep_K
                     ,double& out_press_KPa
                     ,double& out_density_molPL
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_internalEnergy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);

        HSFLSHdll(enthalpy
                  ,entropy
                  ,x_present.data()
                  ,out_tep_K
                  ,out_press_KPa
                  ,out_density_molPL
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_internalEnergy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_HS:") + herr);
            return false;
        }
        return true;
    }
    bool property_ES(double internalEnergy
                     ,double entropy
                     ,double& out_tep_K
                     ,double& out_press_KPa
                     ,double& out_density_molPL
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_enthalpy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);

        ESFLSHdll(internalEnergy
                  ,entropy
                  ,x_present.data()
                  ,out_tep_K
                  ,out_press_KPa
                  ,out_density_molPL
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_enthalpy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_ES:") + herr);
            return false;
        }
        return true;
    }
    bool property_DH(double density_molPL
                     ,double enthalpy
                     ,double& out_tep_K
                     ,double& out_press_KPa
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_internalEnergy
                     ,double& out_entropy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);

        DHFLSHdll(density_molPL
                  ,enthalpy
                  ,x_present.data()
                  ,out_tep_K
                  ,out_press_KPa
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_internalEnergy
                  ,out_entropy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_DH:") + herr);
            return false;
        }
        return true;
    }
    bool property_DS(double density_molPL
                     ,double entropy
                     ,double& out_tep_K
                     ,double& out_press_KPa
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_internalEnergy
                     ,double& out_enthalpy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);

        DSFLSHdll(density_molPL
                  ,entropy
                  ,x_present.data()
                  ,out_tep_K
                  ,out_press_KPa
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_internalEnergy
                  ,out_enthalpy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_DH:") + herr);
            return false;
        }
        return true;
    }
    bool property_DE(double density_molPL
                     ,double internalEnergy
                     ,double& out_tep_K
                     ,double& out_press_KPa
                     ,double& out_densityOfLiquid_molPL
                     ,double& out_densityOfVapor_molPL
                     ,std::vector<double>& out_compositionOfLiquidPhase
                     ,std::vector<double>& out_compositionOfVaporPhase
                     ,double& out_vaporQualityOnAMolarBasis
                     ,double& out_enthalpy
                     ,double& out_entropy
                     ,double& out_Cv
                     ,double& out_Cp
                     ,double& out_speedOfSound)
    {
        long ierr;

        out_compositionOfLiquidPhase.resize(c_nc_max,0);
        out_compositionOfVaporPhase.resize(c_nc_max,0);

        DHFLSHdll(density_molPL
                  ,internalEnergy
                  ,x_present.data()
                  ,out_tep_K
                  ,out_press_KPa
                  ,out_densityOfLiquid_molPL
                  ,out_densityOfVapor_molPL
                  ,out_compositionOfLiquidPhase.data()
                  ,out_compositionOfVaporPhase.data()
                  ,out_vaporQualityOnAMolarBasis
                  ,out_enthalpy
                  ,out_entropy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,ierr,herr,c_error_message_length);
        if (ierr != 0)
        {
            m_strErr << (QString("property_DE:") + herr);
            return false;
        }
        return true;
    }
    ///
    /// \brief getZ 获取压缩系数
    /// \param press_kPa 压力 单位kPa
    /// \param density_molPL 密度 单位mol/L
    /// \param temp_K 温度 单位K
    /// \return
    ///
    double getZ(double press_kPa,double density_molPL,double temp_K) const
    {
        return press_kPa/(getGasConstant() * temp_K * density_molPL);
    }

    ///
    /// \brief 根据压力温度求熵
    ///
    /// 这是已知物质状态情况下的，如果不知道物质状态，使用函数property类别函数，即可得到包扩熵焓等变量的值
    /// \param tep_K 温度 K
    /// \param press_KPa 压力 KPa
    /// \param ph 状态
    /// \return 熵[J/mol-K]
    ///
    double entropy(double tep_K,double press_KPa,Phase ph=Vapor)
    {
        double mol_p_l = density(tep_K,press_KPa,ph);
        /*
        subroutine ENTRO (t,rho,x,s)

        compute entropy as a function of temperature, density and composition
        using core functions (temperature derivative of Helmholtz free energy
        and ideal gas integrals)

        based on derivations in Younglove & McLinden, JPCRD 23 #5, 1994,
        equations A5, A19 - A26

        inputs:
           t--temperature [K]
         rho--molar density [mol/L]
           x--composition [array of mol frac]
        output:
           s--entropy [J/mol-K]
        */
        double e;
        ENTROdll(tep_K,mol_p_l,this->x_present.data(),e);
        return e;
    }

    ///
    /// \brief 根据压力温度求焓
    ///
    /// 这是已知物质状态情况下的，如果不知道物质状态，使用函数property类别函数，即可得到包扩熵焓等变量的值
    /// \param tep_K 温度 K
    /// \param press_KPa 压力 KPa
    /// \param ph 状态
    /// \return 焓 [J/mol]
    ///
    double enthalpy(double tep_K,double press_KPa,Phase ph=Vapor)
    {
        double mol_p_l = density(tep_K,press_KPa,ph);
        /*
        compute enthalpy as a function of temperature, density, and
        composition using core functions (Helmholtz free energy and ideal
        gas integrals)

        based on derivations in Younglove & McLinden, JPCRD 23 #5, 1994,
        equations A7, A18, A19

        inputs:
              t--temperature [K]
            rho--molar density [mol/L]
              x--composition [array of mol frac]
        output:
              h--enthalpy [J/mol]
        */
        double e;
        ENTHALdll(tep_K,mol_p_l,this->x_present.data(),e);
        return e;
    }

    ///
    /// \brief 计算比热
    /// \param tep_K 温度 K
    /// \param press_KPa 压力 KPa
    /// \param cp 程序输出，等压比热
    /// \param cv 程序输出，等容比热
    /// \param ph 状态
    ///
    void cp_cv(double tep_K,double press_KPa,double& cp,double& cv,Phase ph=Vapor)
    {
        double mol_p_l = density(tep_K,press_KPa,ph);
        /*
         subroutine CVCP (t,rho,x,cv,cp)
          compute isochoric (constant volume) and isobaric (constant pressure)
          heat capacity as functions of temperature, density, and composition
          using core functions

          based on derivations in Younglove & McLinden, JPCRD 23 #5, 1994,
          equation A15, A16

          inputs:
                t--temperature [K]
              rho--molar density [mol/L]
                x--composition [array of mol frac]
          outputs:
               cv--isochoric heat capacity [J/mol-K]
               cp--isobaric heat capacity [J/mol-K]
         */

        CVCPdll(tep_K,mol_p_l,this->x_present.data(),cv,cp);

    }

    ///
    /// \brief viscosity_and_thermalConductivity 计算粘度和导热性
    ///
    /// compute the transport properties of thermal conductivity
    /// and viscosity as functions of temperature, density, and composition
    /// \param t_k 温度 [K]
    /// \param density_molpL 密度 [mol/L]
    /// \param out_viscosity_uPas 粘度eta--viscosity (uPa.s)
    /// \param out_thermalConductivity_WperMK 导热性thermal conductivity (W/m.K)
    ///
    bool viscosity_and_thermalConductivity(double t_k,double density_molpL
                                           ,double& out_viscosity_uPas
                                           ,double& out_thermalConductivity_WperMK)
    {
        long ierr(0);
        /*
        subroutine TRNPRP (t,rho,x,eta,tcx,ierr,herr)

        compute the transport properties of thermal conductivity and
        viscosity as functions of temperature, density, and composition

        inputs:
            t--temperature [K]
          rho--molar density [mol/L]
            x--composition array [mol frac]
        outputs:
          eta--viscosity (uPa.s)
          tcx--thermal conductivity (W/m.K)
         ierr--error flag:  0 = successful
                          -31 = temperature out of range for conductivity
                          -32 = density out of range for conductivity
                          -33 = T and D out of range for conductivity
                          -41 = temperature out of range for viscosity
                          -42 = density out of range for viscosity
                          -43 = T and D out of range for viscosity
                          -51 = T out of range for both visc and t.c.
                          -52 = D out of range for both visc and t.c.
                          -53 = T and/or D out of range for visc and t.c.
                           39 = model not found for thermal conductivity
                           40 = model not found for thermal conductivity or viscosity
                           49 = model not found for viscosity
                           50 = ammonia/water mixture (no properties calculated)
                           51 = exactly at t, rhoc for a pure fluid; k is infinite
                            -58,-59 = ECS model did not converge
         herr--error string (character*255 variable if ierr<>0)
        */
        TRNPRPdll (t_k,density_molpL,x_present.data()
                   ,out_viscosity_uPas
                   ,out_thermalConductivity_WperMK,ierr,herr,c_error_message_length);
        if (ierr != 0){
            m_strErr << (QString("TRNPRPdll:") + herr);
            return false;
        }
        return true;
    }
    /*
    void thermal_quantities(double tep_K
                            ,double density_molPL
                            ,double& out_press_KPa
                            ,double& out_internalEnergy
                            ,double& out_enthalpy
                            ,double& out_entropy
                            ,double& out_Cv
                            ,double& out_Cp
                            ,double& out_speedOfSound
                            ,double& out_Z
                            ,double& out_isenthalpicJouleThompsonCoefficient_kPkPa
                            ,double& out_HelmholtzEnergy_JpMol
                            ,double& out_GibbsFreeEnergy_JpMol
                            ,double& out_IsothermalCompressibility_1pKPa
                            ,double& out_volumeExpansivity_1pK
                            ,double& out_dPdrho
                            ,double& out_d2PdD2
                            ,double& out_dPT
                            ,double& out_drhodT
                            ,double& out_drhodP
                            ,double& out_d2PT2
                            ,double& out_d2PdTD
                            ,double& out_sparei
                            )
    {

//      subroutine THERM2 (t,rho,x,p,e,h,s,cv,cp,w,Z,hjt,A,G,
//     &                   xkappa,beta,dPdrho,d2PdD2,dPT,drhodT,drhodP,
//     &                   d2PT2,d2PdTD,spare3,spare4)
//c
//c  compute thermal quantities as a function of temperature, density,
//c  and compositions using core functions (Helmholtz free energy, ideal
//c  gas heat capacity and various derivatives and integrals)
//c
//c  this routine is the same as THERM, except that additional properties
//c  are calculated
//c
//c  inputs:
//c        t--temperature [K]
//c      rho--molar density [mol/L]
//c        x--composition [array of mol frac]
//c  outputs:
//c        p--pressure [kPa]
//c        e--internal energy [J/mol]
//c        h--enthalpy [J/mol]
//c        s--entropy [J/mol-K]
//c       Cv--isochoric heat capacity [J/mol-K]
//c       Cp--isobaric heat capacity [J/mol-K]
//c        w--speed of sound [m/s]
//c        Z--compressibility factor (= PV/RT) [dimensionless]
//c      hjt--isenthalpic Joule-Thompson coefficient [K/kPa]
//c        A--Helmholtz energy [J/mol]
//c        G--Gibbs free energy [J/mol]
//c   xkappa--isothermal compressibility (= -1/V dV/dP = 1/rho dD/dP) [1/kPa]
//c     beta--volume expansivity (= 1/V dV/dT = -1/rho dD/dT) [1/K]
//c   dPdrho--derivative dP/drho [kPa-L/mol]
//c   d2PdD2--derivative d^2P/drho^2 [kPa-L^2/mol^2]
//c      dPT--derivative dP/dT [kPa/K]
//c   drhodT--derivative drho/dT [mol/(L-K)]
//c   drhodP--derivative drho/dP [mol/(L-kPa)]
//c    d2PT2--derivative d2P/dT2 [kPa/K^2]
//c   d2PdTD--derivative d2P/dTd(rho) [J/mol-K]
//c   sparei--2 space holders for possible future properties

        long ierr(0);
        THERM2dll(tep_K,density_molPL,x_present.data()
                  ,out_press_KPa
                  ,out_internalEnergy
                  ,out_enthalpy
                  ,out_entropy
                  ,out_Cv
                  ,out_Cp
                  ,out_speedOfSound
                  ,out_Z
                  ,out_isenthalpicJouleThompsonCoefficient_kPkPa
                  ,out_HelmholtzEnergy_JpMol
                  ,out_GibbsFreeEnergy_JpMol
                  ,out_IsothermalCompressibility_1pKPa
                  ,out_volumeExpansivity_1pK
                  ,out_dPdrho
                  ,out_d2PdD2
                  ,out_dPT
                  ,out_drhodT
                  ,out_drhodP
                  ,out_d2PT2
                  ,out_d2PdTD
                  ,out_sparei
                  );
    }
*/
public:
    //    template<typename T>
    //    static T* get_vector_ptr(std::vector<T>& vec)
    //    {
    //        return &vec[0];
    //    }
private:
    ///
    /// \brief 获取物质字符串，用于SETUPdll函数的第二个参数
    /// \return 物质文件字符串
    ///
    QString getMaterialFileString(){
        QString fileString;
        size_t length = this->material_file.size();
        for(size_t i=0;i<length;++i){
            if(0 == i){
                fileString = (this->FLD_PATH + this->material_file.at(i));
            }
            else{
                fileString += ("|"+this->FLD_PATH + this->material_file.at(i));
            }
        }
        return fileString;
    }
    void getFunPtr()
    {
        m_failFunName.clear();

        ABFL1dll = (fp_ABFL1dllTYPE) m_lib.resolve("ABFL1dll");
        if (!ABFL1dll)
            m_failFunName<<"ABFL1dll";

        ABFL2dll = (fp_ABFL2dllTYPE) m_lib.resolve("ABFL2dll");
        if (!ABFL2dll)
            m_failFunName<<"ABFL2dll";

        ACTVYdll = (fp_ACTVYdllTYPE) m_lib.resolve("ACTVYdll");
        if (!ACTVYdll)
            m_failFunName<<"ACTVYdll";

        AGdll = (fp_AGdllTYPE) m_lib.resolve("AGdll");
        if (!AGdll)
            m_failFunName<<"AGdll";

        CCRITdll = (fp_CCRITdllTYPE) m_lib.resolve("CCRITdll");
        if (!CCRITdll)
            m_failFunName<<"CCRITdll";

        CP0dll = (fp_CP0dllTYPE) m_lib.resolve("CP0dll");
        if (!CP0dll)
            m_failFunName<<"CP0dll";

        CRITPdll= (fp_CRITPdllTYPE) m_lib.resolve("CRITPdll");
        if (!CRITPdll)
            m_failFunName<<"CRITPdll";

        CSATKdll= (fp_CSATKdllTYPE) m_lib.resolve("CSATKdll");
        if (!CSATKdll)
            m_failFunName<<"CSATKdll";

        CV2PKdll = (fp_CV2PKdllTYPE) m_lib.resolve("CV2PKdll");
        if (!CV2PKdll)
            m_failFunName<<"CV2PKdll";

        CVCPKdll = (fp_CVCPKdllTYPE) m_lib.resolve("CVCPKdll");
        if (!CVCPKdll)
            m_failFunName<<"CVCPKdll";

        CVCPdll = (fp_CVCPdllTYPE) m_lib.resolve("CVCPdll");
        if (!CVCPdll)
            m_failFunName<<"CVCPdll";

        DBDTdll= (fp_DBDTdllTYPE) m_lib.resolve("DBDTdll");
        if (!DBDTdll)
            m_failFunName<<"DBDTdll";

        DBFL1dll= (fp_DBFL1dllTYPE) m_lib.resolve("DBFL1dll");
        if (!DBFL1dll)
            m_failFunName<<"DBFL1dll";

        DBFL2dll= (fp_DBFL2dllTYPE) m_lib.resolve("DBFL2dll");
        if (!DBFL2dll)
            m_failFunName<<"DBFL2dll";

        DDDPdll = (fp_DDDPdllTYPE) m_lib.resolve("DDDPdll");
        if (!DDDPdll)
            m_failFunName<<"DDDPdll";

        DDDTdll = (fp_DDDTdllTYPE) m_lib.resolve("DDDTdll");
        if (!DDDTdll)
            m_failFunName<<"DDDTdll";

        DEFLSHdll = (fp_DEFLSHdllTYPE) m_lib.resolve("DEFLSHdll");
        if (!DDDTdll)
            m_failFunName<<"DDDTdll";

        DHD1dll = (fp_DHD1dllTYPE) m_lib.resolve("DHD1dll");
        if (!DHD1dll)
            m_failFunName<<"DHD1dll";

        DHFLSHdll = (fp_DHFLSHdllTYPE) m_lib.resolve("DHFLSHdll");
        if (!DHFLSHdll)
            m_failFunName<<"DHFLSHdll";

        DIELECdll = (fp_DIELECdllTYPE) m_lib.resolve("DIELECdll");
        if (!DIELECdll)
            m_failFunName<<"DIELECdll";

        DOTFILLdll = (fp_DOTFILLdllTYPE) m_lib.resolve("DOTFILLdll");
        if (!DOTFILLdll)
            m_failFunName<<"DOTFILLdll";

        DPDD2dll = (fp_DPDD2dllTYPE) m_lib.resolve("DPDD2dll");
        if (!DPDD2dll)
            m_failFunName<<"DPDD2dll";

        DPDDKdll = (fp_DPDDKdllTYPE) m_lib.resolve("DPDDKdll");
        if (!DPDDKdll)
            m_failFunName<<"DPDDKdll";

        DPDDdll = (fp_DPDDdllTYPE) m_lib.resolve("DPDDdll");
        if (!DPDDdll)
            m_failFunName<<"DPDDdll";

        DPDTKdll = (fp_DPDTKdllTYPE) m_lib.resolve("DPDTKdll");
        if (!DPDTKdll)
            m_failFunName<<"DPDTKdll";

        DPDTdll = (fp_DPDTdllTYPE) m_lib.resolve("DPDTdll");
        if (!DPDTdll)
            m_failFunName<<"DPDTdll";

        DPTSATKdll = (fp_DPTSATKdllTYPE) m_lib.resolve("DPTSATKdll");
        if (!DPTSATKdll)
            m_failFunName<<"DPTSATKdll";

        DSFLSHdll = (fp_DSFLSHdllTYPE) m_lib.resolve("DSFLSHdll");
        if (!DSFLSHdll)
            m_failFunName<<"DSFLSHdll";

        ENTHALdll = (fp_ENTHALdllTYPE) m_lib.resolve("ENTHALdll");
        if (!ENTHALdll)
            m_failFunName<<"ENTHALdll";

        ENTROdll = (fp_ENTROdllTYPE) m_lib.resolve("ENTROdll");
        if (!ENTROdll)
            m_failFunName<<"ENTROdll";

        ESFLSHdll = (fp_ESFLSHdllTYPE) m_lib.resolve("ESFLSHdll");
        if (!ESFLSHdll)
            m_failFunName<<"ESFLSHdll";

        FGCTYdll = (fp_FGCTYdllTYPE) m_lib.resolve("FGCTYdll");
        if (!FGCTYdll)
            m_failFunName<<"FGCTYdll";

        FPVdll = (fp_FPVdllTYPE) m_lib.resolve("FPVdll");
        if (!FPVdll)
            m_failFunName<<"FPVdll";

        GERG04dll = (fp_GERG04dllTYPE) m_lib.resolve("GERG04dll");
        if (!GERG04dll)
            m_failFunName<<"GERG04dll";

        GETFIJdll = (fp_GETFIJdllTYPE) m_lib.resolve("GETFIJdll");
        if (!GETFIJdll)
            m_failFunName<<"GETFIJdll";

        GETKTVdll= (fp_GETKTVdllTYPE) m_lib.resolve("GETKTVdll");
        if (!GETKTVdll)
            m_failFunName<<"GETKTVdll";

        GIBBSdll = (fp_GIBBSdllTYPE) m_lib.resolve("GIBBSdll");
        if (!GIBBSdll)
            m_failFunName<<"GIBBSdll";

        HSFLSHdll = (fp_HSFLSHdllTYPE) m_lib.resolve("HSFLSHdll");
        if (!HSFLSHdll)
            m_failFunName<<"HSFLSHdll";

        INFOdll = (fp_INFOdllTYPE) m_lib.resolve("INFOdll");
        if (!INFOdll)
            m_failFunName<<"INFOdll";

        LIMITKdll = (fp_LIMITKdllTYPE) m_lib.resolve("LIMITKdll");
        if (!LIMITKdll)
            m_failFunName<<"LIMITKdll";

        LIMITSdll = (fp_LIMITSdllTYPE) m_lib.resolve("LIMITSdll");
        if (!LIMITSdll)
            m_failFunName<<"LIMITSdll";

        LIMITXdll = (fp_LIMITXdllTYPE) m_lib.resolve("LIMITXdll");
        if (!LIMITXdll)
            m_failFunName<<"LIMITXdll";

        MELTPdll = (fp_MELTPdllTYPE) m_lib.resolve("MELTPdll");
        if (!MELTPdll)
            m_failFunName<<"MELTPdll";

        MELTTdll = (fp_MELTTdllTYPE) m_lib.resolve("MELTTdll");
        if (!MELTTdll)
            m_failFunName<<"MELTTdll";

        MLTH2Odll = (fp_MLTH2OdllTYPE) m_lib.resolve("MLTH2Odll");
        if (!MLTH2Odll)
            m_failFunName<<"MLTH2Odll";

        NAMEdll = (fp_NAMEdllTYPE) m_lib.resolve("NAMEdll");
        if (!NAMEdll)
            m_failFunName<<"NAMEdll";

        PDFL1dll = (fp_PDFL1dllTYPE) m_lib.resolve("PDFL1dll");
        if (!PDFL1dll)
            m_failFunName<<"PDFL1dll";

        PDFLSHdll = (fp_PDFLSHdllTYPE) m_lib.resolve("PDFLSHdll");
        if (!PDFLSHdll)
            m_failFunName<<"PDFLSHdll";

        PEFLSHdll = (fp_PEFLSHdllTYPE) m_lib.resolve("PEFLSHdll");
        if (!PDFLSHdll)
            m_failFunName<<"PDFLSHdll";

        PHFL1dll = (fp_PHFL1dllTYPE) m_lib.resolve("PHFL1dll");
        if (!PHFL1dll)
            m_failFunName<<"PHFL1dll";

        PHFLSHdll = (fp_PHFLSHdllTYPE) m_lib.resolve("PHFLSHdll");
        if (!PHFLSHdll)
            m_failFunName<<"PHFLSHdll";

        PQFLSHdll = (fp_PQFLSHdllTYPE) m_lib.resolve("PQFLSHdll");
        if (!PQFLSHdll)
            m_failFunName<<"PQFLSHdll";

        PREOSdll = (fp_PREOSdllTYPE) m_lib.resolve("PREOSdll");
        if (!PREOSdll)
            m_failFunName<<"PREOSdll";

        PRESSdll = (fp_PRESSdllTYPE) m_lib.resolve("PRESSdll");
        if (!PRESSdll)
            m_failFunName<<"PRESSdll";

        PSFL1dll = (fp_PSFL1dllTYPE) m_lib.resolve("PSFL1dll");
        if (!PSFL1dll)
            m_failFunName<<"PSFL1dll";

        PSFLSHdll = (fp_PSFLSHdllTYPE) m_lib.resolve("PSFLSHdll");
        if (!PSFLSHdll)
            m_failFunName<<"PSFLSHdll";

        PUREFLDdll = (fp_PUREFLDdllTYPE) m_lib.resolve("PUREFLDdll");
        if (!PUREFLDdll)
            m_failFunName<<"PUREFLDdll";

        QMASSdll = (fp_QMASSdllTYPE) m_lib.resolve("QMASSdll");
        if (!QMASSdll)
            m_failFunName<<"QMASSdll";

        QMOLEdll = (fp_QMOLEdllTYPE) m_lib.resolve("QMOLEdll");
        if (!QMOLEdll)
            m_failFunName<<"QMOLEdll";

        SATDdll = (fp_SATDdllTYPE) m_lib.resolve("SATDdll");
        if (!SATDdll)
            m_failFunName<<"SATDdll";

        SATEdll = (fp_SATEdllTYPE) m_lib.resolve("SATEdll");
        if (!SATEdll)
            m_failFunName<<"SATEdll";

        SATHdll = (fp_SATHdllTYPE) m_lib.resolve("SATHdll");
        if (!SATHdll)
            m_failFunName<<"SATHdll";

        SATPdll = (fp_SATPdllTYPE) m_lib.resolve("SATPdll");
        if (!SATPdll)
            m_failFunName<<"SATPdll";

        SATSdll = (fp_SATSdllTYPE) m_lib.resolve("SATSdll");
        if (!SATSdll)
            m_failFunName<<"SATSdll";

        SATTdll = (fp_SATTdllTYPE) m_lib.resolve("SATTdll");
        if (!SATTdll)
            m_failFunName<<"SATTdll";

        SETAGAdll = (fp_SETAGAdllTYPE) m_lib.resolve("SETAGAdll");
        if (!SETAGAdll)
            m_failFunName<<"SETAGAdll";

        SETKTVdll = (fp_SETKTVdllTYPE) m_lib.resolve("SETKTVdll");
        if (!SETKTVdll)
            m_failFunName<<"SETKTVdll";

        SETMIXdll = (fp_SETMIXdllTYPE) m_lib.resolve("SETMIXdll");
        if (!SETMIXdll)
            m_failFunName<<"SETMIXdll";

        SETMODdll = (fp_SETMODdllTYPE) m_lib.resolve("SETMODdll");
        if (!SETMODdll)
            m_failFunName<<"SETMODdll";

        SETREFdll = (fp_SETREFdllTYPE) m_lib.resolve("SETREFdll");
        if (!SETREFdll)
            m_failFunName<<"SETREFdll";

        SETUPdll = (fp_SETUPdllTYPE) m_lib.resolve("SETUPdll");
        if (!SETUPdll)
            m_failFunName<<"SETUPdll";

        //        SPECGRdll = (fp_SPECGRdllTYPE) m_lib.resolve("SPECGRdll");
        //        if (!SPECGRdll)
        //            m_failFunName<<"SPECGRdll";

        SUBLPdll = (fp_SUBLPdllTYPE) m_lib.resolve("SUBLPdll");
        if (!SUBLPdll)
            m_failFunName<<"SUBLPdll";

        SUBLTdll = (fp_SUBLTdllTYPE) m_lib.resolve("SUBLTdll");
        if (!SUBLTdll)
            m_failFunName<<"SUBLTdll";

        SURFTdll = (fp_SURFTdllTYPE) m_lib.resolve("SURFTdll");
        if (!SURFTdll)
            m_failFunName<<"SURFTdll";

        SURTENdll = (fp_SURTENdllTYPE) m_lib.resolve("SURTENdll");
        if (!SURTENdll)
            m_failFunName<<"SURTENdll";

        TDFLSHdll = (fp_TDFLSHdllTYPE) m_lib.resolve("TDFLSHdll");
        if (!TDFLSHdll)
            m_failFunName<<"TDFLSHdll";

        TEFLSHdll= (fp_TEFLSHdllTYPE) m_lib.resolve("TEFLSHdll");
        if (!TEFLSHdll)
            m_failFunName<<"TEFLSHdll";

        THERM0dll= (fp_THERM0dllTYPE) m_lib.resolve("THERM0dll");
        if (!THERM0dll)
            m_failFunName<<"THERM0dll";

        THERM2dll= (fp_THERM2dllTYPE) m_lib.resolve("THERM2dll");
        if (!THERM2dll)
            m_failFunName<<"THERM2dll";

        THERM3dll= (fp_THERM3dllTYPE) m_lib.resolve("THERM3dll");
        if (!THERM3dll)
            m_failFunName<<"THERM3dll";

        THERMdll= (fp_THERMdllTYPE) m_lib.resolve("THERMdll");
        if (!THERMdll)
            m_failFunName<<"THERMdll";

        THFLSHdll= (fp_THFLSHdllTYPE) m_lib.resolve("THFLSHdll");
        if (!THFLSHdll)
            m_failFunName<<"THFLSHdll";

        TPFLSHdll= (fp_TPFLSHdllTYPE) m_lib.resolve("TPFLSHdll");
        if (!TPFLSHdll)
            m_failFunName<<"TPFLSHdll";

        TPRHOdll= (fp_TPRHOdllTYPE) m_lib.resolve("TPRHOdll");
        if (!TPRHOdll)
            m_failFunName<<"TPRHOdll";

        TQFLSHdll= (fp_TQFLSHdllTYPE) m_lib.resolve("TQFLSHdll");
        if (!TQFLSHdll)
            m_failFunName<<"TQFLSHdll";

        TRNPRPdll = (fp_TRNPRPdllTYPE) m_lib.resolve("TRNPRPdll");
        if (!TRNPRPdll)
            m_failFunName<<"TRNPRPdll";

        TSFLSHdll= (fp_TSFLSHdllTYPE) m_lib.resolve("TSFLSHdll");
        if (!TSFLSHdll)
            m_failFunName<<"TSFLSHdll";

        VIRBdll= (fp_VIRBdllTYPE) m_lib.resolve("VIRBdll");
        if (!VIRBdll)
            m_failFunName<<"VIRBdll";

        VIRCdll= (fp_VIRCdllTYPE) m_lib.resolve("VIRCdll");
        if (!VIRCdll)
            m_failFunName<<"VIRCdll";

        WMOLdll= (fp_WMOLdllTYPE) m_lib.resolve("WMOLdll");
        if (!WMOLdll)
            m_failFunName<<"WMOLdll";

        XMASSdll= (fp_XMASSdllTYPE) m_lib.resolve("XMASSdll");
        if (!XMASSdll)
            m_failFunName<<"XMASSdll";
        XMOLEdll= (fp_XMOLEdllTYPE) m_lib.resolve("XMOLEdll");
        if (!XMOLEdll)
            m_failFunName<<"XMOLEdll";

        if(m_failFunName.size() > 0)
            qDebug()<<QStringLiteral("以下函数未能加载成功")<<m_failFunName;
    }

private:
    //dll的函数指针定义
    typedef void (__stdcall *fp_ABFL1dllTYPE)(double &,double &,double *,long &,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_ABFL2dllTYPE)(double &,double &,double *,long &,long &,double &,double &,double &,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double *,double *,double &,long &,char*,long );
    typedef void (__stdcall *fp_ACTVYdllTYPE)(double &,double &,double *,double &);
    typedef void (__stdcall *fp_AGdllTYPE)(double &,double &,double *,double &,double &);
    typedef void (__stdcall *fp_CCRITdllTYPE)(double &,double &,double &,double *,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_CP0dllTYPE)(double &,double *,double &);
    typedef void (__stdcall *fp_CRITPdllTYPE)(double *,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_CSATKdllTYPE)(long &,double &,long &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_CV2PKdllTYPE)(long &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_CVCPKdllTYPE)(long &,double &,double &,double &,double &);
    typedef void (__stdcall *fp_CVCPdllTYPE)(double &,double &,double *,double &,double &);
    typedef void (__stdcall *fp_DBDTdllTYPE)(double &,double *,double &);
    typedef void (__stdcall *fp_DBFL1dllTYPE)(double &,double &,double *,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_DBFL2dllTYPE)(double &,double &,double *,long &,double &,double &,double &,double &,double &,double *,double *,double &,long &,char*,long );
    typedef void (__stdcall *fp_DDDPdllTYPE)(double &,double &,double *,double &);
    typedef void (__stdcall *fp_DDDTdllTYPE)(double &,double &,double *,double &);
    typedef void (__stdcall *fp_DEFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_DHD1dllTYPE)(double &,double &,double *,double &,double &,double &,double &,double &,double &);
    typedef void (__stdcall *fp_DHFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_DIELECdllTYPE)(double &,double &,double *,double &);
    typedef void (__stdcall *fp_DOTFILLdllTYPE)(long &,double *,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_DPDD2dllTYPE)(double &,double &,double *,double &);
    typedef void (__stdcall *fp_DPDDKdllTYPE)(long &,double &,double &,double &);
    typedef void (__stdcall *fp_DPDDdllTYPE)(double &,double &,double *,double &);
    typedef void (__stdcall *fp_DPDTKdllTYPE)(long &,double &,double &,double &);
    typedef void (__stdcall *fp_DPDTdllTYPE)(double &,double &,double *,double &);
    typedef void (__stdcall *fp_DPTSATKdllTYPE)(long &,double &,long &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_DSFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_ENTHALdllTYPE)(double &,double &,double *,double &);
    typedef void (__stdcall *fp_ENTROdllTYPE)(double &,double &,double *,double &);
    typedef void (__stdcall *fp_ESFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_FGCTYdllTYPE)(double &,double &,double *,double *);
    typedef void (__stdcall *fp_FPVdllTYPE)(double &,double &,double &,double *,double &);
    typedef void (__stdcall *fp_GERG04dllTYPE)(long &,long &,long &,char*,long );
    typedef void (__stdcall *fp_GETFIJdllTYPE)(char*,double *,char*,char*,long ,long ,long );
    typedef void (__stdcall *fp_GETKTVdllTYPE)(long &,long &,char*,double *,char*,char*,char*,char*,long ,long ,long ,long ,long );
    typedef void (__stdcall *fp_GIBBSdllTYPE)(double &,double &,double *,double &,double &);
    typedef void (__stdcall *fp_HSFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_INFOdllTYPE)(long &,double &,double &,double &,double &,double &,double &,double &,double &,double &,double &);
    typedef void (__stdcall *fp_LIMITKdllTYPE)(char*,long &,double &,double &,double &,double &,double &,double &,double &,long &,char*,long ,long );
    typedef void (__stdcall *fp_LIMITSdllTYPE)(char*,double *,double &,double &,double &,double &,long );
    typedef void (__stdcall *fp_LIMITXdllTYPE)(char*,double &,double &,double &,double *,double &,double &,double &,double &,long &,char*,long ,long );
    typedef void (__stdcall *fp_MELTPdllTYPE)(double &,double *,double &,long &,char*,long );
    typedef void (__stdcall *fp_MELTTdllTYPE)(double &,double *,double &,long &,char*,long );
    typedef void (__stdcall *fp_MLTH2OdllTYPE)(double &,double &,double &);
    typedef void (__stdcall *fp_NAMEdllTYPE)(long &,char*,char*,char*,long ,long ,long );
    typedef void (__stdcall *fp_PDFL1dllTYPE)(double &,double &,double *,double &,long &,char*,long );
    typedef void (__stdcall *fp_PDFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_PEFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_PHFL1dllTYPE)(double &,double &,double *,long &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_PHFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_PQFLSHdllTYPE)(double &,double &,double *,long &,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_PREOSdllTYPE)(long &);
    typedef void (__stdcall *fp_PRESSdllTYPE)(double &,double &,double *,double &);
    typedef void (__stdcall *fp_PSFL1dllTYPE)(double &,double &,double *,long &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_PSFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_PUREFLDdllTYPE)(long &);
    typedef void (__stdcall *fp_QMASSdllTYPE)(double &,double *,double *,double &,double *,double *,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_QMOLEdllTYPE)(double &,double *,double *,double &,double *,double *,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_SATDdllTYPE)(double &,double *,long &,long &,double &,double &,double &,double &,double *,double *,long &,char*,long );
    typedef void (__stdcall *fp_SATEdllTYPE)(double &,double *,long &,long &,long &,double &,double &,double &,long &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_SATHdllTYPE)(double &,double *,long &,long &,long &,double &,double &,double &,long &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_SATPdllTYPE)(double &,double *,long &,double &,double &,double &,double *,double *,long &,char*,long );
    typedef void (__stdcall *fp_SATSdllTYPE)(double &,double *,long &,long &,long &,double &,double &,double &,long &,double &,double &,double &,long &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_SATTdllTYPE)(double &,double *,long &,double &,double &,double &,double *,double *,long &,char*,long );
    typedef void (__stdcall *fp_SETAGAdllTYPE)(long &,char*,long );
    typedef void (__stdcall *fp_SETKTVdllTYPE)(long &,long &,char*,double *,char*,long &,char*,long ,long ,long );
    typedef void (__stdcall *fp_SETMIXdllTYPE)(char*,char*,char*,long &,char*,double *,long &,char*,long ,long ,long ,long ,long );
    typedef void (__stdcall *fp_SETMODdllTYPE)(long &,char*,char*,char*,long &,char*,long ,long ,long ,long );
    typedef void (__stdcall *fp_SETREFdllTYPE)(char*,long &,double *,double &,double &,double &,double &,long &,char*,long ,long );
    typedef void (__stdcall *fp_SETUPdllTYPE)(long &,char*,char*,char*,long &,char*,long ,long ,long ,long );
    typedef void (__stdcall *fp_SPECGRdllTYPE)(double &,double &,double &,double &);
    typedef void (__stdcall *fp_SUBLPdllTYPE)(double &,double *,double &,long &,char*,long );
    typedef void (__stdcall *fp_SUBLTdllTYPE)(double &,double *,double &,long &,char*,long );
    typedef void (__stdcall *fp_SURFTdllTYPE)(double &,double &,double *,double &,long &,char*,long );
    typedef void (__stdcall *fp_SURTENdllTYPE)(double &,double &,double &,double *,double *,double &,long &,char*,long );
    typedef void (__stdcall *fp_TDFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_TEFLSHdllTYPE)(double &,double &,double *,long &,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_THERM0dllTYPE)(double &,double &,double *,double &,double &,double &,double &,double &,double &,double &,double &,double &);
    typedef void (__stdcall *fp_THERM2dllTYPE)(double &,double &,double *,double &,double &,double &,double &,double &,double &,double &,double *,double &,double &,double &,double &,double &,double &,double &,double &,double &,double &,double &,double &,double &,double &);
    typedef void (__stdcall *fp_THERM3dllTYPE)(double &,double &,double *,double &,double &,double &,double &,double &,double &,double &,double &,double &,double &);
    typedef void (__stdcall *fp_THERMdllTYPE)(double &,double &,double *,double &,double &,double &,double &,double &,double &,double &,double &);
    typedef void (__stdcall *fp_THFLSHdllTYPE)(double &,double &,double *,long &,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_TPFLSHdllTYPE)(double &,double &,double *,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_TPRHOdllTYPE)(double &,double &,double *,long &,long &,double &,long &,char*,long );
    typedef void (__stdcall *fp_TQFLSHdllTYPE)(double &,double &,double *,long &,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_TRNPRPdllTYPE)(double &,double &,double *,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_TSFLSHdllTYPE)(double &,double &,double *,long &,double &,double &,double &,double &,double *,double *,double &,double &,double &,double &,double &,double &,long &,char*,long );
    typedef void (__stdcall *fp_VIRBdllTYPE)(double &,double *,double &);
    typedef void (__stdcall *fp_VIRCdllTYPE)(double &,double *,double &);
    typedef void (__stdcall *fp_WMOLdllTYPE)(double *,double &);
    typedef void (__stdcall *fp_XMASSdllTYPE)(double *,double *,double &);
    typedef void (__stdcall *fp_XMOLEdllTYPE)(double *,double *,double &);

    //Define explicit function pointers
    fp_ABFL1dllTYPE ABFL1dll;
    fp_ABFL2dllTYPE ABFL2dll;
    fp_ACTVYdllTYPE ACTVYdll;
    fp_AGdllTYPE AGdll;
    fp_CCRITdllTYPE CCRITdll;
    fp_CP0dllTYPE CP0dll;
    fp_CRITPdllTYPE CRITPdll;
    fp_CSATKdllTYPE CSATKdll;
    fp_CV2PKdllTYPE CV2PKdll;
    fp_CVCPKdllTYPE CVCPKdll;
    fp_CVCPdllTYPE CVCPdll;
    fp_DBDTdllTYPE DBDTdll;
    fp_DBFL1dllTYPE DBFL1dll;
    fp_DBFL2dllTYPE DBFL2dll;
    fp_DDDPdllTYPE DDDPdll;
    fp_DDDTdllTYPE DDDTdll;
    fp_DEFLSHdllTYPE DEFLSHdll;
    fp_DHD1dllTYPE DHD1dll;
    fp_DHFLSHdllTYPE DHFLSHdll;
    fp_DIELECdllTYPE DIELECdll;
    fp_DOTFILLdllTYPE DOTFILLdll;
    fp_DPDD2dllTYPE DPDD2dll;
    fp_DPDDKdllTYPE DPDDKdll;
    fp_DPDDdllTYPE DPDDdll;
    fp_DPDTKdllTYPE DPDTKdll;
    fp_DPDTdllTYPE DPDTdll;
    fp_DPTSATKdllTYPE DPTSATKdll;
    fp_DSFLSHdllTYPE DSFLSHdll;
    fp_ENTHALdllTYPE ENTHALdll;
    fp_ENTROdllTYPE ENTROdll;
    fp_ESFLSHdllTYPE ESFLSHdll;
    fp_FGCTYdllTYPE FGCTYdll;
    fp_FPVdllTYPE FPVdll;
    fp_GERG04dllTYPE GERG04dll;
    fp_GETFIJdllTYPE GETFIJdll;
    fp_GETKTVdllTYPE GETKTVdll;
    fp_GIBBSdllTYPE GIBBSdll;
    fp_HSFLSHdllTYPE HSFLSHdll;
    fp_INFOdllTYPE INFOdll;
    fp_LIMITKdllTYPE LIMITKdll;
    fp_LIMITSdllTYPE LIMITSdll;
    fp_LIMITXdllTYPE LIMITXdll;
    fp_MELTPdllTYPE MELTPdll;
    fp_MELTTdllTYPE MELTTdll;
    fp_MLTH2OdllTYPE MLTH2Odll;
    fp_NAMEdllTYPE NAMEdll;
    fp_PDFL1dllTYPE PDFL1dll;
    fp_PDFLSHdllTYPE PDFLSHdll;
    fp_PEFLSHdllTYPE PEFLSHdll;
    fp_PHFL1dllTYPE PHFL1dll;
    fp_PHFLSHdllTYPE PHFLSHdll;
    fp_PQFLSHdllTYPE PQFLSHdll;
    fp_PREOSdllTYPE PREOSdll;
    fp_PRESSdllTYPE PRESSdll;
    fp_PSFL1dllTYPE PSFL1dll;
    fp_PSFLSHdllTYPE PSFLSHdll;
    fp_PUREFLDdllTYPE PUREFLDdll;
    fp_QMASSdllTYPE QMASSdll;
    fp_QMOLEdllTYPE QMOLEdll;
    fp_SATDdllTYPE SATDdll;
    fp_SATEdllTYPE SATEdll;
    fp_SATHdllTYPE SATHdll;
    fp_SATPdllTYPE SATPdll;
    fp_SATSdllTYPE SATSdll;
    fp_SATTdllTYPE SATTdll;
    fp_SETAGAdllTYPE SETAGAdll;
    fp_SETKTVdllTYPE SETKTVdll;
    fp_SETMIXdllTYPE SETMIXdll;
    fp_SETMODdllTYPE SETMODdll;
    fp_SETREFdllTYPE SETREFdll;
    fp_SETUPdllTYPE SETUPdll;
    fp_SPECGRdllTYPE SPECGRdll;
    fp_SUBLPdllTYPE SUBLPdll;
    fp_SUBLTdllTYPE SUBLTdll;
    fp_SURFTdllTYPE SURFTdll;
    fp_SURTENdllTYPE SURTENdll;
    fp_TDFLSHdllTYPE TDFLSHdll;
    fp_TEFLSHdllTYPE TEFLSHdll;
    fp_THERM0dllTYPE THERM0dll;
    fp_THERM2dllTYPE THERM2dll;
    fp_THERM3dllTYPE THERM3dll;
    fp_THERMdllTYPE THERMdll;
    fp_THFLSHdllTYPE THFLSHdll;
    fp_TPFLSHdllTYPE TPFLSHdll;
    fp_TPRHOdllTYPE TPRHOdll;
    fp_TQFLSHdllTYPE TQFLSHdll;
    fp_TRNPRPdllTYPE TRNPRPdll;
    fp_TSFLSHdllTYPE TSFLSHdll;
    fp_VIRBdllTYPE VIRBdll;
    fp_VIRCdllTYPE VIRCdll;
    fp_WMOLdllTYPE WMOLdll;
    fp_XMASSdllTYPE XMASSdll;
    fp_XMOLEdllTYPE XMOLEdll;



    QLibrary m_lib;
    QStringList m_failFunName;
    QString FLD_PATH;
    QString MIX_PATH;


    char hf[c_refprop_char_length*c_nc_max], hrf[c_length_of_reference+1],
    herr[c_error_message_length+1],hfmix[c_refprop_char_length+1];



    std::vector<double> x_present;
    QStringList material_file;
    QStringList m_strErr;
private:

    double molecular_weight;///< 相对分子质量
    double triple_point_temperature;///<三相点温度
    double normal_boiling_point_temperature;///<正常沸点温度
    double critical_temperature;///<气液临界温度
    double critical_pressure;///<临界压力
    double critical_density;///<临界摩尔体积
    double compressibility_at_critical_point;///<临界点压缩因子
    double acentric_factor;///<偏心因子
    double dipole_moment;///<偶极矩
    double gas_constant;///<气体常数
public:
    ///
    /// \brief 获取分子质量[g/mol]
    /// \return 分子质量
    ///
    double getMolecularWeight() const{return this->molecular_weight;}
    ///
    /// \brief 获取三相点温度[K]
    /// \return 三相点温度
    ///
    double getTriplePointTemperature() const{return this->triple_point_temperature;}
    ///
    /// \brief 获取正常沸点温度[K]
    /// \return 正常沸点温度
    ///
    double getNormalBoilingPointTemperature() const{return this->normal_boiling_point_temperature;}
    ///
    /// \brief 获取临界温度[K]
    ///
    /// 使物质由气态变为液态的最高温度叫临界温度。每种物质都有一个特定的温度，在这个温度以上，无论怎样增大压强，气态物质都不会液化，这个温度就是临界温度
    /// \return 获取临界温度
    ///
    double getCriticalTemperature() const{return this->critical_temperature;}
    ///
    /// \brief 获取临界压力[kPa]
    ///
    /// 物质处于临界状态时的压力（压强）。就是在临界温度时使气体液化所需要的最小压力。也就是液体在临界温度时的饱和蒸气压。
    /// 在临界温度和临界压力下，物质的摩尔体积称为临界摩尔体积。临界温度和临界压力下的状态称为临界状态,与临界温度对应的液化压力叫临界压力
    /// \return 获取临界温度
    ///
    double getCriticalPressure() const{return this->critical_pressure;}
    ///
    /// \brief 临界摩尔体积[mol/L]
    ///
    /// 物质处于临界状态时的压力（压强）。就是在临界温度时使气体液化所需要的最小压力。也就是液体在临界温度时的饱和蒸气压。
    /// 在临界温度和临界压力下，物质的摩尔体积称为临界摩尔体积。
    /// \return 临界摩尔体积
    ///
    double getCriticalDensity() const{return this->critical_density;}
    ///
    /// \brief 临界点压缩因子[critical_pressure/(gas_constant*critical_temperature*critical_density)]
    ///
    /// 当实际气体处于临界点此时的压缩因子称为临界压缩因子ZC. 多数气体的临界压缩因子比较接近，0.25-0.31之间
    /// \return 临界点压缩因子
    ///
    double getCompressibilityAtCriticalPoint() const{return this->compressibility_at_critical_point;}
    ///
    /// \brief 偏心因子
    ///
    /// 两个分子间的相互作用力偏离分子中心的程度。对于氩、氪、氙等分子结构为球形对称的惰性气体，两者无偏离，偏心因子值为零；分子结构越复杂或极性越大，两者的偏离越甚，偏心因子值越正。
    /// 可用以关联一些与分子间力有关的性质。例如，气体的压缩因子或逸度系数可表示为对比压力、对比温度和偏心因子的函数，由此得到的压缩因子或逸度系数可使气体的P-V-T关系计算或热力学函数计算获得更精确的结果。
    /// 偏心因子是反映物质分子形状、极性和大小的参数。对于小的球行分子如氩其偏心因子 等于0，偏心因子可表征特定物质对比蒸汽压与简单球形分子之间的偏差。当分子中碳数相同时，烷烃的偏心因子较大，环烷烃和芳烃的较小。
    /// \return 偏心因子
    ///
    double getAcentricFactor() const{return this->acentric_factor;}
    ///
    /// \brief 偶极矩 [debye]
    ///
    /// 正、负电荷中心间的距离r和电荷中心所带电量q的乘积，叫做偶极矩μ=r×q。它是一个矢量，方向规定为从正电中心指向负电中心。偶极矩的单位是D（德拜）。
    /// 根据讨论的对象不同，偶极矩可以指键偶极矩，也可以是分子偶极矩。分子偶极矩可由键偶极矩经矢量加法后得到。实验测得的偶极矩可以用来判断分子的空间构型
    /// \return
    ///
    double getDipoleMoment() const{return this->dipole_moment;}
    ///
    /// \brief 气体常数[J/mol-K]
    /// \return 气体常数
    ///
    double getGasConstant() const{return this->gas_constant;}
};

#endif // EASYREFPROP_H
