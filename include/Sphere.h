#ifndef SPHERE_H
#define SPHERE_H

#include "Interactable.h"
#include "Vector3.h"

class Sphere : public Interactable {
public:
    Sphere() {}
    Sphere(Point3 c, double r, shared_ptr<Material> mat) : centre(c), radius(r), materialPtr(mat) {};
    virtual bool interact(const Ray& r, double tMin, double tMax, Interaction& interaction) const override;
    
public:
    Point3 centre;
    double radius;
    shared_ptr<Material> materialPtr;
};

bool Sphere::interact(const Ray& r, double tMin, double tMax, Interaction& interaction) const {
    Vector3 translatedCentre = r.origin() - centre;
    auto a = r.direction().lengthSquared();
    auto halfB = dot(translatedCentre, r.direction());
    auto c = translatedCentre.lengthSquared() - radius*radius;
    
    auto discriminant = halfB*halfB - a*c;
    if(discriminant < 0) return false;
    auto sqrtDiscriminant = sqrt(discriminant);
    
    auto root = (-halfB - sqrtDiscriminant) / a;
    if(root < tMin || root > tMax) {
        root = (-halfB + sqrtDiscriminant) / a;
        if(root < tMin || root > tMax) {
            return false;
        }
    }
    
    interaction.t = root;
    interaction.p = r.at(interaction.t);
    Vector3 outwardNormal = (interaction.p - centre) / radius;
    interaction.setFaceNormal(r, outwardNormal);
    interaction.materialPtr = materialPtr;
    
    return true;
};

#endif