#include <GeometricCamera.h>

GeometricCamera::GeometricCamera()
{
  m_origin = Vector3f(0.f, 0.f, 0.f);
  m_dir = Vector3f(0.f, 0.f, 1.f);
  m_up = Vector3f(0.f, 1.f, 0.f);
  m_right = Vector3f(1.f, 0.f, 0.f);
  m_dist = 1.f;
  m_ratio = 1.f;
  m_aperture = 1.f;
  m_focal_dist = 100.f;
}

//Generate initial rays from camera
void GeometricCamera::makeSample(const float xpos, const float ypos, Vector3f *ray_origin, Vector3f *ray_direction, RandomGenerator &_random_generator) const
{
  //Find position on film plane taking into account image ratio
  Vector3f film_position = m_origin + (m_right * m_ratio * xpos) + (m_up * ypos);

  //Construct nodal position
  Vector3f nodal_point = m_origin + (m_dir * m_dist);
  float nodal_focal_ratio = m_focal_dist / m_dist;

  //Find ray intersection on focal plane
  Vector3f focal_point = (nodal_point - film_position) * nodal_focal_ratio + film_position;

  //Get sample position on lens
  Vector2f aperture_sample = rejection_sample_circle(_random_generator) * m_aperture;
  Vector3f aperture_position = (m_right * aperture_sample.x()) + (m_up * aperture_sample.y());

  //Given x and y position between -1 and 1 generate ray postioned on camera plane and calculate direction
  *ray_origin = nodal_point + aperture_position;
  *ray_direction = (focal_point - *ray_origin).normalized();
}

//Rejection sampling used for creating rays across the lens
Vector2f GeometricCamera::rejection_sample_circle(RandomGenerator &_random_generator) const
{
  //Initial values
  Vector2f output(0.f, 0.f);
  bool ray_found = false;

  //While sample not found
  while (ray_found != true)
  {
    //Get random values between 0 and 1 then construct vector
    float x_rand = _random_generator.getValue() - 0.5f;
    float y_rand = _random_generator.getValue() - 0.5f;
    output = Vector2f(x_rand, y_rand);

    //Is vector within sphere
    if (output.norm() < 1.f)
    {
      //Use vector
      ray_found = true;
    }
  }

  return output;
}
