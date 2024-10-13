#include "pch.h"
#include "TestObjectScript.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

TestObjectScript::TestObjectScript() {}

TestObjectScript::~TestObjectScript() {}

void TestObjectScript::LateUpdate()
{
	shared_ptr<GameObject> mainObject = GET_SINGLE(SceneManager)->FindObjectByName(L"Main_Object");
	shared_ptr<GameObject> targetObject = GET_SINGLE(SceneManager)->FindObjectByName(L"PARTICLE2");

	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::I))
		pos += nmz(GetTransform()->GetLook()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::K))
		pos -= nmz(GetTransform()->GetLook()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::J))
		pos -= nmz(GetTransform()->GetRight()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::L))
		pos += nmz(GetTransform()->GetRight()) * _speed * DELTA_TIME;

	if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		targetObject->GetParticleSystem()->ParticleStart();
	}

	auto bulletObject = GET_SINGLE(SceneManager)->FindObjectByName(L"Bullet");
	auto is_collision = GET_SINGLE(SceneManager)->Collition(GetGameObject(), bulletObject);
	if (is_collision)
	{
		std::cout << "오브젝트 충돌 발생: " << GetGameObject()->GetID() << "\n";
	}

	GetTransform()->SetLocalPosition(pos);
}
