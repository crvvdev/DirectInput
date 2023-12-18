#include "stdafx.h"
#include "DirectInput.h"

namespace DirectInput
{
    //////////////////////////////////////////////////////////////////////////

    HANDLE h_DeviceHandle = INVALID_HANDLE_VALUE;

    static constexpr wchar_t s_SymName[] = LR"(\\.\{27314068-3117-4371-B7A5-A1750281ED67})";

    enum class IoCode : UINT32
    {
        BeginCode = 0x800,

        SendInput = CTL_CODE(FILE_DEVICE_UNKNOWN, BeginCode + 1, METHOD_BUFFERED, FILE_ANY_ACCESS),
    };

    struct SendInputArgs
    {
        UINT32  InputCount;
        UINT32  InputBytes;
        UINT64  Inputs;
    };

    //////////////////////////////////////////////////////////////////////////

    auto Initialize()
        ->HRESULT
    {
        return E_NOTIMPL;
    }

    static auto SendInputCode(UINT32 aInputCount, LPINPUT aInputs, UINT32 aInputBytes)
        -> UINT
    {
        UINT32 vDosCode     = NOERROR;
        UINT32 vSendCount   = 0u;

        for (;;)
        {
            if (h_DeviceHandle == INVALID_HANDLE_VALUE)
            {
                h_DeviceHandle = CreateFile(
                    s_SymName,
                    FILE_ANY_ACCESS,
                    0,
                    nullptr,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_DEVICE,
                    nullptr);

                if (INVALID_HANDLE_VALUE == h_DeviceHandle)
                {
                    printf("CreateFile failed %d\n", ::GetLastError());
                    vDosCode = GetLastError();
                    break;
                }
            }

            auto vArgs = SendInputArgs { aInputCount, aInputBytes, (UINT64)aInputs };

            auto vReturnBytes = 0ul;
            if (!DeviceIoControl(
                h_DeviceHandle,
                (UINT32)IoCode::SendInput,
                &vArgs, sizeof(vArgs),
                &vSendCount, sizeof(vSendCount),
                &vReturnBytes,
                nullptr))
            {
                printf("DeviceIoControl failed %d\n", ::GetLastError());
                vDosCode = GetLastError();
                break;
            }

            break;
        }

        return SetLastError(vDosCode), vSendCount;
    }

    auto SendInput(UINT32 aInputCount, LPINPUT aInputs, UINT32 aInputBytes)
        -> UINT
    {
        for (auto i = 0u; i < aInputCount; ++i)
        {
            auto& vInput = aInputs[i];
            if (INPUT_KEYBOARD == vInput.type)
            {
                if (KEYEVENTF_SCANCODE & vInput.ki.dwFlags)
                {
                    break;
                }

                vInput.ki.wScan     = MapVirtualKey(vInput.ki.wVk, MAPVK_VK_TO_VSC);
                vInput.ki.dwFlags  |= KEYEVENTF_SCANCODE;

                switch (vInput.ki.wVk)
                {
                case VK_INSERT:
                case VK_DELETE:
                case VK_HOME:
                case VK_END:
                case VK_PRIOR:	//Page Up
                case VK_NEXT:	//Page Down

                case VK_LEFT:
                case VK_UP:
                case VK_RIGHT:
                case VK_DOWN:

                case VK_DIVIDE:

                case VK_LWIN:
                case VK_RCONTROL:
                case VK_RWIN:
                case VK_RMENU:	//ALT
                    vInput.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
                    break;
                }
            }
        }

        return SendInputCode(aInputCount, aInputs, aInputBytes);
    }

}
