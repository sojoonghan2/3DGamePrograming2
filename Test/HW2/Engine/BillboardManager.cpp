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
    Clear(); // 기존 데이터 초기화

    // 기본 버퍼 초기화
    for (auto& pair : _buffers)
    {
        pair.second->Init(maxCount);
    }
}

void BillboardManager::Render(const std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    std::map<uint64, std::vector<std::shared_ptr<GameObject>>> cache;

    // GameObject를 InstanceID별로 분류
    for (const std::shared_ptr<GameObject>& gameObject : gameObjects)
    {
        const uint64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
        cache[instanceId].push_back(gameObject);
    }

    // 분류된 각 그룹을 처리
    for (auto& pair : cache)
    {
        const std::vector<std::shared_ptr<GameObject>>& vec = pair.second;

        if (vec.size() == 1)
        {
            // 단일 객체는 직접 렌더링
            vec[0]->GetMeshRenderer()->Render();
        }
        else
        {
            // 다중 객체는 인스턴싱 렌더링
            const uint64 instanceId = pair.first;

            for (const std::shared_ptr<GameObject>& gameObject : vec)
            {
                BillboardParams params;
                params.position = gameObject->GetTransform()->GetWorldPosition();
                params.scale = gameObject->GetTransform()->GetLocalScale().x; // 균일 스케일 가정
                params.color = Vec4(1.0f, 1.0f, 1.0f, 1.0f); // 기본 흰색

                AddParam(instanceId, params);
            }

            std::shared_ptr<BillboardBuffer>& buffer = _buffers[instanceId];
            buffer->PushData(); // GPU로 데이터 전송

            vec[0]->GetMeshRenderer()->Render(buffer); // 인스턴싱 렌더링
        }
    }
}

void BillboardManager::ClearBuffer()
{
    // 모든 버퍼 초기화
    for (auto& pair : _buffers)
    {
        pair.second->clear();
    }
}

void BillboardManager::Clear()
{
    // 모든 버퍼 맵 제거
    _buffers.clear();
}

void BillboardManager::AddParam(uint64 instanceId, const BillboardParams& params)
{
    if (_buffers.find(instanceId) == _buffers.end())
        _buffers[instanceId] = std::make_shared<BillboardBuffer>();

    _buffers[instanceId]->AddData(params);
}
