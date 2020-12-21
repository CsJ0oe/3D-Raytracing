#include "plane.h"

Plane::Plane()
{
}

Plane::Plane(const PropertyList &propList)
{
    m_position = propList.getPoint("position",Point3f(0,0,0));
    m_normal = propList.getVector("normal",Point3f(0,0,1));
}

Plane::~Plane()
{
}

bool Plane::intersect(const Ray& ray, Hit& hit) const
{
    // assuming vectors are all normalized
    float denom = ray.direction.dot(m_normal);
    if (abs(denom) < 1e-6) return false;
    float t = (m_position - ray.origin).dot(m_normal) / denom;
    if (t >= 0 && t < hit.t()) {
        hit.setT(t);
        hit.setShape(this);
        hit.setNormal(m_normal);
        Point3f at = ray.at(t);
        Vector3f a = m_normal.cross(Vector3f(1, 0, 0));
        Vector3f b = m_normal.cross(Vector3f(0, 1, 0));
        Vector3f c = m_normal.cross(Vector3f(0, 0, 1));
        Vector3f u = a;
        u = u.dot(u) <= b.dot(b) ? b : u;
        u = u.dot(u) <= c.dot(c) ? c : u;
        Vector3f v = m_normal.cross(u);
        float uu = fmod(1.0f + fmod(at.dot(u), 1.0f), 1.0f);
        float vv = fmod(1.0f + fmod(at.dot(v), 1.0f), 1.0f);
        hit.setTextCoord(Vector2f(uu, vv));
        return true;
    }
    return false;
}

REGISTER_CLASS(Plane, "plane")
