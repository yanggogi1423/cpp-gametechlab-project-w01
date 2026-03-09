#pragma once
#include <vector>
#include "datatype.h"

class UPrimitive 
{
protected:
	std::vector<FVertex> vertices;
	FVector Location;
	float Scale;

public:
	UPrimitive();
	virtual ~UPrimitive(); // 상속을 위한 가상 소멸자

	// 1. 매니저(물리 엔진)가 사용할 함수
	void SetLocation(const FVector& InLocation);
	FVector GetLocation() const;

	// 2. 렌더러가 사용할 데이터 제공 함수
	const std::vector<FVertex>& GetVertices() const;

	// 상수 버퍼(CB)에 넘겨줄 최종 패키지 생성
	FConstants GetConstants() const;
};