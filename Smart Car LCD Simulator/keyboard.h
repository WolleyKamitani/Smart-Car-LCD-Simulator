#pragma once

// 键盘相关宏定义
// 更改键盘种类时，仅修改KEYBOARD_TYPE，请勿修改对应数字
#define KEYBOARD_4KEY 0            // 4键键盘
#define KEYBOARD_3X3 1             // 3x3矩阵键盘
#define KEYBOARD_4X4 2             // 4x4矩阵键盘
#define KEYBOARD_TYPE KEYBOARD_3X3 // 默认使用3x3矩阵键盘

// 键盘相关函数声明
uint8 keyboard_check();
int switch_state_get(int switch_id);
