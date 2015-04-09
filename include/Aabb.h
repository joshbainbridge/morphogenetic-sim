#ifndef _AABB_H_
  #define _AABB_H_

  #include <Common.h>

  class Aabb
  {
  public:
    Aabb() : m_min(Vector3f(0.f, 0.f, 0.f)), m_max(Vector3f(0.f, 0.f, 0.f)) {;}
    Aabb(const Vector3f _min, const Vector3f _max) : m_min(_min), m_max(_max) {;}

    inline Vector3f min() const {return m_min;}
    inline Vector3f max() const {return m_max;}

    void updateAabb(const Aabb _input_AABB);
    void updateAabb(const Vector3f _input_vector);

    bool intersect(const Ray3f _ray) const;

  private:
    Vector3f m_min;
    Vector3f m_max;
  };

#endif
