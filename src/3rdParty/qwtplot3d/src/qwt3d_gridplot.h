#ifndef qwt3d_gridplot_h__2005_7_10_begin_guarded_code
#define qwt3d_gridplot_h__2005_7_10_begin_guarded_code

#include <QtCore/QThread>

#include "qwt3d_surfaceplot.h"

namespace Qwt3D
{
//! A class representing  grid-generated surfaces
/**
  A GridPlot ...
  
*/
class QWT3D_EXPORT GridPlot : public SurfacePlot
{
    Q_OBJECT

public:
    GridPlot(QWidget* parent = 0, const QGLWidget* shareWidget = 0);
    virtual ~GridPlot() {}

    int	resolution() const {return resolution_p;} //!< Returns data resolution (1 means all data)

    int createDataset(Qwt3D::Triple** data, unsigned int columns, unsigned int rows,
                      bool uperiodic = false, bool vperiodic = false, bool append = false);
    int createDataset(double** data, unsigned int columns, unsigned int rows,
                      double minx, double maxx, double miny, double maxy, bool append = false);

	/**		
		Sets number of rendering threads using glDrawArrays to \a count (1..10, 8 by default).
		If \a count = 0, the older (list based, slow) implementation will be used.
		\since 0.3.2
		\sa renderThreadsCount
	*/
	void setRenderThreadsCount(int count);
	/**		
		Returns number of rendering threads using glDrawArrays (8 by default).
		If this value is 0, the older (list based, slow) implementation is used.
		\since 0.3.2
		\sa setRenderThreadsCount
	*/
	int renderThreadsCount() const { return m_threadsCount; }

signals:
    void resolutionChanged(int);

public slots:
    void setResolution(int);

protected:  
    virtual void createOpenGlData(const Plotlet& pl);
	virtual void drawOpenGlData();
	void processVertex(const Triple& vert1, const Triple& norm1, const Color& colorData, bool hl, bool& stripStarted, RGBA& lastColor) const;
	void processLineLoopVertex(const Triple& vert1, bool& stripStarted) const;
	void processLineStripVertex(const Triple& vert1, bool& stripStarted) const;

	void drawEnrichment(const Plotlet& pl, Enrichment& p);

    int resolution_p;

protected:
	//! class made protected to access grid data from inherited classes
    class GridData : public Data
    {
    public:
        GridData();
        GridData(unsigned int columns, unsigned int rows); //!< see setSize()

        GridData* clone() const {return new GridData(*this);}

        int columns() const;
        int rows() const;

        bool empty() const { return vertices.empty();}
        void setSize(unsigned int columns, unsigned int rows); //!< destroys content and set new size, elements are uninitialized

        DataMatrix vertices;		//!< mesh vertices
        DataMatrix normals;		//!< mesh normals
        void setPeriodic(bool u, bool v) {uperiodic_ = u; vperiodic_ = v;}
        bool uperiodic() const {return uperiodic_;}
        bool vperiodic() const {return vperiodic_;}

    private:
        bool uperiodic_, vperiodic_;
        void clear();
    };

	// thread vector processor
	class CVertexProcessor: public QThread
	{
	public:
		CVertexProcessor();

		void setup(int dataWidth, int dataLength, const GridData& data, int row, int step, 
			bool useColorMap, const Qwt3D::RGBA& fixedColor, const Color* colorData = NULL,
			bool showMesh = false, const Qwt3D::RGBA& meshColor = Qwt3D::RGBA(0,0,0,0));

		virtual void run();

		void paintGL();

	private:
		void processVertex(bool& stripStarted, int i, int j, int& index, int& size);
		void endVertex(int& index, int& size);

		void processLineStripVertex(bool& stripStarted, int i, int j, int& index, int& size);
		void endLineVertex(int& index, int& size);

		int m_dataWidth, m_dataLength, m_row, m_step;
		TripleVector m_draw_normals, m_draw_vertices;
		ColorVector m_draw_colors;
		std::vector< QPair<int,int> > m_drawList;
		const GridData* m_data;
		bool m_drawFill, m_useColorMap;
		Qwt3D::RGBA m_fixedColor;
		const Color* m_colorData;

		bool m_drawMesh;
		Qwt3D::RGBA m_meshColor;
		TripleVector m_mesh_vertices;
		ColorVector m_mesh_colors;
		std::vector< QPair<int,int> > m_drawMeshList;
	};

	int m_threadsCount;
	CVertexProcessor m_workers[10];
    bool m_useThreads;

    void createNormals(const Plotlet& pl);
    void data2Floor(const Plotlet& pl);
    void isolines2Floor(const Plotlet& pl);
    void setColorFromVertex(const Color& colorData, const Triple& vertex, RGBA& lastColor, bool skip = false) const;
    void calcNormals(GridData& gdata);
    void sewPeriodic(GridData& gdata);
    void readIn(GridData& gdata, Triple** data, unsigned int columns, unsigned int rows);
    void readIn(GridData& gdata, double** data, unsigned int columns, unsigned int rows,
                double minx, double maxx, double miny, double maxy);
};

} // ns


#endif /* include guard */
