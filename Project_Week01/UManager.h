#pragma once

//	None 상태를 갖지 않음
enum class ERunstate
{
	RS_Main,
	RS_InGame,
	RS_Ending
};

enum class EStage
{
	S_None,
	S_Stage1,
	S_Stage2,
	S_Stage3
};

class UManager
{
private:


public:
	UManager() = default;
	~UManager() = default;

private:
	//	Collision 관련
	void CollisionDetection();
	void CollsionResolution();



public:
	void Update(float deltaTime);


};