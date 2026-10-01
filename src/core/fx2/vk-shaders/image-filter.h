layout(set = 1, binding = 0) uniform sampler2D sourceImage;
layout(push_constant) uniform FilterParameters {
    ivec4 dimensions; // source xy, destination zw
    ivec4 options;    // radius, horizontal, unused, unused
    vec4  weights[4];
}
parameters;
vec4 filterPixel(ivec2 pixel) {
#ifdef BLUR
    vec4  result    = texelFetch(sourceImage, pixel, 0) * parameters.weights[0][0];
    ivec2 direction = parameters.options.y != 0 ? ivec2(1, 0) : ivec2(0, 1);
    for (int i = 1; i <= parameters.options.x; ++i) {
        float weight = parameters.weights[i / 4][i % 4];
        result += weight * texelFetch(sourceImage, clamp(pixel + direction * i, ivec2(0), parameters.dimensions.xy - 1), 0);
        result += weight * texelFetch(sourceImage, clamp(pixel - direction * i, ivec2(0), parameters.dimensions.xy - 1), 0);
    }
    return result;
#else
    vec2 low    = vec2(pixel) * vec2(parameters.dimensions.xy) / vec2(parameters.dimensions.zw);
    vec2 high   = vec2(pixel + 1) * vec2(parameters.dimensions.xy) / vec2(parameters.dimensions.zw);
    vec4 result = vec4(0);
    for (int y = int(floor(low.y)); y < int(ceil(high.y)); ++y)
        for (int x = int(floor(low.x)); x < int(ceil(high.x)); ++x) {
            vec2 weight = max(vec2(0), min(high, vec2(x + 1, y + 1)) - max(low, vec2(x, y)));
            result += texelFetch(sourceImage, ivec2(x, y), 0) * weight.x * weight.y;
        }
    vec2 area = high - low;
    return result / (area.x * area.y);
#endif
}
