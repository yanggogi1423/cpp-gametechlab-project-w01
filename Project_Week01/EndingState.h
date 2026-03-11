#pragma once
#include "IState.h"
#include <string>

class EndingState : public IState
{
private:
	// 공정 결과 데이터를 보관할 멤버 변수
	bool m_bIsClear;
	unsigned int m_score;
	std::string m_name;

public:
	// 생성자를 통해 결과를 전달받습니다.
	EndingState(bool bIsClear, unsigned int score, std::string name);
	virtual ~EndingState() = default;

	void OnEnter(class UManager* manager) override;
	IState* Update(float deltaTime, class UManager* manager) override;
	void Render(class URenderer* renderer, class UManager* manager) override;
	void OnExit(class UManager* manager) override;
};

