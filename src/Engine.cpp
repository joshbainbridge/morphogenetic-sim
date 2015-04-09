#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

//Removed to avoid issues concerning C++11 features
//#include <boost/chrono.hpp>
#include <lodepng.h>

#include <Engine.h>
#include <Common.h>
#include <PathtraceIntegrator.h>
#include <GeometricCamera.h>
#include <LambertBsdf.h>
#include <Material.h>
#include <SphereShape.h>
#include <QuadLight.h>
#include <TissueShape.h>
#include <RandomGenerator.h>

Engine::Engine()
{
  //Create image for writing data
  boost::shared_ptr<Image> image(new Image());
  m_image = image;

  //Create stack for render tasks
  boost::shared_ptr<TaskStack> task_stack(new TaskStack());
  m_task_stack = task_stack;
}

//Main render function
void Engine::run(boost::shared_ptr<Scene> _scene, boost::shared_ptr<Settings> _settings)
{
  construct(_scene, _settings);
  createThreads();
  createTasks();
  startThreads();
  saveImage();
  clean();
}

//Construct scene and settings data
void Engine::construct(boost::shared_ptr<Scene> _scene, boost::shared_ptr<Settings> _settings)
{
  m_scene = _scene;
  m_settings = _settings;
  m_image->resolution(m_settings->resolution_x, m_settings->resolution_y);

  for (int i = 0; i < m_scene->countPrimitive(); ++i)
  {
    m_scene->getPrimitive(i)->updateAabb();
  }
}

//Check system for available threads and create RenderThread objects
void Engine::createThreads()
{
  m_nthreads = boost::thread::hardware_concurrency();
  if (m_nthreads != 0)
  {
    std::cout << m_nthreads << " supported threads found." << std::endl;
  }
  else
  {
    m_nthreads = 1;
    std::cout << "0 supported threads found, system won't have thread level parallelization." << std::endl;
  }

  for (int i = 0; i < m_nthreads; ++i)
  {
    //Create a new intergrator for each thread
    boost::shared_ptr<PathtraceIntegrator> integrator(new PathtraceIntegrator());
    boost::shared_ptr<RenderThread> thread(new RenderThread(m_scene, m_settings, m_image, m_task_stack, integrator));
    
    m_render_threads.push_back(thread);
  }
}

//Take image resolution and bucket size to construct tasks for threads
void Engine::createTasks()
{
  int bucket_size = m_settings->bucket_size;
  int resolution_x = m_settings->resolution_x;
  int resolution_y = m_settings->resolution_y;
  int samples = m_settings->samples;
  int task_count_x = std::ceil(resolution_x / static_cast<float>(bucket_size));
  int task_count_y = std::ceil(resolution_y / static_cast<float>(bucket_size));
  int task_count = task_count_x * task_count_y;

  std::cout << task_count << " render tasks created." << std::endl;

  for (int i = 0; i < task_count_x; ++i)
  {
    for (int j = 0; j < task_count_y; ++j)
    {
      //Check for any pixels left after resolution division with bucket size
      int width = bucket_size;
      if ((i * bucket_size + bucket_size) > resolution_x)
      {
        width = resolution_x - (i * bucket_size);
      }

      //Check for any pixels left after resolution division with bucket size
      int height = bucket_size;
      if ((j * bucket_size + bucket_size) > resolution_y)
      {
        height = resolution_y - (j * bucket_size);
      }

      Task task;
      task.pixel_x = i * bucket_size;
      task.pixel_y = j * bucket_size;
      task.width = width;
      task.height = height;
      task.samples = samples;

      m_task_stack->addTask(task);
    }
  }
}

//Start the render threads and join when finished
void Engine::startThreads()
{
//  boost::chrono::high_resolution_clock::time_point timer_start = boost::chrono::high_resolution_clock::now();
  
  for (int i = 0; i < m_nthreads; ++i)
  {
    m_render_threads[i]->start();
  }

  for (int i = 0; i < m_nthreads; ++i)
  {
    m_render_threads[i]->join();
  }
  
  std::cout << "Image saved to build directory." << std::endl;

//  boost::chrono::high_resolution_clock::time_point timer_end = boost::chrono::high_resolution_clock::now();

//  std::cout << "Rendering time was "
//            << boost::chrono::duration_cast<boost::chrono::milliseconds>(timer_end - timer_start).count()
//            << " milliseconds."
//            << std::endl;
}

//Get the image colour data and save to file
void Engine::saveImage()
{
  std::vector< std::vector< Colour3f > > image_output;
  image_output = m_image->getImage();

  std::vector<unsigned char> image(m_settings->resolution_x * m_settings->resolution_y * 4);

  for (int i = 0; i < m_settings->resolution_y; ++i)
  {
    for (int j = 0; j < m_settings->resolution_x; ++j)
    {
      image[4 * m_settings->resolution_x * i + 4 * j + 0] = static_cast<unsigned char>(std::floor(image_output[j][i].x() * 255.f));
      image[4 * m_settings->resolution_x * i + 4 * j + 1] = static_cast<unsigned char>(std::floor(image_output[j][i].y() * 255.f));
      image[4 * m_settings->resolution_x * i + 4 * j + 2] = static_cast<unsigned char>(std::floor(image_output[j][i].z() * 255.f));
      image[4 * m_settings->resolution_x * i + 4 * j + 3] = 255;
    }
  }
  
  std::string framenumber_str = boost::to_string(m_settings->frame_number);
  std::string framepadding = std::string( 4 - framenumber_str.size(), '0');
  std::string filename = "output_" + framepadding + framenumber_str + ".png";
  lodepng::encode(filename, image, m_settings->resolution_x, m_settings->resolution_y);
}

//Remove render threads
void Engine::clean()
{
  m_render_threads.clear();
}
