#include <Aabb.h>

//Update AABB with same class
void Aabb::updateAabb(const Aabb _input_AABB)
{
  Vector3f input_min = _input_AABB.min();
  Vector3f input_max = _input_AABB.max();

  if (input_min.x() < m_min.x())
  {
    m_min.x() = input_min.x();
  }

  if (input_min.y() < m_min.y())
  {
    m_min.y() = input_min.y();
  }

  if (input_min.z() < m_min.z())
  {
    m_min.z() = input_min.z();
  }

  if (input_max.x() > m_max.x())
  {
    m_max.x() = input_max.x();
  }

  if (input_max.y() > m_max.y())
  {
    m_max.y() = input_max.y();
  }

  if (input_max.z() > m_max.z())
  {
    m_max.z() = input_max.z();
  }
}

//Update AABB with Vector, checking against both min and max values
void Aabb::updateAabb(const Vector3f _input_vector)
{
  if (_input_vector.x() < m_min.x())
  {
    m_min.x() = _input_vector.x();
  }

  if (_input_vector.y() < m_min.y())
  {
    m_min.y() = _input_vector.y();
  }

  if (_input_vector.z() < m_min.z())
  {
    m_min.z() = _input_vector.z();
  }

  if (_input_vector.x() > m_max.x())
  {
    m_max.x() = _input_vector.x();
  }

  if (_input_vector.y() > m_max.y())
  {
    m_max.y() = _input_vector.y();
  }

  if (_input_vector.z() > m_max.z())
  {
    m_max.z() = _input_vector.z();
  }
}

//Ray box intersection test following the algrothum described by Williams et. al. in the paper "An Efﬁcient and Robust Ray–Box Intersection Algorithm"
bool Aabb::intersect(const Ray3f _ray) const
{
  float x_min, x_max;
  if (_ray.dir.x() > 0.f)
  {
    x_min = (m_min.x() - _ray.origin.x()) / _ray.dir.x();
    x_max = (m_max.x() - _ray.origin.x()) / _ray.dir.x();
  }
  else
  {
    x_min = (m_max.x() - _ray.origin.x()) / _ray.dir.x();
    x_max = (m_min.x() - _ray.origin.x()) / _ray.dir.x();
  }

  float y_min, y_max;
  if (_ray.dir.y() > 0.f)
  {
    y_min = (m_min.y() - _ray.origin.y()) / _ray.dir.y();
    y_max = (m_max.y() - _ray.origin.y()) / _ray.dir.y();
  }
  else
  {
    y_min = (m_max.y() - _ray.origin.y()) / _ray.dir.y();
    y_max = (m_min.y() - _ray.origin.y()) / _ray.dir.y();
  }

  if ((x_min > y_max) || (y_min > x_max))
  {
    return false;
  }

  if (y_min > x_min)
  {
    x_min = y_min;
  }

  if (y_max < x_max)
  {
    x_max = y_max;
  }

  float z_min, z_max;
  if (_ray.dir.z() > 0.f)
  {
    z_min = (m_min.z() - _ray.origin.z()) / _ray.dir.z();
    z_max = (m_max.z() - _ray.origin.z()) / _ray.dir.z();
  }
  else
  {
    z_min = (m_max.z() - _ray.origin.z()) / _ray.dir.z();
    z_max = (m_min.z() - _ray.origin.z()) / _ray.dir.z();
  }

  if ((x_min > z_max) || (z_min > x_max))
  {
    return false;
  }

  if (z_min > x_min)
  {
    x_min = z_min;
  }

  if (z_max < x_max)
  {
    x_max = z_max;
  }

  return (x_max > 0.f);
}
