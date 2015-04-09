#include <LambertBsdf.h>

//Create ray samples (if this were not a lambert model the inverse of pdf would be used to construct a cdf for importance sampling)
Vector3f LambertBsdf::sample(const Vector3f &_incident, const Vector3f &_normal, RandomGenerator &_random_generator) const
{
  return rejectionSample(_normal, _random_generator);
}

//Evaluate bsdf taking into account the surface differential and normalized distrubution
float LambertBsdf::evaluate(const Vector3f &_incident, const Vector3f &_reflection, const Vector3f &_normal) const
{
  return _reflection.dot(_normal) * pdf(_incident, _reflection, _normal);
}

//Return the probability distrobution of the hemispheres intergral (this is always one when the surafce is perfectly diffuse)
float LambertBsdf::pdf(const Vector3f &_incident, const Vector3f &_reflection, const Vector3f &_normal) const
{
  return 1.f;
}

//Rejection sample the hemisphere to find ray direction
Vector3f LambertBsdf::rejectionSample(const Vector3f &_normal, RandomGenerator &_random_generator) const
{
  //Initial values
  Vector3f output(0.f, 0.f, 0.f);
  bool ray_found = false;
  
  //While sample not found
  while (ray_found != true)
  {
    //Get random values between 0 and 1 then construct vector
    float x_rand = _random_generator.getValue() * 2.f - 1.f;
    float y_rand = _random_generator.getValue() * 2.f - 1.f;
    float z_rand = _random_generator.getValue() * 2.f - 1.f;
    output = Vector3f(x_rand, y_rand, z_rand);
    
    //Is vector within sphere
    if (output.norm() < 1.f)
    {
      //Normalize vector to sphere surface
      output = output.normalized();
      
      //Is vector in front of surface
      if (output.dot(_normal) > 0.f)
      {
        //Use vector
        ray_found = true;
      }
    }
  }
  
  return output;
}
