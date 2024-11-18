#include "pch.h"
#include "Terrain.h"
#include "Resources.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Texture.h"

Terrain::Terrain() : Component(COMPONENT_TYPE::TERRAIN)
{
}

Terrain::~Terrain()
{
}

void Terrain::Init(int32 sizeX, int32 sizeZ)
{
	_sizeX = sizeX;
	_sizeZ = sizeZ;

	_material = GET_SINGLE(Resources)->Get<Material>(L"Terrain");
	_material->SetInt(1, _sizeX);
	_material->SetInt(2, _sizeZ);
	_material->SetFloat(0, _maxTesselation);

	_heightMap = GET_SINGLE(Resources)->Load<Texture>(L"HeightMap", L"..\\Resources\\Texture\\Terrain\\height1.png");
	if (_heightMap)
	{
		_heightMapWidth = static_cast<int>(_heightMap->GetWidth());
		_heightMapHeight = static_cast<int>(_heightMap->GetHeight());
	}

	_material->SetVec2(0, Vec2(_heightMapWidth, _heightMapHeight));
	_material->SetVec2(1, Vec2(1000.f, 5000.f));
	_material->SetTexture(2, _heightMap);

	shared_ptr<MeshRenderer> meshRenderer = GetGameObject()->GetMeshRenderer();
	{
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadTerrainMesh(sizeX, sizeZ);
		meshRenderer->SetMesh(mesh);
	}
	{
		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Terrain");
		meshRenderer->SetMaterial(material);
	}
}

void Terrain::FinalUpdate()
{
	shared_ptr<Camera> mainCamera = GET_SINGLE(SceneManager)->GetActiveScene()->GetMainCamera();
	if (mainCamera == nullptr)
		return;

	Vec3 pos = mainCamera->GetTransform()->GetLocalPosition();
	_material->SetVec4(0, Vec4(pos.x, pos.y, pos.z, 0));
}

float Terrain::GetHeight(float x, float z)
{
	// 높이 맵이 없는 경우 기본값 반환
	if (_heightMap == nullptr)
		return 0.0f;

	// 월드 좌표를 텍스처 좌표로 변환
	float u = (x + (_sizeX / 2.0f)) / static_cast<float>(_heightMapWidth);
	float v = (z + (_sizeZ / 2.0f)) / static_cast<float>(_heightMapHeight);

	// 텍스처 좌표가 범위를 벗어나는 경우 클램핑
	u = std::clamp(u, 0.0f, 1.0f);
	v = std::clamp(v, 0.0f, 1.0f);

	// 텍스처 좌표를 픽셀 좌표로 변환
	int pixelX = static_cast<int>(u * _heightMapWidth);
	int pixelY = static_cast<int>(v * _heightMapHeight);

	// 높이 맵에서 높이값 샘플링
	const float height = _heightMap->GetPixelValue(pixelX, pixelY);

	// 샘플링된 높이를 반환
	return height;
}