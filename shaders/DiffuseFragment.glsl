#version 330 core

uniform vec4 Colour;

uniform vec3 Light_Position;
uniform vec3 Light_Direction;
uniform vec3 Light_Colour;
uniform float Light_Intensity;

in vec3 fragmentNormal;
in vec3 fragmentPosition;

out vec4 fragColour;

void main ()
{
  vec3 fragmentNormal_n = normalize(fragmentNormal);
  float surface_angle_differential = dot(fragmentNormal_n, normalize(Light_Position - fragmentPosition));
  //This will give the effect of an area light as apposed to that of a point
  surface_angle_differential = (surface_angle_differential + 0.5) / 1.5;
  if (surface_angle_differential < 0)
  {
    surface_angle_differential = 0;
  }

  float light_cosine_distrobution = pow(dot(Light_Direction, normalize(Light_Position - fragmentPosition)), 2.0);
  if (light_cosine_distrobution < 0)
  {
    light_cosine_distrobution = 0;
  }

  float light_falloff = 1 / pow(length(Light_Position - fragmentPosition), 2.0);

  vec3 shaded_colour = Colour.rgb * surface_angle_differential * light_cosine_distrobution * light_falloff * Light_Intensity * Light_Colour;
  fragColour = vec4(shaded_colour, 1.0);
}
