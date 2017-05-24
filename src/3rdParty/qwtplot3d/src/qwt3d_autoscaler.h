#pragma once
#ifndef qwt3d_autoscaler_h__2009_10_11_14_15_04_begin_guarded_code
#define qwt3d_autoscaler_h__2009_10_11_14_15_04_begin_guarded_code

#include <vector>
#include "qwt3d_global.h"
#include "qwt3d_valueptr.h"

namespace Qwt3D
{

//! ABC for autoscaler
class QWT3D_EXPORT AutoScaler
{
//friend struct ValuePtrTraits<AutoScaler>;
protected:
  //! Returns a new heap based object of the derived class.  
  virtual AutoScaler* clone() const = 0;
  //! To implement from subclasses
  virtual int execute(double& a, double& b, double start, double stop, int ivals) = 0;
  virtual ~AutoScaler(){}
};

//! Automatic beautifying of linear scales
class QWT3D_EXPORT LinearAutoScaler : public AutoScaler
{
friend class LinearScale;
protected:
  LinearAutoScaler();
  explicit LinearAutoScaler(std::vector<double>& mantisses);
  //! For smart pointer
  AutoScaler* clone() const {return new LinearAutoScaler(*this);}
	int execute(double& a, double& b, double start, double stop, int ivals);

private:
	
	double start_, stop_;
	int intervals_; 

	void init(double start, double stop, int ivals);
	double anchorvalue(double start, double mantisse, int exponent);
	int segments(int& l_intervals, int& r_intervals, double start, double stop, double anchor, double mantissa, int exponent);
  std::vector<double> mantissi_;
};

//template<>
//struct ValuePtrTraits<AutoScaler>  
//{
//  static  AutoScaler* clone( const AutoScaler* p )  
//  { 
//    return p->clone(); 
//  }
//};

} // ns

#endif /* include guard */
