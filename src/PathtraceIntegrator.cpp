#include <PathtraceIntegrator.h>

//Main monte carlo pathtracing implementation
Colour3f PathtraceIntegrator::run(const boost::shared_ptr<Scene> _scene, boost::shared_ptr<RandomGenerator> _random_generator, const Ray3f _sample, const Colour3f _contribution)
{
  //Innitial radiance transmition if no intersection is found
  Colour3f reflected_light(0.f, 0.f, 0.f);
  Transmission scene_intersection;
  
  //Trace intersect scene and if intersection is found
  if (_scene->intersectPrimitives(_sample, &scene_intersection))
  {
    //The intersected material
    boost::shared_ptr< Material > intersection_material = scene_intersection.material;
    
    //Calculate indirect light contribution
    Colour3f indirect_light = Colour3f(0.f, 0.f, 0.f);
    ++m_current_depth;
    if (m_current_depth <= m_limit_depth)
    {
      //Sample Brdf to create new indirect ray
      Ray3f indirect_ray = intersection_material->sample(_sample.dir * -1.f, scene_intersection.position, scene_intersection.normal, *_random_generator);

      //Evaluate the contribution against the surfaces material
      Colour3f evaluation = intersection_material->evaluate(_sample.dir * -1.f, indirect_ray.dir, scene_intersection.position, scene_intersection.normal);
      
      //Gather light from scene
      indirect_light = run(_scene, _random_generator, indirect_ray, _contribution) * evaluation;
    }
    else
    {
      //Stochastic value used for russian roulette termination
      float russian_roulette = _random_generator->getValue();

      //Is value below that of adaptive sampling and absorption threshhold
      if (russian_roulette < (_contribution.mean() * m_absorption))
      {
        //Sample Brdf to create new indirect ray
        Ray3f indirect_ray = intersection_material->sample(_sample.dir * -1.f, scene_intersection.position, scene_intersection.normal, *_random_generator);

        //Evaluate the contribution against the surfaces material
        Colour3f evaluation = intersection_material->evaluate(_sample.dir * -1.f, indirect_ray.dir, scene_intersection.position, scene_intersection.normal);

        //Gather light from scene and divide by absorbtion to satisfy probability of termination
        indirect_light = run(_scene, _random_generator, indirect_ray, evaluation * _contribution) * evaluation / (_contribution.mean() * m_absorption);
      }
    }
    --m_current_depth;
    
    //Calculate direct contribution from each light
    Colour3f direct_light = Colour3f(0.f, 0.f, 0.f);
    int light_count = _scene->countLight();
    for (int i = 0; i < light_count; ++i)
    {
      boost::shared_ptr< LightInterface > light = _scene->getLight(i);
      Vector3f light_sample = light->sampleLight(*_random_generator);
      
      //Construct ray for occlusion testing
      Ray3f light_occlusion_ray;
      light_occlusion_ray.dir = (light_sample - scene_intersection.position).normalized();
      light_occlusion_ray.origin = scene_intersection.position;
      
      //If light is above shading surface as well as faceing the surface
      if (scene_intersection.normal.dot(light_occlusion_ray.dir) > 0.f && light->direction().dot(light_occlusion_ray.dir) < 0.f)
      {
        Transmission light_occlusion_test;
        bool light_intersection_test = _scene->intersectPrimitives(light_occlusion_ray, &light_occlusion_test);
        
        //If no primitives were intersected or if the closest intersection was further than the light
        if (light_intersection_test != true || light_occlusion_test.distance > (light_sample - scene_intersection.position).norm())
        {
          //Calculate the cotribution from each light
          Colour3f light_contribution = light->illumination(light_sample - scene_intersection.position);
          
          //Evaluate the contribution against the surfaces material
          light_contribution *= intersection_material->evaluate(_sample.dir * -1.f, light_occlusion_ray.dir, scene_intersection.position, scene_intersection.normal);
          
          //Add it to the accumlitive contribution of direct illumination
          direct_light += light_contribution;
        }
      }
    }
    
    //Add direct and indirect contributions
    reflected_light = direct_light + indirect_light;
  }
  
  return reflected_light;
}
