#pragma once
#ifndef qwt3d_meshplot_h__2005_7_10_begin_guarded_code
#define qwt3d_meshplot_h__2005_7_10_begin_guarded_code

#include "qwt3d_surfaceplot.h"

namespace Qwt3D
{
//! A class representing plots based on points, edges, cells etc.
/**
	A MeshPlot is more general compared with a GridPlot, in providing 
  surface elements built not from a fixed rectangular grid. There is a cost, 
  as usual. In this case, it means efficiency.
*/
class QWT3D_EXPORT MeshPlot : public SurfacePlot
{
public:
  MeshPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
  virtual ~MeshPlot() {}

  int createDataset(Qwt3D::TripleVector const& data, Qwt3D::CellVector const& poly, bool append = false);
  int facets(unsigned idx = 0) const;
  //virtual int cells(unsigned dimension) const

protected:
  void createNormals(const Plotlet& pl);
  void drawEnrichment(const Plotlet& pl, Enrichment& p);
  void createOpenGlData(const Plotlet& pl);

private:
  class MeshData : public Data
  {
  public:
    MeshData() {datatype_p=Qwt3D::POLYGON;}
    ~MeshData();

    MeshData* clone() const {return new MeshData(*this);}

    bool empty() const { return cells.empty();}
  	
	  Triple const& operator()(unsigned cellnumber, unsigned vertexnumber);
  	
	  CellVector cells;   //!< polygon/cell mesh 
	  TripleVector    nodes; //!< point cloud
	  TripleVector    normals; //!< mesh normals
  };
 
  FLOORSTYLE floorstyle_;
  
  void data2Floor(const Plotlet& pl);
  void isolines2Floor(const Plotlet& pl);
  void setColorFromVertex(const Plotlet& pl, int node, bool skip = false);
};

} // ns
	
#endif /* include guard */
