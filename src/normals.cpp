#include "integrator.h"
#include "scene.h"

class Normals : public Integrator {
public:
    Normals(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {
        /** Find the surface that is visible in the requested direction
            Return its normal color */
        Hit hit;
        scene->intersect(ray, hit);
        if (hit.foundIntersection()) {
            return Color3f(abs(hit.normal().x()), abs(hit.normal().y()), abs(hit.normal().z()));
        } else {
            return scene->backgroundColor();
        }
    }

    std::string toString() const {
        return "Normals[]";
    }
};

REGISTER_CLASS(Normals, "normals")
