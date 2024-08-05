#pragma once
class Component;
namespace KernelEngine
{
	void Draw();
	void Update();

	void Init();
	void Release();

	void Start();
	void Stop();
	void End();

	void AddLog(const char* fmt,...);

	const bool  is_Playing();
	const bool  is_Building();
	const bool  is_Stopping();
	const bool& is_Debugging();

	void StartButton();
	void StopButton();
	void SetDebugging(const bool& _debugging);
}