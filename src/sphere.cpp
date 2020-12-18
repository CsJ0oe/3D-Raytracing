#include "sphere.h"
#include <iostream>

Sphere::Sphere(float radius)
    : m_radius(radius)
{
}

Sphere::Sphere(const PropertyList &propList)
{
    m_radius = propList.getFloat("radius",1.f);
    m_center = propList.getPoint("center",Point3f(0,0,0));
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray& ray, Hit& hit) const
{
    // compute ray-sphere intersection
    Vector3f oc = ray.origin - m_center;
    float a = ray.direction.dot(ray.direction);
    float b = oc.dot(ray.direction) * 2.0;
    float c = oc.dot(oc) - (m_radius * m_radius);
    float discriminant = b*b - 4*a*c;
    if(discriminant < 0) return false;
    float t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    float t2 = (-b + sqrt(discriminant)) / (2.0 * a);
    float t = ( t1>=0 && t2>=0 )? fmin(t1,t2) : fmax(t1,t2);
    if (t >= 0 && t < hit.t()) {
        hit.setT(t);
        hit.setShape(this);
        Point3f at = (ray.at(t) - m_center).normalized();
        hit.setNormal(at);
        float u = 0.5f + atan2(at.x(), at.y()) / (2*acos(-1));
        float v = 1.0f - acos(at.z()) / acos(-1);
        hit.setTextCoord(Vector2f(u, v));
        return true;
    }
    return false;
}

REGISTER_CLASS(Sphere, "sphere")
