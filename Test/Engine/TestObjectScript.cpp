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
			return; // 처음에는 충돌 검사를 하지 않음
		}
		// OBJn 형식에서 n 번호 추출
		wstring objectName = GetGameObject()->GetName();
		size_t index = objectName.find(L"OBJ");
		if (index != wstring::npos)
		{
			// "OBJ" 이후의 번호 추출
			wstring numberStr = objectName.substr(3);  // "OBJ" 다음의 부분만 남김
			int objectNumber = std::stoi(numberStr);   // 문자열을 숫자로 변환

			// 해당 번호의 파티클 오브젝트 이름 생성 (PARTICLEn)
			wstring particleName = L"PARTICLE" + std::to_wstring(objectNumber);

			// 해당 파티클 오브젝트 찾기
			shared_ptr<GameObject> targetObject = GET_SINGLE(SceneManager)->FindObjectByName(particleName);
			if (targetObject && targetObject->GetParticleSystem())
			{
				// 파티클 시작
				//targetObject->GetParticleSystem()->ParticleStart();
				std::cout << "오브젝트 충돌 발생: ";
				std::wcout << particleName << std::endl;
			}
		}
	}
	GetTransform()->SetLocalPosition(pos);
}
