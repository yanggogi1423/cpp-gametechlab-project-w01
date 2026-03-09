#include "Probe.h"

Probe::Probe() : UPrimitive()
{
	__super::UPrimitive();

	vertices =
	{
		{  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top vertex (red)
		{  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right vertex (green)
		{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // Bottom-left vertex (blue)
	};
}

Probe::~Probe()
{
	
}

FVector Probe::GetVelocity()
{
	return velocity;
}

void Probe::SetVeclocity(FVector velocity)
{
	this->velocity = velocity;
}