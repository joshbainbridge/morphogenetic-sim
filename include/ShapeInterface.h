#ifndef _SHAPEINTERFACE_H_
  #define _SHAPEINTERFACE_H_

  #include <Common.h>
  #include <Transmission.h>

  class ShapeInterface
  {
  public:
    virtual ~ShapeInterface() {}

    virtual bool intersect(const Ray3f _ray, Transmission *_output) const =0;

    virtual Vector3f position() const =0;
    virtual Vector3f AABBmin() const =0;
    virtual Vector3f AABBmax() const =0;
  };

#endif
