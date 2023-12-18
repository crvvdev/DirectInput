#pragma once

namespace DirectInput
{
    extern HANDLE h_DeviceHandle;

    auto Initialize()
        -> HRESULT;

    auto SendInput(UINT32 aCount, LPINPUT aInputs, UINT32 aBytes)
        -> UINT;
}
