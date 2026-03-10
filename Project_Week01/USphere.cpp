#include "USphere.h"
#include <cmath>

USphere::USphere(float InRadius, int InSegments)
{
    // 1. 메모리 미리 확보 (중심점 1개 + 둘레 점들)
    vertices.reserve(InSegments + 2);

    // 2. 중심점 추가 (피자의 꼭짓점 역할)
    FVertex center = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    vertices.push_back(center);

    // 3. 둘레를 돌며 점 생성
    for (int i = 0; i <= InSegments; ++i)
    {
        float theta = (2.0f * 3.1415926535f * (float)i) / (float)InSegments;

        FVertex v;
        v.x = InRadius * cosf(theta);
        v.y = InRadius * sinf(theta);
        v.z = 0.0f;

        // 행성 색상 (여기서 설정하거나 나중에 매니저에서 변경)
        v.r = 1.0f; v.g = 0.5f; v.b = 0.0f; v.a = 1.0f;

        vertices.push_back(v);
    }
}