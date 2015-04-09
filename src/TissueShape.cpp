#include <TissueShape.h>
#include <Aabb.h>

TissueShape::TissueShape()
{
  m_count = 0;
  m_feed = 0;
  m_spring_factor = 0.7f;
  m_planar_factor = 0.9f;
  m_bulge_factor = 0.2f;
  m_link_rest_length = 100.f;
  m_repulsion_factor = 0.4f;
  m_repulsion_radius = 300.f;
  m_time_step = 1.f;

  construction();
}

//Simulation intersection function
bool TissueShape::intersect(const Ray3f _ray, Transmission *_output) const
{
  bool intersection_found = false;
  Transmission closest_intersection;

  //Loop through all the primitves
  for (int i = 0; i < m_count; ++i)
  {
    Transmission cell_intersection;
    if(m_cells[i]->intersect(_ray, &cell_intersection) && (cell_intersection.distance < closest_intersection.distance))
    {
      closest_intersection = cell_intersection;
      intersection_found = true;
    }
  }

  //If intersection found, set the material and the transmission, then return true
  if (intersection_found)
  {
    closest_intersection.position += closest_intersection.normal * M_EPSILON;
    *_output = closest_intersection;
    return true;
  }

  //Intersection was not found
  return false;
}

//Find center position
Vector3f TissueShape::position() const
{
  return (AABBmin() + AABBmax()) * 0.5f;
}

//Get min bounding value
Vector3f TissueShape::AABBmin() const
{
  Aabb bounds;

  for (int i = 0; i < m_count; ++i)
  {
    bounds.updateAabb(m_cells[i]->AABBmin());
  }

  return bounds.min();
}

//Get max bounding value
Vector3f TissueShape::AABBmax() const
{
  Aabb bounds;

  for (int i = 0; i < m_count; ++i)
  {
    bounds.updateAabb(m_cells[i]->AABBmax());
  }

  return bounds.max();
}

//Innitial construction of data (using euclid's proof)
void TissueShape::construction()
{
  m_cells.clear();
  m_count = 0;
  m_feed = 0;

	std::vector< boost::shared_ptr<CellShape> > m_cells_new;

	//Cell group radius
	float radius = 100.f;
	//Cell group position
	Vector3f position(0.f, 0.f, 0.f);

	//Construction rectangle ratio
	float pos_x = (1.f + std::sqrt(5.0f)) * 0.5f;
	float neg_x = pos_x * -1.f;

	//Initial cell positions
	std::vector< Vector3f > cell_input;
	cell_input.push_back(Vector3f(-1.f, pos_x, 0.f).normalized() * radius);
	cell_input.push_back(Vector3f(1.f, pos_x, 0.f).normalized() * radius);
	cell_input.push_back(Vector3f(-1.f, neg_x, 0.f).normalized() * radius);
	cell_input.push_back(Vector3f(1.f, neg_x, 0.f).normalized() * radius);

	cell_input.push_back(Vector3f(0.f, -1.f, pos_x).normalized() * radius);
	cell_input.push_back(Vector3f(0.f, 1.f, pos_x).normalized() * radius);
	cell_input.push_back(Vector3f(0.f, -1.f, neg_x).normalized() * radius);
	cell_input.push_back(Vector3f(0.f, 1.f, neg_x).normalized() * radius);

	cell_input.push_back(Vector3f(pos_x, 0.f, -1.f).normalized() * radius);
	cell_input.push_back(Vector3f(pos_x, 0.f, 1.f).normalized() * radius);
	cell_input.push_back(Vector3f(neg_x, 0.f, -1.f).normalized() * radius);
	cell_input.push_back(Vector3f(neg_x, 0.f, 1.f).normalized() * radius);

	//Initial cell links
	std::vector< std::vector< int > > cell_indices;
	std::vector< int > links(5);

	links[0] = 2;
	links[1] = 8;
	links[2] = 11;
	links[3] = 12;
	links[4] = 6;
	cell_indices.push_back(links);
	links[0] = 1;
	links[1] = 6;
	links[2] = 10;
	links[3] = 9;
	links[4] = 8;
	cell_indices.push_back(links);
	links[0] = 4;
	links[1] = 5;
	links[2] = 12;
	links[3] = 11;
	links[4] = 7;
	cell_indices.push_back(links);
	links[0] = 3;
	links[1] = 7;
	links[2] = 9;
	links[3] = 10;
	links[4] = 5;
	cell_indices.push_back(links);

	links[0] = 6;
	links[1] = 12;
	links[2] = 3;
	links[3] = 4;
	links[4] = 10;
	cell_indices.push_back(links);
	links[0] = 5;
	links[1] = 10;
	links[2] = 2;
	links[3] = 1;
	links[4] = 12;
	cell_indices.push_back(links);
	links[0] = 8;
	links[1] = 9;
	links[2] = 4;
	links[3] = 3;
	links[4] = 11;
	cell_indices.push_back(links);
	links[0] = 7;
	links[1] = 11;
	links[2] = 1;
	links[3] = 2;
	links[4] = 9;
	cell_indices.push_back(links);

	links[0] = 10;
	links[1] = 4;
	links[2] = 7;
	links[3] = 8;
	links[4] = 2;
	cell_indices.push_back(links);
	links[0] = 9;
	links[1] = 2;
	links[2] = 6;
	links[3] = 5;
	links[4] = 4;
	cell_indices.push_back(links);
	links[0] = 12;
	links[1] = 1;
	links[2] = 8;
	links[3] = 7;
	links[4] = 3;
	cell_indices.push_back(links);
	links[0] = 11;
	links[1] = 3;
	links[2] = 5;
	links[3] = 6;
	links[4] = 1;
	cell_indices.push_back(links);

	//Creating cells
	for(int i = 0; i < 12; ++i)
	{
		boost::shared_ptr<CellShape> new_cell(new CellShape(m_count));
		new_cell->position(position + cell_input[i]);
		new_cell->radius(50.f);
		m_cells_new.push_back(new_cell);
		++m_count;
	}

	//Linking cells
	for(int i = 0; i < 12; ++i)
	{
		for(int l = 0; l < 5; ++l)
		{
			boost::shared_ptr<CellShape> link_cell;
			link_cell = m_cells_new[cell_indices[i][l] - 1];
			m_cells_new[i]->addLink(link_cell);
		}
	}

	//Adding cells
	for(int i = 0; i < 12; ++i)
	{
		m_cells.push_back(m_cells_new[i]);
	}
}

void TissueShape::simulate()
{
  //Calculate Normal
  for (unsigned int cell_index = 0; cell_index < m_cells.size(); ++cell_index)
  {
    //Offsets the adding of nutriants across the cells
    if(m_feed == 5)
    {
      m_cells[cell_index]->addNutrition(m_feeder);
      splitCell(m_cells[cell_index]);
      m_feed = 0;
    }
    else
    {
      ++m_feed;
    }

    m_cells[cell_index]->calculateNormal();
  }

  //Simulation
  for (unsigned int cell_index = 0; cell_index < m_cells.size(); ++cell_index)
  {
    m_cells[cell_index]->findTarget(m_spring_factor, m_planar_factor, m_bulge_factor, m_link_rest_length);
    collisionOffset(m_cells[cell_index]);
  }

  //Update
  for (unsigned int cell_index = 0; cell_index < m_cells.size(); ++cell_index)
  {
    m_cells[cell_index]->update(m_time_step);
  }
}

//Export cell data in array
int TissueShape::exportData(float *_input) const
{
  Vector3f position;
  for (int i = 0; i < m_count; ++i)
  {
    position = m_cells[i]->position();
    _input[i * 4 + 0] = position.x();
    _input[i * 4 + 1] = position.y();
    _input[i * 4 + 2] = position.z();
    _input[i * 4 + 3] = m_cells[i]->radius();
  }

  return m_count;
}

//Modulus function for cell division
int TissueShape::modulus(const int _a, const int _b) const
{
  int remainder = _a % _b;
  
  if (remainder < 0)
  {
    return remainder += _b;
  }
  else
  {
    return remainder;
  }
}

void TissueShape::collisionOffset(boost::shared_ptr<CellShape> _current_cell)
{
  //Initial offset for collision
  Vector3f collision_offset(0.f, 0.f, 0.f);

  //Loop through all cells
  for (unsigned int cell_index = 0; cell_index < m_cells.size(); ++cell_index)
  {
    //Is cell within collion radius
    if ((m_cells[cell_index]->position() - _current_cell->position()).norm() < m_repulsion_radius)
    {
      //Initial value for cell linked test
      bool cell_linked = false;

      //Is indexed cell linked to current cell
      for (int link_index = 0; link_index < _current_cell->linkListSize(); ++link_index)
      {
        if (m_cells[cell_index]->identifier() == _current_cell->getLink(link_index)->identifier())
          cell_linked = true;
      }

      //If not linked
      if (cell_linked != true)
      {
        if(m_cells[cell_index]->identifier() != _current_cell->identifier())
        {
          //Calculate the collision radius squared
          float repulsion_radius_sqr = m_repulsion_radius * m_repulsion_radius;
          //Find the relative position of indexed cell
          Vector3f relative_position = _current_cell->position() - m_cells[cell_index]->position();
          //Calculate the relative postion squared
          float relative_position_sqr = relative_position.squaredNorm();
          //Calculate the collision offset and cumulatively add each loop
          collision_offset += (relative_position.normalized() * ((repulsion_radius_sqr - relative_position_sqr) / repulsion_radius_sqr));
        }
      }
    }
  }

  //Add collision offset multiplied by simulation paramater to target position
  _current_cell->targetPosition(_current_cell->targetPosition() + collision_offset * m_repulsion_factor);
}

//Split cell and recalculate current and surround cells link list order (this is to maintain intergrity so that normals can be calculated)
void TissueShape::splitCell(boost::shared_ptr<CellShape> _current_cell)
{
  if(_current_cell->nutrition() > 10.f)
  {
    //Create uniform distrobution to select random link for cell bisection
    int link_count = _current_cell->linkListSize();
    boost::uniform_int<int> uniform_int_link(0, link_count - 1);
    int link_first = uniform_int_link(m_generator);

    //Find opposite link
    int half_count = static_cast<int>(static_cast<float>(link_count) * 0.5f);
    int link_last = modulus(link_first + half_count, link_count);
    int direction = 1;

    //Find inner link indicies
    std::vector< unsigned int > link_inner;
    for(int i = modulus(link_last + direction, link_count); i != link_first; i = modulus(i += direction, link_count))
    {
      link_inner.push_back(i);
    }

    //Create new daughter cell and add it to cell and object vector
    boost::shared_ptr<CellShape> new_cell(new CellShape(m_count));
    m_cells.push_back(new_cell);
    ++m_count;

    //Add first two links in clockwise direction to daughter cell
    new_cell->addLink(_current_cell);
    new_cell->addLink(_current_cell->getLink(link_last));

    //Create position vector that represents average postion of all links connected to daughter cell
    Vector3f new_position = _current_cell->position();

    //Add second two link positions cumulatively to new position
    new_position = new_position + _current_cell->getLink(link_last)->position();
    new_position = new_position + _current_cell->getLink(link_first)->position();

    //Loop through inner links, adding in a clockwise direction to daughter cell and cumulating positions
    for(unsigned int i = 0; i < link_inner.size(); ++i)
    {
      boost::shared_ptr<CellShape> link = _current_cell->getLink(link_inner[i]);

      new_cell->addLink(link);
      new_position = new_position + link->position();

      //Loop through inner links and change current cell link to daughter cell
      int index = link->findLink(_current_cell->identifier());
      link->setLink(index, new_cell);
    }
    //Add final link to daughter cell
    new_cell->addLink(_current_cell->getLink(link_first));

    //Find average of link positions and set as daughter cell position
    new_position = new_position / static_cast<float>(new_cell->linkListSize());
    new_cell->position(new_position);

    //Set bisector link from current cell to daughter cell
    boost::shared_ptr<CellShape> link_first_ptr = _current_cell->getLink(link_first);
    int link_first_current_index = link_first_ptr->findLink(_current_cell->identifier());
    int link_first_link_count = link_first_ptr->linkListSize();
    link_first_ptr->insertLink(modulus(link_first_current_index + direction, link_first_link_count), new_cell);

    //Set bisector link from current cell to daughter cell
    boost::shared_ptr<CellShape> link_last_ptr = _current_cell->getLink(link_last);
    int link_last_current_index = link_last_ptr->findLink(_current_cell->identifier());
    int link_last_link_count = link_last_ptr->linkListSize();
    link_last_ptr->insertLink(modulus(link_last_current_index - direction, link_last_link_count), new_cell);

    //Find the identifiers for inner links then iterate throught links using identifiers to erase vector elements
    std::vector< int > link_inner_identifier(link_inner.size());
    for(unsigned int i = 0; i < link_inner.size(); ++i)
    {
      link_inner_identifier[i] = _current_cell->getLink(link_inner[i])->identifier();
    }
    for(unsigned int i = 0; i < link_inner_identifier.size(); ++i)
    {
      _current_cell->eraseLink(_current_cell->findLink(link_inner_identifier[i]));
    }

    //Link current cell to daughter cell
    int new_last_link = _current_cell->findLink(link_last_ptr->identifier());
    int new_link_count = _current_cell->linkListSize();
    _current_cell->insertLink(modulus(new_last_link + direction, new_link_count), new_cell);

    //Distributes energy to surrounding cells
    int new_list_size = _current_cell->linkListSize();
    float distrebuted_cell_nutrition = (_current_cell->nutrition() / new_list_size) * 0.5;
    for(int i = 0; i < new_list_size; ++i)
    {
      float new_value = _current_cell->getLink(i)->nutrition() + distrebuted_cell_nutrition;
      _current_cell->getLink(i)->nutrition(new_value);
    }

    //Set nutrition back to 0
    _current_cell->nutrition(0.f);
  }
}
