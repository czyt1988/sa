#pragma once
#ifndef qwt3d_helper_h__2009_10_11_14_20_35_begin_guarded_code
#define qwt3d_helper_h__2009_10_11_14_20_35_begin_guarded_code

#include <math.h>
#include <float.h>
#include <vector>
#include <algorithm>

namespace
{
	inline double Min_(double a, double b)
	{
		return (a<b) ? a : b;
	}
}

namespace Qwt3D
{

inline bool isPracticallyZero(double a, double b = 0)
{
  if (!b)
		return (fabs (a) <=  DBL_MIN);	

	return (fabs (a - b) <= Min_(fabs(a), fabs(b))*DBL_EPSILON);	
}
 
inline int round(double d)
{
	return (d>0) ? int(d+0.5) : int(d-0.5);
}


} //ns

#endif /* include guard */
