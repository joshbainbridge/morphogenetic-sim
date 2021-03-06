#ifndef _RANDOMGENERATOR_H_
  #define _RANDOMGENERATOR_H_

  #include <boost/random.hpp>

  class RandomGenerator
  {
  public:
    float getValue();

  private:
    boost::mt19937 m_generator;
    boost::uniform_real<float> m_uniform_dist;
  };

#endif
