#pragma once

enum class KEY_TYPE
{
    UP = VK_UP,
    DOWN = VK_DOWN,
    LEFT = VK_LEFT,
    RIGHT = VK_RIGHT,

    W = 'W',
    A = 'A',
    S = 'S',
    D = 'D',

    Q = 'Q',
    E = 'E',
    Z = 'Z',
    C = 'C',

    I = 'I',
    J = 'J',
    K = 'K',
    L = 'L',

    LBUTTON = VK_LBUTTON,
    RBUTTON = VK_RBUTTON,

    SPACE = VK_SPACE,
};

enum class KEY_STATE
{
    NONE,
    PRESS,
    DOWN,
    UP,
    END
};

enum
{
    KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX + 1),
    KEY_STATE_COUNT = static_cast<int32>(KEY_STATE::END),
};

class Input
{
    DECLARE_SINGLE(Input);

public:
    void Init(HWND hwnd);
    void Update();

    // ������ ���� ��
    bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
    // �� ó�� ������ ��
    bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
    // �� ó�� ������ ���� ��
    bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }

    const POINT& GetMousePos() { return _mousePos; }

    // ���콺 ��Ÿ ��ȯ
    POINT GetMouseDelta() const { return _mouseDelta; }

private:
    inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<uint8>(key)]; }

private:
    HWND _hwnd;
    vector<KEY_STATE> _states;

    POINT _mousePos = {};      // ���� ���콺 ��ġ
    POINT _prevMousePos = {};  // ���� �������� ���콺 ��ġ
    POINT _mouseDelta = {};    // ���콺 �̵��� (��Ÿ)
};
