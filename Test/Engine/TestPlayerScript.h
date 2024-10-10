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

private:
	float		_speed = 200.f;
	POINT		m_ptOldCursorPos;
};

