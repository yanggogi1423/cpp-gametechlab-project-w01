#include "UPrimitive.h"
UPrimitive::UPrimitive()
{
	Location = FVector(0, 0, 0);
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

void UPrimitive::SetMass(float InMass)
{
	Mass = InMass;
}

float UPrimitive::GetMass() const
{
	return Mass;
}

const std::vector<FVertex>& UPrimitive::GetVertices() const
{
	return vertices;
}

FConstants UPrimitive::GetConstants() const
{
	FConstants Data(Location);
	Data.scale = Scale;
	return Data;
}