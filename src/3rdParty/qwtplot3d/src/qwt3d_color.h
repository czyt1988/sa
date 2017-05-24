#pragma once
#ifndef qwt3d_color_h__2009_06_12_13_31_18_begin_guarded_code
#define qwt3d_color_h__2009_06_12_13_31_18_begin_guarded_code

//#include <qstring.h>
#include "qwt3d_global.h"
#include "qwt3d_types.h"
#include "qwt3d_valueptr.h"

namespace Qwt3D
{
class Plot3D;
//! Abstract base class for color functors
/*!
Use your own color model by providing an implementation of operator()(double x, double y, double z).
Colors destructor has been declared \c protected, in order to use only heap based objects. Plot3D 
will handle the objects destruction.
See StandardColor for an example
*/
class QWT3D_EXPORT Color
{
public:
  virtual ~Color(){}
  virtual Color* clone() const = 0;
  virtual Qwt3D::RGBA rgba(double x, double y, double z) const = 0; //!< Implement your color model here
  virtual Qwt3D::RGBA rgba(Qwt3D::Triple const& t) const {return this->rgba(t.x,t.y,t.z);} 
  //! Should create a color vector usable by ColorLegend. The default implementation returns vec
  virtual Qwt3D::ColorVector& createVector(Qwt3D::ColorVector& vec) { return vec; }
  //! Called from Appearance::update(). The default implementation is empty.
  virtual void update(const Plot3D&) {} 
};

//template<>
//struct ValuePtrTraits<Color>  
//{
//  static  Color* clone( const Color* p )  
//  { 
//    return p->clone(); 
//  }
//};

} // ns

#endif /* include guard */
