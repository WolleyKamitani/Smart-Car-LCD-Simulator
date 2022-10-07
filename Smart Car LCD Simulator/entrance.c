#include <Windows.h>
#include "interface.h"
#include "interactive.h"
#include "user.h"

#define WINDOW_WIDTH_FIX 16 // 窗口宽度修正
#define WINDOW_HEIGHT_FIX 9 // 窗口高度修正

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

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // 窗口过程函数声明

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,         // 应用程序实例句柄
                      _In_opt_ HINSTANCE hPrevInstance, // 上一个应用程序实例句柄
                      _In_ LPWSTR lpCmdLine,            // 命令行参数
                      _In_ int nCmdShow)                // 窗口显示状态
{
    static TCHAR szClassName[] = TEXT("LCD Simulator"); // 窗口类名

    HWND hwnd;         // 窗口句柄
    MSG msg;           // 消息
    WNDCLASS wndclass; // 窗口类

    wndclass.style = CS_HREDRAW | CS_VREDRAW;         // 窗口风格
    wndclass.lpfnWndProc = WndProc;                   // 窗口过程函数
    wndclass.cbClsExtra = 0;                          // 窗口类额外空间
    wndclass.cbWndExtra = 0;                          // 窗口额外空间
    wndclass.hInstance = hInstance;                   // 应用程序实例句柄
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 应用程序图标
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);   // 鼠标光标
    wndclass.hbrBackground = NULL;                    // 窗口背景色
    wndclass.lpszMenuName = NULL;                     // 窗口菜单
    wndclass.lpszClassName = szClassName;             // 窗口类名

    if (!RegisterClass(&wndclass)) // 注册窗口类
    {
        MessageBox(NULL,                   // 窗口句柄
                   TEXT("窗口注册失败。"), // 消息内容
                   szClassName,            // 消息标题
                   MB_ICONERROR | MB_OK);  // 消息类型
        return 0;
    }

    hwnd = CreateWindow(szClassName,                         // 窗口类名
                        TEXT("智能车 LCD 仿真程序"),         // 窗口标题
                        WS_SYSMENU | WS_CAPTION,             // 窗口风格
                        CW_USEDEFAULT,                       // 窗口位置X
                        CW_USEDEFAULT,                       // 窗口位置Y
                        86 * PIXEL_SIZE + WINDOW_WIDTH_FIX,  // 窗口宽度
                        33 * PIXEL_SIZE + WINDOW_HEIGHT_FIX, // 窗口高度
                        NULL,                                // 父窗口句柄
                        NULL,                                // 窗口菜单句柄
                        hInstance,                           // 应用程序实例句柄
                        NULL);                               // 窗口初始化参数

    ShowWindow(hwnd, nCmdShow); // 显示窗口
    UpdateWindow(hwnd);         // 更新窗口

    while (GetMessage(&msg, NULL, 0, 0)) // 消息循环
    {
        TranslateMessage(&msg); // 转换消息
        DispatchMessage(&msg);  // 派发消息
    }

    return (int)msg.wParam; // 返回消息参数
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;        // 画布句柄
    PAINTSTRUCT ps; // 画布结构体

    static int InitFlag = FALSE;     // 初始化标志
    static int LastHoverButton = -1; // 上一悬停按钮
    static int HoverButton = -1;     // 当前悬停按钮

    switch (message)
    {
    case WM_CREATE: // 窗口创建消息
        // 开启ClearType字体渲染
        SystemParametersInfo(SPI_SETFONTSMOOTHING,
                             TRUE,
                             0,
                             SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
        SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE,
                             0,
                             (PVOID)FE_FONTSMOOTHINGCLEARTYPE,
                             SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

        AddFontResource(TEXT("GuanZhi.ttf")); // 加载字体资源

        hwndLCDUI = CreateUIWindow(hwnd, 3, 3, 42, 27, TRUE);                // 创建LCDUI窗口
        hwndLCDScreen = CreateUIWindow(hwndLCDUI, 4, 1, 30, 24, FALSE);      // 创建LCD屏幕窗口
        hwndKeyboard = CreateUIWindow(hwnd, 48, 4, 35, 25, TRUE);            // 创建键盘窗口
        hwndSwitchUI = CreateUIWindow(hwndKeyboard, 29, 0, 6, 11, FALSE);    // 创建开关UI窗口
        hwndResetButton = CreateUIWindow(hwndKeyboard, 29, 14, 6, 4, FALSE); // 创建重置按钮窗口
        hwndAboutButton = CreateUIWindow(hwndKeyboard, 29, 21, 6, 4, FALSE); // 创建关于按钮窗口

        // 创建按钮窗口
        for (int i = 0; i < 16; i++)
        {
            hwndButton[i] = CreateUIWindow(hwndKeyboard, (i % 4) * 7, (i / 4) * 7, 5, 4, FALSE);
        }

        // 创建开关按钮窗口
        for (int i = 0; i < 2; i++)
        {
            hwndSwitchButton[i] = CreateUIWindow(hwndSwitchUI, (i % 2) * 2 + 1, 3, 1, 4, FALSE);
        }

        SetTimer(hwnd, 1, WHILE_TIME, NULL); // 定时器

        return 0;

    case WM_PAINT: // 窗口绘制消息
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);

        InitLCDUI(hwndLCDUI);                                             // 初始化LCDUI窗口界面
        InitLCDScreen(hwndLCDScreen);                                     // 初始化LCD屏幕窗口界面
        InitKeyboard(hwndKeyboard);                                       // 初始化键盘窗口界面
        InitSwitchUI(hwndSwitchUI);                                       // 初始化拨码开关UI窗口界面
        DrawButton(hwndResetButton, TEXT("重置"), 2, TRUE, FALSE, FALSE); // 初始化复原按钮窗口界面
        DrawButton(hwndAboutButton, TEXT("关于"), 2, TRUE, FALSE, FALSE); // 初始化关于按钮窗口界面

        // 初始化LCD按钮窗口界面
        for (int i = 0; i < 16; i++)
        {
            DrawButton(hwndButton[i], szButtonText[i], 2.25, TRUE, FALSE, FALSE);
        }

        // 绘制拨码开关按钮
        for (int i = 0; i < 2; i++)
        {
            DrawSwitchButton(hwndSwitchButton[i], TRUE, FALSE, FALSE);
        }

        // 用户初始化
        if (!InitFlag)
        {
            UserInit();
            InitFlag = TRUE;
        }
        else
        {
            // 此处放置菜单刷新代码
        }

        return 0;

    case WM_MOUSEMOVE: // 鼠标移动
        LastHoverButton = HoverButton;
        HoverButton = GetMouseHoverButton(hwnd, LOWORD(lParam), HIWORD(lParam));
        MouseMoveButtonAction(HoverButton, LastHoverButton);

        return 0;

    case WM_LBUTTONDOWN: // 鼠标左键按下
        MouseClickButtonAction(HoverButton, TRUE);
        return 0;

    case WM_LBUTTONUP: // 鼠标左键抬起
        MouseClickButtonAction(HoverButton, FALSE);
        return 0;

    case WM_KEYDOWN: // 键盘按下
        KeyboardButtonAction((int)wParam, TRUE);
        return 0;

    case WM_KEYUP: // 键盘抬起
        KeyboardButtonAction((int)wParam, FALSE);
        return 0;

    case WM_TIMER: // 定时器
        UserWhile();
        return 0;

    case WM_SETCURSOR: // 设置光标
        if (HoverButton != -1)
        {
            return 0;
        }
        return DefWindowProc(hwnd, message, wParam, lParam);

    case WM_DESTROY:        // 销毁窗口
        PostQuitMessage(0); // 发送退出消息
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam); // 调用系统默认处理函数
}