#pragma once
#ifndef qwt3d_color_std_h__2009_06_12_13_29_12_begin_guarded_code
#define qwt3d_color_std_h__2009_06_12_13_29_12_begin_guarded_code

#include "qwt3d_color.h"

namespace Qwt3D
{
class Plot3D;
//! Standard color model for Plot3D - implements the data driven operator()(double x, double y, double z)
/*!
The class has a ColorVector representing z values, which will be used by operator()(double x, double y, double z)
*/
class QWT3D_EXPORT StandardColor : public Color
{
public:
	//! Initializes with data and set up a ColorVector with a size of 100 z values (default);
  explicit StandardColor(unsigned size = 100);
  Color* clone() const {return new StandardColor(*this);}

  //! Returns z-dependent color from ColorVector (black == RGBA(0,0,0), for values out of range)
  Qwt3D::RGBA rgba(double x, double y, double z) const; 
	void setColorVector(Qwt3D::ColorVector const& cv);
	void reset(unsigned size=100); //!< Resets the standard colors; 
	void setAlpha(double a); //!< Sets unitary alpha value for all colors
	
  
  void update(const Plot3D& val);
  /** 
		\brief Creates color vector
		
		Creates a color vector used by ColorLegend. This is essentially a copy from the internal used vector.
		\return The vector created
	*/
	Qwt3D::ColorVector& createVector(Qwt3D::ColorVector& vec) {vec = colors_; return vec;}

protected:
	Qwt3D::ColorVector colors_;

private:
  double zmin_, zmax_;
};

} // ns

#endif /* include guard */

