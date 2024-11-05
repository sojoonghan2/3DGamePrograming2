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
    if (!_heightMap)
        return 0.0f;

    // ���� �� �̹��� ������ Ȯ��
    const Image* img = _heightMap->_image.GetImage(0, 0, 0);
    if (!img)
        return 0.0f;

    int width = static_cast<int>(img->width);
    int height = static_cast<int>(img->height);

    // ���� ��ǥ�� �ؽ�ó ��ǥ�� ��ȯ
    float terrainWidth = _sizeX * 50.0f;  // ��: �ͷ����� ���� �� (m)
    float terrainHeight = _sizeZ * 50.0f; // ��: �ͷ����� ���� �� (m)
    float scaledX = (x / terrainWidth) * (width - 1);
    float scaledZ = (z / terrainHeight) * (height - 1);

    // �»�� �ȼ� ��ǥ
    int gridX = static_cast<int>(scaledX);
    int gridZ = static_cast<int>(scaledZ);

    // ���� �˻�
    if (gridX < 0 || gridX >= width - 1 || gridZ < 0 || gridZ >= height - 1)
        return 0.0f;

    // �� ���� �ȼ����� ���� �� ����
    uint8_t* pixelData00 = img->pixels + gridZ * img->rowPitch + gridX * 4;          // �»��
    uint8_t* pixelData10 = img->pixels + gridZ * img->rowPitch + (gridX + 1) * 4;    // ����
    uint8_t* pixelData01 = img->pixels + (gridZ + 1) * img->rowPitch + gridX * 4;    // ���ϴ�
    uint8_t* pixelData11 = img->pixels + (gridZ + 1) * img->rowPitch + (gridX + 1) * 4; // ���ϴ�

    // RGBA �� R ä�� ���� ���� ������ ����
    float h00 = pixelData00[0] / 255.0f;
    float h10 = pixelData10[0] / 255.0f;
    float h01 = pixelData01[0] / 255.0f;
    float h11 = pixelData11[0] / 255.0f;

    // �Ҽ��� �κ��� ����� ���� ����ġ
    float dx = scaledX - gridX;
    float dz = scaledZ - gridZ;

    // �����Ͼ� ������ ���� ���� ���� ���
    float h0 = h00 * (1 - dx) + h10 * dx;
    float h1 = h01 * (1 - dx) + h11 * dx;
    float finalHeight = h0 * (1 - dz) + h1 * dz;

    //std::cout << "Terrain yPos: " << finalHeight * -200 << "\n";
    return finalHeight * 100.f - 200.f; // ���� ���̷� ��ȯ
}
