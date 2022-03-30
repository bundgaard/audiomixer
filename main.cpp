#include <iostream>
#include <string>


#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "audiomixer.h"
#include <wslapi.h>

typedef BOOL (STDAPICALLTYPE *WSL_IS_DISTRIBUTION_REGISTERED)(PCWSTR);

void foo(const std::wstring &distributionName)
{
    HMODULE wslApiDll = LoadLibraryEx(L"wslapi.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (wslApiDll == nullptr)
    {
        return;
    }
    auto _isDistributionRegistered = (WSL_IS_DISTRIBUTION_REGISTERED) GetProcAddress(
            wslApiDll, "WslLaunchInteractive");
    std::wcout << L"Is " << distributionName << L" registered? " << _isDistributionRegistered(distributionName.c_str()) << std::endl;

    FreeLibrary(wslApiDll);
}

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);


wchar_t szClassName[] = L"AClassName";
wchar_t szCaption[] = L"aCaption";

int GetWidth(RECT rct)
{
    return rct.right - rct.left;
}

int GetHeight(RECT rct)
{
    return rct.bottom - rct.top;
}

static bool CenterWindow(HWND hwndChild, HWND hwndParent)
{

    RECT rcChild, rcParent;
    int cxChild, cyChild, cxParent, cyParent;
    int cxScreen, cyScreen, xNew, yNew;
    HDC hdc;

    // Get the Height and Width of the child window
    GetWindowRect(hwndChild, &rcChild);
    cxChild = rcChild.right - rcChild.left;
    cyChild = rcChild.bottom - rcChild.top;

    // Get the Height and Width of the parent window
    GetWindowRect(hwndParent, &rcParent);
    cxParent = rcParent.right - rcParent.left;
    cyParent = rcParent.bottom - rcParent.top;

    // Get the display limits
    hdc = GetDC(hwndChild);
    cxScreen = GetDeviceCaps(hdc, HORZRES);
    cyScreen = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(hwndChild, hdc);

    // Calculate new X position, then adjust for screen
    xNew = rcParent.left + ((cxParent - cxChild) / 2);
    if (xNew < 0)
    {
        xNew = 0;
    } else if ((xNew + cxChild) > cxScreen)
    {
        xNew = cxScreen - cxChild;
    }

    // Calculate new Y position, then adjust for screen
    yNew = rcParent.top + ((cyParent - cyChild) / 2);
    if (yNew < 0)
    {
        yNew = 0;
    } else if ((yNew + cyChild) > cyScreen)
    {
        yNew = cyScreen - cyChild;
    }

    // Set it, and return
    return SetWindowPos(hwndChild,
                        NULL,
                        xNew, yNew,
                        0, 0,
                        SWP_NOSIZE | SWP_NOZORDER);

}

bool AMCreateWindow(HINSTANCE hInst)
{
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = szClassName;
    wc.lpfnWndProc = (WNDPROC) AMWndProc;
    wc.hInstance = hInst;
    wc.lpszMenuName = nullptr;

    wc.hCursor = static_cast<HCURSOR>(LoadCursor(nullptr, IDC_ARROW));
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);

    wc.cbWndExtra = 0;
    wc.cbClsExtra = 0;


    if (RegisterClassEx(&wc) == 0)
    {
        printf("failed to register window");
        return false;
    }

    return true;
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszArg, int nCmdShow)
{

    AllocConsole();
    foo(L"ubuntu");

    if (!AMCreateWindow(hInst))
    {
        return -1;
    }

    HWND hwnd = CreateWindow(szClassName, szCaption,
                             (WS_POPUP) & ~(WS_THICKFRAME),
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             640, 480,
                             nullptr,
                             nullptr,
                             hInst,
                             nullptr);


    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    CenterWindow(hwnd, GetDesktopWindow());
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    FreeConsole();
    return static_cast<int>(msg.wParam);
}
