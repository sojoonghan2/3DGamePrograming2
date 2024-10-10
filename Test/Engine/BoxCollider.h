#pragma once
#include "BaseCollider.h"

class BoxCollider : public BaseCollider
{
public:
	BoxCollider();
	virtual ~BoxCollider();

	virtual void FinalUpdate() override;
	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;

	void SetExtents(Vec3 extents) { _extents = extents; }
	void SetCenter(Vec3 center) { _center = center; }

	Vec3 GetRadius() { return _extents; }
	Vec3 GetCenter() { return _center; }

private:
	// Local ±‚¡ÿ
	Vec3		_extents = Vec3(0, 0, 0);
	Vec3		_center = Vec3(0, 0, 0);

	BoundingOrientedBox _boundingBox;
};

