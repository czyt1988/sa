#pragma once
#ifndef qwt3d_surfaceplot_h__2009_10_11_14_25_06_begin_guarded_code
#define qwt3d_surfaceplot_h__2009_10_11_14_25_06_begin_guarded_code

#include "qwt3d_plot3d.h"

namespace Qwt3D
{
//! A class representing  Surfaces
/**
	A SurfacePlot ...
	
*/
class QWT3D_EXPORT SurfacePlot : public Plot3D
{
  Q_OBJECT

public:
  SurfacePlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );

  void showNormals(bool); //!< Draw normals to every vertex
  bool normals() const { return datanormals_p;} //!< Returns \c true, if normal drawing is on
  void setNormalLength(double val); //!< Sets length of normals in percent per hull diagonale
  double normalLength() const { return normalLength_p; }//!< Returns relative length of normals
  void setNormalQuality(int val); //!< Increases plotting quality of normal arrows
  int normalQuality() const { return normalQuality_p; }//!< Returns plotting quality of normal arrows
  void updateNormals(); //!< Recalculates surface normals;
  Qwt3D::FLOORSTYLE floorStyle() const { return floorstyle_;} //!< Return floor style
  void	setFloorStyle( Qwt3D::FLOORSTYLE val ) {floorstyle_ = val;} //!< Sets floor style
  //! Delete Plotlet with index idx.
  bool removePlotlet(unsigned idx);

protected:  
  bool datanormals_p;
  double normalLength_p;
  int normalQuality_p;

  virtual void createNormals(const Plotlet& pl) = 0;
  virtual void createFloorOpenGlData();
  void drawIntersection(const Qwt3D::TripleVector& intersection, Qwt3D::RGBA col);
  virtual void data2Floor(const Plotlet& app) = 0;
  virtual void isolines2Floor(const Plotlet& app) = 0;

private:
  FLOORSTYLE floorstyle_;
};

} // ns

#endif /* include guard */
