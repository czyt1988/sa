#pragma once
#ifndef qwt3d_data_h__2004_12_20_02_55_begin_guarded_code
#define qwt3d_data_h__2004_12_20_02_55_begin_guarded_code

#include "qwt3d_types.h"

namespace Qwt3D
{

#ifndef QWT3D_NOT_FOR_DOXYGEN

//! Mesh type
enum DATATYPE
{
  GRID,		//!< Rectangular grid
	POLYGON, //!< Convex polygon
  GRAPH,   //!< General graph
  VOXEL,   //!< Voxel... //todo
  /** Generalized graph concept, includes connected 
  and disconnected components up to dimension 3 
  (including faces and voxels)
  */
  CELLCOMPLEX //todo
};	

typedef TripleVector DataColumn;
typedef std::vector<DataColumn> DataMatrix;


class Data
{
public:
  Data() {datatype_p= Qwt3D::POLYGON;}
  //virtual Data* clone() const = 0;
  virtual ~Data() {}
  //! For smart pointer
  virtual Data* clone() const = 0;
  virtual bool empty() const {return true;} //!< no data
  void setHull(Qwt3D::ParallelEpiped const& h) {hull_p = h;}
  Qwt3D::ParallelEpiped const& hull() const {return hull_p;} 
  Qwt3D::DATATYPE dataType() const {return datatype_p;}

protected:
  Qwt3D::DATATYPE datatype_p;
  Qwt3D::ParallelEpiped hull_p;
};

#endif // QWT3D_NOT_FOR_DOXYGEN 
  
} //ns

#endif /* include guard */
