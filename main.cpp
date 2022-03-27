#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "audiomixer.h"

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);


wchar_t szClassName[] = L"AClassName";
wchar_t szCaption[] = L"aCaption";

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

    if (!AMCreateWindow(hInst))
    {
        return -1;
    }

    HWND hwnd = CreateWindow(szClassName, szCaption,
                             (WS_VISIBLE | WS_POPUPWINDOW |WS_THICKFRAME) ,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             640, 480,
                             nullptr,
                             nullptr,
                             hInst,
                             nullptr);


    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}
