#include "Probe.h"

Probe::Probe() : UPrimitive()
{
	vertices =
	{
		{  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top vertex (red)
		{  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right vertex (green)
		{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // Bottom-left vertex (blue)
	};

	index = { 0,1,2 };

}

Probe::~Probe()
{
	
}
const ID3D11Buffer*  Probe::getVertexBuffer()
{
	return vertexBuffer;
}
const ID3D11Buffer*  Probe::getIndexBuffer()
{
	return indexBuffer;
}

void Probe::setVertexBuffer(ID3D11Buffer* pBuffer)
{
	if (vertexBuffer != nullptr) return;
	vertexBuffer = pBuffer;
}
void Probe::setIndexBuffer(ID3D11Buffer* pBuffer)
{
	if (indexBuffer != nullptr) return;
	indexBuffer = pBuffer;
}