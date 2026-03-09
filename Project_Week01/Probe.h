#pragma once
#include "Uprimitive.h"

class Probe : public UPrimitive
{
private:
	FVector velocity;

public:
	Probe();
	~Probe();
	FVector GetVelocity();
	void SetVeclocity(FVector velocity);
};
