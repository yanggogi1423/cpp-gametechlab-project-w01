#pragma once

#include "UPrimitive.h"

struct ID3D11Buffer;

class USphere : public UPrimitive
{
private:
	static ID3D11Buffer* vertexBuffer;
	static ID3D11Buffer* indexBuffer;

	static std::vector<FVertex> vertices;
	static std::vector<unsigned int> index;

public:
	USphere();
	virtual ~USphere() {}


	static const ID3D11Buffer*getVertexBuffer();
	static const ID3D11Buffer*getIndexBuffer();

	static void setVertexBuffer(ID3D11Buffer*);
	static void setIndexBuffer(ID3D11Buffer*);

	static void initialize(float InRadius = 1.0f, unsigned int InSegments = 36);

	virtual const std::vector<FVertex>& GetVertices() const override;
	virtual const std::vector<unsigned int>& GetIndex() const override;

};