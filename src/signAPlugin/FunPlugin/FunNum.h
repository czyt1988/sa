#ifndef FUNNUM_H
#define FUNNUM_H
class SAUIInterface;
///
/// \brief 求和
/// \param ui ui接口
///
void sum(SAUIInterface* ui);
///
/// \brief 求均值
/// \param ui ui接口
///
void mean(SAUIInterface* ui);
///
/// \brief 频率统计
/// \param ui ui接口
///
void hist(SAUIInterface* ui);

///
/// \brief 求差分
/// \param ui ui接口
///
void diff(SAUIInterface* ui);

///
/// \brief 频率统计
/// \param ui ui接口
///
void statistics(SAUIInterface* ui);


#endif // FUNSTATISTICS_H
