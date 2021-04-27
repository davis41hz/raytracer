#ifndef MATERIAL_H
#define MATERIAL_H

#include "Utility.h"

struct Interaction;


class Material {
public:
    virtual bool scatter(const Ray& rIn, const Interaction& interaction, Colour& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const Colour& a) : albedo(a) {}
    
    virtual bool scatter(const Ray& rIn, const Interaction& interaction, Colour& attenuation, Ray& scattered) const override {
        auto scatterDirection = interaction.normal + randomUnitVector();
        
        if(scatterDirection.isNearZero()) {
            scatterDirection = interaction.normal;
        }
        
        scattered = Ray(interaction.p, scatterDirection);
        attenuation = albedo;
        return true;
    }
    
public:
    Colour albedo;
};

class Metal : public Material {
public:
    Metal(const Colour& a, double f) : albedo(a), fuzz(f < 1 ? f : 1){}
    
    virtual bool scatter(const Ray& rIn, const Interaction& interaction, Colour& attenuation, Ray& scattered) const override {
        Vector3 reflected = reflect(unitVector(rIn.direction()), interaction.normal);
                
        scattered = Ray(interaction.p, reflected + fuzz*randomInUnitSphere());
        attenuation = albedo;
        return (dot(scattered.direction(), interaction.normal) > 0);
    }
    
public:
    Colour albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double ir) : indexOfRefraction(ir) {}
    
    virtual bool scatter(const Ray& rIn, const Interaction& interaction, Colour& attenuation, Ray& scattered) const override {
        attenuation = Colour(1.0, 1.0, 1.0);
        double refractionRatio = interaction.outwardFacingNormal ? (1.0/indexOfRefraction) : indexOfRefraction;
        
        Vector3 unitDirection = unitVector(rIn.direction());
        double cosTheta = fmin(dot(-unitDirection, interaction.normal), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta*cosTheta);
        
        bool cannotRefract = refractionRatio * sinTheta > 1.0;
        Vector3 direction;
        if(cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble()) {
            direction = reflect(unitDirection, interaction.normal);
        } else {
            direction = refract(unitDirection, interaction.normal, refractionRatio);
        }
        
        scattered = Ray(interaction.p, direction);
        return true;
    }
        
public:
    double indexOfRefraction;
    
private:
    static double reflectance(double cosine, double refractionIndex) {
        // Schlick's Approx
        auto r0 = (1-refractionIndex) / (1+refractionIndex);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1-cosine),5);
    }
        
};


#endif