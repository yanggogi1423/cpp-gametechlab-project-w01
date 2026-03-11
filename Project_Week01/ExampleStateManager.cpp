#include "ExampleStateManager.h"

ExampleStateManager::ExampleStateManager(UManager * manager)
{
	Manager = manager;
	// 예시: 상태 머신 초기화
	stateMachine = new StateMachine(Manager);
}

ExampleStateManager::~ExampleStateManager()
{
	delete stateMachine;
}

void ExampleStateManager::Update(URenderer* renderer)
{
	stateMachine->Update(renderer);
}