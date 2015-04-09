#ifndef _CAMERAINTERFACE_H_
  #define _CAMERAINTERFACE_H_

  #include <Common.h>
  #include <RandomGenerator.h>

  class CameraInterface
  {
  public:
    virtual ~CameraInterface() {}

    inline Vector3f direction() const {return m_dir;}
    inline Vector3f origin() const {return m_origin;}
    inline Vector3f up() const {return m_up;}
    inline Vector3f right() const {return m_right;}
    inline float distance() const {return m_dist;}
    inline float ratio() const {return m_ratio;}

    inline void origin(const Vector3f &_origin){m_origin = _origin;}
    inline void distance(const float &_dist){m_dist = _dist;}
    inline void ratio(const float &_ratio){m_ratio = _ratio;}

    void direction(const Vector3f &_dir);

    virtual void makeSample(const float xpos, const float ypos, Vector3f *ray_origin, Vector3f *ray_direction, RandomGenerator &_random_generator) const =0;

  protected:
    //Protected variables
    Vector3f m_origin;
    Vector3f m_dir;
    Vector3f m_up;
    Vector3f m_right;
    float m_dist;
    float m_ratio;
  };

#endif
