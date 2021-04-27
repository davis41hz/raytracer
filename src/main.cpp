#include "Camera.h"
#include "Utility.h"
#include "Colour.h"
#include "InteractableList.h"
#include "Sphere.h"
#include "Material.h"
#include <iostream>

InteractableList generateCoverScene() {
    InteractableList world;
    
    auto groundMat = make_shared<Lambertian>(Colour(0.5,0.5,0.5));
    world.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, groundMat));
    
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = randomDouble();
            Point3 centre(a + 0.9*randomDouble(), 0.2, b + randomDouble());
            
            if((centre - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphereMat;
                
                if(chooseMat < 0.8) { // diffuse
                    auto albedo = Colour::random() * Colour::random();
                    sphereMat = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(centre, 0.2, sphereMat));
                } else if (chooseMat < 0.95) { // metallic
                    auto albedo = Colour::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphereMat = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(centre, 0.2, sphereMat));
                } else { // glass
                    sphereMat = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(centre, 0.2, sphereMat));
                }
            }
        }
    }
    
    auto mat1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0,1,0), 1.0, mat1));
    
    auto mat2 = make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4,1,0), 1.0, mat2));
    
    auto mat3 = make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4,1,0), 1.0, mat3));
    
    return world;
}

Colour rayColour(const Ray& r, const Interactable& world, int depth) {
    Interaction interaction;
    
    if(depth <= 0) return Colour(0,0,0);
    
    if(world.interact(r, 0.001, infinity, interaction)) {
        Ray scattered;
        Colour attenuation;
        if(interaction.materialPtr->scatter(r, interaction, attenuation, scattered))
            return attenuation * rayColour(scattered, world, depth-1);
        return Colour(0,0,0);
    }
    
    Vector3 unitDirection = unitVector(r.direction());
    auto t = 0.5*(unitDirection.y() + 1.0);
    return (1.0-t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0);
}

int main() {
    
    // Image
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 1920;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 500;
    const int maxDepth = 50;
    
    // World
    auto world = generateCoverScene();

    
    // Camera
    Point3 lookFrom(13,2,3);
    Point3 lookAt(0,0,0);
    Vector3 up(0,1,0);
    auto focusDistance = 10.0;
    auto aperture = 0.1;
    Camera camera(lookFrom, lookAt, up, 20, aspectRatio, aperture, focusDistance);
    
    
    // Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    
    for(int h = imageHeight-1; h >= 0; --h) {
        std::cerr << "\rScanlines remaining: " << h << ' ' << std::flush;
        for(int w = 0; w < imageWidth; ++w) {
            Colour pixelColour(0, 0, 0);
            for (int sample = 0; sample < samplesPerPixel; ++sample) {
                auto u = (w + randomDouble()) / (imageWidth-1);
                auto v = (h + randomDouble()) / (imageHeight-1);
                Ray r = camera.getRay(u, v);
                pixelColour += rayColour(r, world, maxDepth);
            }
            
            writeColour(std::cout, pixelColour, samplesPerPixel);
        }
    }
    
    std::cerr << "\nComplete.\n";    
}