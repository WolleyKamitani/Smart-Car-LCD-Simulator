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

int ActiveButton = -1;                      // ��ǰ��Ӧ��ť
BOOL ButtonHoverState[20] = {FALSE};        // ��ť��ͣ״̬
BOOL SwitchButtonState[2] = {FALSE, FALSE}; // ���뿪��״̬

RECT rect; // ���νṹ��
POINT pt;  // ��ṹ��

/**
 * @brief ��ȡ��굱ǰ��ͣ�İ�ť
 *
 * @param hwndMain �����ھ��
 * @param x        ���x����
 * @param y        ���y����
 * @return int     ��ť��ţ�0-15ΪLCD��ť��16��17Ϊ���뿪�ذ�ť
 *                 18Ϊ���ð�ť��19Ϊ���ڰ�ť��-1Ϊ��Ч����
 */
int GetMouseHoverButton(HWND hwndMain, LONG x, LONG y)
{
    pt.x = x;
    pt.y = y;

    // �������Ƿ���ͣ��LCD��ť��
    for (int i = 0; i < 16; i++)
    {
        GetWindowRect(hwndButton[i], &rect);
        MapWindowPoints(HWND_DESKTOP, hwndMain, (LPPOINT)&rect, 2);
        if (PtInRect(&rect, pt))
        {
            return i;
        }
    }

    // �������Ƿ���ͣ�ڲ��뿪�ذ�ť��
    for (int i = 0; i < 2; i++)
    {
        GetWindowRect(hwndSwitchButton[i], &rect);
        MapWindowPoints(HWND_DESKTOP, hwndMain, (LPPOINT)&rect, 2);
        if (PtInRect(&rect, pt))
        {
            return i + 16;
        }
    }

    // �������Ƿ���ͣ�����ð�ť��
    GetWindowRect(hwndResetButton, &rect);
    MapWindowPoints(HWND_DESKTOP, hwndMain, (LPPOINT)&rect, 2);
    if (PtInRect(&rect, pt))
    {
        return 18;
    }

    // �������Ƿ���ͣ�ڹ��ڰ�ť��
    GetWindowRect(hwndAboutButton, &rect);
    MapWindowPoints(HWND_DESKTOP, hwndMain, (LPPOINT)&rect, 2);
    if (PtInRect(&rect, pt))
    {
        return 19;
    }

    return -1;
}

/**
 * @brief ����ƶ���ť�¼�
 *
 * @param HoverButton     ��ǰ��ť���
 * @param LastHoverButton ��һ��ť���
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
            DrawButton(hwndResetButton, TEXT("����"), 2,
                       FALSE, TRUE, FALSE);
        }
        else if (HoverButton == 19)
        {
            DrawButton(hwndAboutButton, TEXT("����"), 2,
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
            DrawButton(hwndResetButton, TEXT("����"), 2,
                       FALSE, FALSE, FALSE);
        }
        else if (LastHoverButton == 19)
        {
            DrawButton(hwndAboutButton, TEXT("����"), 2,
                       FALSE, FALSE, FALSE);
        }
    }
}

/**
 * @brief �������ť�¼�
 *
 * @param HoverButton ��ť���
 * @param isClickDown ����Ƿ�Ϊ����״̬
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
            DrawButton(hwndResetButton, TEXT("����"), 2,
                       FALSE, TRUE, isClickDown);
        }
        else if (HoverButton == 19)
        {
            DrawButton(hwndAboutButton, TEXT("����"), 2,
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
            if (MessageBox(hwndKeyboard, TEXT("���ܳ� LCD �������\n���ߣ����Ŵ�ѧ���ܳ���\n\n�Ƿ�ǰ�����ʹ��˵��ҳ�棿"), TEXT("����"), MB_YESNO) == IDYES)
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
 * @brief ���̰��°�ť�¼�
 *
 * @param PressKey  ���µļ��̰���ASCII��
 * @param isKeyDown �����Ƿ�Ϊ����״̬
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