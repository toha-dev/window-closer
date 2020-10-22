#include "window_closer.h"

void WindowCloser::AddToStartup()
{
	HKEY hKey;

	char czExePath[_MAX_PATH + 1];
	GetModuleFileName(NULL, czExePath, _MAX_PATH);

	LONG lnRes = RegOpenKeyEx(
		HKEY_CURRENT_USER,
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, KEY_WRITE,
		&hKey
	);

	if (ERROR_SUCCESS == lnRes)
	{
		lnRes = RegSetValueEx(
			hKey,
			czStartName,
			0,
			REG_SZ,
			(unsigned char*)czExePath,
			strlen(czExePath)
		);
	}

	RegCloseKey(hKey);
}

void WindowCloser::TerminateForegroundWindow()
{
	DWORD foregroundWindowProcessId = GetForegroundWindowProcessId();
	if (foregroundWindowProcessId == NULL)
		return;

	HANDLE hProc = OpenProcess(
		PROCESS_TERMINATE,
		FALSE,
		foregroundWindowProcessId
	);

	if (hProc) TerminateProcess(hProc, 9);

	CloseHandle(hProc);
}

const DWORD WindowCloser::GetForegroundWindowProcessId()
{
	HWND hwnd = GetForegroundWindow();
	DWORD foregroundThreadId = GetWindowThreadProcessId(
		hwnd,
		NULL
	);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPTHREAD,
		NULL
	);

	THREADENTRY32 tInfo;
	tInfo.dwSize = sizeof(THREADENTRY32);

	if (Thread32First(hSnapshot, &tInfo))
	{
		do
		{
			if (tInfo.th32ThreadID == foregroundThreadId)
			{
				CloseHandle(hSnapshot);
				hSnapshot = CreateToolhelp32Snapshot(
					TH32CS_SNAPPROCESS,
					NULL
				);

				PROCESSENTRY32 pInfo;
				pInfo.dwSize = sizeof(PROCESSENTRY32);

				if (Process32First(hSnapshot, &pInfo))
				{
					do
					{
						if (pInfo.th32ProcessID == tInfo.th32OwnerProcessID)
						{
							CloseHandle(hSnapshot);
							return pInfo.th32ProcessID;
						}
					} while (Process32Next(hSnapshot, &pInfo));
				}

				CloseHandle(hSnapshot);
				break;
			}
		} while (Thread32Next(hSnapshot, &tInfo));
	}

	return NULL;
}
