#include "USphere.h"
#include <cmath>

USphere::USphere(float InRadius, int InSegments) 
{
    FVertex center;
    center.x = 0; center.y = 0; center.z = 0;
    center.r = 1.0f; center.g = 1.0f; center.b = 1.0f; center.a = 1.0f;

    // 원의 둘레를 따라 정점 생성
    for (int i = 0; i <= InSegments; ++i) {
        // 각도 계산 (라디안)
        float theta = (2.0f * 3.1415926535f * (float)i) / (float)InSegments;

        FVertex v;
        v.x = InRadius * cosf(theta); // x = r * cos
        v.y = InRadius * sinf(theta); // y = r * sin
        v.z = 0.0f;

        // 일단 흰색으로 설정 (나중에 행성마다 색상을 바꿀 수 있게 설계 가능)
        v.r = 1.0f; v.g = 1.0f; v.b = 1.0f; v.a = 1.0f;

        vertices.push_back(v); // 부모인 UPrimitive의 vertices 리스트에 저장
    }
}