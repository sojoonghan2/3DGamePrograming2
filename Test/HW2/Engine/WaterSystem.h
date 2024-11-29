#pragma once
#include "Component.h"

class WaterSystem : public Component
{
public:
	WaterSystem();
	virtual ~WaterSystem();

public:
	virtual void FinalUpdate();
	void Render(vector<shared_ptr<GameObject>>& gameObjects);
};

