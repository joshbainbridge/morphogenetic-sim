#ifndef _TISSUESHAPE_H_
  #define _TISSUESHAPE_H_

  #include <vector>

  #include <boost/shared_ptr.hpp>
  #include <boost/random.hpp>

  #include <Common.h>
  #include <CellShape.h>
  #include <ShapeInterface.h>
  #include <RandomGenerator.h>

  class TissueShape : public ShapeInterface
  {
  public:
    TissueShape();

    inline float springFactor() const {return m_spring_factor;}
    inline float planarFactor() const {return m_planar_factor;}
    inline float bulgeFactor() const {return m_bulge_factor;}
    inline float linkRestLength() const {return m_link_rest_length;}
    inline float repulsionFactor() const {return m_repulsion_factor;}
    inline float repulsionRadius() const {return m_repulsion_radius;}
    inline float timeStep() const {return m_time_step;}

    inline void springFactor(const float _spring_factor){m_spring_factor = _spring_factor;}
    inline void planarFactor(const float _planar_factor){m_planar_factor = _planar_factor;}
    inline void bulgeFactor(const float _bulge_factor){m_bulge_factor = _bulge_factor;}
    inline void linkRestLength(const float _link_rest_length){m_link_rest_length = _link_rest_length;}
    inline void repulsionFactor(const float _repulsion_factor){m_repulsion_factor = _repulsion_factor;}
    inline void repulsionRadius(const float _repulsion_radius){m_repulsion_radius = _repulsion_radius;}
    inline void timeStep(const float _time_step){m_time_step = _time_step;}

    bool intersect(const Ray3f _ray, Transmission *_output) const;

    Vector3f position() const;
    Vector3f AABBmin() const;
    Vector3f AABBmax() const;

    void construction();
    void simulate();
    int exportData(float *_input) const;

  private:
    std::vector< boost::shared_ptr<CellShape> > m_cells;
    int m_count;
    int m_feed;
    float m_spring_factor;
    float m_planar_factor;
    float m_bulge_factor;
    float m_link_rest_length;
    float m_repulsion_factor;
    float m_repulsion_radius;
    float m_time_step;
    RandomGenerator m_feeder;
    boost::mt19937 m_generator;

    int modulus(const int _a, const int _b) const;
    void collisionOffset(boost::shared_ptr<CellShape> _current_cell);
    void splitCell(boost::shared_ptr<CellShape> _current_cell);
  };

#endif
