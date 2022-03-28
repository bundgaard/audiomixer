int main(int argc, char **argv, char **env)
{
printf("argc %d, '%s' '%s'\n", argc, argv[0], env[0]);
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
    printf("Total devices on this machine %u\n", deviceCount);
    IMMDevice *pDevice;
    hr = deviceEnumerator->GetDefaultAudioEndpoint(eCapture, eCommunications, &pDevice);
    if (FAILED(hr))
    {
        printf("Unable to get default device: %lx\n", hr);
        result = -1;
        goto Exit;
    }

    IAudioEndpointVolume *pEndpointVolume;
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, nullptr,
                           reinterpret_cast<void **>(&pEndpointVolume));
    if (FAILED(hr))
    {
        printf("Unable to active volume endpoint: %lx\n", hr);
        result = -1;
        goto Exit;
    }
    BOOL isMuted;
    hr = pEndpointVolume->GetMute(&isMuted);
    if (FAILED(hr))
    {
        printf("Unable to get mute state: %lx\n", hr);
        result = -1;
        goto Exit;
    }
    printf("Endpoint was muted: %s\n", isMuted ? "yes" : "no");

    hr = pEndpointVolume->SetMute(!isMuted, nullptr);
    if (FAILED(hr))
    {
        printf("Unable to mute endpoint: %lx\n", hr);
        result = -1;
        goto Exit;
    }
    hr = pEndpointVolume->GetMute(&isMuted);
    if (FAILED(hr))
    {
        printf("Unable to get mute state: %lx\n", hr);
        result = -1;
        goto Exit;
    }

    printf("Endpoint is now muted %s\n", isMuted ? "yes" : "no");

    Exit:
    SafeRelease(&pEndpointVolume);
    SafeRelease(&pDevice);
    SafeRelease(&deviceCollection);
    SafeRelease(&deviceEnumerator);
    CoUninitialize();

    return result;
}
