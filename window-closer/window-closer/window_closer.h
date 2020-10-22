#pragma once
#include <Windows.h>
#include <TlHelp32.h>

class WindowCloser
{
public:

	static void AddToStartup();

	static void TerminateForegroundWindow();
	
private:

	static const DWORD GetForegroundWindowProcessId();


	static constexpr const char* czStartName = "window-closer";
};