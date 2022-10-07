#include <Windows.h>
#include "interface.h"

HDC hdc;         // �������
HDC mdc;         // ��껭�����
HBITMAP bmp;     // ͼ����
PAINTSTRUCT ps;  // �����ṹ��
RECT rect;       // ���νṹ��
RECT rectClient; // �ͻ������νṹ��
HFONT hFont;     // ������
HBRUSH hBrush;   // ��ˢ���
TEXTMETRIC tm;   // ����ṹ��
POINT pt;        // ��ṹ��

/**
 * @brief ����UI����
 *
 * @param hwndParent   �����ھ��
 * @param x            x���꣬��λΪPIXEL_SIZE
 * @param y            y���꣬��λΪPIXEL_SIZE
 * @param width        ���ڿ�ȣ���λΪPIXEL_SIZE
 * @param height       ���ڸ߶ȣ���λΪPIXEL_SIZE
 * @param isParentMain �������Ƿ�Ϊ������
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
 * @brief ��ʼ��LCDUI���ڽ���
 *
 * @param hwndLCDUI LCDUI���ھ��
 */
void InitLCDUI(HWND hwndLCDUI)
{
    GetClientRect(hwndLCDUI, &rectClient);
    hdc = BeginPaint(hwndLCDUI, &ps);
    mdc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
    SelectObject(mdc, bmp);
    // ���ƴ��ڱ���
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(mdc, &rectClient, hBrush);
    // ����LCD��Ӱ
    SetRect(&rect,
            PIXEL_SIZE,
            PIXEL_SIZE,
            rectClient.right,
            rectClient.bottom);
    hBrush = CreateSolidBrush(RGB(14, 58, 96));
    FillRect(mdc, &rect, hBrush);
    // ����LCD�װ�
    SetRect(&rect,
            0,
            0,
            rectClient.right - PIXEL_SIZE,
            rectClient.bottom - PIXEL_SIZE);
    hBrush = CreateSolidBrush(RGB(24, 93, 152));
    FillRect(mdc, &rect, hBrush);
    // ����LCD��ɫ�߿�
    SetRect(&rect,
            3 * PIXEL_SIZE,
            BORDER_WIDTH,
            rectClient.right - 4 * PIXEL_SIZE,
            rectClient.bottom - PIXEL_SIZE - BORDER_WIDTH);
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(mdc, &rect, hBrush);
    // ����LCD���ɫ�߿�
    SetRect(&rect,
            4 * PIXEL_SIZE - BORDER_WIDTH,
            1 * PIXEL_SIZE - BORDER_WIDTH,
            rectClient.right - 6 * PIXEL_SIZE + BORDER_WIDTH,
            rectClient.bottom - 2 * PIXEL_SIZE + BORDER_WIDTH);
    hBrush = CreateSolidBrush(RGB(24, 24, 24));
    FillRect(mdc, &rect, hBrush);
    // ����LCD���ɫ����
    SetRect(&rect,
            rectClient.right - 6 * PIXEL_SIZE,
            3 * PIXEL_SIZE,
            rectClient.right - 4 * PIXEL_SIZE,
            rectClient.bottom - 3 * PIXEL_SIZE);
    FillRect(mdc, &rect, hBrush);
    // ����LCD��ɫ����
    SetRect(&rect,
            rectClient.right - 6 * PIXEL_SIZE + BORDER_WIDTH,
            7 * PIXEL_SIZE,
            rectClient.right - 4 * PIXEL_SIZE,
            rectClient.bottom - 7 * PIXEL_SIZE);
    hBrush = CreateSolidBrush(RGB(183, 127, 36));
    FillRect(mdc, &rect, hBrush);
    // ���Ƶװ���ഥ������
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
    // ���Ƶװ��Ҳ�����
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
    // ���Ƶװ���
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
 * @brief ��ʼ��LCD��Ļ���ڽ���
 *
 * @param hwndLCDScreen LCD��Ļ���ھ��
 */
void InitLCDScreen(HWND hwndLCDScreen)
{
    GetClientRect(hwndLCDScreen, &rectClient);
    hdc = BeginPaint(hwndLCDScreen, &ps);
    mdc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
    SelectObject(mdc, bmp);
    // ���ƴ��ڱ���
    hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    FillRect(mdc, &rectClient, hBrush);
    BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, mdc, 0, 0, SRCCOPY);
    DeleteObject(hBrush);
    DeleteObject(bmp);
    DeleteDC(mdc);
    EndPaint(hwndLCDScreen, &ps);
}

/**
 * @brief ��ʼ�����̴��ڽ���
 *
 * @param hwndKeyboard ���̴��ھ��
 */
void InitKeyboard(HWND hwndKeyboard)
{
    GetClientRect(hwndKeyboard, &rectClient);
    hdc = BeginPaint(hwndKeyboard, &ps);
    mdc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
    SelectObject(mdc, bmp);
    // ���ƴ��ڱ���
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(mdc, &rectClient, hBrush);
    BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, mdc, 0, 0, SRCCOPY);
    DeleteObject(hBrush);
    DeleteObject(bmp);
    DeleteDC(mdc);
    EndPaint(hwndKeyboard, &ps);
}

/**
 * @brief ��ʼ�����뿪��UI���ڽ���
 *
 * @param hwndSwitchUI ���뿪��UI���ھ��
 */
void InitSwitchUI(HWND hwndSwitchUI)
{
    GetClientRect(hwndSwitchUI, &rectClient);
    hdc = BeginPaint(hwndSwitchUI, &ps);
    mdc = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
    SelectObject(mdc, bmp);
    // ���ƴ��ڱ���
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(mdc, &rectClient, hBrush);
    // ���Ʋ��뿪����Ӱ
    SetRect(&rect,
            1 * PIXEL_SIZE,
            1 * PIXEL_SIZE,
            rectClient.right,
            rectClient.bottom);
    hBrush = CreateSolidBrush(RED_SHADOW_COLOR);
    FillRect(mdc, &rect, hBrush);
    // ���Ʋ��뿪�ر���
    SetRect(&rect,
            0,
            0,
            rectClient.right - 1 * PIXEL_SIZE,
            rectClient.bottom - 1 * PIXEL_SIZE);
    hBrush = CreateSolidBrush(RED_SURFACE_COLOR);
    FillRect(mdc, &rect, hBrush);
    // ���Ʋ��뿪������
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
 * @brief ���ư�ť���ڽ���
 *
 * @param hwndButton   ��ť���ھ��
 * @param szText       ��ť����
 * @param sizeText     ��ť���ִ�С����λΪPIXEL_SIZE
 * @param isInit       �Ƿ�Ϊ��ʼ����ť���ڽ���
 * @param isHover      �Ƿ�Ϊ�����ͣ״̬
 * @param isPress      �Ƿ�Ϊ��갴��״̬
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

    // ���ƴ��ڱ���
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(mdc, &rectClient, hBrush);

    // ���ư�ť��Ӱ�����
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

    // ���ư�ť����
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
 * @brief ���Ʋ��뿪�ذ�ť���ڽ���
 *
 * @param hwndSwitchButton ���뿪�ذ�ť���ھ��
 * @param isInit           �Ƿ�Ϊ��ʼ�����뿪�ذ�ť���ڽ���
 * @param isOn             �Ƿ�Ϊ��״̬
 * @param isHover          �Ƿ�Ϊ�����ͣ״̬
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

    // ���ƴ��ڱ���
    hBrush = CreateSolidBrush(GRAY_SHADOW_COLOR);
    FillRect(mdc, &rectClient, hBrush);

    // ���ư�ť
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