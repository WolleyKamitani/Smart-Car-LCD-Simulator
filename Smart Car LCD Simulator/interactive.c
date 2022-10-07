#include <Windows.h>
#include "interface.h"
#include "interactive.h"
#include "user.h"

extern HWND hwndKeyboard;
extern HWND hwndButton[16];
extern HWND hwndSwitchButton[2];
extern HWND hwndResetButton;
extern HWND hwndAboutButton;
extern WCHAR szButtonText[16][2];

int ActiveButton = -1;                      // 当前响应按钮
BOOL ButtonHoverState[20] = {FALSE};        // 按钮悬停状态
BOOL SwitchButtonState[2] = {FALSE, FALSE}; // 拨码开关状态

RECT rect; // 矩形结构体
POINT pt;  // 点结构体

/**
 * @brief 获取鼠标当前悬停的按钮
 *
 * @param hwndMain 主窗口句柄
 * @param x        鼠标x坐标
 * @param y        鼠标y坐标
 * @return int     按钮编号，0-15为LCD按钮，16、17为拨码开关按钮
 *                 18为重置按钮，19为关于按钮，-1为无效坐标
 */
int GetMouseHoverButton(HWND hwndMain, LONG x, LONG y)
{
    pt.x = x;
    pt.y = y;

    // 检测鼠标是否悬停在LCD按钮上
    for (int i = 0; i < 16; i++)
    {
        GetWindowRect(hwndButton[i], &rect);
        MapWindowPoints(HWND_DESKTOP, hwndMain, (LPPOINT)&rect, 2);
        if (PtInRect(&rect, pt))
        {
            return i;
        }
    }

    // 检测鼠标是否悬停在拨码开关按钮上
    for (int i = 0; i < 2; i++)
    {
        GetWindowRect(hwndSwitchButton[i], &rect);
        MapWindowPoints(HWND_DESKTOP, hwndMain, (LPPOINT)&rect, 2);
        if (PtInRect(&rect, pt))
        {
            return i + 16;
        }
    }

    // 检测鼠标是否悬停在重置按钮上
    GetWindowRect(hwndResetButton, &rect);
    MapWindowPoints(HWND_DESKTOP, hwndMain, (LPPOINT)&rect, 2);
    if (PtInRect(&rect, pt))
    {
        return 18;
    }

    // 检测鼠标是否悬停在关于按钮上
    GetWindowRect(hwndAboutButton, &rect);
    MapWindowPoints(HWND_DESKTOP, hwndMain, (LPPOINT)&rect, 2);
    if (PtInRect(&rect, pt))
    {
        return 19;
    }

    return -1;
}

/**
 * @brief 鼠标移动按钮事件
 *
 * @param HoverButton     当前按钮编号
 * @param LastHoverButton 上一按钮编号
 */
void MouseMoveButtonAction(int HoverButton, int LastHoverButton)
{
    if (HoverButton != -1 && HoverButton != LastHoverButton)
    {
        SetCursor(LoadCursor(NULL, IDC_HAND));

        if (HoverButton < 16)
        {
            DrawButton(hwndButton[HoverButton], szButtonText[HoverButton], 2.25,
                       FALSE, TRUE, FALSE);
        }
        else if (HoverButton < 18)
        {
            DrawSwitchButton(hwndSwitchButton[HoverButton - 16],
                             FALSE, SwitchButtonState[HoverButton - 16], TRUE);
        }
        else if (HoverButton == 18)
        {
            DrawButton(hwndResetButton, TEXT("重置"), 2,
                       FALSE, TRUE, FALSE);
        }
        else if (HoverButton == 19)
        {
            DrawButton(hwndAboutButton, TEXT("关于"), 2,
                       FALSE, TRUE, FALSE);
        }
    }

    if (HoverButton != LastHoverButton && LastHoverButton != -1)
    {
        if (LastHoverButton < 16)
        {
            DrawButton(hwndButton[LastHoverButton], szButtonText[LastHoverButton], 2.25,
                       FALSE, FALSE, FALSE);
        }
        else if (LastHoverButton < 18)
        {
            DrawSwitchButton(hwndSwitchButton[LastHoverButton - 16],
                             FALSE, SwitchButtonState[LastHoverButton - 16], FALSE);
        }
        else if (LastHoverButton == 18)
        {
            DrawButton(hwndResetButton, TEXT("重置"), 2,
                       FALSE, FALSE, FALSE);
        }
        else if (LastHoverButton == 19)
        {
            DrawButton(hwndAboutButton, TEXT("关于"), 2,
                       FALSE, FALSE, FALSE);
        }
    }
}

/**
 * @brief 鼠标点击按钮事件
 *
 * @param HoverButton 按钮编号
 * @param isClickDown 鼠标是否为按下状态
 */
void MouseClickButtonAction(int HoverButton, int isClickDown)
{
    if (HoverButton != -1)
    {
        if (HoverButton < 16)
        {
            DrawButton(hwndButton[HoverButton], szButtonText[HoverButton], 2.25,
                       FALSE, TRUE, isClickDown);
        }
        else if (HoverButton < 18)
        {
            DrawSwitchButton(hwndSwitchButton[HoverButton - 16],
                             FALSE, SwitchButtonState[HoverButton - 16], TRUE);
        }
        else if (HoverButton == 18)
        {
            DrawButton(hwndResetButton, TEXT("重置"), 2,
                       FALSE, TRUE, isClickDown);
        }
        else if (HoverButton == 19)
        {
            DrawButton(hwndAboutButton, TEXT("关于"), 2,
                       FALSE, TRUE, isClickDown);
        }
    }

    if (isClickDown)
    {
        if (HoverButton < 16)
        {
            ActiveButton = HoverButton;
        }
        else if (HoverButton < 18)
        {
            SwitchButtonState[HoverButton - 16] = !SwitchButtonState[HoverButton - 16];
        }
        else if (HoverButton == 18)
        {
            UserInit();

            for (int i = 0; i < 2; i++)
            {
                SwitchButtonState[i] = FALSE;
                DrawSwitchButton(hwndSwitchButton[i], FALSE, FALSE, FALSE);
            }
        }
        else if (HoverButton == 19)
        {
            if (MessageBox(hwndKeyboard, TEXT("智能车 LCD 仿真程序\n作者：厦门大学智能车队\n\n是否前往软件使用说明页面？"), TEXT("关于"), MB_YESNO) == IDYES)
            {
                ShellExecute(NULL, TEXT("open"), TEXT("https://github.com/WolleyKamitani/Smart-Car-LCD-Simulator"), NULL, NULL, SW_SHOW);
            }
        }
    }
    else
    {
        ActiveButton = -1;
    }
}

/**
 * @brief 键盘按下按钮事件
 *
 * @param PressKey  按下的键盘按键ASCII码
 * @param isKeyDown 键盘是否为按下状态
 */
void KeyboardButtonAction(int PressKey, int isKeyDown)
{
    for (int i = 0; i < 16; i++)
    {
        if (PressKey == szButtonText[i][0])
        {
            if (isKeyDown)
            {
                ActiveButton = i;
            }
            else
            {
                ActiveButton = -1;
            }

            DrawButton(hwndButton[i], szButtonText[i], 2.25,
                       FALSE, FALSE, isKeyDown);

            break;
        }
    }
}