#pragma once
#include <vector>
#include "DataType.h" // 우리가 만든 규격집 포함

class UPrimitive {
protected:
	std::vector<FVertex> vertices; // 정적 모양 데이터 (VB용)
	FVector Location;//실시간 위치 데이터 (FVector 기반)
	float Scale; // 크기 배율

public:
	UPrimitive() : Location(0, 0, 0), Scale(1.0f) {}
	virtual ~UPrimitive() {} // 상속을 위한 가상 소멸자

	// 1. 매니저(물리 엔진)가 사용할 함수
	void SetLocation(const FVector& InLocation)
	{
		Location = InLocation;
	}

	FVector GetLocation() const
	{
		return Location;
	}

	// 2. 렌더러가 사용할 데이터 제공 함수
	const std::vector<FVertex>& GetVertices() const
	{
		return vertices;
	}

	// 상수 버퍼(CB)에 넘겨줄 최종 패키지 생성
	FConstants GetConstants() const
	{
		FConstants Data(Location);
		Data.scale = Scale;
		return Data;
	}
};