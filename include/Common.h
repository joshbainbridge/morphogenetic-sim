#ifndef _COMMON_H_
  #define _COMMON_H_

  #include <cfloat>
  #include <cmath>

  #include <Eigen/Core>
  #include <Eigen/Geometry>

  //Common defines constants and linear algebraic types used through out the project
  //Algebraic types are based on the Eigen library that is optimised for expression templating, cache coherency and SSE vectorizaion

  #ifdef M_PI
    #undef M_PI
  #endif

  #define M_PI 3.14159265358979323846f
  #define M_INV_PI 0.31830988618379067154f
  #define M_PI_180 0.01745329251994329577f
  #define M_180_PI 57.2957795130823208768f

  #define M_EPSILON FLT_EPSILON
  #define M_INFINITY FLT_MAX

  template < typename t_origin, typename t_direction > struct Ray
  {
    t_origin origin;
    t_direction dir;
  };

  template < typename type > bool compare(const type _a, const type _b)
  {
    return std::fabs(static_cast<float>(_a) - static_cast<float>(_b)) < M_EPSILON;
  }

  typedef Eigen::Matrix< float, 2, 1 > Vector2f;
  typedef Eigen::Matrix< float, 3, 1 > Vector3f;
  typedef Eigen::Matrix< float, 4, 1 > Vector4f;
  typedef Eigen::Matrix< float, 4, 4 > TransMatrix;
  typedef Ray< Vector2f, Vector2f > Ray2f;
  typedef Ray< Vector3f, Vector3f > Ray3f;
  typedef Ray< Vector4f, Vector4f > Ray4f;
  typedef Eigen::Array< float, 3, 1 > Colour3f;

#endif
