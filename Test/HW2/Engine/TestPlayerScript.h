#pragma once
#include "MonoBehaviour.h"

class TestPlayerScript : public MonoBehaviour
{
public:
	TestPlayerScript();
	virtual ~TestPlayerScript();

	virtual void LateUpdate() override;

	void KeyboardInput();
	void MouseInput();

	void CollisionTerrain();
	void AvoidObstacles();

	bool PlayerCollision();

	void ClearConsole() {
#ifdef _WIN32
		system("cls"); // Windows���� �ܼ� Ŭ����
#else
		system("clear"); // Linux, macOS���� �ܼ� Ŭ����
#endif
	}

private:
	float					_speed = 1000.f;
	POINT					m_ptOldCursorPos;

	shared_ptr<GameObject>	_terrain;
};