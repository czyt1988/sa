#pragma once
#ifndef qwt3d_openglhelper_h__2009_10_11_14_23_25_begin_guarded_code
#define qwt3d_openglhelper_h__2009_10_11_14_23_25_begin_guarded_code

//#include <qglobal.h>
#include <QtOpenGL/qgl.h>
#ifdef Q_OS_MAC
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
namespace Qwt3D
{

#ifndef QWT3D_NOT_FOR_DOXYGEN

class GLStateBewarer
{
public:
	
	GLStateBewarer(GLenum what, bool on, bool persist=false)
	{
		state_ = what;
		stateval_ = glIsEnabled(what);	
		if (on)
			turnOn(persist);
		else
			turnOff(persist);
	}

	~GLStateBewarer() 
	{
		if (stateval_)
			glEnable(state_);
		else
			glDisable(state_);
	}
	
	void turnOn(bool persist = false)
	{
		glEnable(state_);
		if (persist)
			stateval_ = true;
	}
	
	void turnOff(bool persist = false)
	{
		glDisable(state_);
		if (persist)
			stateval_ = false;
	}


private:
	
	GLenum state_;
	bool stateval_;

};

inline const GLubyte* gl_error()
{
	GLenum errcode;
	const GLubyte* err = 0;
	
	if ((errcode = glGetError()) != GL_NO_ERROR)
	{
		err = gluErrorString(errcode);
	}
	return err;
}

inline void SaveGlDeleteLists(GLuint& lstidx, GLsizei range)
{
	if (glIsList(lstidx))
		glDeleteLists(lstidx, range);
	lstidx = 0;
}

//! get OpenGL transformation matrices
/**
	Don't rely on (use) this in display lists !
	\param modelMatrix should be a GLdouble[16]
	\param projMatrix should be a GLdouble[16]
	\param viewport should be a GLint[4]
*/
inline void getMatrices(GLdouble* modelMatrix, GLdouble* projMatrix, GLint* viewport)
{
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,	modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,	projMatrix);
}

//! simplified glut routine (glUnProject): windows coordinates_p --> object coordinates_p 
/**
	Don't rely on (use) this in display lists !
*/
inline bool ViewPort2World(double& objx, double& objy, double& objz, double winx, double winy, double winz)
{
	GLdouble modelMatrix[16];
  GLdouble projMatrix[16];
  GLint viewport[4];

	getMatrices(modelMatrix, projMatrix, viewport);
	int res = gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

	return (res == GL_FALSE) ? false : true;
}

//! simplified glut routine (glProject): object coordinates_p --> windows coordinates_p 
/**
	Don't rely on (use) this in display lists !
*/
inline bool World2ViewPort(double& winx, double& winy, double& winz, double objx, double objy, double objz )
{
	GLdouble modelMatrix[16];
  GLdouble projMatrix[16];
  GLint viewport[4];

	getMatrices(modelMatrix, projMatrix, viewport);
	int res = gluProject(objx, objy, objz, modelMatrix, projMatrix, viewport, &winx, &winy, &winz);

	return (res == GL_FALSE) ? false : true;
}

inline void setLineWidth(GLfloat val)
{
  GLfloat lw[2];
  glGetFloatv(GL_LINE_WIDTH_RANGE, lw);

  if (val < lw[0])
    val = lw[0];
  else if (val > lw[1])
    val = lw[1];

  glLineWidth(val);
}
#endif // QWT3D_NOT_FOR_DOXYGEN


} // ns


#endif /* include guard */
