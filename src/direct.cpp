#include "integrator.h"
#include "scene.h"
#include "material.h"

class Direct : public Integrator
{
public:
    Direct(const PropertyList &props) {}

    Color3f Li(const Scene *scene, const Ray &ray) const {
        Color3f radiance = Color3f::Zero();

        /* Find the surface that is visible in the requested direction */
        //TODO: FIX ME ENSEIRB STUDENT

        // IF NOT SURFACE ARE VISIBLE RETURN BACKGROUND
        //TODO: FIX ME ENSEIRB STUDENT


        // IF a SURFACE IS Visible

        // Compute DIrect lighting by summing all light sources contribution
        // Do not forget to test the visibility with shadow ray

        //TODO: FIX ME ENSEIRB STUDENT

        // Return the final value of the radiance
        return radiance;
    }

    std::string toString() const {
        return "Direct[]";
    }
};

REGISTER_CLASS(Direct, "direct")
