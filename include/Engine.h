#ifndef _ENGINE_H_
  #define _ENGINE_H_

  #include <vector>
  #include <string>

  #include <boost/shared_ptr.hpp>

  #include <Scene.h>
  #include <Settings.h>
  #include <Image.h>
  #include <TaskStack.h>
  #include <RenderThread.h>
  #include <IntegratorInterface.h>

  class Engine
  {
  public:
    Engine();

    void run(boost::shared_ptr<Scene> _scene, boost::shared_ptr<Settings> _settings);

  private:
    boost::shared_ptr<Scene> m_scene;
    boost::shared_ptr<Settings> m_settings;
    boost::shared_ptr<Image> m_image;
    boost::shared_ptr<TaskStack> m_task_stack;
    std::vector< boost::shared_ptr<RenderThread> > m_render_threads;
    int m_nthreads;

    void construct(boost::shared_ptr<Scene> _scene, boost::shared_ptr<Settings> _settings);
    void createThreads();
    void createTasks();
    void startThreads();
    void saveImage();
    void clean();
  };
	
#endif
