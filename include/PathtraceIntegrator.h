#ifndef _PATHTRACEINTEGRATOR_H_
  #define _PATHTRACEINTEGRATOR_H_

  #include <boost/shared_ptr.hpp>

  #include <IntegratorInterface.h>
  #include <Scene.h>
  #include <RandomGenerator.h>
  #include <Common.h>

  class PathtraceIntegrator : public IntegratorInterface
  {
  public:
    PathtraceIntegrator() : IntegratorInterface() {}

    Colour3f run(const boost::shared_ptr<Scene> _scene, boost::shared_ptr<RandomGenerator> _random_generator, const Ray3f _sample, const Colour3f _contribution);
  };

#endif
