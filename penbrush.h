#pragma once


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
    return true;
}
