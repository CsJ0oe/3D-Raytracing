#include "integrator.h"
#include "scene.h"

class TexcoordsIntegrator : public Integrator {
public:
    TexcoordsIntegrator(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {
        /* Return the component-wise absolute
           value of the shading Texcoords as a color */
        Hit hit;
        scene->intersect(ray, hit);
        if (hit.foundIntersection()) {
            return Color3f(hit.textCoord().x(), hit.textCoord().y(), 0.0f);
        } else {
            return scene->backgroundColor();
        }
    }

    std::string toString() const {
        return "TexcoordsIntegrator[]";
    }
};

REGISTER_CLASS(TexcoordsIntegrator, "texcoords")
