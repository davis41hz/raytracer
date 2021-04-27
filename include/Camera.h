#ifndef CAMERA_H
#define CAMERA_H

#include "Utility.h"

class Camera {
public:
    Camera(Point3 lookFrom, Point3 lookAt, Vector3 up, double verticalFOV, double aspectRatio, double aperture, double focusDistance) {
        auto theta = degreesToRadians(verticalFOV);
        auto h = tan(theta/2);
        auto viewportHeight = 2.0 * h;
        auto viewportWidth = aspectRatio * viewportHeight;

        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(up, w));
        v = cross(w, u);
        
        origin = lookFrom;
        horizontal = focusDistance * viewportWidth * u;
        vertical = focusDistance* viewportHeight * v;
        lowerLeftCorner = origin - horizontal/2 - vertical/2 - focusDistance*w;
        lensRadius = aperture/2;
    }
    
    Ray getRay(double s, double t) const {
        Vector3 rd = lensRadius * randomInUnitDisk();
        Vector3 offset = u * rd.x() + v * rd.y();
        return Ray(origin + offset, lowerLeftCorner + s*horizontal + t*vertical - origin - offset);
    }
    
private:
    Point3 origin, lowerLeftCorner;
    Vector3 horizontal, vertical;
    Vector3 u, v, w;
    double lensRadius;
};

#endif