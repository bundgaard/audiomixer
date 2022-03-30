#include <windowsx.h>
#include "audiomixer.h"
#include "penbrush.h"

static int buttonWidth = 64;
static int buttonHeight = 64;
static RECT rectDraggable = {10, 10, buttonWidth, buttonHeight};
static RECT rectExit = {640 - buttonWidth, 10, 640, buttonHeight};
static RECT rectPushButtonArea = {64, 64, 640 - 64, 480 - 64};
static bool isMoving = false;

void DrawMoveable(HDC hdc, T_COLORPENBRUSH *moveableAreaPenBrushOut, bool b)
{
    T_COLORPENBRUSH moveableAreaPenBrush;
    CreateColorPenBrush(&moveableAreaPenBrush, RGB(64, 64, 255));
    SelectObject(hdc, moveableAreaPenBrush.pen);
    SelectObject(hdc, moveableAreaPenBrush.brush);
    RoundRect(hdc, rectDraggable.left, rectDraggable.top, rectDraggable.right, rectDraggable.bottom, 32, 32);
    HCURSOR pHicon = LoadCursor(nullptr, IDC_SIZEALL);

    DrawIconEx(hdc,
               6, 4,
               pHicon,
               buttonWidth, buttonHeight,
               0,
               nullptr,
               DI_NORMAL);


    *moveableAreaPenBrushOut = moveableAreaPenBrush;

}

void DrawPushButton(HDC hdc, T_COLORPENBRUSH *out, RECT rct, bool b)
{
    T_COLORPENBRUSH buttonPenBrush;
    CreateColorPenBrush(&buttonPenBrush, RGB(202, 2, 2));

    SelectObject(hdc, buttonPenBrush.pen);
    SelectObject(hdc, buttonPenBrush.brush);


    RoundRect(hdc, rct.left, rct.top, rct.right, rct.bottom, 32, 32);
    DrawText(hdc,
             L"MUTE",
             static_cast<int>(wcslen(L"MUTE")),
             &rct,
             DT_SINGLELINE | DT_VCENTER | DT_CENTER);
    *out = buttonPenBrush;

}

void DrawExitArea(HWND hwnd, HDC hdc, T_COLORPENBRUSH *out, bool b)
{
    T_COLORPENBRUSH penbrush;
    CreateColorPenBrush(&penbrush, RGB(0, 255, 0));
    SelectObject(hdc, penbrush.brush);
    SelectObject(hdc, penbrush.pen);
    RoundRect(hdc, rectExit.left, rectExit.top, rectExit.right, rectExit.bottom, 8, 8);

    DrawText(hdc, L"X", static_cast<int>(wcslen(L"X")), &rectExit, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
    *out = penbrush;
}

void CreateFont(HFONT *hFont, LONG fontSize)
{
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(LOGFONT));
    lf.lfHeight = fontSize;
    lf.lfWeight = 800;
    wcscpy_s(lf.lfFaceName, L"Comic Code");
    *hFont = CreateFontIndirect(&lf);

}

LRESULT CALLBACK AMWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    T_COLORPENBRUSH exitAreaBrush;
    T_COLORPENBRUSH moveableAreaPenBrush;
    T_COLORPENBRUSH buttonPenBrush;


    static POINT lastMousePt;
    static bool insideDraggable = false;
    static bool insideExit = false;
    static bool insidePushButton = false;

    switch (msg)
    {
        case WM_NCCREATE:
        {
            GetCursorPos(&lastMousePt);
        }
            break;

        case WM_MOUSEMOVE:
        {
            RECT clientRect;
            POINT currentMousePt;
            GetClientRect(hwnd, &clientRect);
            GetCursorPos(&currentMousePt);
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            if (PtInRect(&rectDraggable, lastMousePt) == TRUE)
            {
                insideDraggable = true;
            } else
            {
                insideDraggable = false;
            }

            if (PtInRect(&rectExit, lastMousePt) == TRUE)
            {
                wchar_t buf[256] = {0};
                wsprintf(buf, L"Inside exit\n");
                OutputDebugString(buf);
                insideExit = true;
            } else
            {
                insideExit = false;
            }

            if(PtInRect(&rectPushButtonArea, lastMousePt) ==TRUE) {
                insidePushButton = true;
            } else {
                insidePushButton = false;
            }
            lastMousePt = POINT{x, y};

            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            if (PtInRect(&rectDraggable, lastMousePt) == TRUE)
            {
                SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            }

            if (PtInRect(&rectExit, lastMousePt) == TRUE)
            {
                PostQuitMessage(0);
            }
            return 0;
        }
        case WM_PAINT:
        {
            hdc = BeginPaint(hwnd, &ps);

            GetWindowRect(hwnd, &rectPushButtonArea);
            rectPushButtonArea.left += 64;
            rectPushButtonArea.top += 64;
            rectPushButtonArea.right -= 74;
            rectPushButtonArea.bottom -= 74;

            HFONT hFont;
            CreateFont(&hFont, 72);
            SelectObject(hdc, hFont);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            DrawMoveable(hdc, &moveableAreaPenBrush, insideDraggable);
            DrawExitArea(hwnd, hdc, &exitAreaBrush, insideExit);
            DrawPushButton(hdc, &buttonPenBrush, rectPushButtonArea, insidePushButton);

            EndPaint(hwnd, &ps);

            DeleteObject(&hFont);
            DestroyColorPenBrush(&buttonPenBrush);
            DestroyColorPenBrush(&moveableAreaPenBrush);
            DestroyColorPenBrush(&exitAreaBrush);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);;
}