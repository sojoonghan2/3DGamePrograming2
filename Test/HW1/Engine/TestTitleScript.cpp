#include "pch.h"
#include "TestTitleScript.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "MeshRenderer.h"

TestTitleScript::TestTitleScript() {}

TestTitleScript::~TestTitleScript() {}

void TestTitleScript::LateUpdate()
{
	if (!initialized)
	{
		if (INPUT->GetButton(KEY_TYPE::LBUTTON) || INPUT->GetButton(KEY_TYPE::RBUTTON))
		{
			// 이 스크립트를 가지고 있는 객체의 MeshRenderer 가져오기
			shared_ptr<MeshRenderer> meshRenderer = GetGameObject()->GetMeshRenderer();

			if (meshRenderer)
			{
				// 렌더링 상태를 토글 (렌더링 활성화/비활성화)
				bool isEnabled = meshRenderer->IsEnabled();
				meshRenderer->SetEnabled(!isEnabled);  // 현재 상태를 반대로 설정
			}
			initialized = true;
		}
	}

}
