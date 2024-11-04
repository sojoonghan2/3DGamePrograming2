#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

TestCameraScript::TestCameraScript() 
{
}

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		return;
	}
	KeyboardInput();
	MouseInput();
}

void TestCameraScript::KeyboardInput()
{

}

void TestCameraScript::MouseInput()
{
	
}