#pragma once

// ������غ궨��
// ���ļ�������ʱ�����޸�KEYBOARD_TYPE�������޸Ķ�Ӧ����
#define KEYBOARD_4KEY 0            // 4������
#define KEYBOARD_3X3 1             // 3x3�������
#define KEYBOARD_4X4 2             // 4x4�������
#define KEYBOARD_TYPE KEYBOARD_3X3 // Ĭ��ʹ��3x3�������

// ������غ�������
uint8 keyboard_check();
int switch_state_get(int switch_id);
