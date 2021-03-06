#include <cmath>

#include <QuadLight.h>

//Uniformally sample across light surface
Vector3f QuadLight::sampleLight(RandomGenerator &_random_generator) const
{
  float x_rand = (_random_generator.getValue() - 0.5f) * m_width;
  float y_rand = (_random_generator.getValue() - 0.5f) * m_height;

  return m_origin + (m_right * x_rand) + (m_up * y_rand);
}

//Get implicit lighting contribution from light
Colour3f QuadLight::illumination(const Vector3f &light_direction) const
{
  Colour3f illumination = m_illum * m_width * m_height;
  float light_falloff = 1.f / light_direction.squaredNorm();
  float cosine_distrobution = std::pow(light_direction.normalized().dot(m_dir * -1), m_sine_exponent);
  
  return illumination * light_falloff * cosine_distrobution;
}
