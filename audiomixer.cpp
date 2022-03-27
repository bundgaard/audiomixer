#include "audiomixer.h"
#include <cstdio>

typedef struct tagCOLORPENBRUSH
{
    COLORREF color;
    HPEN pen;
    HBRUSH brush;
} T_COLORPENBRUSH, *PT_COLORPENBRUSH;

bool CreateColorPenBrush(PT_COLORPENBRUSH colorPenBrushStruct, COLORREF color)
{
    LOGBRUSH lb;

    if (colorPenBrushStruct == nullptr)
    {
        return false;
    }
    colorPenBrushStruct->color = color;
    colorPenBrushStruct->pen = CreatePen(PS_SOLID, 0, color);
    lb.lbColor = color;
    lb.lbStyle = BS_SOLID;

    colorPenBrushStruct->brush = CreateBrushIndirect(&lb);
    return true;

}

bool DestroyColorPenBrush(PT_COLORPENBRUSH ptColorpenbrush)
{
    if (ptColorpenbrush == nullptr)
        return false;

    ptColorpenbrush->color = 0;
    if (ptColorpenbrush->pen != nullptr)
    {
        DeleteObject(ptColorpenbrush->pen);
    }
    ptColorpenbrush->pen = nullptr;
    if (ptColorpenbrush->brush != nullptr)
    {
        DeleteObject(ptColorpenbrush->brush);
    }
    ptColorpenbrush->brush = nullptr;
}

static RECT rectDraggable = {10, 10, 100, 100};

LRESULT CALLBACK AMWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HFONT pHfont;

    switch (msg)
    {
        case WM_NCHITTEST:
        {
            const LRESULT result = DefWindowProc(hwnd, msg, wParam, lParam);
            POINT pt;
            GetCursorPos(&pt);
            RECT rc = rectDraggable;
            MapWindowPoints(hwnd, nullptr, reinterpret_cast<POINT *>(&rc), (sizeof(RECT) / sizeof(POINT)));

            return (result == HTCLIENT) && (PtInRect(&rc, pt)) ? HTCAPTION : result;
        }
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            RECT rct;
            GetClientRect(hwnd, &rct);
            rct.left += 40;
            rct.top += 40;
            rct.right -= 40;
            rct.bottom -= 40;
            LOGFONT lf;
            ZeroMemory(&lf, sizeof(LOGFONT));
            lf.lfHeight = 72;
            lf.lfWeight = 800;
            wcscpy_s(lf.lfFaceName, L"Comic Code");
            pHfont = CreateFontIndirect(&lf);

            SelectObject(hdc, pHfont);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            T_COLORPENBRUSH moveableAreaPenBrush;
            CreateColorPenBrush(&moveableAreaPenBrush, RGB(64, 64, 255));
            SelectObject(hdc, moveableAreaPenBrush.pen);
            SelectObject(hdc, moveableAreaPenBrush.brush);
            RoundRect(hdc, rectDraggable.left, rectDraggable.top, rectDraggable.right, rectDraggable.bottom, 32, 32);

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
            EndPaint(hwnd, &ps);
            DestroyColorPenBrush(&buttonPenBrush);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);;
}