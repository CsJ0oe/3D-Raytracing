#include "integrator.h"
#include "scene.h"
#include "material.h"

class Whitted : public Integrator
{
public:
    Whitted(const PropertyList &props) {
        m_maxRecursion = props.getInteger("maxRecursion",4);
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {

        Color3f radiance = Color3f::Zero();

        // stopping criteria is recursion level > maxRecursion
        // STOP 
        //TODO : FIX ME ENSEIRB STUDENTS

        /* Find the surface that is visible in the requested direction */
             //TODO : FIX ME ENSEIRB STUDENTS

        Hit hit;
        scene->intersect(ray, hit);
        if (hit.foundIntersection())
        {
            
            //TODO : FIX ME ENSEIRB STUDENTS
            // DIRECT LIGHTING IMPLEMENTATION goes here 

            // reflexions
            //TODO : FIX ME ENSEIRB STUDENTS

            // refraction
            //TODO : FIX ME ENSEIRB STUDENTS


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
