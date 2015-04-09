#include <Material.h>

Material::Material(boost::shared_ptr< BsdfInterface > _bsdf)
{
  m_colour = Colour3f(0.f, 0.f, 0.f);
  m_bsdf = _bsdf;
}

//Get ray sample from surface using the bsdf
Ray3f Material::sample(const Vector3f &_incident, const Vector3f &_position, const Vector3f &_normal, RandomGenerator &_random_generator) const
{
  Ray3f output;
  output.dir = m_bsdf->sample(_incident, _normal, _random_generator);
  output.origin = _position;
  return output;
}

//Evaluate material taking into consideration the bsdf
Colour3f Material::evaluate(const Vector3f &_incident, const Vector3f &_reflection, const Vector3f &_position, const Vector3f &_normal) const
{
  return m_colour * m_bsdf->evaluate(_incident, _reflection, _normal);
}
