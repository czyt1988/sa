#pragma once
#ifndef qwt3d_parametricsurface_h__2004_03_05_23_43_begin_guarded_code
#define qwt3d_parametricsurface_h__2004_03_05_23_43_begin_guarded_code

#include "qwt3d_gridmapping.h"

namespace Qwt3D
{

class GridPlot;


//! Abstract base class for parametric surfaces
/**

*/
class QWT3D_EXPORT ParametricSurface : public GridMapping
{

public:
  ParametricSurface(); //!< Constructs ParametricSurface object w/o assigned GridPlot.
  //! Constructs ParametricSurface object and assigns a GridPlot
  explicit ParametricSurface(Qwt3D::GridPlot& plotWidget); 
  //! Constructs ParametricSurface object and assigns a GridPlot
  explicit ParametricSurface(Qwt3D::GridPlot* plotWidget); 
  //! Overwrite this
  virtual Qwt3D::Triple operator()(double u, double v) = 0; 
	//! Assigns a new GridPlot and creates a data representation for it.
	virtual bool create(Qwt3D::GridPlot& plotWidget);
	//! Creates data representation for the actual assigned GridPlot.
	virtual bool create(bool append = false);
  //! Assigns the object to another widget. To see the changes, you have to call this function before create().
  void assign(Qwt3D::GridPlot& plotWidget);
  //! Assigns the object to another widget. To see the changes, you have to call this function before create().
  void assign(Qwt3D::GridPlot* plotWidget);
  //! Provide information about periodicity of the 'u' resp. 'v' domains.
  void setPeriodic(bool u, bool v); 

private:
  bool uperiodic_, vperiodic_;
};

} // ns

#endif /* include guarded */
