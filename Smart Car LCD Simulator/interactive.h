#pragma once
#include <Windows.h>

// 软件交互相关函数声明
int GetMouseHoverButton(HWND hwndMain, LONG x, LONG y);
void MouseMoveButtonAction(int HoverButton, int LastHoverButton);
void MouseClickButtonAction(int HoverButton, int isClickDown);
void KeyboardButtonAction(int PressKey, int isKeyDown);

// 软件交互相关全局变量
extern int ActiveButton;
extern BOOL SwitchButtonState[2];
