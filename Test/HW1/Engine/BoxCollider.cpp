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
    // BoxCollider와 BoxCollider의 충돌 검사
    if (auto otherBox = dynamic_pointer_cast<BoxCollider>(otherCollider))
    {
        // Box의 월드 좌표 계산
        Vec3 worldCenterA = GetGameObject()->GetTransform()->GetWorldPosition() + _center;
        Vec3 minA = worldCenterA - _extents; // 현재 박스의 최소 점 (월드 좌표)
        Vec3 maxA = worldCenterA + _extents; // 현재 박스의 최대 점 (월드 좌표)

        Vec3 worldCenterB = otherBox->GetGameObject()->GetTransform()->GetWorldPosition() + otherBox->GetCenter();
        Vec3 minB = worldCenterB - otherBox->GetExtents(); // 다른 박스의 최소 점 (월드 좌표)
        Vec3 maxB = worldCenterB + otherBox->GetExtents(); // 다른 박스의 최대 점 (월드 좌표)

        // AABB 충돌 검사 (월드 좌표 기준)
        return (minA.x <= maxB.x && maxA.x >= minB.x &&
            minA.y <= maxB.y && maxA.y >= minB.y &&
            minA.z <= maxB.z && maxA.z >= minB.z);
    }

    // BoxCollider와 SphereCollider의 충돌 검사
    if (auto otherSphere = dynamic_pointer_cast<SphereCollider>(otherCollider))
    {
        // Sphere의 월드 좌표 계산
        Vec3 sphereCenter = otherSphere->GetGameObject()->GetTransform()->GetWorldPosition() + otherSphere->GetCenter();
        float sphereRadius = otherSphere->GetRadius();

        // Box의 월드 좌표 계산
        Vec3 worldCenter = GetGameObject()->GetTransform()->GetWorldPosition() + _center;
        Vec3 min = worldCenter - _extents;
        Vec3 max = worldCenter + _extents;

        Vec3 closestPoint = sphereCenter;

        // Box의 경계에서 Sphere의 중심에 가장 가까운 점을 찾기
        closestPoint.x = std::max<float>(min.x, std::min<float>(sphereCenter.x, max.x));
        closestPoint.y = std::max<float>(min.y, std::min<float>(sphereCenter.y, max.y));
        closestPoint.z = std::max<float>(min.z, std::min<float>(sphereCenter.z, max.z));

        // 거리 계산
        Vec3 distanceVec = closestPoint - sphereCenter;
        return distanceVec.LengthSquared() <= (sphereRadius * sphereRadius);
    }

    return false; // 지원하지 않는 Collider일 경우
}