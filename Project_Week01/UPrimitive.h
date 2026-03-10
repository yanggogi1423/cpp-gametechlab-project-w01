#pragma once
#include <vector>
#include "datatype.h"



class UPrimitive 
{
protected:
    FVector Location;
    FVector Velocity;
    float Radius;
    float Scale;
    float Mass;
    bool bIsColliding;

public:
    UPrimitive()
        : Location(0.0f, 0.0f, 0.0f), Velocity(0.0f, 0.0f, 0.0f), Scale(1.0f), Mass(1.0f), bIsColliding(false) 
    {
    }

    virtual ~UPrimitive()   
    {
    }

    // 1. 매니저(물리 엔진)가 사용할 함수
    void SetLocation(const FVector& InLocation);
    FVector GetLocation() const;

    void SetVelocity(const FVector& InVelocity); 
    FVector GetVelocity() const;                 

    void SetMass(float InMass);
    float GetMass() const;

    void SetColliding(bool bInColliding);
    bool GetColliding() const;



};