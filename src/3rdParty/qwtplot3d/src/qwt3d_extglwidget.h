#pragma once
#ifndef qwt3d_extglwidget_h__2009_10_11_14_18_36_begin_guarded_code
#define qwt3d_extglwidget_h__2009_10_11_14_18_36_begin_guarded_code

#include "qwt3d_types.h"

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

namespace Qwt3D
{
  
//! An enhanced QGLWidget
/*!
  The class covers mouse/keyboard handling, lighting and basic transformations, like
  scaling, shifting and rotating objects.
*/
class QWT3D_EXPORT ExtGLWidget : public QGLWidget
{
  Q_OBJECT

public:
  ExtGLWidget ( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
  virtual ~ExtGLWidget() {}
	
  // transformations
  
	double xRotation() const { return xRot_;}  //!< Returns rotation around X axis [-360..360] (some angles are equivalent)
	double yRotation() const { return yRot_;}  //!< Returns rotation around Y axis [-360..360] (some angles are equivalent)
	double zRotation() const { return zRot_;}  //!< Returns rotation around Z axis [-360..360] (some angles are equivalent)

	double xShift() const { return xShift_;} //!< Returns shift along X axis (object coordinates)
	double yShift() const { return yShift_;} //!< Returns shift along Y axis (object coordinates)
	double zShift() const { return zShift_;} //!< Returns shift along Z axis (object coordinates)
	
	double xViewportShift() const { return xVPShift_;} //!< Returns relative shift [-1..1] along X axis (view coordinates)
	double yViewportShift() const { return yVPShift_;} //!< Returns relative shift [-1..1] along Y axis (view coordinates)
	
	double xScale() const { return xScale_;} //!< Returns scaling for X values [0..inf]
	double yScale() const { return yScale_;} //!< Returns scaling for Y values [0..inf]
	double zScale() const { return zScale_;} //!< Returns scaling for Z values [0..inf]

	double zoom() const { return zoom_;} //!< Returns zoom (0..inf)
	bool ortho() const { return ortho_; } //!< Returns orthogonal (true) or perspective (false) projection

	
  // input devices

	void assignMouse(MouseState xrot, MouseState yrot, MouseState zrot,
									 MouseState xscale, MouseState yscale, MouseState zscale,
									 MouseState zoom, MouseState xshift, MouseState yshift);
	
	bool mouseEnabled() const; //!< Returns true, if the widget accept mouse input from the user
	void assignKeyboard(
     KeyboardState xrot_n, KeyboardState xrot_p
    ,KeyboardState yrot_n, KeyboardState yrot_p
    ,KeyboardState zrot_n, KeyboardState zrot_p
		,KeyboardState xscale_n, KeyboardState xscale_p 
    ,KeyboardState yscale_n, KeyboardState yscale_p
    ,KeyboardState zscale_n, KeyboardState zscale_p
		,KeyboardState zoom_n, KeyboardState zoom_p
    ,KeyboardState xshift_n, KeyboardState xshift_p
    ,KeyboardState yshift_n, KeyboardState yshift_p
    );
	
	bool keyboardEnabled() const; //!< Returns true, if the widget accept keyboard input from the user
  //! Sets speed for keyboard driven transformations
  void setKeySpeed(double rot, double scale, double shift); 
  //! Gets speed for keyboard driven transformations
  void keySpeed(double& rot, double& scale, double& shift) const; 
 
  
  // lighting

  bool lightingEnabled() const; //!< Returns true, if Lighting is enabled, false else
  //! Turn light on
  void illuminate(unsigned light = 0);
  //! Turn light off
  void blowout(unsigned light = 0);

  void setMaterialComponent(GLenum property, double r, double g, double b, double a = 1.0);    
  void setMaterialComponent(GLenum property, double intensity);    
  void setShininess(double exponent);
  void setLightComponent(GLenum property, double r, double g, double b, double a = 1.0, unsigned light=0);    
  void setLightComponent(GLenum property, double intensity, unsigned light=0);    
  //! Returns Light 'idx' rotation around X axis [-360..360] (some angles are equivalent)
  double xLightRotation(unsigned idx = 0) const { return (idx<8) ? lights_[idx].rot.x : 0;}
  //! Returns Light 'idx' rotation around Y axis [-360..360] (some angles are equivalent)
  double yLightRotation(unsigned idx = 0) const { return (idx<8) ? lights_[idx].rot.y : 0;}
  //! Returns Light 'idx' rotation around Z axis [-360..360] (some angles are equivalent)
  double zLightRotation(unsigned idx = 0) const { return (idx<8) ? lights_[idx].rot.z : 0;}
  //! Returns shift of Light 'idx 'along X axis (object coordinates)
  double xLightShift(unsigned idx = 0) const {return (idx<8) ? lights_[idx].shift.x : 0;} 
  //! Returns shift of Light 'idx 'along Y axis (object coordinates)
  double yLightShift(unsigned idx = 0) const {return (idx<8) ? lights_[idx].shift.y : 0;} 
  //! Returns shift of Light 'idx 'along Z axis (object coordinates)
  double zLightShift(unsigned idx = 0) const {return (idx<8) ? lights_[idx].shift.z : 0;}
  

signals:
	//! Emitted, if the rotation is changed
  void rotationChanged( double xAngle, double yAngle, double zAngle ); 
	//! Emitted, if the shift is changed
	void shiftChanged( double xShift, double yShift, double zShift );
	//! Emitted, if the viewport shift is changed
	void vieportShiftChanged( double xShift, double yShift );
	//! Emitted, if the scaling is changed
  void scaleChanged( double xScale, double yScale, double zScale );
	//! Emitted, if the zoom is changed
	void zoomChanged(double);
	//! Emitted, if the projection mode is changed
  void projectionChanged(bool);


public slots:
	void setRotation( double xVal, double yVal, double zVal ); 																														
	void setShift( double xVal, double yVal, double zVal );    																														
	void setViewportShift( double xVal, double yVal );         																														
	void setScale( double xVal, double yVal, double zVal );    																														
	void setZoom( double );                                    																														
  void setOrtho(bool);                                       																														
  
	void enableMouse(bool val=true); //!< Enable mouse input   																														
	void disableMouse(bool val =true); //!< Disable mouse input																														
	void enableKeyboard(bool val=true); //!< Enable keyboard input   																														
	void disableKeyboard(bool val =true); //!< Disable keyboard input																														

  void enableLighting(bool val = true); //!< Turn Lighting on or off
  void disableLighting(bool val = true); //!< Turn Lighting on or off
  //! Rotate ligthsource[idx]
  void setLightRotation( double xVal, double yVal, double zVal, unsigned int idx = 0 ); 																														
  //! Shift ligthsource[idx]
	void setLightShift( double xVal, double yVal, double zVal, unsigned int idx = 0 );    																														


protected:
	void mousePressEvent( QMouseEvent *e );
	void mouseReleaseEvent( QMouseEvent *e );
	void mouseMoveEvent( QMouseEvent *e );
	void wheelEvent( QWheelEvent *e );		
  void keyPressEvent( QKeyEvent *e );

	void initializeGL();
  bool initializedGL() const {return initializedGL_;}
  void applyLights();

private:
  // trafos  
  GLdouble xRot_, yRot_, zRot_, xShift_, yShift_, zShift_, zoom_
           , xScale_, yScale_, zScale_, xVPShift_, yVPShift_;
	
	bool ortho_;
	
  // mouse
  QPoint lastMouseMovePosition_;
	bool mpressed_;

	MouseState xrot_mstate_, 
			yrot_mstate_, 
			zrot_mstate_, 
			xscale_mstate_, 
			yscale_mstate_, 
			zscale_mstate_,
      zoom_mstate_,
			xshift_mstate_,
			yshift_mstate_;

	bool mouse_input_enabled_;

	void setRotationMouse(MouseState bstate, double accel, QPoint diff);
	void setScaleMouse(MouseState bstate, double accel, QPoint diff);
	void setShiftMouse(MouseState bstate, double accel, QPoint diff);

  // keyboard
	bool kpressed_;

	KeyboardState xrot_kstate_[2], 
			yrot_kstate_[2], 
			zrot_kstate_[2], 
			xscale_kstate_[2], 
			yscale_kstate_[2], 
			zscale_kstate_[2],
      zoom_kstate_[2],
			xshift_kstate_[2],
			yshift_kstate_[2];

	bool kbd_input_enabled_;
  double kbd_rot_speed_, kbd_scale_speed_, kbd_shift_speed_;

	void setRotationKeyboard(KeyboardState kseq, double speed);
	void setScaleKeyboard(KeyboardState kseq, double speed);
	void setShiftKeyboard(KeyboardState kseq, double speed);

  // lighting
  struct Light
  {  
    Light() : unlit(true){}
    bool unlit;  
    Qwt3D::Triple rot;
    Qwt3D::Triple shift;
  };
  std::vector<Light> lights_;
  bool lighting_enabled_;
  void applyLight(unsigned idx);

  bool initializedGL_;
};


} // ns

#endif /* include guard */
