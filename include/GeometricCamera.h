#ifndef _GEOMETRICCAMERA_H_
  #define _GEOMETRICCAMERA_H_

  #include <Common.h>
  #include <CameraInterface.h>
  #include <RandomGenerator.h>

  class GeometricCamera : public CameraInterface
  {
  public:
    GeometricCamera();

    inline float aperture() const {return m_aperture;}
    inline float focalDistance() const {return m_focal_dist;}

    inline void aperture(const float &_aperture){m_aperture = _aperture;}
    inline void focalDistance(const float &_focal_dist){m_focal_dist = _focal_dist;}

    void makeSample(const float xpos, const float ypos, Vector3f *ray_origin, Vector3f *ray_direction, RandomGenerator &_random_generator) const;

  private:
    float m_aperture;
    float m_focal_dist;

    Vector2f rejection_sample_circle(RandomGenerator &_random_generator) const;
  };

#endif
