#include <QCoreApplication>
#include <czyMath.h>
#include <iostream>
using namespace std;
const size_t LENGTH = 16;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    typedef double DATA;
    DATA testData[LENGTH];

    cout<<"testData:"<<endl;
    for(size_t i(0);i<LENGTH;++i)
    {
        testData[i] = i*sin(double(i));
        cout<<testData[i]<<",";
    }
    cout<<endl;
    cout<<"sum:"<<czy::Math::sum(testData,testData+LENGTH)<<endl;
    cout<<"mean:"<<czy::Math::mean(testData,testData+LENGTH)<<endl;
    cout<<"var:"<<czy::Math::var(testData,testData+LENGTH)<<endl;
    cout<<"std_var:"<<czy::Math::std_var(testData,testData+LENGTH)<<endl;
    cout<<"std_var:"<<czy::Math::std_var(testData,testData+LENGTH)<<endl;
    for(unsigned i(0);i<5;++i)
    {
        cout<<"central_moment("<<i<<"):"
           <<czy::Math::central_moment(testData,testData+LENGTH,i)<<endl;
    }
    for(unsigned i(0);i<5;++i)
    {
        cout<<"origin_moment("<<i<<"):"
           <<czy::Math::origin_moment(testData,testData+LENGTH,i)<<endl;
    }
    cout<<"skewness:"<<czy::Math::skewness(testData,testData+LENGTH)<<endl;
    cout<<"kurtosis:"<<czy::Math::kurtosis(testData,testData+LENGTH)<<endl;
    cout<<"peak_to_peak_value:"<<czy::Math::peak_to_peak_value(testData,testData+LENGTH)<<endl;

    return a.exec();
}
