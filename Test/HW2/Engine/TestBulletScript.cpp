#include "pch.h"
#include "TestBulletScript.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

float TestBulletScript::s_lastFireTime = 0.f;
const float TestBulletScript::s_fireInterval = 5.f; // 간격으로 발사
int32 TestBulletScript::s_currentBulletIndex = 0;

TestBulletScript::TestBulletScript()
{
    _myIndex = s_currentBulletIndex;
    s_currentBulletIndex++;

    // 50개의 총알을 순환하도록
    if (s_currentBulletIndex >= 50)
        s_currentBulletIndex = 0;
}

TestBulletScript::~TestBulletScript()
{
}

void TestBulletScript::LateUpdate()
{
    KeyboardInput();
    if (_running)
    {
        _currentTime += DELTA_TIME;
        Vec3 pos = GetTransform()->GetLocalPosition();
        pos += Normalize(_parentLook) * _speed * DELTA_TIME;
        GetTransform()->SetLocalPosition(pos);

        if (_currentTime >= _lifeTime)
        {
            _running = false;
            _currentTime = 0.f;
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
        s_lastFireTime += DELTA_TIME;

        if (!_running &&
            s_lastFireTime >= s_fireInterval &&
            (_myIndex == s_currentBulletIndex))  // 수정된 부분
        {
            s_lastFireTime = 0.f;
            s_currentBulletIndex = (s_currentBulletIndex + 1) % 50;  // 수정된 부분

            _running = true;
            _currentTime = 0.f;
            _parent = GetTransform()->GetParent().lock();
            if (_parent) _parentLook = _parent->GetLook();
            GetTransform()->RemoveParent();
            GetTransform()->SetLocalPosition(0, 0, 0);
            Vec3 pos = _parent ? _parent->GetLocalPosition() : GetTransform()->GetLocalPosition();
            GetTransform()->SetLocalPosition(pos);
        }
    }
    else
    {
        s_lastFireTime = s_fireInterval;
    }
}