#include <Windows.h>
#include "interface.h"

HDC hdc;         // 画布句柄
HDC mdc;         // 光标画布句柄
HBITMAP bmp;     // 图像句柄
PAINTSTRUCT ps;  // 画布结构体
RECT rect;       // 矩形结构体
RECT rectClient; // 客户区矩形结构体
HFONT hFont;     // 字体句柄
HBRUSH hBrush;   // 画刷句柄
TEXTMETRIC tm;   // 字体结构体
POINT pt;        // 点结构体

/**
 * @brief 创建UI窗口
 *
 * @param hwndParent   父窗口句柄
 * @param x            x坐标，单位为PIXEL_SIZE
 * @param y            y坐标，单位为PIXEL_SIZE
 * @param width        窗口宽度，单位为PIXEL_SIZE
 * @param height       窗口高度，单位为PIXEL_SIZE
 * @param isParentMain 父窗口是否为主窗口
 * @return HWND
 */
HWND CreateUIWindow(HWND hwndParent, int x, int y, int width, int height, int isParentMain)
{
    HWND hwndTemp = NULL;

    switch (isParentMain)
    {
    case FALSE:
        hwndTemp = CreateWindow(
            TEXT("Static"), NULL,
            WS_CHILD | WS_VISIBLE,
            x * PIXEL_SIZE, y * PIXEL_SIZE,
            width * PIXEL_SIZE, height * PIXEL_SIZE,
            hwndParent, NULL, NULL, NULL);
        break;

    case TRUE:
        hwndTemp = CreateWindow(
            TEXT("Static"), NULL,
            WS_CHILD | WS_VISIBLE,
            x * PIXEL_SIZE, y * PIXEL_SIZE - WINDOW_CAPTION_HEIGHT,
            width * PIXEL_SIZE, height * PIXEL_SIZE,
            hwndParent, NULL, NULL, NULL);
        break;

    default:
        hwndTemp = NULL;
    }

    return hwndTemp;
}

/**
 * @brief 初始化LCDUI窗口界面
 *
 * @param hwndLCDUI LCDUI窗口句柄
 */
void InitLCDUI(HWND hwndLCDUI)
{
    GetClientRect(hwndLCDUI, &rectClient);
    hdc = BeginPaint(hwndLCDUI, &ps);
    mdc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
    SelectObject(mdc, bmp);
    // 绘制窗口背景
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(mdc, &rectClient, hBrush);
    // 绘制LCD阴影
    SetRect(&rect,
            PIXEL_SIZE,
            PIXEL_SIZE,
            rectClient.right,
            rectClient.bottom);
    hBrush = CreateSolidBrush(RGB(14, 58, 96));
    FillRect(mdc, &rect, hBrush);
    // 绘制LCD底板
    SetRect(&rect,
            0,
            0,
            rectClient.right - PIXEL_SIZE,
            rectClient.bottom - PIXEL_SIZE);
    hBrush = CreateSolidBrush(RGB(24, 93, 152));
    FillRect(mdc, &rect, hBrush);
    // 绘制LCD白色边框
    SetRect(&rect,
            3 * PIXEL_SIZE,
            BORDER_WIDTH,
            rectClient.right - 4 * PIXEL_SIZE,
            rectClient.bottom - PIXEL_SIZE - BORDER_WIDTH);
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(mdc, &rect, hBrush);
    // 绘制LCD深灰色边框
    SetRect(&rect,
            4 * PIXEL_SIZE - BORDER_WIDTH,
            1 * PIXEL_SIZE - BORDER_WIDTH,
            rectClient.right - 6 * PIXEL_SIZE + BORDER_WIDTH,
            rectClient.bottom - 2 * PIXEL_SIZE + BORDER_WIDTH);
    hBrush = CreateSolidBrush(RGB(24, 24, 24));
    FillRect(mdc, &rect, hBrush);
    // 绘制LCD深灰色接线
    SetRect(&rect,
            rectClient.right - 6 * PIXEL_SIZE,
            3 * PIXEL_SIZE,
            rectClient.right - 4 * PIXEL_SIZE,
            rectClient.bottom - 3 * PIXEL_SIZE);
    FillRect(mdc, &rect, hBrush);
    // 绘制LCD黄色接线
    SetRect(&rect,
            rectClient.right - 6 * PIXEL_SIZE + BORDER_WIDTH,
            7 * PIXEL_SIZE,
            rectClient.right - 4 * PIXEL_SIZE,
            rectClient.bottom - 7 * PIXEL_SIZE);
    hBrush = CreateSolidBrush(RGB(183, 127, 36));
    FillRect(mdc, &rect, hBrush);
    // 绘制底板左侧触点文字
    for (int i = 0; i < 7; i++)
    {
        SetRect(&rect,
                0 * PIXEL_SIZE,
                (int)((2 * i + 6.5f) * PIXEL_SIZE),
                1 * PIXEL_SIZE,
                (int)((2 * i + 7.5f) * PIXEL_SIZE));
        hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
        FillRect(mdc, &rect, hBrush);
        SetRect(&rect,
                2 * PIXEL_SIZE,
                (int)((2 * i + 6.5f) * PIXEL_SIZE),
                3 * PIXEL_SIZE,
                (int)((2 * i + 7.5f) * PIXEL_SIZE));
        hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
        FillRect(mdc, &rect, hBrush);
    }
    // 绘制底板右侧文字
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    for (int i = 0; i < 8; i++)
    {
        SetRect(
            &rect,
            rectClient.right - 3 * PIXEL_SIZE,
            (int)(2 * i + 5.5f) * PIXEL_SIZE,
            rectClient.right - 2 * PIXEL_SIZE,
            (int)(2 * i + 6.5f) * PIXEL_SIZE);
        FillRect(mdc, &rect, hBrush);
    }
    // 绘制底板打孔
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            switch (j)
            {
            case 1:
            case 3:
            case 5:
            case 7:
                SetRect(&rect,
                        (i % 2) * (rectClient.right - 4 * PIXEL_SIZE) + (j % 3) * PIXEL_SIZE,
                        (i / 2) * (rectClient.bottom - 6 * PIXEL_SIZE) + (j / 3 + 1) * PIXEL_SIZE,
                        (i % 2) * (rectClient.right - 4 * PIXEL_SIZE) + (j % 3 + 1) * PIXEL_SIZE,
                        (i / 2) * (rectClient.bottom - 6 * PIXEL_SIZE) + (j / 3 + 2) * PIXEL_SIZE);
                hBrush = CreateSolidBrush(RGB(180, 180, 180));
                FillRect(mdc, &rect, hBrush);
                break;
            case 4:
                SetRect(&rect,
                        (i % 2) * (rectClient.right - 4 * PIXEL_SIZE) + (j % 3) * PIXEL_SIZE,
                        (i / 2) * (rectClient.bottom - 6 * PIXEL_SIZE) + (j / 3 + 1) * PIXEL_SIZE,
                        (i % 2) * (rectClient.right - 4 * PIXEL_SIZE) + (j % 3 + 1) * PIXEL_SIZE,
                        (i / 2) * (rectClient.bottom - 6 * PIXEL_SIZE) + (j / 3 + 2) * PIXEL_SIZE);
                hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
                FillRect(mdc, &rect, hBrush);
                break;
            default:
                break;
            }
        }
    }
    BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, mdc, 0, 0, SRCCOPY);
    DeleteObject(hBrush);
    DeleteObject(bmp);
    DeleteDC(mdc);
    EndPaint(hwndLCDUI, &ps);
}

/**
 * @brief 初始化LCD屏幕窗口界面
 *
 * @param hwndLCDScreen LCD屏幕窗口句柄
 */
void InitLCDScreen(HWND hwndLCDScreen)
{
    GetClientRect(hwndLCDScreen, &rectClient);
    hdc = BeginPaint(hwndLCDScreen, &ps);
    mdc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
    SelectObject(mdc, bmp);
    // 绘制窗口背景
    hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    FillRect(mdc, &rectClient, hBrush);
    BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, mdc, 0, 0, SRCCOPY);
    DeleteObject(hBrush);
    DeleteObject(bmp);
    DeleteDC(mdc);
    EndPaint(hwndLCDScreen, &ps);
}

/**
 * @brief 初始化键盘窗口界面
 *
 * @param hwndKeyboard 键盘窗口句柄
 */
void InitKeyboard(HWND hwndKeyboard)
{
    GetClientRect(hwndKeyboard, &rectClient);
    hdc = BeginPaint(hwndKeyboard, &ps);
    mdc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
    SelectObject(mdc, bmp);
    // 绘制窗口背景
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(mdc, &rectClient, hBrush);
    BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, mdc, 0, 0, SRCCOPY);
    DeleteObject(hBrush);
    DeleteObject(bmp);
    DeleteDC(mdc);
    EndPaint(hwndKeyboard, &ps);
}

/**
 * @brief 初始化拨码开关UI窗口界面
 *
 * @param hwndSwitchUI 拨码开关UI窗口句柄
 */
void InitSwitchUI(HWND hwndSwitchUI)
{
    GetClientRect(hwndSwitchUI, &rectClient);
    hdc = BeginPaint(hwndSwitchUI, &ps);
    mdc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
    SelectObject(mdc, bmp);
    // 绘制窗口背景
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(mdc, &rectClient, hBrush);
    // 绘制拨码开关阴影
    SetRect(&rect,
            1 * PIXEL_SIZE,
            1 * PIXEL_SIZE,
            rectClient.right,
            rectClient.bottom);
    hBrush = CreateSolidBrush(RED_SHADOW_COLOR);
    FillRect(mdc, &rect, hBrush);
    // 绘制拨码开关表面
    SetRect(&rect,
            0,
            0,
            rectClient.right - 1 * PIXEL_SIZE,
            rectClient.bottom - 1 * PIXEL_SIZE);
    hBrush = CreateSolidBrush(RED_SURFACE_COLOR);
    FillRect(mdc, &rect, hBrush);
    // 绘制拨码开关文字
    hFont = CreateFont((int)(2.25 * PIXEL_SIZE),
                       0, 0, 0, 0,
                       FALSE, FALSE, FALSE,
                       GB2312_CHARSET,
                       OUT_DEFAULT_PRECIS,
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY,
                       DEFAULT_PITCH,
                       TEXT("GuanZhi"));
    SelectObject(mdc, hFont);
    SetTextColor(mdc, RED_TEXT_COLOR);
    SetBkColor(mdc, RED_SURFACE_COLOR);
    GetTextMetrics(mdc, &tm);
    SetTextAlign(mdc, TA_CENTER);
    SetRect(&rect,
            0 * PIXEL_SIZE,
            0 * PIXEL_SIZE,
            rectClient.right - 1 * PIXEL_SIZE,
            3 * PIXEL_SIZE);
    TextOut(mdc,
            (int)(rect.right / 2 + PIXEL_SIZE / 8),
            (int)(rect.bottom / 2 - tm.tmHeight / 2),
            TEXT("ON"),
            2);
    SetRect(&rect,
            1 * PIXEL_SIZE,
            rectClient.bottom - 4 * PIXEL_SIZE,
            2 * PIXEL_SIZE,
            rectClient.bottom - 1 * PIXEL_SIZE);
    TextOut(mdc,
            (int)((rect.right + rect.left) / 2 + PIXEL_SIZE / 8),
            (int)((rect.bottom + rect.top) / 2 - tm.tmHeight / 2),
            TEXT("1"),
            1);
    SetRect(&rect,
            3 * PIXEL_SIZE,
            rectClient.bottom - 4 * PIXEL_SIZE,
            4 * PIXEL_SIZE,
            rectClient.bottom - 1 * PIXEL_SIZE);
    TextOut(mdc,
            (int)((rect.right + rect.left) / 2 + PIXEL_SIZE / 8),
            (int)((rect.bottom + rect.top) / 2 - tm.tmHeight / 2),
            TEXT("2"),
            1);
    BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, mdc, 0, 0, SRCCOPY);
    DeleteObject(hBrush);
    DeleteObject(hFont);
    DeleteObject(bmp);
    DeleteDC(mdc);
    EndPaint(hwndSwitchUI, &ps);
}

/**
 * @brief 绘制按钮窗口界面
 *
 * @param hwndButton   按钮窗口句柄
 * @param szText       按钮文字
 * @param sizeText     按钮文字大小，单位为PIXEL_SIZE
 * @param isInit       是否为初始化按钮窗口界面
 * @param isHover      是否为鼠标悬停状态
 * @param isPress      是否为鼠标按下状态
 */
void DrawButton(HWND hwndButton, WCHAR *szText, float sizeText,
                BOOL isInit, BOOL isHover, BOOL isPress)
{
    GetClientRect(hwndButton, &rectClient);

    if (isInit)
    {
        hdc = BeginPaint(hwndButton, &ps);
    }
    else
    {
        hdc = GetDC(hwndButton);
    }

    mdc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
    SelectObject(mdc, bmp);

    // 绘制窗口背景
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(mdc, &rectClient, hBrush);

    // 绘制按钮阴影与表面
    SetRect(&rect,
            1 * PIXEL_SIZE,
            1 * PIXEL_SIZE,
            rectClient.right,
            rectClient.bottom);

    if (!isPress)
    {
        hBrush = CreateSolidBrush(GRAY_SHADOW_COLOR);
        FillRect(mdc, &rect, hBrush);

        SetRect(&rect,
                0,
                0,
                rectClient.right - 1 * PIXEL_SIZE,
                rectClient.bottom - 1 * PIXEL_SIZE);
    }

    if (isHover)
    {
        hBrush = CreateSolidBrush(OPE_GRAY_SURFACE_COLOR);
        FillRect(mdc, &rect, hBrush);
    }
    else
    {
        hBrush = CreateSolidBrush(DEF_GRAY_SURFACE_COLOR);
        FillRect(mdc, &rect, hBrush);
    }

    // 绘制按钮文字
    hFont = CreateFont((int)(sizeText * PIXEL_SIZE),
                       0, 0, 0, 0,
                       FALSE, FALSE, FALSE,
                       GB2312_CHARSET,
                       OUT_DEFAULT_PRECIS,
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY,
                       DEFAULT_PITCH,
                       TEXT("GuanZhi"));
    SelectObject(mdc, hFont);
    SetTextColor(mdc, GRAY_SHADOW_COLOR);
    if (isHover)
    {
        SetBkColor(mdc, OPE_GRAY_SURFACE_COLOR);
    }
    else
    {
        SetBkColor(mdc, DEF_GRAY_SURFACE_COLOR);
    }
    GetTextMetrics(mdc, &tm);
    SetTextAlign(mdc, TA_CENTER);
    TextOut(mdc,
            (int)((rect.right + rect.left) / 2 + PIXEL_SIZE / 8),
            (int)((rect.bottom + rect.top) / 2 - tm.tmHeight / 2),
            szText,
            (int)wcslen(szText));

    BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, mdc, 0, 0, SRCCOPY);
    DeleteObject(hBrush);
    DeleteObject(hFont);
    DeleteObject(bmp);
    DeleteDC(mdc);

    if (isInit)
    {
        EndPaint(hwndButton, &ps);
    }
    else
    {
        ReleaseDC(hwndButton, hdc);
    }
}

/**
 * @brief 绘制拨码开关按钮窗口界面
 *
 * @param hwndSwitchButton 拨码开关按钮窗口句柄
 * @param isInit           是否为初始化拨码开关按钮窗口界面
 * @param isOn             是否为开状态
 * @param isHover          是否为鼠标悬停状态
 */
void DrawSwitchButton(HWND hwndSwitchButton, BOOL isInit, BOOL isOn, BOOL isHover)
{
    GetClientRect(hwndSwitchButton, &rectClient);

    if (isInit)
    {
        hdc = BeginPaint(hwndSwitchButton, &ps);
    }
    else
    {
        hdc = GetDC(hwndSwitchButton);
    }

    mdc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
    SelectObject(mdc, bmp);

    // 绘制窗口背景
    hBrush = CreateSolidBrush(GRAY_SHADOW_COLOR);
    FillRect(mdc, &rectClient, hBrush);

    // 绘制按钮
    if (isOn)
    {
        SetRect(&rect,
                0,
                0,
                rectClient.right,
                rectClient.bottom / 2);
    }
    else
    {
        SetRect(&rect,
                0,
                rectClient.bottom / 2,
                rectClient.right,
                rectClient.bottom);
    }

    if (isHover)
    {
        hBrush = CreateSolidBrush(OPE_GRAY_SURFACE_COLOR);
        FillRect(mdc, &rect, hBrush);
    }
    else
    {
        hBrush = CreateSolidBrush(DEF_GRAY_SURFACE_COLOR);
        FillRect(mdc, &rect, hBrush);
    }

    BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, mdc, 0, 0, SRCCOPY);
    DeleteObject(hBrush);
    DeleteObject(bmp);
    DeleteDC(mdc);

    if (isInit)
    {
        EndPaint(hwndSwitchButton, &ps);
    }
    else
    {
        ReleaseDC(hwndSwitchButton, hdc);
    }
}