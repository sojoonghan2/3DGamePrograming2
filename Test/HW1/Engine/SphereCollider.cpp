#include "pch.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"

SphereCollider::SphereCollider() : BaseCollider(ColliderType::Sphere)
{

}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::FinalUpdate()
{
	_boundingSphere.Center = GetGameObject()->GetTransform()->GetWorldPosition();

	Vec3 scale = GetGameObject()->GetTransform()->GetLocalScale();
	_boundingSphere.Radius = _radius * max(max(scale.x, scale.y), scale.z);
}

bool SphereCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	return _boundingSphere.Intersects(rayOrigin, rayDir, OUT distance);
}

bool SphereCollider::Intersects(shared_ptr<BaseCollider> otherCollider)
{
    // 다른 Collider가 SphereCollider인 경우
    if (auto otherSphere = dynamic_pointer_cast<SphereCollider>(otherCollider))
    {
        float distance = (GetCenter() - otherSphere->GetCenter()).Length();
        return distance < (GetRadius() + otherSphere->GetRadius());
    }

    // 다른 Collider가 BoxCollider인 경우
    if (auto otherBox = dynamic_pointer_cast<BoxCollider>(otherCollider))
    {
        Vec3 closestPoint = GetCenter();

        // Box의 경계에 가장 가까운 점 찾기
        closestPoint.x = std::max<float>(otherBox->GetMin().x, std::min<float>(GetCenter().x, otherBox->GetMax().x));
        closestPoint.y = std::max<float>(otherBox->GetMin().y, std::min<float>(GetCenter().y, otherBox->GetMax().y));
        closestPoint.z = std::max<float>(otherBox->GetMin().z, std::min<float>(GetCenter().z, otherBox->GetMax().z));

        // 거리 계산
        Vec3 distanceVec = closestPoint - GetCenter();
        return distanceVec.LengthSquared() <= (GetRadius() * GetRadius());
    }

    return false; // 지원하지 않는 collider일 경우
}
