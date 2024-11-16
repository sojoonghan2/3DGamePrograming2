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

    // 총알이 살아있는 시간 (2초)
    float _lifeTime = 2.0f;
    // 현재 경과 시간
    float _currentTime = 0.f;

    // 현재 발사할 총알의 인덱스
    static int32 s_bulletIndex;
    // 이 총알의 인덱스
    uint32 _myIndex;           
};