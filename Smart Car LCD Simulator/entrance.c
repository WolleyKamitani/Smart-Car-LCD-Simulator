#include <Windows.h>
#include "interface.h"
#include "interactive.h"
#include "user.h"

#define WINDOW_WIDTH_FIX 16 // ���ڿ������
#define WINDOW_HEIGHT_FIX 9 // ���ڸ߶�����

HWND hwndLCDUI;
HWND hwndLCDScreen;
HWND hwndKeyboard;
HWND hwndButton[16];
HWND hwndSwitchUI;
HWND hwndSwitchButton[2];
HWND hwndResetButton;
HWND hwndAboutButton;

WCHAR szButtonText[16][2] = {
    TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4"),
    TEXT("Q"), TEXT("W"), TEXT("E"), TEXT("R"),
    TEXT("A"), TEXT("S"), TEXT("D"), TEXT("F"),
    TEXT("Z"), TEXT("X"), TEXT("C"), TEXT("V")};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // ���ڹ��̺�������

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,         // Ӧ�ó���ʵ�����
                      _In_opt_ HINSTANCE hPrevInstance, // ��һ��Ӧ�ó���ʵ�����
                      _In_ LPWSTR lpCmdLine,            // �����в���
                      _In_ int nCmdShow)                // ������ʾ״̬
{
    static TCHAR szClassName[] = TEXT("LCD Simulator"); // ��������

    HWND hwnd;         // ���ھ��
    MSG msg;           // ��Ϣ
    WNDCLASS wndclass; // ������

    wndclass.style = CS_HREDRAW | CS_VREDRAW;         // ���ڷ��
    wndclass.lpfnWndProc = WndProc;                   // ���ڹ��̺���
    wndclass.cbClsExtra = 0;                          // ���������ռ�
    wndclass.cbWndExtra = 0;                          // ���ڶ���ռ�
    wndclass.hInstance = hInstance;                   // Ӧ�ó���ʵ�����
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Ӧ�ó���ͼ��
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);   // �����
    wndclass.hbrBackground = NULL;                    // ���ڱ���ɫ
    wndclass.lpszMenuName = NULL;                     // ���ڲ˵�
    wndclass.lpszClassName = szClassName;             // ��������

    if (!RegisterClass(&wndclass)) // ע�ᴰ����
    {
        MessageBox(NULL,                   // ���ھ��
                   TEXT("����ע��ʧ�ܡ�"), // ��Ϣ����
                   szClassName,            // ��Ϣ����
                   MB_ICONERROR | MB_OK);  // ��Ϣ����
        return 0;
    }

    hwnd = CreateWindow(szClassName,                         // ��������
                        TEXT("���ܳ� LCD �������"),         // ���ڱ���
                        WS_SYSMENU | WS_CAPTION,             // ���ڷ��
                        CW_USEDEFAULT,                       // ����λ��X
                        CW_USEDEFAULT,                       // ����λ��Y
                        86 * PIXEL_SIZE + WINDOW_WIDTH_FIX,  // ���ڿ��
                        33 * PIXEL_SIZE + WINDOW_HEIGHT_FIX, // ���ڸ߶�
                        NULL,                                // �����ھ��
                        NULL,                                // ���ڲ˵����
                        hInstance,                           // Ӧ�ó���ʵ�����
                        NULL);                               // ���ڳ�ʼ������

    ShowWindow(hwnd, nCmdShow); // ��ʾ����
    UpdateWindow(hwnd);         // ���´���

    while (GetMessage(&msg, NULL, 0, 0)) // ��Ϣѭ��
    {
        TranslateMessage(&msg); // ת����Ϣ
        DispatchMessage(&msg);  // �ɷ���Ϣ
    }

    return (int)msg.wParam; // ������Ϣ����
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;        // �������
    PAINTSTRUCT ps; // �����ṹ��

    static int InitFlag = FALSE;     // ��ʼ����־
    static int LastHoverButton = -1; // ��һ��ͣ��ť
    static int HoverButton = -1;     // ��ǰ��ͣ��ť

    switch (message)
    {
    case WM_CREATE: // ���ڴ�����Ϣ
        // ����ClearType������Ⱦ
        SystemParametersInfo(SPI_SETFONTSMOOTHING,
                             TRUE,
                             0,
                             SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
        SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE,
                             0,
                             (PVOID)FE_FONTSMOOTHINGCLEARTYPE,
                             SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

        AddFontResource(TEXT("GuanZhi.ttf")); // ����������Դ

        hwndLCDUI = CreateUIWindow(hwnd, 3, 3, 42, 27, TRUE);                // ����LCDUI����
        hwndLCDScreen = CreateUIWindow(hwndLCDUI, 4, 1, 30, 24, FALSE);      // ����LCD��Ļ����
        hwndKeyboard = CreateUIWindow(hwnd, 48, 4, 35, 25, TRUE);            // �������̴���
        hwndSwitchUI = CreateUIWindow(hwndKeyboard, 29, 0, 6, 11, FALSE);    // ��������UI����
        hwndResetButton = CreateUIWindow(hwndKeyboard, 29, 14, 6, 4, FALSE); // �������ð�ť����
        hwndAboutButton = CreateUIWindow(hwndKeyboard, 29, 21, 6, 4, FALSE); // �������ڰ�ť����

        // ������ť����
        for (int i = 0; i < 16; i++)
        {
            hwndButton[i] = CreateUIWindow(hwndKeyboard, (i % 4) * 7, (i / 4) * 7, 5, 4, FALSE);
        }

        // �������ذ�ť����
        for (int i = 0; i < 2; i++)
        {
            hwndSwitchButton[i] = CreateUIWindow(hwndSwitchUI, (i % 2) * 2 + 1, 3, 1, 4, FALSE);
        }

        SetTimer(hwnd, 1, WHILE_TIME, NULL); // ��ʱ��

        return 0;

    case WM_PAINT: // ���ڻ�����Ϣ
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);

        InitLCDUI(hwndLCDUI);                                             // ��ʼ��LCDUI���ڽ���
        InitLCDScreen(hwndLCDScreen);                                     // ��ʼ��LCD��Ļ���ڽ���
        InitKeyboard(hwndKeyboard);                                       // ��ʼ�����̴��ڽ���
        InitSwitchUI(hwndSwitchUI);                                       // ��ʼ�����뿪��UI���ڽ���
        DrawButton(hwndResetButton, TEXT("����"), 2, TRUE, FALSE, FALSE); // ��ʼ����ԭ��ť���ڽ���
        DrawButton(hwndAboutButton, TEXT("����"), 2, TRUE, FALSE, FALSE); // ��ʼ�����ڰ�ť���ڽ���

        // ��ʼ��LCD��ť���ڽ���
        for (int i = 0; i < 16; i++)
        {
            DrawButton(hwndButton[i], szButtonText[i], 2.25, TRUE, FALSE, FALSE);
        }

        // ���Ʋ��뿪�ذ�ť
        for (int i = 0; i < 2; i++)
        {
            DrawSwitchButton(hwndSwitchButton[i], TRUE, FALSE, FALSE);
        }

        // �û���ʼ��
        if (!InitFlag)
        {
            UserInit();
            InitFlag = TRUE;
        }
        else
        {
            // �˴����ò˵�ˢ�´���
        }

        return 0;

    case WM_MOUSEMOVE: // ����ƶ�
        LastHoverButton = HoverButton;
        HoverButton = GetMouseHoverButton(hwnd, LOWORD(lParam), HIWORD(lParam));
        MouseMoveButtonAction(HoverButton, LastHoverButton);

        return 0;

    case WM_LBUTTONDOWN: // ����������
        MouseClickButtonAction(HoverButton, TRUE);
        return 0;

    case WM_LBUTTONUP: // ������̧��
        MouseClickButtonAction(HoverButton, FALSE);
        return 0;

    case WM_KEYDOWN: // ���̰���
        KeyboardButtonAction((int)wParam, TRUE);
        return 0;

    case WM_KEYUP: // ����̧��
        KeyboardButtonAction((int)wParam, FALSE);
        return 0;

    case WM_TIMER: // ��ʱ��
        UserWhile();
        return 0;

    case WM_SETCURSOR: // ���ù��
        if (HoverButton != -1)
        {
            return 0;
        }
        return DefWindowProc(hwnd, message, wParam, lParam);

    case WM_DESTROY:        // ���ٴ���
        PostQuitMessage(0); // �����˳���Ϣ
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam); // ����ϵͳĬ�ϴ�����
}