#include <czyMath_Smooth.h>




void czy::Math::linear_smooth_3(const double* in, double* out, int N)
{
    linear_smooth_3(in,in+N,out);
}


void czy::Math::linear_smooth_5(const double* in, double* out, int N)
{
    linear_smooth_5(in,in+N,out);
}


void czy::Math::linear_smooth_7(const double *in, double *out, int N)
{
    linear_smooth_7(in,in+N,out);
}


void czy::Math::quadratic_smooth_5(const double *in, double *out, int N)
{
    quadratic_smooth_5(in,in+N,out);
}


void czy::Math::quadratic_smooth_7(const double *in, double *out, int N)
{
    quadratic_smooth_7(in,in+N,out);
}


void czy::Math::cubic_smooth_5(const double *in, double *out, int N)
{
    cubic_smooth_5(in,in+N,out);
}


void czy::Math::cubic_smooth_7(const double *in, double *out, int N)
{
    cubic_smooth_7(in,in+N,out);
}
