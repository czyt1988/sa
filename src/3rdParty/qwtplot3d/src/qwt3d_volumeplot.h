/*************************************************************
 * VolumePlot extension
 *
 * (c) Sintegrial Technologies 2013-2014
 *             http://www.sintegrial.com
 ************************************************************/

#pragma once
#ifndef qwt3d_volumeplot_h__2004_03_06_01_52_begin_guarded_code
#define qwt3d_volumeplot_h__2004_03_06_01_52_begin_guarded_code

#include "qwt3d_plot3d.h"


namespace Qwt3D
{

#pragma pack(push,1)

/*!
    \brief Single 3d point used by VolumePlot.
    \since 0.3.1
*/
struct Voxel
{
    Voxel(float fx, float fy, float fz, quint8 ur = 0, quint8 ug = 0, quint8 ub = 0, quint8 ua = 255):
        x(fx), y(fy), z(fz),
        r(ur), g(ug), b(ub), a(ua)
    {
    }

	bool isValid() const
	{
        return !IS_NAN(x) && !IS_NAN(y) && !IS_NAN(z);
	}

    float x,y,z;
    quint8 r,g,b,a;

    typedef QList<Voxel> Array;
};

#pragma pack(pop)


/*!
    \brief A class representing a 3d point cloud (i.e. set of voxels).
    To create the data, use createDataset() method.
    \since 0.3.1
*/
class QWT3D_EXPORT VolumePlot : public Plot3D
{
public:
    VolumePlot(QWidget *parent = 0, const QGLWidget *shareWidget = 0);

    int createDataset(Voxel::Array const &nodes, bool append = false);

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

        Voxel::Array nodes;
    };
};


} // ns

#endif /* include guard */
