#include <iostream>

#include <TaskStack.h>

//Add a task to the stack for later rendering
void TaskStack::addTask(const Task _task)
{
  m_tasks.push_back(_task);
  m_count += 1;
  m_total += 1;
}

//Take a task from the stack for processing
int TaskStack::takeTask(Task *_task)
{
  int available_tasks = 1;
  
  //If there are tasks then send, else, reset task stack
  m_mutex.lock();
  if (m_count != 0)
  {
    *_task = m_tasks.back();
    m_tasks.pop_back();
    m_count -= 1;

    float current_percentage = static_cast<float>(m_count) / static_cast<float>(m_total);
    if (current_percentage <= m_percentage - 0.05f)
    {
      m_percentage = current_percentage;
      std::cout << "Current percentage complete: " << std::fixed << std::setprecision(3) << 100.f - m_percentage * 100.f << std::endl;
    }
  }
  else
  {
    available_tasks = 0;
    m_total = 0;
    m_percentage = 1.f;
  }
  m_mutex.unlock();

  return available_tasks;
}
