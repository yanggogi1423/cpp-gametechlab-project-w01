#include "ExampleStateManager.h"

ExampleStateManager::ExampleStateManager()
{
	// 예시: 상태 머신 초기화
	 stateMachine = new StateMachine();
}

ExampleStateManager::~ExampleStateManager()
{
	delete stateMachine;
}

void ExampleStateManager::Update(URenderer* renderer)
{
	stateMachine->Update(renderer);
}