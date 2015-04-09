#ifndef _QUADLIGHT_H_
  #define _QUADLIGHT_H_

  #include <LightInterface.h>
  #include <RandomGenerator.h>

  class QuadLight : public LightInterface
  {
  public:
    QuadLight() : LightInterface() {;}

    inline void illumination(const Colour3f &_illum){m_illum = _illum;}
    Colour3f illumination(const Vector3f &light_direction) const;

    Vector3f sampleLight(RandomGenerator &_random_generator) const;
  };

#endif
