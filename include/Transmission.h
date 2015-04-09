#ifndef _TRANSMISSION_H_
  #define _TRANSMISSION_H_

  #include <boost/shared_ptr.hpp>

  #include <Common.h>
  #include <Material.h>

  struct Transmission
  {
    Transmission() : distance(M_INFINITY) {;}

    Vector3f position;
    Vector3f normal;
    float distance;
    boost::shared_ptr<Material> material;
  };

#endif
