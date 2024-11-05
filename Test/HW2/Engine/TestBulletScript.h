#pragma once
#include "MonoBehaviour.h"
#include "pch.h"

class TestBulletScript : public MonoBehaviour
{
public:
	TestBulletScript();
	virtual ~TestBulletScript();

	virtual void LateUpdate() override;

	void KeyboardInput();
	void MouseInput();

private:
	float					_speed = 1000.f;
	bool					_running = false;

	Vec3					_parentLook;
	shared_ptr<Transform>	_parent;
};

