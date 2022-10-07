#include "interactive.h"
#include "headfile.h"
#include "keyboard.h"

/**
 * @brief 返回当前所按下的按键值
 *
 * @return uint8 根据 keyboard.h 中的键盘设置进行返回，无按键按下时返回 0，默认为 3x3 矩阵键盘
 *               当设置为 3x3 矩阵键盘时，对应按下 1~D 按键返回 1~9
 *               当设置为四键键盘时，对应按下 1~4 按键返回 1~4
 *               当设置为 4x4 矩阵键盘时，对应按下 1~V 按键返回 1~16
 *               进行移植时，此函数需要根据键盘原理自行修改
 */
uint8 keyboard_scan()
{
    switch (KEYBOARD_TYPE)
    {
    case KEYBOARD_4KEY:
        if (ActiveButton < 4)
        {
            return ActiveButton + 1;
        }
        else
        {
            return 0;
        }

    case KEYBOARD_3X3:
        if (ActiveButton < 11 && ActiveButton >= 0 && (ActiveButton + 1) % 4 != 0)
        {
            return (ActiveButton + 1) % 4 + (ActiveButton + 1) / 4 * 3;
        }
        else
        {
            return 0;
        }

    case KEYBOARD_4X4:
        return ActiveButton + 1;

    default:
        return 0;
    }
}

/**
 * @brief 检测键盘按键
 *
 * @return uint8 刚按下去时返回实际健值，未按下或按下不放时返回 0
 */
uint8 keyboard_check()
{
    uint8 last_key;
    static uint8 temp_key = 0;

    last_key = temp_key;
    temp_key = keyboard_scan();

    if (temp_key != last_key)
    {
        return temp_key;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief 返回拨码开关状态
 *        进行移植时，此函数需要根据拨码开关原理自行修改
 *
 * @param switch_id 拨码开关编号，只能为 1 或 2
 * @return int      拨码开关状态，0 为关闭，1 为打开, -1 为错误
 */
int switch_state_get(int switch_id)
{
    switch (switch_id)
    {
    case 1:
    case 2:
        return SwitchButtonState[switch_id - 1];
        break;

    default:
        return -1;
        break;
    }
}