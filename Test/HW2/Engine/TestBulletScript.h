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

    // �Ѿ��� ����ִ� �ð� (2��)
    float _lifeTime = 2.0f;
    // ���� ��� �ð�
    float _currentTime = 0.f;

    // ���� �߻��� �Ѿ��� �ε���
    static int32 s_bulletIndex;
    // �� �Ѿ��� �ε���
    uint32 _myIndex;           
};