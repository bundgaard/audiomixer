#include <iostream>

#include <Windows.h>

#include <mmdeviceapi.h>
#include <mmsystem.h>

void listMixerDevices()
{
    MMRESULT mr;
    UINT numDevs = mixerGetNumDevs();

    std::cout << "Mixer: " << numDevs << std::endl;

    for (auto i = 0; i < numDevs; i++)
    {
        MIXERCAPS mixerCaps;
        mr = mixerGetDevCaps(i, &mixerCaps, sizeof(MIXERCAPS));
        if (mr != MMSYSERR_NOERROR)
        {
            std::cout << "mixerGetDevCaps failure" << mr << std::endl;
            return;
        }

        std::cout << "Name: " << mixerCaps.szPname << std::endl;
    }
}

int main()
{
    MMRESULT mr;
    std::cout << "\t\tMixer devices" << std::endl;

    listMixerDevices();
    HMIXER hMixer;
    mr = mixerOpen(&hMixer, 0, NULL, NULL, MIXER_OBJECTF_MIXER);
    if (mr != MMSYSERR_NOERROR)
    {
        std::cout << "mixerOpen encountered error" << mr << std::endl;
        return 2;
    }
    MIXERLINE mixerLine = {0};
    mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
    mixerLine.cbStruct = sizeof(MIXERLINE);

    mr = mixerGetLineInfo(reinterpret_cast<HMIXEROBJ>(hMixer), &mixerLine,
                          MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE);
    if (mr != MMSYSERR_NOERROR)
    {
        std::cout << "getLine encountered error" << mr << std::endl;
        return 99;
    }
    std::cout << mixerLine.szName << std::endl;
    mr = mixerClose(hMixer);
    if (mr != MMSYSERR_NOERROR)
    {
        std::cout << "mixerOpen encountered error" << mr << std::endl;
        return 3;
    }

    return 0;
}
