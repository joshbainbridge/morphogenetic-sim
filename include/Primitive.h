#ifndef _PRIMITIVE_H_
  #define _PRIMITIVE_H_

  #include <vector>

  #include <boost/shared_ptr.hpp>

  #include <Common.h>
  #include <Material.h>
  #include <ShapeInterface.h>
  #include <Transmission.h>
  #include <Aabb.h>

  class Primitive
  {
  public:
    Primitive(boost::shared_ptr<ShapeInterface> _shape, boost::shared_ptr<Material> _material);

    inline TransMatrix transform() const {return m_transform;}
    inline Aabb aabb() const {return m_aabb;}

    inline void transform(const TransMatrix &_transform){m_transform = _transform;}

    bool intersect(const Ray3f _ray, Transmission *_output) const;

    void addPrimitive(boost::shared_ptr<Primitive> _primitive);
    boost::shared_ptr<Primitive> getPrimitive(const int _index) const;
    void clearPrimitives();

    void updateAabb();

  private:
    boost::shared_ptr<ShapeInterface> m_shape;
    boost::shared_ptr<Material> m_material;
    std::vector< boost::shared_ptr<Primitive> > m_primitives;
    int m_primitive_count;
    TransMatrix m_transform;
    Aabb m_aabb;
  };

#endif
