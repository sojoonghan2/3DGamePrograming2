#include "pch.h"
#include "TestCameraScript.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

TestCameraScript::TestCameraScript() {}

TestCameraScript::~TestCameraScript() {}

void TestCameraScript::LateUpdate()
{
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

	GetTransform()->SetLocalPosition(pos);
}
