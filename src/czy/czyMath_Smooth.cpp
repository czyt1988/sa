#include <czyMath_Smooth.h>




void SA::Math::linear_smooth_3(const double* in, double* out, int N)
{
    linear_smooth_3(in,in+N,out);
}


void SA::Math::linear_smooth_5(const double* in, double* out, int N)
{
    linear_smooth_5(in,in+N,out);
}


void SA::Math::linear_smooth_7(const double *in, double *out, int N)
{
    linear_smooth_7(in,in+N,out);
}


void SA::Math::quadratic_smooth_5(const double *in, double *out, int N)
{
    quadratic_smooth_5(in,in+N,out);
}


void SA::Math::quadratic_smooth_7(const double *in, double *out, int N)
{
    quadratic_smooth_7(in,in+N,out);
}


void SA::Math::cubic_smooth_5(const double *in, double *out, int N)
{
    cubic_smooth_5(in,in+N,out);
}


void SA::Math::cubic_smooth_7(const double *in, double *out, int N)
{
    cubic_smooth_7(in,in+N,out);
}
