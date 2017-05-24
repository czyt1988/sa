#ifndef REFPROPMATERIALDATA_H
#define REFPROPMATERIALDATA_H

//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2014, 中科院工程热物理研究所  （版权声明）
///    All rights reserved.
///
/// \file RefpropMaterialData.h
/// \brief 对应EasyREFPROP枚举变量的翻译和对Qt控件的集成
///
/// 对应EasyREFPROP枚举变量的中文翻译和对Qt控件的集成
///
/// \version 1.0
/// \author 陈宗衍
/// \date 2014-04-02
///
//////////////////////////////////////////////////////////////////////////

#include <EasyREFPROP.h>
#include <QComboBox>
#include <QStandardItemModel>
#include <QMap>
#include <QList>
#include <QRegExp>
#include <QStringRef>
class RefpropMaterialData
{
    class priv_refpropMaterialData{
    public:
        priv_refpropMaterialData(){}
//         priv_refpropMaterialData(const QString& full_name
//                        ,const QString& short_name
//                        ,const QString& chemical_formula
//                        ,const QString& chinese_name):
//             m_fullName(full_name),m_shortName(short_name),m_chemical_formula(chemical_formula),m_chinese(chinese_name)
//         {
// 			m_type = NOT_DEFINE;
//         }
		priv_refpropMaterialData(const QString& full_name
			,const QString& short_name
			,const QString& chemical_formula
			,const QString& chinese_name
			,const int type):
		m_fullName(full_name),m_shortName(short_name),m_chemical_formula(chemical_formula),m_chinese(chinese_name)
			,m_type(type)
		{}
        QString fullName() const{return m_fullName;}
        QString shortName() const{return m_shortName;}
        QString chemicalFormula() const{return m_chemical_formula;}
        QString chinese() const{return m_chinese;}
		int type() const{return m_type;}
		enum{
			NOT_DEFINE = 0x1
			,PURE = 0x2
			,MIX = 0x4
			,REFRIGERATING = 0xC//其必定是混合物	
		};
		bool operator == (const priv_refpropMaterialData& a) const
		{
			return (a.shortName() == this->shortName() 
				&& a.fullName() == this->fullName() 
				&& a.chemicalFormula() == this->chemicalFormula()
				&& a.chinese() == this->chinese());
		}
    private:
        QString m_fullName;
        QString m_shortName;
        QString m_chinese;
        QString m_chemical_formula;
		int m_type;///< 标定物质的性质
    };

    QMap<EasyREFPROP::Material,priv_refpropMaterialData> materials;
public:
	enum{
		NOT_DEFINE = 0x1
		,PURE = 0x2
		,MIX = 0x4
		,REFRIGERATING = 0xC//其必定是混合物	
		,ALL = 0xFFFF
	};
    RefpropMaterialData(){
		addMaterialDataInfo(EasyREFPROP::air                                    ,QStringLiteral("air"),QStringLiteral("air"),QStringLiteral("air"),QStringLiteral("空气"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_butene                              ,QStringLiteral("1-butene"),QStringLiteral("butene"),QStringLiteral("CH3-CH2-CH=CH2"),QStringLiteral("丁烯"),MIX);
		addMaterialDataInfo(EasyREFPROP::propanone                              ,QStringLiteral("propanone"),QStringLiteral("acetone"),QStringLiteral("(CH3)2CO"),QStringLiteral("丙酮"),MIX);
		addMaterialDataInfo(EasyREFPROP::ammonia                                ,QStringLiteral("ammonia"),QStringLiteral("ammonia"),QStringLiteral("NH3"),QStringLiteral("氨"),MIX);
		addMaterialDataInfo(EasyREFPROP::argon                                  ,QStringLiteral("argon"),QStringLiteral("argon"),QStringLiteral("Ar"),QStringLiteral("氩"),PURE);
		addMaterialDataInfo(EasyREFPROP::benzene                                ,QStringLiteral("benzene"),QStringLiteral("benzene"),QStringLiteral("C6H6"),QStringLiteral("苯"),MIX);
		addMaterialDataInfo(EasyREFPROP::n_butane                               ,QStringLiteral("n-butane"),QStringLiteral("butane"),QStringLiteral("CH3-2(CH2)-CH3"),QStringLiteral("正丁烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::dodecane                               ,QStringLiteral("dodecane"),QStringLiteral("dodecane"),QStringLiteral("CH3-10(CH2)-CH3"),QStringLiteral("十二烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::methylcyclohexane                      ,QStringLiteral("methylcyclohexane"),QStringLiteral("methylcyclohexane"),QStringLiteral("C6H11(CH3)"),QStringLiteral("甲基环己烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::cis_2_butene                           ,QStringLiteral("cis-2-butene"),QStringLiteral("cis-butene"),QStringLiteral("CH3-CH=CH-CH3"),QStringLiteral("顺式-2-丁烯"),MIX);
		addMaterialDataInfo(EasyREFPROP::n_propylcyclohexane                    ,QStringLiteral("n-propylcyclohexane"),QStringLiteral("propylcyclohexane"),QStringLiteral("(C6H11)CH2CH2CH3"),QStringLiteral("正丙基环己烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::decafluorobutane                       ,QStringLiteral("decafluorobutane"),QStringLiteral("perfluorobutane"),QStringLiteral("C4F10"),QStringLiteral("十氟丁烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::dodecafluoropentane                    ,QStringLiteral("dodecafluoropentane"),QStringLiteral("perfluoropentane"),QStringLiteral("C5F12"),QStringLiteral("全氟正戊烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::trifluoroiodomethane                   ,QStringLiteral("trifluoroiodomethane"),QStringLiteral("trifluoroiodomethane"),QStringLiteral("CF3I"),QStringLiteral("三氟碘甲烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::carbon_monoxide                        ,QStringLiteral("carbon monoxide"),QStringLiteral("carbon monoxide"),QStringLiteral("CO"),QStringLiteral("一氧化碳"),MIX);
		addMaterialDataInfo(EasyREFPROP::carbon_dioxide                         ,QStringLiteral("carbon dioxide"),QStringLiteral("carbon dioxide"),QStringLiteral("CO2"),QStringLiteral("二氧化碳"),MIX);
		addMaterialDataInfo(EasyREFPROP::carbon_oxide_sulfide                   ,QStringLiteral("carbon oxide sulfide"),QStringLiteral("carbonyl sulfide"),QStringLiteral("COS"),QStringLiteral("羰基硫"),MIX);
		addMaterialDataInfo(EasyREFPROP::cyclohexane                            ,QStringLiteral("cyclohexane"),QStringLiteral("cyclohexane"),QStringLiteral("cyclo-C6H12"),QStringLiteral("环己烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::cyclopentane                           ,QStringLiteral("cyclopentane"),QStringLiteral("cyclopentane"),QStringLiteral("C5H10"),QStringLiteral("环戊烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::cyclopropane                           ,QStringLiteral("cyclopropane"),QStringLiteral("cyclopropane"),QStringLiteral("cyclo-C3H6"),QStringLiteral("环丙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::deuterium                              ,QStringLiteral("deuterium"),QStringLiteral("deuterium"),QStringLiteral("D2"),QStringLiteral("氘"),PURE);
		addMaterialDataInfo(EasyREFPROP::deuterium_oxide                        ,QStringLiteral("deuterium oxide"),QStringLiteral("heavy water"),QStringLiteral("D2O"),QStringLiteral("氧化氘"),MIX);
		addMaterialDataInfo(EasyREFPROP::octamethylcyclotetrasiloxane           ,QStringLiteral("octamethylcyclotetrasiloxane"),QStringLiteral("D4"),QStringLiteral("C8H24O4Si4"),QStringLiteral("八甲环四硅氧烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::decamethylcyclopentasiloxane           ,QStringLiteral("decamethylcyclopentasiloxane"),QStringLiteral("D5"),QStringLiteral("C10H30O5Si5"),QStringLiteral("十甲基环戊硅氧烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::dodecamethylcyclohexasiloxane          ,QStringLiteral("dodecamethylcyclohexasiloxane"),QStringLiteral("D6"),QStringLiteral("C12H36Si6O6"),QStringLiteral("十二甲基环己硅氧烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::decane                                 ,QStringLiteral("decane"),QStringLiteral("decane"),QStringLiteral("CH3-8(CH2)-CH3"),QStringLiteral("正十烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::dimethyl_ester_carbonic_acid           ,QStringLiteral("dimethyl ester carbonic acid"),QStringLiteral("dimethyl carbonate"),QStringLiteral("C3H6O3"),QStringLiteral("碳酸二甲酯"),MIX);
		addMaterialDataInfo(EasyREFPROP::methoxymethane                         ,QStringLiteral("methoxymethane"),QStringLiteral("dimethylether"),QStringLiteral("(CH3)2O"),QStringLiteral("甲醚"),MIX);
		addMaterialDataInfo(EasyREFPROP::ethane                                 ,QStringLiteral("ethane"),QStringLiteral("ethane"),QStringLiteral("CH3CH3"),QStringLiteral("乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::ethyl_alcohol                          ,QStringLiteral("ethyl alcohol"),QStringLiteral("ethanol"),QStringLiteral("C2H6O"),QStringLiteral("酒精"),MIX);
		addMaterialDataInfo(EasyREFPROP::ethene                                 ,QStringLiteral("ethene"),QStringLiteral("ethylene"),QStringLiteral("CH2=CH2"),QStringLiteral("乙烯"),MIX);
		addMaterialDataInfo(EasyREFPROP::fluorine                               ,QStringLiteral("fluorine"),QStringLiteral("fluorine"),QStringLiteral("F2"),QStringLiteral("氟"),PURE);
		addMaterialDataInfo(EasyREFPROP::hydrogen_sulfide                       ,QStringLiteral("hydrogen sulfide"),QStringLiteral("hydrogen sulfide"),QStringLiteral("H2S"),QStringLiteral("氢化硫"),MIX);
		addMaterialDataInfo(EasyREFPROP::helium_4                               ,QStringLiteral("helium-4"),QStringLiteral("helium"),QStringLiteral("He"),QStringLiteral("氦"),PURE);
		addMaterialDataInfo(EasyREFPROP::heptane                                ,QStringLiteral("heptane"),QStringLiteral("heptane"),QStringLiteral("CH3-5(CH2)-CH3"),QStringLiteral("庚烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::hexane                                 ,QStringLiteral("hexane"),QStringLiteral("hexane"),QStringLiteral("CH3-4(CH2)-CH3"),QStringLiteral("己烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::hydrogen_normal                        ,QStringLiteral("hydrogen (normal)"),QStringLiteral("hydrogen (normal)"),QStringLiteral("H2"),QStringLiteral("氢"),PURE);
		addMaterialDataInfo(EasyREFPROP::_2_methyl_1_propene                    ,QStringLiteral("2-methyl-1-propene"),QStringLiteral("isobutene"),QStringLiteral("CH2=C(CH3)2"),QStringLiteral("2甲基1丙烯"),MIX);
		addMaterialDataInfo(EasyREFPROP::_2_methylpentane                       ,QStringLiteral("2-methylpentane"),QStringLiteral("isohexane"),QStringLiteral("(CH3)2CH(CH2)2CH3"),QStringLiteral("异己烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_2_methylbutane                        ,QStringLiteral("2-methylbutane"),QStringLiteral("isopentane"),QStringLiteral("(CH3)2CHCH2CH3"),QStringLiteral("异戊烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_2_methylpropane                       ,QStringLiteral("2-methylpropane"),QStringLiteral("isobutane"),QStringLiteral("CH(CH3)3"),QStringLiteral("异丁烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::krypton                                ,QStringLiteral("krypton"),QStringLiteral("krypton"),QStringLiteral("Kr"),QStringLiteral("氪"),PURE);
		addMaterialDataInfo(EasyREFPROP::decamethyltetrasiloxane                ,QStringLiteral("decamethyltetrasiloxane"),QStringLiteral("MD2M"),QStringLiteral("C10H30Si4O3"),QStringLiteral("十甲基甲硅氧烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::dodecamethylpentasiloxane              ,QStringLiteral("dodecamethylpentasiloxane"),QStringLiteral("MD3M"),QStringLiteral("C12H36Si5O4"),QStringLiteral("十二甲基戊硅氧烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::tetradecamethylhexasiloxane            ,QStringLiteral("tetradecamethylhexasiloxane"),QStringLiteral("MD4M"),QStringLiteral("C14H42O5Si6"),QStringLiteral("十四甲基六硅氧烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::octamethyltrisiloxane                  ,QStringLiteral("octamethyltrisiloxane"),QStringLiteral("MDM"),QStringLiteral("C8H24O2Si3"),QStringLiteral("八甲三硅氧烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::methane                                ,QStringLiteral("methane"),QStringLiteral("methane"),QStringLiteral("CH4"),QStringLiteral("甲烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::methanol                               ,QStringLiteral("methanol"),QStringLiteral("methanol"),QStringLiteral("CH3OH"),QStringLiteral("甲醇"),MIX);
		addMaterialDataInfo(EasyREFPROP::methyl_Z_Z_9_12_octadecadienoate       ,QStringLiteral("methyl (Z,Z)-9,12-octadecadienoate"),QStringLiteral("methyl linoleate"),QStringLiteral("C19H34O2"),QStringLiteral("亚油酸甲酸"),MIX);
		addMaterialDataInfo(EasyREFPROP::methyl_Z_Z_Z_9_12_15_octadecatrienoate ,QStringLiteral("methyl (Z,Z,Z)-9,12,15-octadecatrienoate"),QStringLiteral("methyl linolenate"),QStringLiteral("C19H32O2"),QStringLiteral("亚麻酸甲酯"),MIX);
		addMaterialDataInfo(EasyREFPROP::hexamethyldisiloxane                   ,QStringLiteral("hexamethyldisiloxane"),QStringLiteral("MM"),QStringLiteral("C6H18OSi2"),QStringLiteral("六甲基二硅醚"),MIX);
		addMaterialDataInfo(EasyREFPROP::methyl_cis_9_octadecenoate             ,QStringLiteral("methyl cis-9-octadecenoate"),QStringLiteral("methyl oleate"),QStringLiteral("C19H36O2"),QStringLiteral("油酸甲酯"),MIX);
		addMaterialDataInfo(EasyREFPROP::methyl_hexadecanoate                   ,QStringLiteral("methyl hexadecanoate"),QStringLiteral("methyl palmitate"),QStringLiteral("C17H34O2"),QStringLiteral("棕榈酸甲酯"),MIX);
		addMaterialDataInfo(EasyREFPROP::methyl_octadecanoate                   ,QStringLiteral("methyl octadecanoate"),QStringLiteral("methyl stearate"),QStringLiteral("C19H38O2"),QStringLiteral("硬脂酸甲酯"),MIX);
		addMaterialDataInfo(EasyREFPROP::dinitrogen_monoxide                    ,QStringLiteral("dinitrogen monoxide"),QStringLiteral("nitrous oxide"),QStringLiteral("N2O"),QStringLiteral("一氧化二氮"),MIX);
		addMaterialDataInfo(EasyREFPROP::neon                                   ,QStringLiteral("neon"),QStringLiteral("neon"),QStringLiteral("Ne"),QStringLiteral("氖"),PURE);
		addMaterialDataInfo(EasyREFPROP::_2_2_dimethylpropane                   ,QStringLiteral("2,2-dimethylpropane"),QStringLiteral("neopentane"),QStringLiteral("C(CH3)4"),QStringLiteral("新戊烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::nitrogen_trifluoride                   ,QStringLiteral("nitrogen trifluoride"),QStringLiteral("nitrogen trifluoride"),QStringLiteral("NF3"),QStringLiteral("三氟化氮"),MIX);
		addMaterialDataInfo(EasyREFPROP::nitrogen                               ,QStringLiteral("nitrogen"),QStringLiteral("nitrogen"),QStringLiteral("N2"),QStringLiteral("氮"),PURE);
		addMaterialDataInfo(EasyREFPROP::nonane                                 ,QStringLiteral("nonane"),QStringLiteral("nonane"),QStringLiteral("CH3-7(CH2)-CH3"),QStringLiteral("壬烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::octane                                 ,QStringLiteral("octane"),QStringLiteral("octane"),QStringLiteral("CH3-6(CH2)-CH3"),QStringLiteral("辛烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::orthohydrogen                          ,QStringLiteral("orthohydrogen"),QStringLiteral("orthohydrogen"),QStringLiteral("H2"),QStringLiteral("正氢"),PURE);
		addMaterialDataInfo(EasyREFPROP::oxygen                                 ,QStringLiteral("oxygen"),QStringLiteral("oxygen"),QStringLiteral("O2"),QStringLiteral("氧"),PURE);
		addMaterialDataInfo(EasyREFPROP::parahydrogen                           ,QStringLiteral("parahydrogen"),QStringLiteral("parahydrogen"),QStringLiteral("H2"),QStringLiteral("仲氢"),PURE);
		addMaterialDataInfo(EasyREFPROP::pentane                                ,QStringLiteral("pentane"),QStringLiteral("pentane"),QStringLiteral("CH3-3(CH2)-CH3"),QStringLiteral("戊烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::propane                                ,QStringLiteral("propane"),QStringLiteral("propane"),QStringLiteral("CH3CH2CH3"),QStringLiteral("丙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::propene                                ,QStringLiteral("propene"),QStringLiteral("propylene"),QStringLiteral("CH2=CH-CH3"),QStringLiteral("丙烯"),MIX);
		addMaterialDataInfo(EasyREFPROP::propyne                                ,QStringLiteral("propyne"),QStringLiteral("propyne"),QStringLiteral("CH3CCH"),QStringLiteral("丙炔"),MIX);
		addMaterialDataInfo(EasyREFPROP::trichlorofluoromethane                 ,QStringLiteral("trichlorofluoromethane"),QStringLiteral("R11"),QStringLiteral("CCl3F"),QStringLiteral("三氯氟甲烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_2_trichloro_1_2_2_trifluoroethane ,QStringLiteral("1,1,2-trichloro-1,2,2-trifluoroethane"),QStringLiteral("R113"),QStringLiteral("CCl2FCClF2"),QStringLiteral("三氯三氟乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_2_dichloro_1_1_2_2_tetrafluoroethane,QStringLiteral("1,2-dichloro-1,1,2,2-tetrafluoroethane"),QStringLiteral("R114"),QStringLiteral("CClF2CClF2"),QStringLiteral("二氯四氟乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::chloropentafluoroethane                ,QStringLiteral("chloropentafluoroethane"),QStringLiteral("R115"),QStringLiteral("CClF2CF3"),QStringLiteral("一氯五氟乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::hexafluoroethane                       ,QStringLiteral("hexafluoroethane"),QStringLiteral("R116"),QStringLiteral("CF3CF3"),QStringLiteral("六氟乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::dichlorodifluoromethane                ,QStringLiteral("dichlorodifluoromethane"),QStringLiteral("R12"),QStringLiteral("CCl2F2"),QStringLiteral("二氯二氟甲烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_2_2_dichloro_1_1_1_trifluoroethane    ,QStringLiteral("2,2-dichloro-1,1,1-trifluoroethane"),QStringLiteral("R123"),QStringLiteral("CHCl2CF3"),QStringLiteral("三氟二氯乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_2_3_3_3_tetrafluoroprop_1_ene         ,QStringLiteral("2,3,3,3-tetrafluoroprop-1-ene"),QStringLiteral("R1234yf"),QStringLiteral("CF3CF=CH2"),QStringLiteral("R1234yf"),MIX);
		addMaterialDataInfo(EasyREFPROP::trans_1_3_3_3_tetrafluoropropene       ,QStringLiteral("trans-1,3,3,3-tetrafluoropropene"),QStringLiteral("R1234ze"),QStringLiteral("CHF=CHCF3 (trans)"),QStringLiteral("R1234ze"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_chloro_1_2_2_2_tetrafluoroethane    ,QStringLiteral("1-chloro-1,2,2,2-tetrafluoroethane"),QStringLiteral("R124"),QStringLiteral("CHClFCF3"),QStringLiteral("四氟一氯乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::pentafluoroethane                      ,QStringLiteral("pentafluoroethane"),QStringLiteral("R125"),QStringLiteral("CHF2CF3"),QStringLiteral("五氟乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::chlorotrifluoromethane                 ,QStringLiteral("chlorotrifluoromethane"),QStringLiteral("R13"),QStringLiteral("CClF3"),QStringLiteral("氯三氟甲烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_1_2_tetrafluoroethane             ,QStringLiteral("1,1,1,2-tetrafluoroethane"),QStringLiteral("R134a"),QStringLiteral("CF3CH2F"),QStringLiteral("四氟乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::tetrafluoromethane                     ,QStringLiteral("tetrafluoromethane"),QStringLiteral("R14"),QStringLiteral("CF4"),QStringLiteral("四氟化碳"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_dichloro_1_fluoroethane           ,QStringLiteral("1,1-dichloro-1-fluoroethane"),QStringLiteral("R141b"),QStringLiteral("CCl2FCH3"),QStringLiteral("一氟二氯乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_chloro_1_1_difluoroethane           ,QStringLiteral("1-chloro-1,1-difluoroethane"),QStringLiteral("R142b"),QStringLiteral("CClF2CH3"),QStringLiteral("二氟氯乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_1_trifluoroethane                 ,QStringLiteral("1,1,1-trifluoroethane"),QStringLiteral("R143a"),QStringLiteral("CF3CH3"),QStringLiteral("1,1,1-三氟乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_difluoroethane                    ,QStringLiteral("1,1-difluoroethane"),QStringLiteral("R152a"),QStringLiteral("CHF2CH3"),QStringLiteral("1,1-二氟乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::fluoroethane                           ,QStringLiteral("fluoroethane"),QStringLiteral("R161"),QStringLiteral("C2H5F"),QStringLiteral("氟乙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::dichlorofluoromethane                  ,QStringLiteral("dichlorofluoromethane"),QStringLiteral("R21"),QStringLiteral("CHCl2F"),QStringLiteral("二氯一氟甲烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::octafluoropropane                      ,QStringLiteral("octafluoropropane"),QStringLiteral("R218"),QStringLiteral("CF3CF2CF3"),QStringLiteral("八氟丙烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::chlorodifluoromethane                  ,QStringLiteral("chlorodifluoromethane"),QStringLiteral("R22"),QStringLiteral("CHClF2"),QStringLiteral("氯二氟甲烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_1_2_3_3_3_heptafluoropropane      ,QStringLiteral("1,1,1,2,3,3,3-heptafluoropropane"),QStringLiteral("R227ea"),QStringLiteral("CF3CHFCF3"),QStringLiteral("R227ea"),MIX);
		addMaterialDataInfo(EasyREFPROP::trifluoromethane                       ,QStringLiteral("trifluoromethane"),QStringLiteral("R23"),QStringLiteral("CHF3"),QStringLiteral("三氟甲烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_1_2_3_3_hexafluoropropane         ,QStringLiteral("1,1,1,2,3,3-hexafluoropropane"),QStringLiteral("R236ea"),QStringLiteral("CF3CHFCHF2"),QStringLiteral("R236ea"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_1_3_3_3_hexafluoropropane         ,QStringLiteral("1,1,1,3,3,3-hexafluoropropane"),QStringLiteral("R236fa"),QStringLiteral("CF3CH2CF3"),QStringLiteral("R236fa"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_2_2_3_pentafluoropropane          ,QStringLiteral("1,1,2,2,3-pentafluoropropane"),QStringLiteral("R245ca"),QStringLiteral("CHF2CF2CH2F"),QStringLiteral("R245ca"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_1_3_3_pentafluoropropane          ,QStringLiteral("1,1,1,3,3-pentafluoropropane"),QStringLiteral("R245fa"),QStringLiteral("CF3CH2CHF2"),QStringLiteral("R245fa"),MIX);
		addMaterialDataInfo(EasyREFPROP::difluoromethane                        ,QStringLiteral("difluoromethane"),QStringLiteral("R32"),QStringLiteral("CH2F2"),QStringLiteral("二氟甲烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::_1_1_1_3_3_pentafluorobutane           ,QStringLiteral("1,1,1,3,3-pentafluorobutane"),QStringLiteral("R365mfc"),QStringLiteral("CF3CH2CF2CH3"),QStringLiteral("R365mfc"),MIX);
		addMaterialDataInfo(EasyREFPROP::fluoromethane                          ,QStringLiteral("fluoromethane"),QStringLiteral("R41"),QStringLiteral("CH3F"),QStringLiteral("氟代甲烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::octafluorocyclobutane                  ,QStringLiteral("octafluorocyclobutane"),QStringLiteral("RC318"),QStringLiteral("cyclo-C4F8"),QStringLiteral("过氟化环丁烷"),MIX);
		addMaterialDataInfo(EasyREFPROP::sulfur_hexafluoride                    ,QStringLiteral("sulfur hexafluoride"),QStringLiteral("sulfur hexafluoride"),QStringLiteral("SF6"),QStringLiteral("六氟化硫"),MIX);
		addMaterialDataInfo(EasyREFPROP::sulfur_dioxide                         ,QStringLiteral("sulfur dioxide"),QStringLiteral("sulfur dioxide"),QStringLiteral("SO2"),QStringLiteral("二氧化硫"),MIX);
		addMaterialDataInfo(EasyREFPROP::trans_2_butene                         ,QStringLiteral("trans-2-butene"),QStringLiteral("trans-butene"),QStringLiteral("CH3-CH=CH-CH3"),QStringLiteral("反式-2-丁烯"),MIX);
		addMaterialDataInfo(EasyREFPROP::methylbenzene                          ,QStringLiteral("methylbenzene"),QStringLiteral("toluene"),QStringLiteral("CH3-C6H5"),QStringLiteral("甲苯"),MIX);
		addMaterialDataInfo(EasyREFPROP::water                                  ,QStringLiteral("water"),QStringLiteral("water"),QStringLiteral("H2O"),QStringLiteral("水"),MIX);
		addMaterialDataInfo(EasyREFPROP::xenon                                  ,QStringLiteral("xenon"),QStringLiteral("xenon"),QStringLiteral("Xe"),QStringLiteral("氙"),PURE);
    }

	void addMaterialDataInfo(EasyREFPROP::Material mat
		,const QString& full_name
		,const QString& short_name
		,const QString& chemical_formula
		,const QString& chinese_name
		,const int type
		){
			materials[mat] = priv_refpropMaterialData(full_name,short_name,chemical_formula,chinese_name,type);
	}
    QString materialID2ChineseString(EasyREFPROP::Material mat){
        return materials[mat].chinese();
    }

    void connectCombox(QComboBox* pCom){
        for(auto ite=materials.begin();ite!=materials.end();++ite)
            pCom->addItem(QStringLiteral("%1 %2").arg(ite.value().chinese()).arg(ite.value().shortName())
                          ,QVariant::fromValue((int)ite.key()));
    }
	/// 
	/// \brief 根据物质的type性质设置需要显示的内容
	/// \param pCom 指针
	/// \param filter 物质已经设定的type
	///
	void filterCombox(QComboBox* pCom,int filter)
	{
		pCom->clear();
		for(auto ite=materials.begin();ite!=materials.end();++ite){
			if ((filter & ite.value().type()) != 0)
			{
				pCom->addItem(QStringLiteral("%1 %2").arg(ite.value().chinese()).arg(ite.value().shortName())
					,QVariant::fromValue((int)ite.key()));
			}
		}
	}
	void filterCombox(QComboBox* pCom,QString keysWord)
	{
		pCom->clear();
		QStringList keys = keysWord.split(QRegExp("\\s|,"));
		QRegExp reg(keys.join("\\i|"));
		QString str;
		for(C_ITERATOR ite=materials.begin();ite!=materials.end();++ite){
			str = ite.value().chinese() + " "
				+ ite.value().fullName() + " "
				+ ite.value().shortName() + " "
				+ ite.value().chemicalFormula();
			if (str.contains(reg))
			{
				pCom->addItem(QStringLiteral("%1 %2").arg(ite.value().chinese()).arg(ite.value().shortName())
					,QVariant::fromValue((int)ite.key()));
			}
		}
	}
	int count() const
	{
		return materials.size();
	}
	
	typedef QMap<EasyREFPROP::Material,priv_refpropMaterialData>::const_iterator C_ITERATOR;
	C_ITERATOR getBeginIte(){
		return materials.begin();
	}
	bool isEnd(C_ITERATOR ite) const
	{
		return ite == materials.end();
	}
	C_ITERATOR getNext(C_ITERATOR ite) const{
		return ++ite;
	}
	QString getFullName(C_ITERATOR ite) const
	{
		return ite.value().fullName();
	}
	QString getShortName(C_ITERATOR ite) const
	{
		return ite.value().shortName();
	}
	QString getChineseName(C_ITERATOR ite) const
	{
		return ite.value().chinese();
	}
	QString getChemicalFormula(C_ITERATOR ite) const
	{
		return ite.value().chemicalFormula();
	}
	QList<EasyREFPROP::Material> getMaterialID(C_ITERATOR ite) const{
		return materials.keys(ite.value());
	}
};

#endif // REFPROPMATERIALDATA_H
