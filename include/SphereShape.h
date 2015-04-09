#ifndef _SPHERESHAPE_H_
  #define _SPHERESHAPE_H_

  #include <ShapeInterface.h>
  #include <Common.h>
  #include <Transmission.h>

  class SphereShape : public ShapeInterface
  {
  public:
    SphereShape() : m_pos(Vector3f(0.f, 0.f, 0.f)), m_rad(1.f) {;}

    inline Vector3f position() const {return m_pos;}
    inline float radius() const {return m_rad;}

    inline void position(const Vector3f &_pos){m_pos = _pos;}
    inline void radius(const float _rad){m_rad = _rad;}

    bool intersect(const Ray3f _ray, Transmission *_output) const;

    Vector3f AABBmin() const
    {
      return Vector3f(m_pos.x() - m_rad, m_pos.y() - m_rad, m_pos.z() - m_rad);
    }

    Vector3f AABBmax() const
    {
      return Vector3f(m_pos.x() + m_rad, m_pos.y() + m_rad, m_pos.z() + m_rad);
    }

  protected:
    Vector3f m_pos;
    float m_rad;
  };

#endif
