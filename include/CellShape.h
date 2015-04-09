#ifndef _CELLSHAPE_H_
  #define _CELLSHAPE_H_

  #include <vector>

  #include <boost/shared_ptr.hpp>

  #include <Common.h>
  #include <SphereShape.h>
  #include <RandomGenerator.h>

  class CellShape : public SphereShape
  {
  public:
    CellShape(int _identifier)
      : SphereShape()
      , m_nutrition(0.f)
      , m_identifier(_identifier)
      , m_target_pos(Vector3f(0.f, 0.f, 0.f))
      , m_normal(Vector3f(0.f, 0.f, 0.f))
    {;}

    inline float nutrition() const {return m_nutrition;}
    inline int identifier() const {return m_identifier;}
    inline Vector3f targetPosition() const {return m_target_pos;}
    inline Vector3f normal() const {return m_normal;}
    inline int linkListSize() const {return m_links.size();}
    inline boost::shared_ptr<CellShape> getLink(const int _index) const {return m_links[_index];}

    inline void targetPosition(const Vector3f &_target_pos){m_target_pos = _target_pos;}
    inline void normal(const Vector3f &_normal){m_normal = _normal;}
    inline void nutrition(const float &_nutrition){m_nutrition = _nutrition;}
    inline void identifier(const unsigned int &_identifier){m_identifier = _identifier;}
    inline void setLink(const int _index, boost::shared_ptr<CellShape> _cell){m_links[_index] = _cell;}

    void addLink(const boost::shared_ptr<CellShape> &_link);
    void insertLink(const int _index, const boost::shared_ptr<CellShape> &_link);
    void eraseLink(const int _index);
    int findLink(const int _identifier) const;
    void addNutrition(RandomGenerator &_random_sampler);
    void calculateNormal();
    void findTarget(const float _spring_factor, const float _planar_factor, const float _bulge_factor, const float _link_rest_length);
    void update(const float _time_step);

  private:
    std::vector< boost::shared_ptr<CellShape> > m_links;
    float m_nutrition;
    int m_identifier;
    Vector3f m_target_pos;
    Vector3f m_normal;

    Vector3f springTarget(const float _link_rest_length) const;
    Vector3f planarTarget() const;
    Vector3f bulgeTarget(const float _link_rest_length) const;
  };

#endif
