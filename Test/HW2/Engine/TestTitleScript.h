#pragma once
#include "MonoBehaviour.h"

class TestTitleScript : public MonoBehaviour
{
public:
	TestTitleScript();
	virtual ~TestTitleScript();

	virtual void LateUpdate() override;

private:
	float		_speed = 100.f;
	bool		initialized = false;
};

