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

private:
	float					_speed = 300.f;
	POINT					m_ptOldCursorPos;

	shared_ptr<GameObject>	_terrain;
};