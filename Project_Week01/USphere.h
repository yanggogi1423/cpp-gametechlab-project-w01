#pragma once

#include "UPrimitive.h"

struct ID3D11Buffer;

class USphere : public UPrimitive
{
private:
	static ID3D11Buffer* vertexBuffer;
	static ID3D11Buffer* indexBuffer;

public:
	USphere(float InRadius = 1.0f, int InSegments = 36);
	virtual ~USphere() {}


	static  const  ID3D11Buffer*getVertexBuffer();
	static  const  ID3D11Buffer*getIndexBuffer();

	static void setVertexBuffer(ID3D11Buffer*);
	static void setIndexBuffer(ID3D11Buffer*);

};