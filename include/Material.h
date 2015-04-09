#ifndef _MATERIAL_H_
  #define _MATERIAL_H_

  #include <boost/shared_ptr.hpp>

  #include <Common.h>
  #include <BsdfInterface.h>
  #include <RandomGenerator.h>

  class Material
  {
  public:
    Material(boost::shared_ptr< BsdfInterface > _bsdf);

    inline Colour3f colour() const {return m_colour;}
    inline boost::shared_ptr< BsdfInterface > bsdf() const {return m_bsdf;}

    inline void colour(const Colour3f &_colour){m_colour = _colour;}
    inline void bsdf(const boost::shared_ptr< BsdfInterface > &_bsdf){m_bsdf = _bsdf;}
    
    Ray3f sample(const Vector3f &_incident, const Vector3f &_position, const Vector3f &_normal, RandomGenerator &_random_generator) const;
    Colour3f evaluate(const Vector3f &_incident, const Vector3f &_reflection, const Vector3f &_position, const Vector3f &_normal) const;
    
  private:
    Colour3f m_colour;
    boost::shared_ptr< BsdfInterface > m_bsdf;
  };

#endif
