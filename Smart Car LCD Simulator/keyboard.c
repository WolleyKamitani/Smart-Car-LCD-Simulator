#include "interactive.h"
#include "headfile.h"
#include "keyboard.h"

/**
 * @brief ���ص�ǰ�����µİ���ֵ
 *
 * @return uint8 ���� keyboard.h �еļ������ý��з��أ��ް�������ʱ���� 0��Ĭ��Ϊ 3x3 �������
 *               ������Ϊ 3x3 �������ʱ����Ӧ���� 1~D �������� 1~9
 *               ������Ϊ�ļ�����ʱ����Ӧ���� 1~4 �������� 1~4
 *               ������Ϊ 4x4 �������ʱ����Ӧ���� 1~V �������� 1~16
 *               ������ֲʱ���˺�����Ҫ���ݼ���ԭ�������޸�
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
 * @brief �����̰���
 *
 * @return uint8 �հ���ȥʱ����ʵ�ʽ�ֵ��δ���»��²���ʱ���� 0
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
 * @brief ���ز��뿪��״̬
 *        ������ֲʱ���˺�����Ҫ���ݲ��뿪��ԭ�������޸�
 *
 * @param switch_id ���뿪�ر�ţ�ֻ��Ϊ 1 �� 2
 * @return int      ���뿪��״̬��0 Ϊ�رգ�1 Ϊ��, -1 Ϊ����
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