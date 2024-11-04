#include "pch.h"
#include "TestBulletScript.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

TestBulletScript::TestBulletScript()
{
}

TestBulletScript::~TestBulletScript()
{
}

void TestBulletScript::LateUpdate()
{
	KeyboardInput();
	MouseInput();

    if (_running)
    {
        Vec3 pos = GetTransform()->GetLocalPosition();
        pos += nmz(_parentLook) * _speed * DELTA_TIME; // Look ���͸� �̿��Ͽ� �̵�
        GetTransform()->SetLocalPosition(pos);
    }
    if (GetTransform()->GetLocalPosition().z > 3000.f)
    {
        _running = false;
        if(_parent) GetTransform()->SetLocalPosition(Vec3(_parent->GetTransform()->GetLocalPosition()));
        GetTransform()->SetParent(_parent);
    }

    //std::cout << GetTransform()->GetLocalPosition().x << ", " << GetTransform()->GetLocalPosition().y << ", " << GetTransform()->GetLocalPosition().z << "\n";
}

void TestBulletScript::KeyboardInput()
{
    Vec3 pos = GetTransform()->GetLocalPosition();

    if (INPUT->GetButton(KEY_TYPE::SPACE))
    {
        // �Ѿ� �߻� ����
        if (!_running) {
            _running = true;

            _parent = GetTransform()->GetParent().lock();
            if (_parent)_parentLook = _parent->GetLook();

            GetTransform()->RemoveParent();

            // �θ��� ��ġ�� �Ѿ� ��ġ �ʱ�ȭ
            pos = _parent ? _parent->GetLocalPosition() : pos;
        }
        // �Ѿ��� ��ġ�� ������Ʈ
        GetTransform()->SetLocalPosition(pos);
    }
}

void TestBulletScript::MouseInput()
{
}