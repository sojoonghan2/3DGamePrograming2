#pragma once
#include "pch.h"
#include "GameObject.h"

class BulletObject : public GameObject, public enable_shared_from_this<BulletObject>
{
public:
	BulletObject();
	~BulletObject();

private:
	float						_fBulletEffectiveRange = 50.0f;
	float						_fMovingDistance = 0.0f;
	XMFLOAT3					_xmf3FirePosition = XMFLOAT3(0.0f, 0.0f, 1.0f);
	float						_fElapsedTimeAfterFire = 0.0f;
};

