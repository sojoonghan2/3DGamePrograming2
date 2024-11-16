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

    if (_running)
    {
        // �ð� ������Ʈ
        _currentTime += DELTA_TIME;

        // �Ѿ� �̵�
        Vec3 pos = GetTransform()->GetLocalPosition();
        pos += Normalize(_parentLook) * _speed * DELTA_TIME;
        GetTransform()->SetLocalPosition(pos);

        // �ð��� �� �Ǹ� �ʱ�ȭ
        if (_currentTime >= _lifeTime)
        {
            _running = false;
            _currentTime = 0.f;  // Ÿ�̸� ����
            if (_parent)
            {
                GetTransform()->SetLocalPosition(Vec3(_parent->GetTransform()->GetLocalPosition()));
                GetTransform()->SetParent(_parent);
            }
        }
    }
}

void TestBulletScript::KeyboardInput()
{
    if (INPUT->GetButton(KEY_TYPE::SPACE))
    {
        if (!_running) {
            _running = true;
            _currentTime = 0.f;  // Ÿ�̸� �ʱ�ȭ
            _parent = GetTransform()->GetParent().lock();
            if (_parent) _parentLook = _parent->GetLook();
            GetTransform()->RemoveParent();

            Vec3 pos = _parent ? _parent->GetLocalPosition() : GetTransform()->GetLocalPosition();
            GetTransform()->SetLocalPosition(pos);
        }
    }
}