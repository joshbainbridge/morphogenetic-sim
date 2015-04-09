#ifndef _RENDERTHREAD_H_
  #define _RENDERTHREAD_H_

  #include <boost/thread.hpp>
  #include <boost/shared_ptr.hpp>

  #include <Scene.h>
  #include <Settings.h>
  #include <Image.h>
  #include <TaskStack.h>
  #include <Task.h>
  #include <IntegratorInterface.h>
  #include <RandomGenerator.h>

  class RenderThread
  { 
  public:
    RenderThread(
        boost::shared_ptr<Scene> _scene_ptr,
        boost::shared_ptr<Settings> _settings_ptr,
        boost::shared_ptr<Image> _image_ptr,
        boost::shared_ptr<TaskStack> _task_stack_ptr,
        boost::shared_ptr<IntegratorInterface> _integrator
        );

    void start();
    void join();
    void process();

  private:
    boost::thread m_thread;
    boost::shared_ptr<Scene> m_scene_ptr;
    boost::shared_ptr<Settings> m_settings_ptr;
    boost::shared_ptr<Image> m_image_ptr;
    boost::shared_ptr<RandomGenerator> m_random_generator;
    boost::shared_ptr<TaskStack> m_task_stack_ptr;
    boost::shared_ptr<IntegratorInterface> m_integrator;
    Task m_task;
  };
  
#endif
