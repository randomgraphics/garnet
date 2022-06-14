#include "pch.h"
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Eigen/Eigen>
#include "directxmath.h"

using namespace GN;
using namespace DirectX;

static size_t COUNT = 1024 * 1024 * 64;

template<typename V4, typename M4>
V4 matrixTransform(const M4 & m) {
    V4 v1(0.1f, 0.2f, 0.3f, 0.4f);
    V4 v2 = V4(0.f, 0.f, 0.f, 0.f);
    for (size_t i = 0; i < COUNT; ++i) { v2 += m * v1; }
    return v2;
}

glm::vec4 glmtest(const float * t) {
    auto m = glm::translate(glm::mat4(1.), glm::vec3(t[0], t[1], t[2]));
    GN_FUNCTION_PROFILER();
    return matrixTransform<glm::vec4, glm::mat4>(m);
}

Eigen::Vector4f EigenTest(const float * t) {
    auto m = Eigen::Affine3f(Eigen::Translation<float, 3>(t[0], t[1], t[2])).matrix();
    GN_FUNCTION_PROFILER();
    return matrixTransform<Eigen::Vector4f, Eigen::Matrix4f>(m);
}

DirectX::XMVECTOR DirectXMathTest(const float * t) {
    XMMATRIX m = XMMatrixTranslation(t[0], t[1], t[2]);
    GN_FUNCTION_PROFILER();
    XMVECTOR v1 = XMVectorSet(0.1f, 0.2f, 0.3f, 0.4f);
    XMVECTOR v2 = XMVectorSet(0.f, 0.f, 0.f, 0.f);
    for (size_t i = 0; i < COUNT; ++i) { v2 += XMVector4Transform(v1, m); }
    return v2;
}

int main() {
    float t[3];
    for (int i = 0; i < 3; ++i) { t[i] = (float) rand() / RAND_MAX; }

    {
        auto v = DirectXMathTest(t);
        printf("DirectXMath: %f, %f, %f, %f\n", DirectX::XMVectorGetX(v), DirectX::XMVectorGetY(v), DirectX::XMVectorGetZ(v), DirectX::XMVectorGetW(v));
    }
    {
        auto v = glmtest(t);
        printf("glm: %f, %f, %f, %f\n", v.x, v.y, v.z, v.w);
    }
    {
        auto v = EigenTest(t);
        printf("Eigen: %f, %f, %f, %f\n", v.x(), v.y(), v.z(), v.w());
    }
}
