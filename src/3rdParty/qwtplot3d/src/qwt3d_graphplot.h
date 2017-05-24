#pragma once
#ifndef qwt3d_graphplot_h__2009_10_11_14_19_41_begin_guarded_code
#define qwt3d_graphplot_h__2009_10_11_14_19_41_begin_guarded_code

#include "qwt3d_plot3d.h"
#include "qwt3d_data.h"

namespace Qwt3D
{

//! TODO
class QWT3D_EXPORT GraphPlot : public Plot3D
{
//    Q_OBJECT

public:
    GraphPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
 
    int createDataset(Qwt3D::TripleVector const& nodes, Qwt3D::EdgeVector const& edges, bool append = false);

protected:
    void createOpenGlData(const Plotlet& pl);

private:
    class GraphData : public Data
    {
    public:
        GraphData();
        virtual ~GraphData();
        GraphData* clone() const {return new GraphData(*this);}
        bool empty() const;

        TripleVector nodes;
        EdgeVector edges;
    };
};

} // ns

#endif /* include guard */

