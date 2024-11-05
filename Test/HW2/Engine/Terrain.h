#pragma once
#include "Component.h"
#include <vector>

class Terrain : public Component
{
public:
	Terrain();
	virtual ~Terrain();

	void Init(int32 sizeX, int32 sizeZ);

	float GetHeight(float x, float z);

	virtual void FinalUpdate() override;

private:
	int32						_sizeX = 15;
	int32						_sizeZ = 15;
	float						_maxTesselation = 4.f;

	shared_ptr<class Texture>	_heightMap;
	shared_ptr<class Material>	_material;

	int32                       _heightMapWidth = 0;
	int32                       _heightMapHeight = 0;
};
