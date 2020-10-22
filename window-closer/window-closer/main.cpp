#include "window_closer.h"


int CALLBACK WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow
)
{
	WindowCloser::AddToStartup();

	DWORD sleepTime = 100;
	bool keysPressed = false;

	while (true)
	{
		if (	GetKeyState(VK_CONTROL)	& 0x8000
			&&	GetKeyState(VK_MENU)	& 0x8000
			&&	GetKeyState(VK_F4)		& 0x8000)
		{
			if (!keysPressed)
			{
				WindowCloser::TerminateForegroundWindow();

				keysPressed = true;
			}
		}

		if (	GetKeyState(VK_CONTROL)	== 0
			&&	GetKeyState(VK_MENU)	== 0
			&&	GetKeyState(VK_F4)		== 0)
		{
			keysPressed = false;
		}

		Sleep(sleepTime);
	}

	return 0;
}
