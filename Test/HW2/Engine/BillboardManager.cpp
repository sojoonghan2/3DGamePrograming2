#include "pch.h"
#include "BillboardManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"

BillboardManager::BillboardManager()
{
}

BillboardManager::~BillboardManager()
{
    Clear();
}

void BillboardManager::Init(uint32 maxCount)
{
    Clear(); // ���� ������ �ʱ�ȭ

    // �⺻ ���� �ʱ�ȭ
    for (auto& pair : _buffers)
    {
        pair.second->Init(maxCount);
    }
}

void BillboardManager::Render(const std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    std::map<uint64, std::vector<std::shared_ptr<GameObject>>> cache;

    // GameObject�� InstanceID���� �з�
    for (const std::shared_ptr<GameObject>& gameObject : gameObjects)
    {
        const uint64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
        cache[instanceId].push_back(gameObject);
    }

    // �з��� �� �׷��� ó��
    for (auto& pair : cache)
    {
        const std::vector<std::shared_ptr<GameObject>>& vec = pair.second;

        if (vec.size() == 1)
        {
            // ���� ��ü�� ���� ������
            vec[0]->GetMeshRenderer()->Render();
        }
        else
        {
            // ���� ��ü�� �ν��Ͻ� ������
            const uint64 instanceId = pair.first;

            for (const std::shared_ptr<GameObject>& gameObject : vec)
            {
                BillboardParams params;
                params.position = gameObject->GetTransform()->GetWorldPosition();
                params.scale = gameObject->GetTransform()->GetLocalScale().x; // ���� ������ ����
                params.color = Vec4(1.0f, 1.0f, 1.0f, 1.0f); // �⺻ ���

                AddParam(instanceId, params);
            }

            std::shared_ptr<BillboardBuffer>& buffer = _buffers[instanceId];
            buffer->PushData(); // GPU�� ������ ����

            vec[0]->GetMeshRenderer()->Render(buffer); // �ν��Ͻ� ������
        }
    }
}

void BillboardManager::ClearBuffer()
{
    // ��� ���� �ʱ�ȭ
    for (auto& pair : _buffers)
    {
        pair.second->clear();
    }
}

void BillboardManager::Clear()
{
    // ��� ���� �� ����
    _buffers.clear();
}

void BillboardManager::AddParam(uint64 instanceId, const BillboardParams& params)
{
    if (_buffers.find(instanceId) == _buffers.end())
        _buffers[instanceId] = std::make_shared<BillboardBuffer>();

    _buffers[instanceId]->AddData(params);
}
