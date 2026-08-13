#include "renderer/shading/ShadingFunctions.h"

# define PI 3.14159265358979323846
const double SHADOW_BIAS = 0.01;

Color ray_trace(const ShadeContext& context)
{
    Vector3 finalColorVec(0, 0, 0);
    
    for (const PointLight& lightSrc : *context.lights) {
        Vector3 lp = lightSrc.pos() - context.hitRec->point;
        
        Material hitMaterial = context.materials->at(context.hitRec->materialIdx);
        Vector3 hitNormal = hitMaterial.smooth() ? context.hitRec->pointNormal : context.hitRec->normal;

        double lightVal = std::max(
            0.0,
            dot(hitNormal, normalized(lp))
        );

        if(lightVal == 0) {
            return BLACK;
        }

        double lsRaduis = lp.length();

        //Cast shadow
        Ray shadowRay(
            context.hitRec->point,
            normalized(lp)
        );
        MeshHit shadowHit;

        if (context.objects->hit(shadowRay, {SHADOW_BIAS, lsRaduis}, shadowHit)) {
            return BLACK; // * Vector3(1/shadowHit.t);
        }

        //Light contribution
        double lsArea = 4 * PI * lsRaduis * lsRaduis;
        lightVal *= lightSrc.intensity() / lsArea;

        Vector3 colorVec = component_wise(
            color_to_norm_vec(lightSrc.color()),
            hitMaterial.albedo()
        );

        finalColorVec += color_to_norm_vec(lightSrc.color()) * lightVal;
    }

    finalColorVec = Vector3(
        std::min(1.0, finalColorVec.x()),
        std::min(1.0, finalColorVec.y()),
        std::min(1.0, finalColorVec.z())
    );

    return norm_vec_to_color(finalColorVec);
}

Color surface_normal_shading(const ShadeContext& context)
{
    Vector3 vec = Vector3(
        std::abs(context.hitRec->normal.x()),
        std::abs(context.hitRec->normal.y()),
        std::abs(context.hitRec->normal.z())
    );

    return norm_vec_to_color(vec);
}

/*
Color barycentric_shading(const ShadeContext context)
{
    Triangle tri = context.hitRec->triangle;
    Point3 p = context.hitRec->point;
    
    float totalArea = tri.area();
    
    Triangle a (tri[0], tri[2], p);
    Triangle b (tri[0], tri[1], p);
    
    float u = a.area() / totalArea;
    float v = b.area() / totalArea;
    
    return norm_vec_to_color(Vector3(u, v, 0.0));
}
*/
