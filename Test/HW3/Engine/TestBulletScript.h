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
    float _lifeTime = 2.0f;    // �Ѿ��� ����ִ� �ð�
    float _currentTime = 0.f;  // ���� ��� �ð�

    // �߻� ���� ����
    static float s_lastFireTime;     // �߻� ������ �ð� ����
    static const float s_fireInterval; // �߻� ���� (��: 0.3��)
    static int32 s_currentBulletIndex;
    uint32 _myIndex;
};