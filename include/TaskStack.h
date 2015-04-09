#ifndef _TASKSTACK_H_
  #define _TASKSTACK_H_
  
  #include <vector>
  #include <boost/thread.hpp>

  #include <Task.h>

  class TaskStack
  { 
  public:
    TaskStack()
      : m_count(0)
      , m_total(0)
      , m_percentage(1.f)
    {;}

    void addTask(const Task _task);
    int takeTask(Task *_task);

  private:
    std::vector<Task> m_tasks;
    int m_count;
    int m_total;
    float m_percentage;
    boost::mutex m_mutex;
  };
  
#endif
