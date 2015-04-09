#ifndef _BSDFINTERFACE_H_
  #define _BSDFINTERFACE_H_

  #include <Common.h>
  #include <RandomGenerator.h>

  class BsdfInterface
  {
  public:
    virtual ~BsdfInterface() {}
    
    virtual Vector3f sample(const Vector3f &_incident, const Vector3f &_normal, RandomGenerator &_random_generator) const =0;
    virtual float evaluate(const Vector3f &_incident, const Vector3f &_reflection, const Vector3f &_normal) const =0;
    virtual float pdf(const Vector3f &_incident, const Vector3f &_reflection, const Vector3f &_normal) const =0;
  };

#endif