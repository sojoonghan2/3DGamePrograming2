#pragma once
#include "MonoBehaviour.h"

class TestPlayerScript : public MonoBehaviour
{
public:
	TestPlayerScript();
	virtual ~TestPlayerScript();

	virtual void LateUpdate() override;

private:
	float		_speed = 100.f;
};

