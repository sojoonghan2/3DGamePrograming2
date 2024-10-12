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

	if (GET_SINGLE(SceneManager)->Collition(GetGameObject()))
	{
		std::cout << "�浹 �߻�" << "\n";
	}
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
	if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		const POINT& pos2 = INPUT->GetMousePos();
		GET_SINGLE(SceneManager)->Pick(pos2.x, pos2.y);
	}
}