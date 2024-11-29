#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "BulletObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

#include "TestObjectScript.h"
#include "TestPlayerScript.h"
#include "TestBulletScript.h"
#include "TestTitleScript.h"
#include "TestCameraScript.h"
#include "Resources.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MeshData.h"

#include "BillboardBuffer.h"
#include "BillboardManager.h"

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	// 기존 데이터 삭제
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
		return 0;

	return findIt->second;
}

shared_ptr<GameObject> SceneManager::Pick(int32 screenX, int32 screenY)
{
	shared_ptr<Camera> camera = GetActiveScene()->GetMainCamera();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	Matrix projectionMatrix = camera->GetProjectionMatrix();

	// ViewSpace에서 Picking 진행
	float viewX = (+2.0f * screenX / width - 1.0f) / projectionMatrix(0, 0);
	float viewY = (-2.0f * screenY / height + 1.0f) / projectionMatrix(1, 1);

	Matrix viewMatrix = camera->GetViewMatrix();
	Matrix viewMatrixInv = viewMatrix.Invert();

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	float minDistance = FLT_MAX;
	shared_ptr<GameObject> picked;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetCollider() == nullptr)
			continue;

		// ViewSpace에서의 Ray 정의
		Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

		// WorldSpace에서의 Ray 정의
		rayOrigin = XMVector3TransformCoord(rayOrigin, viewMatrixInv);
		rayDir = XMVector3TransformNormal(rayDir, viewMatrixInv);
		rayDir.Normalize();

		// WorldSpace에서 연산
		float distance = 0.f;
		if (gameObject->GetCollider()->Intersects(rayOrigin, rayDir, OUT distance) == false)
			continue;

		if (distance < minDistance)
		{
			minDistance = distance;
			picked = gameObject;
			
		}
	}
	//if (picked) std::cout << "Pick!: " << picked->GetID() << "\n";
	return picked;
}

bool SceneManager::Collition(shared_ptr<GameObject> obj1,
	shared_ptr<GameObject> obj2)
{
	auto obj1Collider = obj1->GetCollider();
	auto obj2Collider = obj2->GetCollider();

	if (!obj1Collider) return 0;
	if (!obj2Collider) return 0;

	// 충돌 체크
	if (obj1Collider->Intersects(obj2Collider))
	{
		return true;
	}
	return false;
}

shared_ptr<GameObject> SceneManager::FindObjectByName(const wstring& name)
{
	if (_activeScene == nullptr)
		return nullptr;

	const vector<shared_ptr<GameObject>>& objects = _activeScene->GetGameObjects();

	for (const auto& obj : objects)
	{
		if (obj->GetName() == name)
		{
			return obj; // 이름이 일치하는 오브젝트를 찾으면 반환
		}
	}

	return nullptr; // 찾지 못하면 nullptr 반환
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion

#pragma region ComputeShader
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		// UAV 용 Texture 생성
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture(L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->SetShader(shader);
		material->SetInt(0, 1);
		GEngine->GetComputeDescHeap()->SetUAV(texture->GetUAVHandle(), UAV_REGISTER::u0);

		// 쓰레드 그룹 (1 * 1024 * 1)
		material->Dispatch(1, 1024, 1);
	}
#pragma endregion

	shared_ptr<Scene> scene = make_shared<Scene>();

	shared_ptr<GameObject> mainObject = make_shared<GameObject>();
	mainObject->SetName(L"Main_Object");
	mainObject->AddComponent(make_shared<Transform>());
	mainObject->AddComponent(make_shared<TestPlayerScript>());
	mainObject->GetTransform()->SetLocalPosition(Vec3(1000.f, 70.f, 100.f));
	mainObject->AddComponent(make_shared<BoxCollider>());
	dynamic_pointer_cast<BoxCollider>(mainObject->GetCollider())->SetCenter(Vec3(0.f, 0.f, 0.f));
	dynamic_pointer_cast<BoxCollider>(mainObject->GetCollider())->SetExtents(Vec3(200.f, 200.f, 200.f));
	scene->AddGameObject(mainObject);

#pragma region FBX & Camera
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Helicopter.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Helicopter");
			gameObject->GetTransform()->SetParent(mainObject->GetTransform());
			gameObject->SetCheckFrustum(true);
			gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
			scene->AddGameObject(gameObject);
		}

		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<TestCameraScript>());
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
		camera->GetTransform()->SetParent(mainObject->GetTransform());
		camera->GetCamera()->SetFar(10000.f);
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 30.f, -100.f));
		//camera->GetTransform()->SetLocalPosition(Vec3(1000.f, 80.f, 0.f));

		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region Bullet
	{
		for (int i{}; i < 50; ++i)
		{
			shared_ptr<GameObject> bullet = make_shared<GameObject>();
			wstring bulletName = L"Bullet" + to_wstring(i);
			bullet->SetName(bulletName);
			bullet->AddComponent(make_shared<Transform>());
			bullet->AddComponent(make_shared<TestBulletScript>());
			bullet->SetCheckFrustum(true);
			bullet->GetTransform()->SetParent(mainObject->GetTransform());
			bullet->GetTransform()->SetLocalPosition(Vec3(0.f, 100000000.f, 0.f));
			bullet->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
			bullet->SetStatic(false);
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
				meshRenderer->SetMesh(cubeMesh);
			}
			{
				shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
				meshRenderer->SetMaterial(material->Clone());
			}
			bullet->AddComponent(make_shared<BoxCollider>());
			dynamic_pointer_cast<BoxCollider>(bullet->GetCollider())->SetCenter(Vec3(0.f, 0.f, 0.f));
			dynamic_pointer_cast<BoxCollider>(bullet->GetCollider())->SetExtents(Vec3(10.f, 10.f, 10.f));
			bullet->AddComponent(meshRenderer);
			scene->AddGameObject(bullet);
		}
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍음
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Sky01.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion

#pragma region Object & Particle
	{
		for (int i{}; i < 10; ++i)
		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			wstring objectName = L"OBJ" + to_wstring(i);
			obj->SetName(objectName);
			obj->AddComponent(make_shared<Transform>());
			obj->SetCheckFrustum(true);

			float randxPos = GetRandomFloat(500, 2500);
			float randyPos = GetRandomFloat(50, 500);
			float randzPos = GetRandomFloat(500, 2500);

			obj->GetTransform()->SetLocalPosition(Vec3(randxPos, randyPos, randzPos));
			obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
			obj->AddComponent(make_shared<TestObjectScript>());
			obj->SetStatic(true);
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
				meshRenderer->SetMesh(sphereMesh);
			}
			{
				shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
				meshRenderer->SetMaterial(material->Clone());
			}
			obj->AddComponent(make_shared<BoxCollider>());
			dynamic_pointer_cast<BoxCollider>(obj->GetCollider())->SetExtents(Vec3(100.f, 100.f, 100.f));
			dynamic_pointer_cast<BoxCollider>(obj->GetCollider())->SetCenter(Vec3(0.f, 0.f, 0.f));
			obj->AddComponent(meshRenderer);
			scene->AddGameObject(obj);

			shared_ptr<GameObject> particle = make_shared<GameObject>();
			wstring particleName = L"PARTICLE" + to_wstring(i);
			particle->SetName(particleName);
			particle->AddComponent(make_shared<Transform>());
			particle->GetTransform()->SetParent(obj->GetTransform());
			shared_ptr<ParticleSystem> particleSystem = make_shared<ParticleSystem>();
			particle->AddComponent(particleSystem);
			particle->SetCheckFrustum(true);
			particleSystem->ParticleStop();
			scene->AddGameObject(particle);
		}
	}
#pragma endregion

#pragma region Terrain & FlowerBillboard
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"Terrain");
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<Terrain>());
		obj->AddComponent(make_shared<MeshRenderer>());
		obj->GetTransform()->SetLocalScale(Vec3(50.f, 250.f, 50.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.f, -200.f, 0.f));
		obj->SetStatic(true);
		obj->GetTerrain()->Init(64, 64);
		obj->SetCheckFrustum(true);
		scene->AddGameObject(obj);

		GET_SINGLE(BillboardManager)->Init(50);

		shared_ptr<Mesh> pointMesh = GET_SINGLE(Resources)->LoadPointMesh();
		shared_ptr<Material> billboardMaterial = GET_SINGLE(Resources)->Get<Material>(L"Billboard");

		for (int i = 0; i < 50; ++i)
		{
			shared_ptr<GameObject> billboard = make_shared<GameObject>();
			wstring billboardName = L"FlowerBillboard" + to_wstring(i);
			billboard->SetName(billboardName);
			billboard->AddComponent(make_shared<Transform>());
			billboard->SetCheckFrustum(true);
			billboard->SetStatic(true);

			float randxPos = GetRandomFloat(0, 3000);
			float randzPos = GetRandomFloat(0, 3000);

			// Transform 설정
			billboard->GetTransform()->SetLocalPosition(Vec3(randxPos,
				obj->GetTerrain()->GetHeight(randxPos, randzPos) - 200.f, randzPos));
			billboard->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));

			// MeshRenderer 설정
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			meshRenderer->SetMesh(pointMesh);
			meshRenderer->SetMaterial(billboardMaterial->Clone());
			billboard->AddComponent(meshRenderer);

			// BillboardParams 생성
			BillboardParams params;
			params.position = billboard->GetTransform()->GetWorldPosition();
			// 균일 스케일 가정
			params.scale = billboard->GetTransform()->GetLocalScale().x;
			// 기본 흰색
			params.color = Vec4(1.f, 1.f, 1.f, 1.f);

			// BillboardManager에 데이터 추가
			GET_SINGLE(BillboardManager)->AddParam(meshRenderer->GetInstanceID(), params);

			// Scene에 GameObject 추가
			scene->AddGameObject(billboard);
		}
	}
#pragma endregion

#pragma region StartBillboard
	{
		GET_SINGLE(BillboardManager)->Init(50);

		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Star", L"..\\Resources\\Texture\\Star.png");
		shared_ptr<Mesh> pointMesh = GET_SINGLE(Resources)->LoadPointMesh();
		shared_ptr<Material> billboardMaterial = GET_SINGLE(Resources)->Get<Material>(L"Billboard");

		for (int i = 0; i < 50; ++i)
		{
			shared_ptr<GameObject> billboard = make_shared<GameObject>();
			wstring billboardName = L"StarBillboard" + to_wstring(i);
			billboard->SetName(billboardName);
			billboard->AddComponent(make_shared<Transform>());
			billboard->SetCheckFrustum(true);
			billboard->SetStatic(true);

			// 해당 랜덤 계산을 CPU 말고 GPU에서 시키고 싶음
			float randxPos = GetRandomFloat(-2000, 5000);
			float randzPos = GetRandomFloat(-2000, 5000);

			// MeshRenderer 설정
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			meshRenderer->SetMesh(pointMesh);
			auto newMaterial = billboardMaterial->Clone();
			newMaterial->SetTexture(0, texture);

			meshRenderer->SetMaterial(newMaterial);
			billboard->AddComponent(meshRenderer);

			// Transform 설정
			billboard->GetTransform()->SetLocalPosition(Vec3(randxPos, 1000, randzPos));
			billboard->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));

			// BillboardParams 생성
			BillboardParams params;
			params.position = billboard->GetTransform()->GetWorldPosition();
			// 균일 스케일 가정
			params.scale = billboard->GetTransform()->GetLocalScale().x;
			// 기본 흰색
			params.color = Vec4(1.f, 1.f, 1.f, 1.f);

			// BillboardManager에 데이터 추가
			GET_SINGLE(BillboardManager)->AddParam(meshRenderer->GetInstanceID(), params);

			// Scene에 GameObject 추가
			scene->AddGameObject(billboard);
		}
	}
#pragma endregion

#pragma region Water
	{
		shared_ptr<GameObject> water = make_shared<GameObject>();
		water->SetName(L"Water");
		water->AddComponent(make_shared<Transform>());

		// Transform 설정
		water->GetTransform()->SetLocalPosition(Vec3(1000.f, 70.f, 1000.f));
		water->GetTransform()->SetLocalScale(Vec3(200.f, 200.f, 200.f));

		// MeshRenderer 설정
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		meshRenderer->SetMesh(GET_SINGLE(Resources)->LoadRectangleMesh());
		meshRenderer->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Water"));
		water->AddComponent(meshRenderer);

		// 씬에 추가
		scene->AddGameObject(water);
	}
#pragma endregion

#pragma region UI_Test
	shared_ptr<GameObject> obj = make_shared<GameObject>();
	obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
	obj->AddComponent(make_shared<Transform>());
	obj->GetTransform()->SetLocalScale(Vec3(150.f, 150.f, 150.f));
	obj->GetTransform()->SetLocalPosition(Vec3(-300.f, 200.f, 500.f));
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		meshRenderer->SetMesh(mesh);
	}
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"PlayerUI", L"..\\Resources\\Texture\\PlayerUI.jpg");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		meshRenderer->SetMaterial(material);
	}
	obj->AddComponent(meshRenderer);
	scene->AddGameObject(obj);
#pragma endregion

#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, -1, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetTransform()->SetParent(mainObject->GetTransform());
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		scene->AddGameObject(light);
	}
#pragma endregion

#pragma region Title
	{
		shared_ptr<GameObject> title = make_shared<GameObject>();
		title->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
		title->AddComponent(make_shared<Transform>());
		title->GetTransform()->SetLocalScale(Vec3(800.f, 800.f, 0.f));
		title->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 500.f));
		title->AddComponent(make_shared<TestTitleScript>());
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Title", L"..\\Resources\\Texture\\Title.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		title->AddComponent(meshRenderer);
		scene->AddGameObject(title);
	}
#pragma endregion

	return scene;
}