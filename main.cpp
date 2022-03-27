#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include "safe_release.h"

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);


int main()
{
    int result = 0;
    HRESULT hr;

    IMMDeviceEnumerator *deviceEnumerator = nullptr;
    IMMDeviceCollection *deviceCollection = nullptr;
    UINT deviceCount = 0;

    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        printf("Unable to initialize COM: %lx\n", hr);
        result = -1;
        goto Exit;
    }


    hr = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));
    if (FAILED(hr))
    {
        printf("Unable to initialize COM: %lx\n", hr);
        result = -1;
        goto Exit;
    }

    hr = deviceEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &deviceCollection);
    if (FAILED(hr))
    {
        printf("Unable to iterate over endpoints: %lx\n", hr);
        result = -1;
        goto Exit;
    }
    deviceCollection->GetCount(&deviceCount);
    printf("Devices %u\n", deviceCount);
    

    Exit:
    SafeRelease(&deviceCollection);
    SafeRelease(&deviceEnumerator);
    CoUninitialize();

    return result;
}
