#include "pch.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Transform.h"

BoxCollider::BoxCollider() : BaseCollider(ColliderType::Box)
{

}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::FinalUpdate()
{
	_boundingBox.Center = GetGameObject()->GetTransform()->GetWorldPosition();

	Vec3 scale = GetGameObject()->GetTransform()->GetLocalScale();
	_boundingBox.Extents = _extents * scale;
}

bool BoxCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	return _boundingBox.Intersects(rayOrigin, rayDir, OUT distance);
}

bool BoxCollider::Intersects(shared_ptr<BaseCollider> otherCollider)
{
    // BoxCollider�� BoxCollider�� �浹 �˻�
    if (auto otherBox = dynamic_pointer_cast<BoxCollider>(otherCollider))
    {
        // Box�� ���� ��ǥ ���
        Vec3 worldCenterA = GetGameObject()->GetTransform()->GetWorldPosition() + _center;
        Vec3 minA = worldCenterA - _extents; // ���� �ڽ��� �ּ� �� (���� ��ǥ)
        Vec3 maxA = worldCenterA + _extents; // ���� �ڽ��� �ִ� �� (���� ��ǥ)

        Vec3 worldCenterB = otherBox->GetGameObject()->GetTransform()->GetWorldPosition() + otherBox->GetCenter();
        Vec3 minB = worldCenterB - otherBox->GetExtents(); // �ٸ� �ڽ��� �ּ� �� (���� ��ǥ)
        Vec3 maxB = worldCenterB + otherBox->GetExtents(); // �ٸ� �ڽ��� �ִ� �� (���� ��ǥ)

        // AABB �浹 �˻� (���� ��ǥ ����)
        return (minA.x <= maxB.x && maxA.x >= minB.x &&
            minA.y <= maxB.y && maxA.y >= minB.y &&
            minA.z <= maxB.z && maxA.z >= minB.z);
    }

    // BoxCollider�� SphereCollider�� �浹 �˻�
    if (auto otherSphere = dynamic_pointer_cast<SphereCollider>(otherCollider))
    {
        // Sphere�� ���� ��ǥ ���
        Vec3 sphereCenter = otherSphere->GetGameObject()->GetTransform()->GetWorldPosition() + otherSphere->GetCenter();
        float sphereRadius = otherSphere->GetRadius();

        // Box�� ���� ��ǥ ���
        Vec3 worldCenter = GetGameObject()->GetTransform()->GetWorldPosition() + _center;
        Vec3 min = worldCenter - _extents;
        Vec3 max = worldCenter + _extents;

        Vec3 closestPoint = sphereCenter;

        // Box�� ��迡�� Sphere�� �߽ɿ� ���� ����� ���� ã��
        closestPoint.x = std::max<float>(min.x, std::min<float>(sphereCenter.x, max.x));
        closestPoint.y = std::max<float>(min.y, std::min<float>(sphereCenter.y, max.y));
        closestPoint.z = std::max<float>(min.z, std::min<float>(sphereCenter.z, max.z));

        // �Ÿ� ���
        Vec3 distanceVec = closestPoint - sphereCenter;
        return distanceVec.LengthSquared() <= (sphereRadius * sphereRadius);
    }

    return false; // �������� �ʴ� Collider�� ���
}