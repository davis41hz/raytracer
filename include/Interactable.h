#ifndef INTERACT_H
#define INTERACT_H

#include "Ray.h"
#include "Utility.h"

class Material;

struct Interaction {
    Point3 p;
    Vector3 normal;
    double t;
    bool outwardFacingNormal;
    shared_ptr<Material> materialPtr;
    
    inline void setFaceNormal(const Ray& r, const Vector3& outwardNormal) {
        outwardFacingNormal = dot(r.direction(), outwardNormal) < 0;
        normal = outwardFacingNormal ? outwardNormal : -outwardNormal;
    }
};

class Interactable {
public:
    virtual bool interact(const Ray& r, double tMin, double tMax, Interaction& interaction) const = 0;
};

#endif