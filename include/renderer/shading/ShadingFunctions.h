#pragma once

#include "ShadeContext.h"
#include "utils/Color.h"

Color ray_trace(const ShadeContext& context);

Color surface_normal_shading(const ShadeContext& context);

Color barycentric_shading(const ShadeContext context);
