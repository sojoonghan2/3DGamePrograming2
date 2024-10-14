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

	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::I))
		pos += nmz(GetTransform()->GetLook()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::K))
		pos -= nmz(GetTransform()->GetLook()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::J))
		pos -= nmz(GetTransform()->GetRight()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::L))
		pos += nmz(GetTransform()->GetRight()) * _speed * DELTA_TIME;

	auto bulletObject = GET_SINGLE(SceneManager)->FindObjectByName(L"Bullet");

	auto is_collision = GET_SINGLE(SceneManager)->Collition(GetGameObject(), bulletObject);

	if (is_collision)
	{
		if (!initialized)
		{
			initialized = true;
			return; // ó������ �浹 �˻縦 ���� ����
		}
		// OBJn ���Ŀ��� n ��ȣ ����
		wstring objectName = GetGameObject()->GetName();
		size_t index = objectName.find(L"OBJ");
		if (index != wstring::npos)
		{
			// "OBJ" ������ ��ȣ ����
			wstring numberStr = objectName.substr(3);  // "OBJ" ������ �κи� ����
			int objectNumber = std::stoi(numberStr);   // ���ڿ��� ���ڷ� ��ȯ

			// �ش� ��ȣ�� ��ƼŬ ������Ʈ �̸� ���� (PARTICLEn)
			wstring particleName = L"PARTICLE" + std::to_wstring(objectNumber);

			// �ش� ��ƼŬ ������Ʈ ã��
			shared_ptr<GameObject> targetObject = GET_SINGLE(SceneManager)->FindObjectByName(particleName);
			if (targetObject && targetObject->GetParticleSystem())
			{
				// ��ƼŬ ����
				//targetObject->GetParticleSystem()->ParticleStart();
				std::cout << "������Ʈ �浹 �߻�: ";
				std::wcout << particleName << std::endl;
			}
		}
	}
	GetTransform()->SetLocalPosition(pos);
}
