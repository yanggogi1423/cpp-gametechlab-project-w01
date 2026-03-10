#include "UPrimitive.h"

UPrimitive::UPrimitive()
{
    Location = FVector(0.0f, 0.0f, 0.0f);
    Velocity = FVector(0.0f, 0.0f, 0.0f);
    Scale = 1.0f;
    Mass = 1.0f;
}

UPrimitive::~UPrimitive()
{
}

void UPrimitive::SetLocation(const FVector& InLocation)
{
    Location = InLocation;
}

FVector UPrimitive::GetLocation() const
{
    return Location;
}

void UPrimitive::SetVelocity(const FVector& InVelocity)
{
    Velocity = InVelocity;
}

FVector UPrimitive::GetVelocity() const
{
    return Velocity;
}

void UPrimitive::SetMass(float InMass)
{
    Mass = InMass;
}

float UPrimitive::GetMass() const
{
    return Mass;
}

void UPrimitive::SetColliding(bool bInColliding)  
{
    bIsColliding = bInColliding;
}

bool UPrimitive::GetColliding() const
{
    return bIsColliding;
}

const std::vector<FVertex>& UPrimitive::GetVertices() const
{
    return vertices;
}

FConstants UPrimitive::GetConstants() const
{
    // 현재 Location을 기반으로 상수 버퍼 데이터 패키징
    FConstants Data(Location);
    Data.scale = Scale;
    return Data;
}
