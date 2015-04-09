#include "GLWindow.h"
#include <iostream>
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/Obj.h>
#include <ngl/Random.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <boost/foreach.hpp>

#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(const QGLFormat _format, int _timer, QWidget *_parent ) : QGLWidget( _format, _parent )
{
  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize( _parent->size() );
  // Now set the initial GLWindow attributes to default values
  m_rotate = false;
  m_spinXFace = 0;
  m_spinYFace = 0;

  m_timerValue = _timer;

  m_cell_count = 0;
}

//This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
//This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{
  ngl::NGLInit::instance();
  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  //Setup camera
  ngl::Vec3 from(0.f, 0.f, 1250.f);
  ngl::Vec3 to(0.f, 0.f, 0.f);
  ngl::Vec3 up(0.f, 1.f, 0.f);
  m_cam = new ngl::Camera(from, to, up);
  m_cam->setShape(30.f, 1.f, 50.f, 1600.f);

  //Construct shader
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->createShaderProgram("Diffuse");
  shader->attachShader("DiffuseVertex",ngl::VERTEX);
  shader->attachShader("DiffuseFragment",ngl::FRAGMENT);
  shader->loadShaderSource("DiffuseVertex","shaders/DiffuseVertex.glsl");
  shader->loadShaderSource("DiffuseFragment","shaders/DiffuseFragment.glsl");
  shader->compileShader("DiffuseVertex");
  shader->compileShader("DiffuseFragment");
  shader->attachShaderToProgram("Diffuse","DiffuseVertex");
  shader->attachShaderToProgram("Diffuse","DiffuseFragment");
  shader->linkProgramObject("Diffuse");
  (*shader)["Diffuse"]->use();

  //Set light information (light intensity is that of the lights area)
  shader->setShaderParam1f("Light_Intensity", 900.f * 900.f);
  shader->setShaderParam3f("Light_Colour", 1.65f, 1.6f, 1.55f);
  shader->setShaderParam3f("Light_Position", 0.f, 1200.f, 0.f);
  shader->setShaderParam3f("Light_Direction", 0.f, -1.f, 0.f);

  //Create sphere primitive
  ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
  prim->createSphere("Sphere", 1.f, 20);

  //Get innitial data from simulation
  m_cell_count = m_tissue->exportData(m_tissue_data);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow::resizeGL( int _w, int _h)
{
  glViewport(0, 0, _w, _h);
  m_cam->setShape(30.f, static_cast<float>(_w) / static_cast<float>(_h), 30.f, 1600.f);
}

//Load camera data to shaders
void GLWindow::loadMatricesToShader()
{
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Diffuse"]->use();

  ngl::Mat4 M;
  ngl::Mat4 MV;
  ngl::Mat4 MVP;

  M = m_transform.getMatrix();
  MV = M * m_mouseGlobalTX * m_cam->getViewMatrix();
  MVP = MV * m_cam->getProjectionMatrix();

  shader->setShaderParamFromMat4("MVP", MVP);
  shader->setShaderParamFromMat4("M", M);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void GLWindow::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Update camera data
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  m_mouseGlobalTX = rotY * rotX;

  //Find position of simulation
  Vector3f tissue_position = m_tissue->position() * -1;

  //Set colour and offset light postion with simulation position
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Diffuse"]->use();
  shader->setShaderParam4f("Colour", 0.9f, 0.9f, 0.9f, 1.f);
  shader->setShaderParam3f("Light_Position", tissue_position.x(), tissue_position.y() + 1200.f, tissue_position.z());

  ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

  //Draw simulation taking into account position
  for (int i = 0; i < m_cell_count; ++i)
  {
    ngl::Vec4 position = ngl::Vec4(m_tissue_data[i * 4 + 0] + tissue_position.x(), m_tissue_data[i * 4 + 1] + tissue_position.y(), m_tissue_data[i * 4 + 2] + tissue_position.z(), 0.f);
    m_transform.setScale(m_tissue_data[i * 4 + 3], m_tissue_data[i * 4 + 3], m_tissue_data[i * 4 + 3]);
    m_transform.setPosition(position);
    loadMatricesToShader();

    // draw the sphere
    prim->draw("Sphere");
  }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent ( QMouseEvent * _event )
{
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    m_spinYFace = ( m_spinYFace + (_event->x() - m_origX) ) % 360 ;
    m_spinXFace = ( m_spinXFace + (_event->y() - m_origY) ) % 360 ;
    m_origX = _event->x();
    m_origY = _event->y();
  }
  // re-draw GL
  updateGL();
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent ( QMouseEvent * _event  )
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate = true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent ( QMouseEvent * _event )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate = false;
  }
}

void GLWindow::timerEvent( QTimerEvent *_event)
{
  m_tissue->simulate();
  m_cell_count = m_tissue->exportData(m_tissue_data);
  updateGL();
}

//Reset simulation to innitial construction
void GLWindow::reset()
{
  m_tissue->construction();
  m_cell_count = m_tissue->exportData(m_tissue_data);
  updateGL();
}

//Return camera position for offline renderer
Vector3f GLWindow::getCameraPos() const
{
  ngl::Mat4 rotation = m_mouseGlobalTX;
  ngl::Vec3 camera_position = rotation.inverse().getBackVector();
  return Vector3f(camera_position.m_x, camera_position.m_y, camera_position.m_z) * 1250.f;
}

void GLWindow::startSimTimer()
{
  m_timer=startTimer(m_timerValue);
}

void GLWindow::stopSimTimer()
{
  killTimer(m_timer);
}

GLWindow::~GLWindow()
{
	ngl::NGLInit *init = ngl::NGLInit::instance();
	init->NGLQuit();
}
