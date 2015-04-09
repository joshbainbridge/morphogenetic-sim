#include <CellShape.h>

//Add link to end of cell's link list
void CellShape::addLink(const boost::shared_ptr<CellShape> &_link)
{
  m_links.push_back(_link);
}

//Insert a link into cell's link list
void CellShape::insertLink(const int _index, const boost::shared_ptr<CellShape> &_link)
{
  m_links.insert(m_links.begin() + _index, _link);
}

//Remove a link from the cell's link list
void CellShape::eraseLink(const int _index)
{
  m_links.erase(m_links.begin() + _index);
}

//Find the index to a specific cell linked using their ID
int CellShape::findLink(const int _identifier) const
{
  for(unsigned int i = 0; i < m_links.size(); ++i)
  {
    if(m_links[i]->identifier() == _identifier)
    {
      return i;
    }
  }

  //If the cell is not linked, return 0
  return 0;
}

//Add nutrition using a uniformaly random variable
void CellShape::addNutrition(RandomGenerator &_random_sampler)
{
  m_nutrition += _random_sampler.getValue() * 0.1;
}

//Calculate the cell normal by averaging the normal from each plane between the cell's links
void CellShape::calculateNormal()
{
  //Vector to cumulate indiviual normal from every two links
  Vector3f link_normal_summation(0.f, 0.f, 0.f);
  for (unsigned int link_index = 0; link_index < m_links.size(); ++link_index)
  {
    Vector3f link_one_position = m_links[link_index]->position();
    Vector3f link_two_position = m_links[(link_index + 1) % m_links.size()]->position();

    //Find cross product of indexed link and next link
    Vector3f cross_product = (link_one_position - m_pos).cross(link_two_position - m_pos);
    //Noramlize cross product
    Vector3f normal_cross_product = cross_product.normalized();
    //Cumulate value
    link_normal_summation += normal_cross_product;
  }

  //Normaize summation
  m_normal = link_normal_summation.normalized();
}

//Find new postion target for next simulation cycle
void CellShape::findTarget(const float _spring_factor, const float _planar_factor, const float _bulge_factor, const float _link_rest_length)
{
  //Cumulative vector offsets based on simulation factors
  m_target_pos = m_pos;
  m_target_pos += (springTarget(_link_rest_length) - m_pos) * _spring_factor;
  m_target_pos += (planarTarget() - m_pos) * _planar_factor;
  m_target_pos += (bulgeTarget(_link_rest_length) - m_pos) * _bulge_factor;
}

//Update position to target and calculate radius based upon distance to linked cells
void CellShape::update(const float _time_step)
{
  //Set sphere postion to target
  m_pos = m_target_pos;

  //Add together the length of of each link
  float link_length_summation = 0.f;
  for (unsigned int link_index = 0; link_index < m_links.size(); ++link_index)
  {
    link_length_summation += (m_links[link_index]->targetPosition() - m_target_pos).norm() * 0.5f;
  }

  //Average the length and set as radius (size will never be 0)
  m_rad = link_length_summation / m_links.size();
}

//Find the spring target
Vector3f CellShape::springTarget(const float _link_rest_length) const
{
  //Vector to cumulate indiviual target from each link
  Vector3f link_spring_summation(0.f, 0.f, 0.f);
  for (unsigned int link_index = 0; link_index < m_links.size(); ++link_index)
  {
    //Cumulate spring value
    link_spring_summation += (m_links[link_index]->position() + (m_pos - m_links[link_index]->position()).normalized() * _link_rest_length);
  }

  //Average summation between links
  return link_spring_summation * (1.f / static_cast<float>(m_links.size()));
}

//Find the planar target
Vector3f CellShape::planarTarget() const
{
  //Vector to cumulate indiviual target from each link
  Vector3f link_position_summation(0.f, 0.f, 0.f);
  for (unsigned int link_index = 0; link_index < m_links.size(); ++link_index)
  {
    //Cumulate planar value
    link_position_summation += m_links[link_index]->position();
  }

  //Average summation between links
  return link_position_summation * (1.f / static_cast<float>(m_links.size()));
}

//Find the bulge target
Vector3f CellShape::bulgeTarget(const float _link_rest_length) const
{
  //Vector to cumulate indiviual target from each link
  float link_bulge_summation = 0.f;
  for (unsigned int link_index = 0; link_index < m_links.size(); ++link_index)
  {
    //Construct paramaters for bulge calculation
    Vector3f relative_position = m_links[link_index]->position() - m_pos;

    //Cumulate bulge value
    if(_link_rest_length > relative_position.norm())
    {
      float link_projection_scalar = relative_position.dot(m_normal);
      Vector3f link_projection_vector = (m_normal * link_projection_scalar) - relative_position;
      link_bulge_summation += std::sqrt((_link_rest_length * _link_rest_length) - link_projection_vector.squaredNorm()) + link_projection_scalar;
    }
  }

  //Average summation between links
  float bulge_distance = link_bulge_summation * (1.f / static_cast<float>(m_links.size()));

  //Find target postion by scaling normal
  return m_pos + m_normal * bulge_distance;
}
