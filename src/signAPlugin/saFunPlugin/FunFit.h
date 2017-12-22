#ifndef FUNFIT_H
#define FUNFIT_H

///
/// \brief 拟合的相关函数
///
class FunFit
{
public:
    FunFit();
    static void polyfit();
    static void polyfitInChart();
    static void polyfitInValue();
private:
    static bool getPolyfitConfig(int& order);
};

#endif // FUNFIT_H
