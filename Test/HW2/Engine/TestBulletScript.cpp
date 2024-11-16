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
        // 시간 업데이트
        _currentTime += DELTA_TIME;

        // 총알 이동
        Vec3 pos = GetTransform()->GetLocalPosition();
        pos += Normalize(_parentLook) * _speed * DELTA_TIME;
        GetTransform()->SetLocalPosition(pos);

        // 시간이 다 되면 초기화
        if (_currentTime >= _lifeTime)
        {
            _running = false;
            _currentTime = 0.f;  // 타이머 리셋
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
            _currentTime = 0.f;  // 타이머 초기화
            _parent = GetTransform()->GetParent().lock();
            if (_parent) _parentLook = _parent->GetLook();
            GetTransform()->RemoveParent();

            Vec3 pos = _parent ? _parent->GetLocalPosition() : GetTransform()->GetLocalPosition();
            GetTransform()->SetLocalPosition(pos);
        }
    }
}