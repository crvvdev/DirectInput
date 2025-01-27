#include "stdafx.h"
#include "DirectInput.h"

int main()
{
	HRESULT hr = S_OK;

	(void)getchar();
	for (;;)
	{
		auto vSendCount = 0;

		for (int i = 0; i < 100; i++)
		{
			double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
			double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
			double fx = 1420 * (65535.0f / fScreenWidth);
			double fy = 650 * (65535.0f / fScreenHeight);

			INPUT input = {};
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
			input.mi.dx = static_cast<ULONG>(fx);
			input.mi.dy = static_cast<ULONG>(fy);

			vSendCount = DirectInput::SendInput(1, &input, sizeof(input));

			Sleep(50);
		}

		INPUT vInputs[2];

		ZeroMemory(vInputs, sizeof(INPUT) * _countof(vInputs));
		vInputs[0].type = INPUT_KEYBOARD;
		vInputs[0].ki.wVk = VK_LWIN;
		vInputs[0].ki.dwFlags = 0;

		vInputs[1].type = INPUT_KEYBOARD;
		vInputs[1].ki.wVk = VK_TAB;
		vInputs[1].ki.dwFlags = 0;

		vSendCount = DirectInput::SendInput(_countof(vInputs), vInputs, sizeof(vInputs[0]));

		Sleep(3000);

		ZeroMemory(vInputs, sizeof(INPUT) * _countof(vInputs));
		vInputs[0].type = INPUT_KEYBOARD;
		vInputs[0].ki.wVk = VK_TAB;
		vInputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

		vInputs[1].type = INPUT_KEYBOARD;
		vInputs[1].ki.wVk = VK_LWIN;
		vInputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

		vSendCount = DirectInput::SendInput(_countof(vInputs), vInputs, sizeof(vInputs[0]));

		break;
	}

	if (DirectInput::h_DeviceHandle)
	{
		::CloseHandle(DirectInput::h_DeviceHandle);
	}

	return hr;
}
