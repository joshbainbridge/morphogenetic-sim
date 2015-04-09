#include <SphereShape.h>

//Spheres interesection function
bool SphereShape::intersect(const Ray3f _ray, Transmission *_output) const
{
  //Sphere position relative to ray origin
  Vector3f relative_pos = m_pos - _ray.origin;
  //Scalar to point of projection of sphere position onto ray direction
  float projection_scalar = relative_pos.dot(_ray.dir);

  //Is sphere in front of ray
  if (projection_scalar > 0.f)
  {
    //Vector of projection from sphere position onto ray direction
    Vector3f projection_vector = _ray.dir * projection_scalar;
    //Distance from projected vector to sphere
    float position_projection_scalar = (projection_vector - relative_pos).norm();

    //Does ray intersect sphere
    if (position_projection_scalar < m_rad)
    {
      //Find intersection
      _output->distance = projection_scalar - std::sqrt((m_rad * m_rad) - (position_projection_scalar * position_projection_scalar));
      _output->position = _ray.origin + _ray.dir * (_output->distance);
      _output->normal = (_output->position - m_pos).normalized();
      return true;
    }
  }

  return false;
}
