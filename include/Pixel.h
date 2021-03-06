#ifndef _PIXEL_H_
  #define _PIXEL_H_

  #include <vector>

  #include <Common.h>
  #include <Transmission.h>

  class Pixel
  {
  public:
    Colour3f pixelColour() const;
    void addSample(const Colour3f _sample);
    Colour3f getSample(const int _index) const;

  private:
    std::vector< Colour3f > m_samples;
  };

#endif
