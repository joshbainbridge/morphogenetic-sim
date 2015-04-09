#ifndef _LAMBERTBSDF_H_
  #define _LAMBERTBSDF_H_

  #include <Common.h>
  #include <RandomGenerator.h>
  #include <BsdfInterface.h>

  class LambertBsdf : public BsdfInterface
  {
  public:    
    Vector3f sample(const Vector3f &_incident, const Vector3f &_normal, RandomGenerator &_random_generator) const;
    float evaluate(const Vector3f &_incident, const Vector3f &_reflection, const Vector3f &_normal) const;
    float pdf(const Vector3f &_incident, const Vector3f &_reflection, const Vector3f &_normal) const;
    
  private:
    Vector3f rejectionSample(const Vector3f &_normal, RandomGenerator &_random_generator) const;
  };

#endif