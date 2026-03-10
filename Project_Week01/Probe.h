#pragma once
#include "Uprimitive.h"


struct ID3D11Buffer;

class Probe : public UPrimitive
{
private:
	static ID3D11Buffer* vertexBuffer;
	static ID3D11Buffer* indexBuffer;


public:
	Probe();
	virtual ~Probe();

	static  const  ID3D11Buffer* getVertexBuffer();
	static  const  ID3D11Buffer* getIndexBuffer();

	static void setVertexBuffer(ID3D11Buffer*);
	static void setIndexBuffer(ID3D11Buffer*);

};
