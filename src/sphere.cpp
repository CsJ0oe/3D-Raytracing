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
    /// TODO: compute ray-sphere intersection

    Vector3f oc = ray.origin - this->m_center;
    float a = ray.direction.dot(ray.direction);
    float b = 2.0 * oc.dot(ray.direction);
    float c = oc.dot(oc) - (this->m_radius * this->m_radius);
    float discriminant = b*b - 4*a*c;
    if(discriminant < 0) return false;
    float t = (-b - sqrt(discriminant)) / (2.0*a);
    if (t < hit.t()) {
        hit.setT(t);
        hit.setShape(this);
        Point3f at = ray.at(t);
        hit.setNormal((at - m_center).normalized());
    }
    return true;
}

REGISTER_CLASS(Sphere, "sphere")
