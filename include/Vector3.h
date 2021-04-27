#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class Vector3 {
public:
    Vector3() : v{0,0,0} {}
    Vector3(double v0, double v1, double v2) : v{v0, v1, v2} {}
    
    double x() const { return v[0]; }
    double y() const { return v[1]; }
    double z() const { return v[2]; }
    
    Vector3 operator - () const { return Vector3(-v[0], -v[1], -v[2]); }
    double operator [] (int i) const { return v[i]; }
    double& operator [] (int i) { return v[i]; }
    
    Vector3& operator += (const Vector3& other) {
        v[0] += other.v[0];
        v[1] += other.v[1];
        v[2] += other.v[2];
        return *this;
    }
    
    Vector3& operator *= (const double s) {
        v[0] *= s;
        v[1] *= s;
        v[2] *= s;
        return *this;
    }
    
    Vector3& operator /= (const double s) {
        return *this *= 1/s;
    }
    
    double length() const {
        return sqrt(lengthSquared());
    }
    
    double lengthSquared() const {
        return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    }
    
    bool isNearZero() const {
        const auto epsilon = 1e-8;
        return (fabs(v[0]) < epsilon) && (fabs(v[1]) < epsilon) && (fabs(v[2]) < epsilon);
    }
    
    inline static Vector3 random() {
        return Vector3(randomDouble(), randomDouble(), randomDouble());
    }
    
    inline static Vector3 random(double min, double max) {
        return Vector3(randomDouble(min,max), randomDouble(min,max), randomDouble(min,max));
    }
    
    
public:
    double v[3];
};

using Point3 = Vector3;
using Colour = Vector3;

// utilities

inline std::ostream& operator << (std::ostream& out, const Vector3& w) {
    return out << w.v[0] << ' ' << w.v[1] << ' ' << w.v[2];
}

inline Vector3 operator + (const Vector3& w, const Vector3& x) {
    return Vector3(w.v[0] + x.v[0], w.v[1] + x.v[1], w.v[2] + x.v[2]);
}

inline Vector3 operator - (const Vector3& w, const Vector3& x) {
    return Vector3(w.v[0] - x.v[0], w.v[1] - x.v[1], w.v[2] - x.v[2]);
}

inline Vector3 operator * (const Vector3& w, const Vector3& x) {
    return Vector3(w.v[0] * x.v[0], w.v[1] * x.v[1], w.v[2] * x.v[2]);
}

inline Vector3 operator * (double s, const Vector3& w) {
    return Vector3(s * w.v[0], s * w.v[1], s * w.v[2]);
}

inline Vector3 operator * (const Vector3& w, double s) {
    return s * w;
}

inline Vector3 operator / (const Vector3& w, double s) {
    return (1/s) * w;
}

inline double dot(const Vector3& w, const Vector3& x) {
    return w.v[0] * x.v[0] + w.v[1] * x.v[1] + w.v[2] * x.v[2];
}

inline Vector3 cross(const Vector3& w, const Vector3 x) {
    return Vector3(w.v[1] * x.v[2] - w.v[2] * x.v[1],
                   w.v[2] * x.v[0] - w.v[0] * x.v[2],
                   w.v[0] * x.v[1] - w.v[1] * x.v[0]);
}

inline Vector3 unitVector(Vector3 w) {
    return w / w.length();
}

inline Vector3 randomInUnitSphere() {
    while(true) {
        auto p = Vector3::random(-1, 1);
        if(p.lengthSquared() >= 1) continue;
        return p;
    }
}

inline Vector3 randomUnitVector() {
    return unitVector(randomInUnitSphere());
} 

inline Vector3 reflect(const Vector3& w, const Vector3& n) {
    return w - 2.0*dot(w,n)*n;
}

inline Vector3 refract(const Vector3& w, const Vector3& n, double etaRatio) {
    auto cosTheta = fmin(dot(-w, n), 1.0);
    Vector3 rOutPerp = etaRatio * (w + cosTheta*n);
    Vector3 rOutParallel = -sqrt(fabs(1.0 - rOutPerp.lengthSquared())) * n;
    return rOutPerp + rOutParallel;
}

inline Vector3 randomInUnitDisk() {
    while(true) {
        auto p = Vector3(randomDouble(-1,1), randomDouble(-1,1), 0);
        if(p.lengthSquared() >= 1) continue;
        return p;
    }
}




#endif