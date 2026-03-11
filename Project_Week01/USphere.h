#pragma once

#include "UPrimitive.h"

struct ID3D11Buffer;

class USphere : public UPrimitive
{
private:
	ImageName imageName;

public:
	USphere();
	virtual ~USphere() {};

	virtual DirectX::XMMATRIX GetTransformMatrix();
	ImageName getImageName();
};