#include "USphere.h"
#include <cmath>


USphere::USphere()
{
}

DirectX::XMMATRIX USphere::GetTransformMatrix()
{
	DirectX::XMMATRIX ret = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f);
	return ret;
}

ImageName USphere::getImageName()
{
	return imageName;
}

