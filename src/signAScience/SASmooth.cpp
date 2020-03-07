#include "SASmooth.h"

SA::SASmooth::SASmooth()
{

}

void SA::SASmooth::linearSmooth3(const double *in, double *out, int N)
{
    linearSmooth3(in,in+N,out);
}

void SA::SASmooth::linearSmooth5(const double *in, double *out, int N)
{
    linearSmooth5(in,in+N,out);
}

void SA::SASmooth::linearSmooth7(const double *in, double *out, int N)
{
    linearSmooth7(in,in+N,out);
}

void SA::SASmooth::quadraticSmooth5(const double *in, double *out, int N)
{
    quadraticSmooth5(in,in+N,out);
}

void SA::SASmooth::quadraticSmooth7(const double *in, double *out, int N)
{
    quadraticSmooth7(in,in+N,out);
}

void SA::SASmooth::cubicSmooth5(const double *in, double *out, int N)
{
    cubicSmooth5(in,in+N,out);
}

void SA::SASmooth::cubicSmooth7(const double *in, double *out, int N)
{
    cubicSmooth7(in,in+N,out);
}
