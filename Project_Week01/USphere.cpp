#include "USphere.h"
#include <cmath>

ID3D11Buffer* USphere::vertexBuffer = nullptr;
ID3D11Buffer* USphere::indexBuffer = nullptr;
std::vector<FVertex> USphere::vertices;
std::vector<unsigned int> USphere::index;


 void USphere::initialize(float InRadius, unsigned int InSegments)
{
    // 1. 메모리 미리 확보 (중심점 1개 + 둘레 점들)
    vertices.reserve(InSegments + 2);

    // 2. 중심점 추가 (피자의 꼭짓점 역할)
    FVertex center = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    vertices.push_back(center);

    // 3. 둘레를 돌며 점 생성
    for (int i = 0; i <= InSegments; ++i)
    {
        float theta = (2.0f * 3.1415926535f * static_cast<float>(i)) / static_cast<float>(InSegments);

        FVertex v;
        v.x = InRadius * cosf(theta);
        v.y = InRadius * sinf(theta);
        v.z = 0.0f;

        // 행성 색상 (여기서 설정하거나 나중에 매니저에서 변경)
        v.r = 1.0f; v.g = 0.5f; v.b = 0.0f; v.a = 1.0f;

        vertices.push_back(v);
    }

    index.reserve(InSegments + 2);
    for (int i = 1; i <= InSegments; ++i)
    {
        // 중심점(0) -> 현재 둘레 점(i) -> 다음 둘레 점(i + 1)
        index.push_back(0);
        index.push_back(i);
        index.push_back(i + 1);
    }

}

USphere::USphere()
{
}

const ID3D11Buffer* USphere::getVertexBuffer()
{
    return vertexBuffer;
}
const  ID3D11Buffer* USphere::getIndexBuffer()
{
    return indexBuffer;
}

void USphere::setVertexBuffer(ID3D11Buffer* pBuffer)
{
    if (vertexBuffer != nullptr) return;
    vertexBuffer = pBuffer;
}
void USphere::setIndexBuffer(ID3D11Buffer* pBuffer)
{
    if (indexBuffer != nullptr) return;
    indexBuffer = pBuffer;
}


