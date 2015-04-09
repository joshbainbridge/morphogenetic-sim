#ifndef _INTEGRATORINTERFACE_H_
  #define _INTEGRATORINTERFACE_H_

  #include <boost/shared_ptr.hpp>

  #include <Scene.h>
  #include <RandomGenerator.h>
  #include <Common.h>

  class IntegratorInterface
  {
  public:
    IntegratorInterface() : m_current_depth(0), m_limit_depth(0), m_absorption(0.f) {;}
    virtual ~IntegratorInterface() {}
    
    virtual Colour3f run(const boost::shared_ptr<Scene> _scene, boost::shared_ptr<RandomGenerator> _random_generator, const Ray3f _sample, const Colour3f _contribution) = 0;
    
    inline int currentDepth() const {return m_current_depth;}
    inline int limitDepth() const {return m_limit_depth;}
    inline float absorption() const {return m_absorption;}

    inline void currentDepth(const int _current_depth){m_current_depth = _current_depth;}
    inline void limitDepth(const int _limit_depth){m_limit_depth = _limit_depth;}
    inline void absorption(const float _absorption){m_absorption = _absorption;}
    
  protected:
    int m_current_depth;
    int m_limit_depth;
    float m_absorption;
  };

#endif
