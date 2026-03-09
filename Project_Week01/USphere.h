#pragma once

#include "UPrimitive.h"

class USphere : public UPrimitive
{
public:
	USphere(float InRadius = 1.0f, int InSegments = 36);
	virtual ~USphere() {}
};