#pragma once
#include "MonoBehaviour.h"
#include "pch.h"

class TestBulletScript : public MonoBehaviour
{
public:
    TestBulletScript();
    virtual ~TestBulletScript();
    virtual void LateUpdate() override;
    void KeyboardInput();

private:
    float _speed = 1000.f;
    bool _running = false;
    Vec3 _parentLook;
    shared_ptr<Transform> _parent;
    float _lifeTime = 2.0f;    // 총알이 살아있는 시간
    float _currentTime = 0.f;  // 현재 경과 시간

    // 발사 간격 관련
    static float s_lastFireTime;     // 발사 딜레이 시간 누적
    static const float s_fireInterval; // 발사 간격 (예: 0.3초)
    static int32 s_currentBulletIndex;
    uint32 _myIndex;
};