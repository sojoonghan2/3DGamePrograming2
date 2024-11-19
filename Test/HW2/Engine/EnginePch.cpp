#include "pch.h"
#include "EnginePch.h"
#include "Engine.h"

unique_ptr<Engine> GEngine = make_unique<Engine>();

wstring s2ws(const string& s)
{
	int32 len;
	int32 slength = static_cast<int32>(s.length()) + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring ret(buf);
	delete[] buf;
	return ret;
}

string ws2s(const wstring& s)
{
	int32 len;
	int32 slength = static_cast<int32>(s.length()) + 1;
	len = ::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	string r(len, '\0');
	::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

Vec3 Normalize(Vec3 tmp) {
	float length = std::sqrt(tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);
	// ������ ���̰� 0�� ��� ����ȭ �� �� �����Ƿ� üũ
	if (length == 0) {
		return Vec3(0, 0, 0); // ���̰� 0�� ��� 0���͸� ��ȯ
	}
	return Vec3(tmp.x / length, tmp.y / length, tmp.z / length);
}

// �ܼ� ��ũ�Ѹ�
void ClearConsole()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

float GetRandomFloat(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}