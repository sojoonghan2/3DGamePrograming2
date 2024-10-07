#include "pch.h"
#include "TestPlayerScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

TestPlayerScript::TestPlayerScript()
{
}

TestPlayerScript::~TestPlayerScript()
{
}

void TestPlayerScript::LateUpdate()
{
	KeyboardInput();
	MouseInput();
}

void TestPlayerScript::KeyboardInput()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += nmz(GetTransform()->GetLook()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= nmz(GetTransform()->GetLook()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= nmz(GetTransform()->GetRight()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += nmz(GetTransform()->GetRight()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= DELTA_TIME * 0.5f; // X�� ȸ��
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::E))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += DELTA_TIME * 0.5f; // X�� ȸ��
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::C))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::Z))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y -= DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}


	GetTransform()->SetLocalPosition(pos);
}

void TestPlayerScript::MouseInput()
{
	// ���콺 ������ ��ư�� ���� �ִ��� Ȯ��
	if (INPUT->GetButton(KEY_TYPE::LBUTTON))
	{
		// ���콺 �̵����� ������ (�� ���� �����Ӹ��� ���ŵ�)
		POINT mouseDelta = INPUT->GetMouseDelta();

		// ���� ���� ȸ������ ������
		Vec3 rotation = GetTransform()->GetLocalRotation();

		// ���콺 X�� �̵��� Y�� ȸ���� ������ �� (�¿� ȸ��)
		rotation.y += mouseDelta.x * 0.005f;

		// ���콺 Y�� �̵��� X�� ȸ���� ������ �� (���� ȸ��)
		rotation.x += mouseDelta.y * 0.005f;

		// ȸ������ �ٽ� ����
		GetTransform()->SetLocalRotation(rotation);
	}
}