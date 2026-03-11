#pragma once
#include "Uprimitive.h"


class Probe : public UPrimitive
{
private:
public:
	Probe();
	virtual ~Probe();
	virtual DirectX::XMMATRIX GetTransformMatrix();
};
