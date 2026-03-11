#pragma once

#include "UPrimitive.h"

struct ID3D11Buffer;

class USphere : public UPrimitive
{
private:
	ImageName imageName = ImageName::PLANET1;

public:
	USphere();

	virtual ~USphere() {};

	virtual DirectX::XMMATRIX GetTransformMatrix();
	ImageName getImageName();
	static int count;

};