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
        pos += Normalize(GetTransform()->GetLook()) * _speed * DELTA_TIME;
    if (INPUT->GetButton(KEY_TYPE::K))
        pos -= Normalize(GetTransform()->GetLook()) * _speed * DELTA_TIME;
    if (INPUT->GetButton(KEY_TYPE::J))
        pos -= Normalize(GetTransform()->GetRight()) * _speed * DELTA_TIME;
    if (INPUT->GetButton(KEY_TYPE::L))
        pos += Normalize(GetTransform()->GetRight()) * _speed * DELTA_TIME;

    // 모든 총알에 대해 충돌 검사
    for (int i = 0; i < 50; ++i)
    {
        wstring bulletName = L"Bullet" + to_wstring(i);
        auto bulletObject = GET_SINGLE(SceneManager)->FindObjectByName(bulletName);

        if (bulletObject)  // 총알이 존재하면
        {
            auto is_collision = GET_SINGLE(SceneManager)->Collition(GetGameObject(), bulletObject);
            if (is_collision)
            {
                if (!initialized)
                {
                    initialized = true;
                    return;
                }

                // OBJn 형식에서 n 번호 추출
                wstring objectName = GetGameObject()->GetName();
                size_t index = objectName.find(L"OBJ");
                if (index != wstring::npos)
                {
                    wstring numberStr = objectName.substr(3);
                    int objectNumber = std::stoi(numberStr);
                    wstring particleName = L"PARTICLE" + std::to_wstring(objectNumber);

                    shared_ptr<GameObject> targetObject = GET_SINGLE(SceneManager)->FindObjectByName(particleName);
                    if (targetObject && targetObject->GetParticleSystem())
                    {
                        targetObject->GetParticleSystem()->ParticleStart();
                        std::cout << "오브젝트 충돌 발생: ";
                        std::wcout << particleName << std::endl;
                    }
                }
                break;  // 충돌이 발생하면 더 이상 검사하지 않음
            }
        }
    }

    GetTransform()->SetLocalPosition(pos);
}