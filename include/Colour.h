#ifndef COLOUR_H
#define COLOUR_H

#include "Vector3.h"
#include <iostream>

void writeColour(std::ostream& out, Colour pixelColour, int samplesPerPixel) {
    auto r = pixelColour.x();
    auto g = pixelColour.y();
    auto b = pixelColour.z();
    
    auto scale = 1.0 / samplesPerPixel;
    r = sqrt(scale * r); // sqrt for gamma correction
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif