#include "integrator.h"
#include "scene.h"
#include "material.h"

class Whitted : public Integrator
{
public:
    Whitted(const PropertyList &props) {
        m_maxRecursion = props.getInteger("maxRecursion", 4);
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {

        Color3f radiance = Color3f::Zero();

        // stopping criteria is recursion level > maxRecursion
        if (ray.recursionLevel > m_maxRecursion) return radiance;

        /* Find the surface that is visible in the requested direction */
        Hit hit;
        scene->intersect(ray, hit);
        Point3f x = ray.at(hit.t());
        if (hit.foundIntersection())
        {
            // Compute Direct lighting by summing all light sources contribution
            // Do not forget to test the visibility with shadow ray
            for (Light* l : scene->lightList())
            {
                float distToLightSource = .0f;
                Ray bounceRay = Ray(x + .0001f * hit.normal(), l->direction(x, &distToLightSource));
                Hit bounceHit = Hit();
                scene->intersect(bounceRay, bounceHit);
                if (bounceHit.foundIntersection() && bounceHit.t() <= distToLightSource) continue;
                Color3f p = hit.shape()->material()->brdf(-ray.direction, l->direction(x), hit.normal(), hit.textCoord());
                radiance += p * fmax(l->direction(x).dot(hit.normal()), .0f) * l->intensity(x);

            }
            // reflexions
            Vector3f reflexionDir = ray.direction - 2 * hit.normal().dot(ray.direction) * hit.normal();
            Ray reflexionRay = Ray(x + .0001f * hit.normal(), reflexionDir);
            reflexionRay.recursionLevel = ray.recursionLevel + 1;
            Color3f reflexionColor = Li(scene, reflexionRay);
            radiance += reflexionColor * hit.shape()->material()->reflectivity();

            // TODO : refraction

        }else if(ray.recursionLevel == 0)
            return scene->backgroundColor();

        return radiance;
    }

    std::string toString() const {
        return tfm::format("Whitted[\n"
                           "  max recursion = %f\n"
                           " ]\n",
                           m_maxRecursion);
    }
private:
    int m_maxRecursion;
};

REGISTER_CLASS(Whitted, "whitted")
