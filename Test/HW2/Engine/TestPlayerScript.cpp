#include "pch.h"
#include "TestPlayerScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Terrain.h"

TestPlayerScript::TestPlayerScript()
{

}

TestPlayerScript::~TestPlayerScript()
{
}

void TestPlayerScript::LateUpdate()
{
	static bool initialized = false;
	if (!initialized)
	{
		_terrain = GET_SINGLE(SceneManager)->FindObjectByName(L"Terrain");
		initialized = true;
		return; // ó������ �浹 �˻縦 ���� ����
	}
	KeyboardInput();
	MouseInput();
	CollisionTerrain();

	if (PlayerCollision())
	{
		//AvoidObstacles();
	}
}

void TestPlayerScript::KeyboardInput()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += Normalize(GetTransform()->GetLook()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= Normalize(GetTransform()->GetLook()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= Normalize(GetTransform()->GetRight()) * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += Normalize(GetTransform()->GetRight()) * _speed * DELTA_TIME;

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


void TestPlayerScript::CollisionTerrain()
{
	ClearConsole();
	// ����Ʈ�� ��� �ڵ�
	std::cout << "***********************\n";
	std::cout << "xPos: " << GetTransform()->GetLocalPosition().x << "\n";
	std::cout << "yPos: " << GetTransform()->GetLocalPosition().y << "\n";
	std::cout << "zPos: " << GetTransform()->GetLocalPosition().z << "\n";
	std::cout << "Height: " << _terrain->GetTerrain()->GetHeight(
		GetTransform()->GetLocalPosition().x,
		GetTransform()->GetLocalPosition().z) << "\n";
	std::cout << "***********************\n";

	float x = GetTransform()->GetLocalPosition().x;
	float z = GetTransform()->GetLocalPosition().z;

	// �ͷ��� ���� ���
	float terrainHeight = _terrain->GetTerrain()->GetHeight(x, z) - 200;

	// ���� ��ü�� ��ġ
	Vec3 currentPosition = GetTransform()->GetLocalPosition();

	// �浹 ����
	if (currentPosition.y < terrainHeight)
	{
		// �ͷ��� ���� ��ġ ����
		currentPosition.y = terrainHeight;
		GetTransform()->SetLocalPosition(currentPosition);
	}

}

void TestPlayerScript::AvoidObstacles()
{
	Vec3 forward = Normalize(GetTransform()->GetLook());
	// Y���� �ణ �÷� ȸ���� ���� ����
	Vec3 upwardAdjustment = Vec3(0.0f, 0.001f, 0.0f);
	// ���� �̼� ������ ���ο� ����
	Vec3 newLookDirection = Normalize(forward + upwardAdjustment); 

	// �� �������� �� ���� ����
	GetTransform()->LookAt(newLookDirection);
}

bool TestPlayerScript::PlayerCollision()
{
	vector<shared_ptr<GameObject>> Objects(10);
	for (int i = 0; i < Objects.size(); ++i)
	{
		// "OBJ" �ڿ� ���ڸ� �ٿ��� �̸��� ����
		std::wstring objectName = L"OBJ" + std::to_wstring(i);

		// �ش� �̸��� ������Ʈ�� ã�� ���Ϳ� �Ҵ�
		Objects[i] = GET_SINGLE(SceneManager)->FindObjectByName(objectName);

		auto is_collision = GET_SINGLE(SceneManager)->Collition(GetGameObject(), Objects[i]);
		if (is_collision)
		{
			std::cout << "�÷��̾� �浹 �߻�: ";
			std::wcout << objectName << "\n";
			return true;
		}
	}
	return false;
}