#pragma once
#include <Windows.h>

// ���������غ�������
int GetMouseHoverButton(HWND hwndMain, LONG x, LONG y);
void MouseMoveButtonAction(int HoverButton, int LastHoverButton);
void MouseClickButtonAction(int HoverButton, int isClickDown);
void KeyboardButtonAction(int PressKey, int isKeyDown);

// ����������ȫ�ֱ���
extern int ActiveButton;
extern BOOL SwitchButtonState[2];
