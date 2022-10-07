#pragma once

// 软件界面相关宏定义
#define PIXEL_SIZE 16            // 界面单个像素长度
#define WINDOW_CAPTION_HEIGHT 30 // 窗口标题栏高度
#define BORDER_WIDTH 4           // 描边宽度

#define DEF_GRAY_SURFACE_COLOR RGB(230, 230, 230) // 默认灰色界面表面颜色
#define OPE_GRAY_SURFACE_COLOR RGB(210, 210, 210) // 操作灰色界面表面颜色
#define GRAY_SHADOW_COLOR RGB(160, 160, 160)      // 灰色界面阴影颜色
#define GRAY_TEXT_COLOR RGB(160, 160, 160)        // 灰色界面文字颜色
#define RED_SURFACE_COLOR RGB(217, 75, 75)        // 红色界面表面颜色
#define RED_SHADOW_COLOR RGB(188, 42, 42)         // 红色界面阴影颜色
#define RED_TEXT_COLOR RGB(255, 255, 255)         // 红色界面文字颜色

// 软件界面相关函数声明
HWND CreateUIWindow(HWND hwndParent, int x, int y, int width, int height, int isParentMain);
void InitLCDUI(HWND hwndLCDUI);
void InitLCDScreen(HWND hwndLCDScreen);
void InitKeyboard(HWND hwndKeyboard);
void InitSwitchUI(HWND hwndSwitchUI);
void DrawButton(HWND hwndButton, WCHAR *szText, float sizeText, BOOL isInit, BOOL isHover, BOOL isPress);
void DrawSwitchButton(HWND hwndSwitchButton, BOOL isInit, BOOL isOn, BOOL isHover);
