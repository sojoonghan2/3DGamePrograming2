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
			// �� ��ũ��Ʈ�� ������ �ִ� ��ü�� MeshRenderer ��������
			shared_ptr<MeshRenderer> meshRenderer = GetGameObject()->GetMeshRenderer();

			if (meshRenderer)
			{
				// ������ ���¸� ��� (������ Ȱ��ȭ/��Ȱ��ȭ)
				bool isEnabled = meshRenderer->IsEnabled();
				meshRenderer->SetEnabled(!isEnabled);  // ���� ���¸� �ݴ�� ����
			}
			initialized = true;
		}
	}

}
