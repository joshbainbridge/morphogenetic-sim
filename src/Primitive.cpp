#include <Primitive.h>

Primitive::Primitive(boost::shared_ptr<ShapeInterface> _shape, boost::shared_ptr<Material> _material)
{
  m_shape = _shape;
  m_material = _material;
  m_primitive_count = 0;

  m_transform <<
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1;

  m_aabb.updateAabb(Aabb(m_shape->AABBmin(), m_shape->AABBmax()));
}

//Update AABB according to its shape member and primitive hierarchy
void Primitive::updateAabb()
{
  m_aabb.updateAabb(Aabb(m_shape->AABBmin(), m_shape->AABBmax()));
  for (int i = 0; i < m_primitive_count; ++i)
  {
    m_primitives[i]->updateAabb();
    m_aabb.updateAabb(m_primitives[i]->aabb());
  }
}

//Find primitve intersection, discarding rays that don't intersect the AABB
bool Primitive::intersect(const Ray3f _ray, Transmission *_output) const
{
  //Intersect with bounding volume
  if(m_aabb.intersect(_ray))
  {
    //If there are primitives, test them (if not, then just test shape)
    if (m_primitive_count != 0)
    {
      bool intersection_found = false;
      Transmission closest_intersection;

      //Loop through and test all primitives
      for (int i = 0; i < m_primitive_count; ++i)
      {
        Transmission primitive_intersection;
        if(m_primitives[i]->intersect(_ray, &primitive_intersection) && (primitive_intersection.distance < closest_intersection.distance))
        {
          closest_intersection = primitive_intersection;
          intersection_found = true;
        }
      }

      //Now test against shape
      Transmission primitive_intersection;
      if (m_shape->intersect(_ray, &primitive_intersection) && (primitive_intersection.distance < closest_intersection.distance))
      {
        closest_intersection = primitive_intersection;
        closest_intersection.material = m_material;
        intersection_found = true;
      }

      //If intersection found, set the output
      if (intersection_found)
      {
        *_output = closest_intersection;
        return true;
      }
    }
    else
    {
      //Just intersect shape
      if (m_shape->intersect(_ray, _output))
      {
        _output->material = m_material;
        return true;
      }
    }
  }

  return false;
}

//Add primitive to form a bounding volume hierarchy
void Primitive::addPrimitive(boost::shared_ptr<Primitive> _primitive)
{
  m_primitives.push_back(_primitive);
  ++m_primitive_count;
}

//Return primitive using index
boost::shared_ptr<Primitive> Primitive::getPrimitive(const int _index) const
{
  return m_primitives[_index];
}

//Remove all nested primitives
void Primitive::clearPrimitives()
{
  m_primitives.clear();
  m_primitive_count = 0;
}
