#pragma once
#include "Component.h"

class Terrain : public Component
{
public:
	Terrain();
	virtual ~Terrain();

	void Init(int32 sizeX, int32 sizeZ);

	virtual void FinalUpdate() override;

	float GetHeightAtPosition(float xPos, float yPos);

private:
	int32 _sizeX = 15;
	int32 _sizeZ = 15;
	float _maxTesselation = 4.f;

	shared_ptr<class Material> _material;
};


