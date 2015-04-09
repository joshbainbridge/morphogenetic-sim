#ifndef _TASK_H_
  #define _TASK_H_

  struct Task
  {
    Task()
      : pixel_x(0)
      , pixel_y(0)
      , width(0)
      , height(0)
      , samples(0)
    {;}
    
    int pixel_x;
    int pixel_y;
    int width;
    int height;
    int samples;
  };

#endif
