#pragma once

// ���������غ궨��
#define PIXEL_SIZE 16            // ���浥�����س���
#define WINDOW_CAPTION_HEIGHT 30 // ���ڱ������߶�
#define BORDER_WIDTH 4           // ��߿��

#define DEF_GRAY_SURFACE_COLOR RGB(230, 230, 230) // Ĭ�ϻ�ɫ���������ɫ
#define OPE_GRAY_SURFACE_COLOR RGB(210, 210, 210) // ������ɫ���������ɫ
#define GRAY_SHADOW_COLOR RGB(160, 160, 160)      // ��ɫ������Ӱ��ɫ
#define GRAY_TEXT_COLOR RGB(160, 160, 160)        // ��ɫ����������ɫ
#define RED_SURFACE_COLOR RGB(217, 75, 75)        // ��ɫ���������ɫ
#define RED_SHADOW_COLOR RGB(188, 42, 42)         // ��ɫ������Ӱ��ɫ
#define RED_TEXT_COLOR RGB(255, 255, 255)         // ��ɫ����������ɫ

// ���������غ�������
HWND CreateUIWindow(HWND hwndParent, int x, int y, int width, int height, int isParentMain);
void InitLCDUI(HWND hwndLCDUI);
void InitLCDScreen(HWND hwndLCDScreen);
void InitKeyboard(HWND hwndKeyboard);
void InitSwitchUI(HWND hwndSwitchUI);
void DrawButton(HWND hwndButton, WCHAR *szText, float sizeText, BOOL isInit, BOOL isHover, BOOL isPress);
void DrawSwitchButton(HWND hwndSwitchButton, BOOL isInit, BOOL isOn, BOOL isHover);
