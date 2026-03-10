#include "Probe.h"

ID3D11Buffer* Probe::vertexBuffer = nullptr;
ID3D11Buffer* Probe::indexBuffer = nullptr;
std::vector<FVertex> Probe::vertices;
std::vector<unsigned int> Probe::index;

Probe::Probe() : UPrimitive()
{}

Probe::~Probe()
{}

void Probe::initialize()
{
	vertices =
	{
		{  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top vertex (red)
		{  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right vertex (green)
		{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // Bottom-left vertex (blue)
	};

	index = { 0,1,2 };

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

const std::vector<FVertex>& Probe::GetVertices() const
{
	return vertices;
}

const std::vector<unsigned int>& Probe::GetIndex() const
{
	return index;
}