#pragma once
#ifndef qwt3d_appearance_h__2009_06_10_12_11_18_begin_guarded_code
#define qwt3d_appearance_h__2009_06_10_12_11_18_begin_guarded_code

#include "qwt3d_types.h"
#include "qwt3d_valueptr.h"
#include "qwt3d_color_std.h"


namespace Qwt3D
{
  class Plot3D;
  class Color;
  class Enrichment;
  //! A class representing the outer look of datasets 
  /**
  An Appearance object holds/modifies visible properties on a
  per dataset base. These include colors, line widths, shading 
  and plot styles, some data related Enrichments and more. Most 
  of the time, they are an optional element, available when adding 
  a new data set. The data's visual properties are accessible by 
  exploiting the associated Appearance object.
  */
class QWT3D_EXPORT Appearance
{
public:
  explicit Appearance(unsigned colors = 100);
  virtual Appearance* clone() const {return new Appearance(*this);}
  Appearance(const Appearance& val); 
  Appearance& operator=(const Appearance& val); 
  virtual ~Appearance();
  
  /**
  Gives inherited classes the possibility to adapt the appearance to 
  properties of the widget immediately before a new OpenGL representation
  is produced by Plot3D::updateData(). The argument will contain the new
  hull prior to the call. Standard behavior provides the contained StandardColor
  with the new hulls 'z' range.
  */
  virtual void update(const Plot3D& plot);
  
  void setPlotStyle(Qwt3D::PLOTSTYLE val); //!< Set plotting style
  Qwt3D::Enrichment* setPlotStyle( Qwt3D::Enrichment const& val); //!< Set user-defined plot style
  Qwt3D::PLOTSTYLE plotStyle() const { return appproxy_.plotstyle; }//!< Returns plotting style
  //! Returns current Enrichment object used for plotting styles (if set, zero else)
  Qwt3D::Enrichment* userStyle() const { return userplotstyle_p; }
  
  void setShading( Qwt3D::SHADINGSTYLE val ); //!<Set shading style
  Qwt3D::SHADINGSTYLE shading() const { return appproxy_.shading; }//!< Returns shading style
  
  void setSmoothMesh(bool val) {appproxy_.smoothdatamesh = val;} //!< Enables/disables smooth data mesh lines. Default is false
  bool smoothDataMesh() const {return appproxy_.smoothdatamesh;} //!< True if mesh antialiasing is on
  void setMeshColor(Qwt3D::RGBA rgba) {appproxy_.meshcolor = rgba;} //!< Sets color for data mesh
  Qwt3D::RGBA meshColor() const {return appproxy_.meshcolor;} //!< Returns color for data mesh
  void setMeshLineWidth(double lw){if (lw < 0)return; appproxy_.meshLineWidth = lw;} //!< Sets line width for data mesh
  double meshLineWidth() const {return appproxy_.meshLineWidth;} //!< Returns line width for data mesh
  void setDataColor(const Color& col); //!< Sets new data color object
  const Qwt3D::ValuePtr<Qwt3D::Color>& dataColor() const {return datacolor_p;} //!< Returns data color object

  // tweaks
  void setPolygonOffset(double d){appproxy_.polygonOffset = d;} //!< Set polgon offset (hint for OpenGL)
  double polygonOffset() const {return appproxy_.polygonOffset;} //!< Returns relative value for polygon offset [0..1]

  virtual Qwt3D::Enrichment* addEnrichment(Qwt3D::Enrichment const&); //!< Add an Enrichment
  virtual bool degrade(Qwt3D::Enrichment*); //!< Remove an Enrichment

  typedef std::list<Qwt3D::Enrichment*> EnrichmentList;
  typedef EnrichmentList::iterator ELIT;
  typedef EnrichmentList::const_iterator ELCIT;
  EnrichmentList elist;

protected:
  Qwt3D::ValuePtr<Qwt3D::Color> datacolor_p;
  Qwt3D::Enrichment* userplotstyle_p;


private:
  // op= and assignment ctor functionality
  void copy(const Appearance& val);

#ifndef QWT3D_NOT_FOR_DOXYGEN
  // simplifies copy semantics for Appearance 
  // by encapsulating most of the non-polymorph members;
  class AppProxy
  {
  public:
    // set standard values
    AppProxy();
    //isolines
    Qwt3D::RGBA meshcolor;
    double meshLineWidth;
    bool smoothdatamesh;
    Qwt3D::PLOTSTYLE plotstyle;
    Qwt3D::SHADINGSTYLE shading;
    double polygonOffset;
  };
#endif // QWT3D_NOT_FOR_DOXYGEN

  AppProxy appproxy_;
};

} //ns

#endif /* include guard */
