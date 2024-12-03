#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void FinalUpdate() override;
	void PushData();

public:
	// Parent 기준
	const Vec3& GetLocalPosition() { return _localPosition; }
	const Vec3& GetLocalRotation() { return _localRotation; }
	const Vec3& GetLocalRevolution() { return _localRevolution; }
	const Vec3& GetLocalScale() { return _localScale; }

	// TEMP
	float GetBoundingSphereRadius() { return max(max(_localScale.x, _localScale.y), _localScale.z); }

	const Matrix& GetLocalToWorldMatrix() { return _matWorld; }
	Vec3 GetWorldPosition() { return _matWorld.Translation(); }

	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp() { return _matWorld.Up(); }
	Vec3 GetLook() { return _matWorld.Backward(); }

	Vec3 GetParentLook() const {
		if (auto parent = _parent.lock()) {
			return parent->GetLook();
		}
		return Vec3();
	}

	void SetLocalPosition(const Vec3& position) { _localPosition = position; }
	void SetLocalPosition(float x, float y, float z) {
		Vec3 vec;
		vec.x = x;
		vec.y = y;
		vec.z = z;
		_localPosition = vec;
	}
	void SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; }
	void SetLocalRevolution(const Vec3& rotation) { _localRevolution = rotation; }
	void SetLocalScale(const Vec3& scale) { _localScale = scale; }

	void LookAt(const Vec3& dir);

	static bool CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());
	static Vec3 DecomposeRotationMatrix(const Matrix& rotation);

public:
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	void RemoveParent() { _parent.reset(); }
	weak_ptr<Transform> GetParent() { return _parent; }

private:
	// Parent 기준
	Vec3 _localPosition = {};
	Vec3 _localRotation = {};
	Vec3 _localRevolution = {};
	Vec3 _localScale = { 1.f, 1.f, 1.f };

	Matrix _matLocal= {};
	Matrix _matWorld = {};

	weak_ptr<Transform> _parent;
};

