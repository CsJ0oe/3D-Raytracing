#include "integrator.h"
#include "scene.h"

class Direct : public Integrator {
public:
    Direct(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {
        /** TODO : Find the surface that is visible in the requested direction
                   Return its ambiant color */
        Hit hit;
        scene->intersect(ray, hit);
        if (hit.foundIntersection()) {
            // Compute Direct lighting by summing all light sources contribution
            // Do not forget to test the visibility with shadow ray

            Color3f radiance = Color3f::Zero();
            for (Light* l : scene->lightList())
            {
                Point3f x = ray.at(hit.t());
                Color3f p = hit.shape()->material()->brdf(-ray.direction,
                                                           l->direction(x),
                                                           hit.normal(),
                                                           Vector2f(0.0f) //hit.textCoord()
                                                           );
                radiance += p * fmax(l->direction(x).dot(hit.normal()), .0f) * l->intensity(x);
            }
            return radiance;
        } else {
            return scene->backgroundColor();
        }
    }

    std::string toString() const {
        return "Direct[]";
    }
};

REGISTER_CLASS(Direct, "direct")
