#include <MainWindow.h>
#include <ui_MainWindow.h>

#include <cmath>
#include <boost/shared_ptr.hpp>

#include <Common.h>
#include <GeometricCamera.h>
#include <LambertBsdf.h>
#include <Material.h>
#include <QuadLight.h>

//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  //Create an openGL format and pass to the new GLWidget
  QGLFormat format;
  format.setVersion(3,2);
  format.setProfile( QGLFormat::CoreProfile);

  //Setup the user interface
  m_ui->setupUi(this);

  //Create our GL window for drawing the spring
  m_gl=new GLWindow(format,m_ui->m_timerValue->value(),this);

  //Add the glWindow to the UI
  m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,12,1);

  //Create render objects as well as tissue simulation with member pointers
  m_pathtracer = boost::shared_ptr<Engine>(new Engine());
  m_scene = boost::shared_ptr<Scene>(new Scene());
  m_settings = boost::shared_ptr<Settings>(new Settings());
  m_tissue = boost::shared_ptr<TissueShape>(new TissueShape());

  //Create other render objects that don't need to be directly accessed
  boost::shared_ptr<LambertBsdf> bsdf(new LambertBsdf());
  boost::shared_ptr<Material> material(new Material(bsdf));
  boost::shared_ptr<Primitive> primitive(new Primitive(m_tissue, material));
  boost::shared_ptr<GeometricCamera> camera(new GeometricCamera());
  boost::shared_ptr<QuadLight> light(new QuadLight());

  //Innitial render settings
  m_settings->resolution_x = m_ui->m_resolution_x->value();
  m_settings->resolution_y = m_ui->m_resolution_y->value();
  m_settings->bucket_size = m_ui->m_bucket_size->value();
  m_settings->samples = m_ui->m_samples->value();
  m_settings->depth = m_ui->m_depth->value();
  m_settings->absorption = m_ui->m_absorption->value();

  //Innitial simulation settings
  m_tissue->springFactor(m_ui->m_spring_factor->value());
  m_tissue->planarFactor(m_ui->m_planar_factor->value());
  m_tissue->bulgeFactor(m_ui->m_bulge_factor->value());
  m_tissue->linkRestLength(m_ui->m_link_rest_length->value());
  m_tissue->repulsionFactor(m_ui->m_repulsion_factor->value());
  m_tissue->repulsionRadius(m_ui->m_repulsion_radius->value());
  m_tissue->timeStep(m_ui->m_dt->value());

  //Material colour
  material->colour(Colour3f(0.9f, 0.9f, 0.9f));

  //Innitial camera settings
  camera->origin(Vector3f(0.f, 0.f, 1250.f));
  camera->direction(Vector3f(0.f, 0.f, -1.f));
  //This sets the vertical FOV to 35 degrees
  camera->distance(1.f / std::tan(30.f * 0.5f * M_PI_180));
  camera->ratio(static_cast<float>(m_settings->resolution_x) / static_cast<float>(m_settings->resolution_y));
  camera->focalDistance(1150.f);
  camera->aperture(75.f);

  //Light data
  light->origin(Vector3f(0.f, 1200.f, 0.f));
  light->direction(Vector3f(0.f, -1.f, 0.f));
  light->height(900.f);
  light->width(900.f);
  light->illumination(Colour3f(1.85f, 1.63f, 1.41f));
  light->exponent(2.f);

  //Set camera, add simulation as a primitive and add a light
  m_scene->setCamera(camera);
  m_scene->addPrimitive(primitive);
  m_scene->addLight(light);

  //Send tissue object to OpenGl as it is chared
  m_gl->setTissue(m_tissue);

  // now we wire up the UI components to the slots
  connect(m_ui->m_resolution_x,SIGNAL(valueChanged(int)),this,SLOT(setResolutionX(int)));
  connect(m_ui->m_resolution_y,SIGNAL(valueChanged(int)),this,SLOT(setResolutionY(int)));
  connect(m_ui->m_bucket_size,SIGNAL(valueChanged(int)),this,SLOT(setBucketSize(int)));
  connect(m_ui->m_samples,SIGNAL(valueChanged(int)),this,SLOT(setSamples(int)));
  connect(m_ui->m_depth,SIGNAL(valueChanged(int)),this,SLOT(setDepth(int)));
  connect(m_ui->m_absorption,SIGNAL(valueChanged(double)),this,SLOT(setAbsorption(double)));

  connect(m_ui->m_spring_factor,SIGNAL(valueChanged(double)),this,SLOT(setSpingFactor(double)));
  connect(m_ui->m_planar_factor,SIGNAL(valueChanged(double)),this,SLOT(setPlanarFactor(double)));
  connect(m_ui->m_bulge_factor,SIGNAL(valueChanged(double)),this,SLOT(setBulgeFactor(double)));
  connect(m_ui->m_link_rest_length,SIGNAL(valueChanged(double)),this,SLOT(setLinkRestLength(double)));
  connect(m_ui->m_repulsion_factor,SIGNAL(valueChanged(double)),this,SLOT(setRepulsionFactor(double)));
  connect(m_ui->m_repulsion_radius,SIGNAL(valueChanged(double)),this,SLOT(setRepulsionRadius(double)));

  connect(m_ui->m_render,SIGNAL(clicked()),this,SLOT(render()));
  connect(m_ui->m_sim,SIGNAL(clicked(bool)),this,SLOT(toggleSim(bool)));
  connect(m_ui->m_reset,SIGNAL(clicked()),m_gl,SLOT(reset()));
  connect(m_ui->m_timerValue,SIGNAL(valueChanged(int)),m_gl,SLOT(setTimerDuration(int)));
  connect(m_ui->m_dt,SIGNAL(valueChanged(double)),this,SLOT(setDT(double)));
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete m_ui;
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::toggleSim(bool _s)
{
  if(_s == true)
  {
    m_gl->startSimTimer();
  }
  else
  {
    m_gl->stopSimTimer();
  }
}

//Render using offline renderer
void MainWindow::render()
{
  //Update camera position to match OpenGL
  Vector3f camera_position = m_gl->getCameraPos();
  Vector3f tissue_position = m_tissue->position();
  m_scene->getCamera()->origin(camera_position + tissue_position);
  m_scene->getCamera()->direction(camera_position * -1);

  m_pathtracer->run(m_scene, m_settings);
}
