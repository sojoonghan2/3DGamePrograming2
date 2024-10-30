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
		return; // 처음에는 충돌 검사를 하지 않음
	}
	KeyboardInput();
	MouseInput();
	CollisionTerrain();

	//std::cout << GetTransform()->GetLocalPosition().x << ", " << GetTransform()->GetLocalPosition().y << ", " << GetTransform()->GetLocalPosition().z << "\n";

	vector<shared_ptr<GameObject>> Objects(10);
	for (int i = 0; i < Objects.size(); ++i)
	{
		// "OBJ" 뒤에 숫자를 붙여서 이름을 만듦
		std::wstring objectName = L"OBJ" + std::to_wstring(i);

		// 해당 이름의 오브젝트를 찾고 벡터에 할당
		Objects[i] = GET_SINGLE(SceneManager)->FindObjectByName(objectName);

		auto is_collision = GET_SINGLE(SceneManager)->Collition(GetGameObject(), Objects[i]);
		if (is_collision)
		{
			std::cout << "플레이어 충돌 발생: ";
			std::wcout << objectName << "\n";
		}
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
	// 마우스 오른쪽 버튼이 눌려 있는지 확인
	if (INPUT->GetButton(KEY_TYPE::LBUTTON))
	{
		// 마우스 이동량을 가져옴 (이 값은 프레임마다 갱신됨)
		POINT mouseDelta = INPUT->GetMouseDelta();

		// 현재 로컬 회전값을 가져옴
		Vec3 rotation = GetTransform()->GetLocalRotation();

		// 마우스 X축 이동은 Y축 회전에 영향을 줌 (좌우 회전)
		rotation.y += mouseDelta.x * 0.005f;

		// 마우스 Y축 이동은 X축 회전에 영향을 줌 (상하 회전)
		rotation.x += mouseDelta.y * 0.005f;

		// 회전값을 다시 설정
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
	if (GetTransform()->GetLocalPosition().y <= _terrain->GetTerrain()->GetHeight(GetTransform()->GetLocalPosition().x,
		GetTransform()->GetLocalPosition().z))
	{
		std::cout << "first: " << GetTransform()->GetLocalPosition().y << "\n";
		std::cout << "second: " << _terrain->GetTerrain()->GetHeight(GetTransform()->GetLocalPosition().x, GetTransform()->GetLocalPosition().z) << "\n";
		std::cout << "터레인 충돌 발생\n";
	}
}