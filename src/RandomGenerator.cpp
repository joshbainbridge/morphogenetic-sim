#include <RandomGenerator.h>

//Generate uniform sample (mutates class)
float RandomGenerator::getValue()
{
  return m_uniform_dist(m_generator);
}
