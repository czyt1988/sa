#pragma once
#ifndef qwt3d_types_h__2009_10_11_14_25_46_begin_guarded_code
#define qwt3d_types_h__2009_10_11_14_25_46_begin_guarded_code

#if defined(_MSC_VER) /* MSVC Compiler */
  #pragma warning ( disable : 4786 )
#endif

#include <string>

#include <QtGlobal>
#if defined(Q_OS_WIN)
	#include <windows.h>

    #define IS_NAN(x) _isnan(x)
#else
    #define IS_NAN(x) isnan(x)
#endif

#include "qwt3d_portability.h"
#include "qwt3d_helper.h"
#include "qwt3d_openglhelper.h"

// Common namespace for all QwtPlot3D classes
namespace Qwt3D
{

const double PI = 3.14159265358979323846264338328;

//! Plotting style
enum PLOTSTYLE
{
	NOPLOT     , //!< No visible data
	WIREFRAME  , //!< Wireframe style
	HIDDENLINE , //!< Hidden Line style
	FILLED     , //!< Color filled polygons w/o edges 
	FILLEDMESH , //!< Color filled polygons w/ separately colored edges
	POINTS     , //!< User defined style (used by Enrichments)
	USER         //!< User defined style (used by Enrichments)
};

//! Shading style
enum SHADINGSTYLE
{
	FLAT,      //!< Flat shading (OpenGL)
	GOURAUD    //!< Gouraud Shading (OpenGL)
};

//! Style of Coordinate system
enum COORDSTYLE
{
	NOCOORD, //!< Coordinate system is not visible 
	BOX,     //!< Boxed
	FRAME		 //!< Frame - 3 visible axes
};

//! Different types of axis scales
enum SCALETYPE
{
	LINEARSCALE,//!< Linear scaling 
	LOG10SCALE,	//!< Logarithmic scaling (base 10)
	USERSCALE   //!< User-defined (for extensions)
};

//! Plotting style for floor data (projections)
enum FLOORSTYLE
{
	NOFLOOR,   //!< Empty floor
	FLOORISO,  //!< Isoline projections visible
	FLOORDATA //!< Projected polygons visible
};

//! The 12 axes
/**
\image html axes.png 
*/
enum AXIS
{
	X1 = 0,   //!<  1st x-axis
	X2 = 3,   //!<  2nd x-axis
	X3 = 4,   //!<  3th x-axis
	X4 = 5,   //!<  4th x-axis
	Y1 = 1,   //!<  1st y-axis
	Y2 = 8,   //!<  2nd y-axis
	Y3 = 7,   //!<  3th y-axis
	Y4 = 6,   //!<  4th y-axis
	Z1 = 2,   //!<  1st z-axis
	Z2 = 9,   //!<  2nd z-axis
	Z3 = 11,  //!<  3th z-axis
	Z4 = 10   //!<  4th z-axis
};

//! The 6 sides
enum SIDE
{
  NOSIDEGRID = 0,
  LEFT   = 1 << 0,
  RIGHT  = 1 << 1,
  CEIL   = 1 << 2,
  FLOOR  = 1 << 3,
  FRONT  = 1 << 4,
  BACK   = 1 << 5
};

//! Possible anchor points for drawing operations
enum ANCHOR
{
	BottomLeft,
	BottomRight,
	BottomCenter,
	TopLeft,
	TopRight,
	TopCenter,
	CenterLeft,
	CenterRight,
	Center
};


//! Tuple <tt>[x,y]</tt>
struct QWT3D_EXPORT Tuple
{
	Tuple() : x(0), y(0) {} //!< Calls Tuple(0,0)
	Tuple(double X, double Y) : x(X), y(Y) {} //!< Initialize Tuple with x and y
	//! Tuple coordinates
  double x,y; 
};

//! Triple <tt>[x,y,z]</tt>
/**
Consider Triples also as vectors in R^3
*/
struct QWT3D_EXPORT Triple
{
	//! Initialize Triple with x,y and z
	explicit Triple(double xv = 0,double yv = 0,double zv = 0) 
		: x(xv), y(yv), z(zv)
	{
	}
	
#ifndef QWT3D_NOT_FOR_DOXYGEN
#ifdef Q_OS_IRIX
  Triple(const Triple& val)
  {
    if (&val == this)
       return;
    x = val.x;
    y = val.y;
    z = val.z;
  }
  const Triple& operator=(const Triple& val)
  {
    if (&val == this)
      return *this;
    x = val.x;
    y = val.y;
    z = val.z;
    return *this;
  }
#endif 
#endif // QWT3D_NOT_FOR_DOXYGEN
  
	//! Triple coordinates
	double x,y,z; 

	Triple& operator+=(const Triple& t)
	{
		x += t.x;
		y += t.y;
		z += t.z;

		return *this;
	}
	
	Triple& operator-=(const Triple& t)
	{
		x -= t.x;
		y -= t.y;
		z -= t.z;

		return *this;
	}

	Triple& operator*=(double d)
	{
		x *= d;
		y *= d;
		z *= d;

		return *this;
	}

	Triple& operator/=(double d)
	{
		x /= d;
		y /= d;
		z /= d;

		return *this;
	}

	Triple& operator*=(const Triple& t) // scale
	{
		x *= t.x;
		y *= t.y;
		z *= t.z;

		return *this;
	}

	bool operator!=(const Triple& t) const
	{
		return !isPracticallyZero(x,t.x) || !isPracticallyZero(y,t.y) || !isPracticallyZero(z,t.z);
	}
	
	bool operator==(const Triple& t) const
	{
		return !operator!=(t);
	}

	double length() const
	{
		double l2 = x*x + y*y + z*z;
		return (isPracticallyZero(l2)) ? 0 :sqrt(l2);
	}
	
	void normalize()
	{
		double l = length();
		if (l)
			*this /= l;
		else
			reset();
	}

	//! \since 0.3.2
	void crossProduct(Triple const& u, Triple const& v)
	{
		/* compute the cross product (u x v for right-handed [ccw]) */
		x = u.y * v.z - u.z * v.y;
		y = u.z * v.x - u.x * v.z;
		z = u.x * v.y - u.y * v.x;
	}

	//! \since 0.3.2
	bool isValid() const
	{
        return !IS_NAN(x) && !IS_NAN(y) && !IS_NAN(z);
	}

	//! \since 0.3.2
	void reset()
	{
		x = y = z = 0.0;
	}
};

inline const Triple operator+(const Triple& t, const Triple& t2)
{
	return Triple(t) += t2;
}

inline const Triple operator-(const Triple& t, const Triple& t2)
{
	return Triple(t) -= t2;
}

inline const Triple operator*(double d, const Triple& t)
{
	return Triple(t) *= d;
}

inline const Triple operator*(const Triple& t, double d)
{
	return Triple(t) *= d;
}

inline const Triple operator/(double d, const Triple& t)
{
	return Triple(t) /= d;
}

inline const Triple operator/(const Triple& t, double d)
{
	return Triple(t) /= d;
}

inline const Triple operator*(const Triple& t, const Triple& t2)
{
	return Triple(t) *= t2;
}

//! Parallelepiped spanned by 2 Triples
/**
Please use \em normalized Parallelepipeds:\n\n
minVertex.x <= maxVertex.x\n
minVertex.y <= maxVertex.y\n
minVertex.z <= maxVertex.z\n
*/
struct QWT3D_EXPORT ParallelEpiped
{
	//! Construct non-initialized Parallelepiped
	ParallelEpiped()
	{
	}

	//! Construct initialized Parallelepiped
	/**
		minv -> minVertex\n
		maxv -> maxVertex\n
	*/
	ParallelEpiped(Triple minv, Triple maxv)
	: minVertex(minv), maxVertex(maxv)
	{
	}
	
	Triple minVertex;
	Triple maxVertex;
};

inline ParallelEpiped sum(const ParallelEpiped& a, const ParallelEpiped& b)
{
  Triple mi = a.minVertex - b.minVertex;
  mi.x = (mi.x<0) ? a.minVertex.x : b.minVertex.x; 
  mi.y = (mi.y<0) ? a.minVertex.y : b.minVertex.y; 
  mi.z = (mi.z<0) ? a.minVertex.z : b.minVertex.z; 
  
  Triple ma = a.maxVertex - b.maxVertex;
  ma.x = (ma.x>0) ? a.maxVertex.x : b.maxVertex.x; 
  ma.y = (ma.y>0) ? a.maxVertex.y : b.maxVertex.y; 
  ma.z = (ma.z>0) ? a.maxVertex.z : b.maxVertex.z; 

  return ParallelEpiped(mi, ma);
}

//! Free vector
/**
	FreeVectors represent objects like normal vectors and other vector fields inside R^3 
*/
struct QWT3D_EXPORT FreeVector
{
	FreeVector()
	{
	}

	//! Construct initialized vector
	/**
		b -> base\n
		e -> top\n
	*/
	FreeVector(Triple b, Triple t)
	: base(b), top(t)
	{
	}
	
	Triple base;
	Triple top;
};

//! A free vector field in R^3
typedef std::vector<FreeVector> FreeVectorField;

//! A point field in R^3
typedef std::vector<Triple> TripleVector;
//! Holds indices in a TripleVector interpreted as an oriented (first->second) Edge
typedef std::pair<unsigned,unsigned> Edge;
//! Vector of Edges for a graph or cell complex. You need a TripleVector as base for the node data
typedef std::vector<Edge> EdgeVector;
//! Holds indices in a TripleVector interpreted as counterclockwise node numbering for a convex polygon
typedef std::vector<unsigned> Cell;
//! Vector of convex polygons. You need a TripleVector as base for the node data
typedef std::vector<Cell> CellVector;
//! Returns the sum over the sizes of the single cells
unsigned tesselationSize(Qwt3D::CellVector const& t);
//! Rectangular hull for point cloud
QWT3D_EXPORT Qwt3D::ParallelEpiped hull(TripleVector const& data);

//! Red-Green-Blue-Alpha value
struct QWT3D_EXPORT RGBA
{
	RGBA()
		: r(0), g(0), b(0), a(1)
		{}
	
	RGBA(double rr, double gg, double bb, double aa = 1)
		: r(rr), g(gg), b(bb), a(aa)
		{}

	//! \since 0.3.2
	bool operator==(const RGBA& c)
	{
		return r == c.r && g == c.g && b == c.b && a == c.a;
	}

	//! \since 0.3.2
	bool operator!=(const RGBA& c)
	{
		return !(*this == c);
	}

	double r,g,b,a;
};

//! A Color field
typedef std::vector<RGBA> ColorVector;

#ifndef QWT3D_NOT_FOR_DOXYGEN

QWT3D_EXPORT QColor GL2Qt(GLdouble r, GLdouble g, GLdouble b); //!< RGB -> QColor
QWT3D_EXPORT Qwt3D::RGBA Qt2GL(QColor col); //!< QColor -> RGBA

inline Triple normalizedcross(Triple const& u, Triple const& v)
{
	Triple n;

	/* compute the cross product (u x v for right-handed [ccw]) */
	n.x = u.y * v.z - u.z * v.y;
	n.y = u.z * v.x - u.x * v.z;
	n.z = u.x * v.y - u.y * v.x;

	/* normalize */
	n.normalize();
	
	return n;
}

inline double dotProduct(Triple const& u, Triple const& v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

void convexhull2d( std::vector<unsigned>& idx, const std::vector<Qwt3D::Tuple>& src );


#endif // QWT3D_NOT_FOR_DOXYGEN 

} // ns

#endif /* include guard */
