#pragma once
#ifndef qwt3d_enrichment_h__2004_02_23_19_24_begin_guarded_code
#define qwt3d_enrichment_h__2004_02_23_19_24_begin_guarded_code

#include "qwt3d_global.h"
#include "qwt3d_types.h"

namespace Qwt3D
{

class Plot3D;


//! Abstract base class for data dependent visible user objects
/**
Enrichments provide a framework for user defined OPenGL objects. The base class has a pure virtual 
function clone(). 2 additional functions are per default empty and could also get a new implementation
in derived classes. They can be used for initialization issues or actions not depending on the related
primitive. 
*/
class QWT3D_EXPORT Enrichment
{
public:
  enum TYPE{
    VERTEXENRICHMENT, //!< implemented
    EDGEENRICHMENT,   //!< implemented
    FACEENRICHMENT,
    VOXELENRICHMENT,
    GRAPHENRICHMENT,
    USERENRICHMENT    //!< implemented
  }; //!< Type of the Enrichment.
  
  Enrichment() : plot_p(0) {}
  virtual ~Enrichment(){}
  virtual Enrichment* clone() const = 0; //!< The derived class should give back a new Derived(something) here
  virtual void drawBegin(){}; //!< Empty per default. Can be overwritten.
  virtual void drawEnd(){}; //!< Empty per default. Can be overwritten.
  virtual void assign(Plot3D const& pl) {plot_p = &pl;} //!< Assign to existent plot;
  virtual TYPE type() const = 0; //!< Overwrite 

protected:
  const Plot3D* plot_p;
};

//! Abstract base class for user-defined visible user objects
/**
UserEnrichments introduce a draw routine for customized visible objects, not covered by the 
other types design. draw() is called, when the Plot creates its internal OpenGL data representation 
*/
class QWT3D_EXPORT UserEnrichment : public Enrichment
{
public:
  
  UserEnrichment() : Enrichment() {}
  virtual Enrichment* clone() const = 0; //!< The derived class should give back a new Derived(something) here
  virtual void draw() = 0; //!< Overwrite this
  virtual TYPE type() const {return Enrichment::USERENRICHMENT;} //!< This gives USERENRICHMENT
};

//! Abstract base class for vertex dependent visible user objects
/**
VertexEnrichments introduce a specialized draw routine for vertex dependent data.
draw() is called, when the Plot creates its internal OpenGL data representation 
for every vertex associated to his argument.
*/
class QWT3D_EXPORT VertexEnrichment : public Enrichment
{
public:
  
  VertexEnrichment() : Enrichment() {}
  virtual Enrichment* clone() const = 0; //!< The derived class should give back a new Derived(something) here
  virtual void draw(Triple const&) = 0; //!< Overwrite this
  virtual TYPE type() const {return Enrichment::VERTEXENRICHMENT;} //!< This gives VERTEXENRICHMENT
};

//! Abstract base class for edge dependent visible user objects
/**
EdgeEnrichments introduce a specialized draw routine for edge dependent data.
draw() is called, when the Plot creates its internal OpenGL data representation 
for every edge associated to his argument.
*/
class QWT3D_EXPORT EdgeEnrichment : public Enrichment
{
public:
  
  EdgeEnrichment() : Enrichment() {}
  virtual Enrichment* clone() const = 0; //!< The derived class should give back a new Derived(something) here
  virtual void draw(Triple const& beg, Triple const& end) = 0; //!< Overwrite this
  virtual TYPE type() const {return Enrichment::EDGEENRICHMENT;} //!< This gives EDGEENRICHMENT
};

// todo FaceEnrichment, VoxelEnrichment etc.

} // ns

#endif /* include guard */
