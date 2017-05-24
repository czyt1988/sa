#pragma once
#ifndef __plot3d_2003_06_09_12_14__
#define __plot3d_2003_06_09_12_14__

#include <cassert>

#include "qwt3d_data.h"
#include "qwt3d_extglwidget.h"
#include "qwt3d_coordsys.h"
#include "qwt3d_enrichment_std.h"
#include "qwt3d_appearance.h"

namespace Qwt3D
{
//! Base class for all plotting widgets
/*!
  Plot3D handles all the common features for plotting widgets beyond the low-level extensions of
  ExtGLWidget - coordinate system, labeling and more. It contains some pure virtual functions and is -
  in this respect - an abstract base class. It's nevertheless no pure interface.
    The class provides interfaces for basic data controlled color allocation.
*/
class QWT3D_EXPORT Plot3D : public ExtGLWidget
{
    Q_OBJECT

public:
    Plot3D (QWidget * parent = 0, const QGLWidget * shareWidget = 0);
    virtual ~Plot3D();

    QPixmap renderPixmap (int w=0, int h=0, bool useContext=false);

    void updateData(); //!< Recalculate data

	Qwt3D::CoordinateSystem* coordinates() { return &coordinates_p; } //!< Returns pointer to CoordinateSystem object
	//! \since 0.3.2
	const Qwt3D::CoordinateSystem* coordinates() const { return &coordinates_p; } //!< Returns const pointer to CoordinateSystem object
    void createCoordinateSystem(Qwt3D::Triple beg, Qwt3D::Triple end);
    void setCoordinateStyle(Qwt3D::COORDSTYLE st); //!< Sets style of coordinate system.

    Qwt3D::ColorLegend* legend() { return &legend_;} //!< Returns pointer to ColorLegend object
    void showColorLegend(bool show, unsigned idx = 0);
    //! \since 0.3.1
    bool isLegendVisible() const { return displaylegend_; }

    void setTitlePosition(double rely, double relx = 0.5, Qwt3D::ANCHOR = Qwt3D::TopCenter);
    void setTitleFont(const QString& family, int pointSize, int weight = QFont::Normal, bool italic = false);
    void setTitleColor(Qwt3D::RGBA col) {title_.setColor(col);} //!< Set caption color
    void setTitle(const QString& title) {title_.setString(title);} //!< Set caption text (one row only)

    Qwt3D::ParallelEpiped hull() const { return hull_;} //!< Returns rectangular hull

    //! Set number of isolines. The lines are equidistant between minimal and maximal Z value
    void setIsolines(unsigned steps) {isolinesZ_p.resize(steps); delayisolinecalculation_p = true;}
    //! Set user-defined isoline vector;
    void setIsolines(const std::vector<double>& val) {isolinesZ_p = val; delayisolinecalculation_p = false;}
    unsigned isolines() const { return isolinesZ_p.size();} //!< Returns number of isolines

    void setBackgroundColor(Qwt3D::RGBA rgba); //!< Sets widgets background color
    Qwt3D::RGBA backgroundRGBAColor() const {return bgcolor_;} //!< Returns the widgets background color

    //! Returns number of Plotlets
    unsigned plotlets() const {return plotlets_p.size();}
    //! Returns false, if at least one valid dataset exists.
    inline bool hasData() const;
    //! Returns appearance for Plotlet at position idx
    inline Appearance& appearance(unsigned idx);
    //! Returns appearance for Plotlet at position idx
    inline const Appearance& appearance(unsigned idx) const;
    //! Delete Plotlet with index idx.
    virtual bool removePlotlet(unsigned idx);

    // Convenience/compatibility member for appearance(0) properties

    void setPlotStyle(Qwt3D::PLOTSTYLE val); //!< Set plotting style
    Qwt3D::Enrichment* setPlotStyle(Qwt3D::Enrichment const& val);
    Qwt3D::PLOTSTYLE plotStyle() const { return appearance(0).plotStyle(); }//!< Returns plotting style
    //! Returns current Enrichment object used for plotting styles (if set, zero else)
    Qwt3D::Enrichment* userStyle() const { return appearance(0).userStyle(); }

    void setShading(Qwt3D::SHADINGSTYLE val ); //!<Set shading style
    Qwt3D::SHADINGSTYLE shading() const { return appearance(0).shading(); }//!< Returns shading style

    void setSmoothMesh(bool val) {appearance(0).setSmoothMesh(val);} //!< Enables/disables smooth data mesh lines. Default is false
    bool smoothDataMesh() const {return appearance(0).smoothDataMesh();} //!< True if mesh antialiasing is on
    void setMeshColor(Qwt3D::RGBA rgba); //!< Sets color for data mesh
    Qwt3D::RGBA meshColor() const {return appearance(0).meshColor();} //!< Returns color for data mesh
    void setMeshLineWidth(double lw); //!< Sets line width for data mesh
    double meshLineWidth() const {return appearance(0).meshLineWidth();} //!< Returns line width for data mesh
    void setDataColor(const Qwt3D::Color& col); //!< Sets new data color object
    const Qwt3D::ValuePtr<Qwt3D::Color>& dataColor() const {return appearance(0).dataColor();} //!< Returns data color object

    // tweaks
    void setPolygonOffset(double d);
    double polygonOffset() const {return appearance(0).polygonOffset();} //!< Returns relative value for polygon offset [0..1]

	//! \since 0.3.2
	void enableFastNormals(bool on) { m_fastNormals = on; }

    //!< Add an Enrichment
    virtual Qwt3D::Enrichment* addEnrichment(Qwt3D::Enrichment const& val){return appearance(0).addEnrichment(val);}
    virtual bool degrade(Qwt3D::Enrichment* val){return appearance(0).degrade(val);} //!< Remove an Enrichment

public slots:
    virtual bool save(QString const& fileName, QString const& format); //!<  Saves content

protected:   
    enum OBJECTS
    {
        DataObject,
        LegendObject,
        NormalObject,
        DisplayListSize // only to have a vector length ...
    };

    //! Combines data with their visual appearance
    /**
	  A Plotlet describes the plot's part related to a single dataset. In this respect, it has
	  no own coordinate system (but a hull) and other plot-wide properties. A single
	  data- and the associated Appearance object form a Plotlet.
    */
    struct Plotlet
    {
    public:
        explicit Plotlet(Data* d = 0, const Appearance& a = Appearance());
        ValuePtr<Data> data;
        ValuePtr<Appearance> appearance;
    };

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    Qwt3D::CoordinateSystem coordinates_p;
    
    virtual	void calculateHull();
    virtual void updateAppearances();
    virtual void createOpenGlData();
    virtual void createOpenGlData(const Plotlet& pl) = 0;
	virtual void drawOpenGlData();

    void createCoordinateSystem();
    void setHull(Qwt3D::ParallelEpiped const& h) { hull_ = h; }

    std::vector<Plotlet> plotlets_p;

    std::vector<GLuint> displaylists_p;

    std::vector<double> isolinesZ_p;
    bool delayisolinecalculation_p;

	bool m_fastNormals;

    // debug
    quint64 m_createTime;

    /**
	  Utilized from createDataset members in inherited plot types.
	  Following different strategies (depending on append) in modifying the
	  Plotlet vector, the function assigns data of type DATA to
	  a Plotlets data member. It returns a reference to the new content and the
	  position in the Plotlet vector
  */
    template<typename DATA>
    int prepareDatasetCreation(bool append)
    {
        assert(!plotlets_p.empty());

        if (!append)
        {
            plotlets_p.erase(plotlets_p.begin()+1, plotlets_p.end());
            plotlets_p[0].data = ValuePtr<Data>(new DATA);
        }
        else
        {
            plotlets_p.push_back(Plotlet(new DATA));
        }
        return plotlets_p.size()-1;
    }


private:
    Appearance appearance_;

    Qwt3D::ParallelEpiped hull_;

    bool displaylegend_;
    Qwt3D::ColorLegend legend_;

    Qwt3D::RGBA bgcolor_;
    Label title_;
    Qwt3D::Tuple titlerel_;
    Qwt3D::ANCHOR titleanchor_;
    bool renderpixmaprequest_;
};


// Inline functions

bool Plot3D::hasData() const 
{
    return plotlets() > 0 && !plotlets_p[0].data->empty();
}


/**
The function returns the Appearance object for the Plotlet at idx.
For invalid arguments the return value contains the standard appearance 
(equivalent to idx==0) is returned
*/
Appearance& Plot3D::appearance(unsigned idx)
{
    assert(!plotlets_p.empty());
    if (idx >= plotlets_p.size())
        return *plotlets_p[0].appearance;
    return *plotlets_p[idx].appearance;
}

/**
The function returns the Appearance object for the Plotlet at idx.
For invalid arguments the return value contains the standard appearance 
(equivalent to idx==0) is returned
*/
const Appearance& Plot3D::appearance(unsigned idx) const
{
    assert(!plotlets_p.empty());
    if (idx >= plotlets_p.size())
        return *plotlets_p[0].appearance;
    return *plotlets_p[idx].appearance;
}

} // ns

#endif /* include guarded */
