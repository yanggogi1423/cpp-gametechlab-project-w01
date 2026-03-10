#pragma once
#include <vector>
#include "datatype.h"

class UPrimitive {
protected:
    std::vector<FVertex> vertices; // 정적 모양 데이터 (VB용)
    FVector Location;              // 실시간 위치 데이터
    FVector Velocity;              // 실시간 속도 데이터 (방향 + 크기)
    float Scale;                   // 크기 배율
    float Mass;                    // 질량

public:
    UPrimitive();
    virtual ~UPrimitive();         // 상속을 고려하여 가상 소멸자로 명시

    // 1. 매니저(물리 엔진)가 사용할 함수
    void SetLocation(const FVector& InLocation);
    FVector GetLocation() const;

    void SetVelocity(const FVector& InVelocity); 
    FVector GetVelocity() const;                 

    void SetMass(float InMass);
    float GetMass() const;

    // 2. 렌더러가 사용할 데이터 제공 함수
    const std::vector<FVertex>& GetVertices() const;
    FConstants GetConstants() const;
};