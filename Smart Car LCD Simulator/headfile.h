#pragma once

// 库相关类型定义
typedef unsigned char uint8;       //  8 bits
typedef unsigned short int uint16; // 16 bits
typedef unsigned long int uint32;  // 32 bits
typedef unsigned long long uint64; // 64 bits
typedef char int8;                 //  8 bits
typedef short int int16;           // 16 bits
typedef long int int32;            // 32 bits
typedef long long int64;           // 64 bits
typedef volatile int8 vint8;       //  8 bits
typedef volatile int16 vint16;     // 16 bits
typedef volatile int32 vint32;     // 32 bits
typedef volatile int64 vint64;     // 64 bits
typedef volatile uint8 vuint8;     //  8 bits
typedef volatile uint16 vuint16;   // 16 bits
typedef volatile uint32 vuint32;   // 32 bits
typedef volatile uint64 vuint64;   // 64 bits

// 库相关宏定义
#define NULL ((void *)0)

#define TFT_X_MAX 160
#define TFT_Y_MAX 128
#define POINT_SIZE 3

#define RED 0xF800    //红色
#define BLUE 0x001F   //蓝色
#define YELLOW 0xFFE0 //黄色
#define GREEN 0x07E0  //绿色
#define WHITE 0xFFFF  //白色
#define BLACK 0x0000  //黑色
#define GRAY 0X8430   //灰色
#define BROWN 0XBC40  //棕色
#define PURPLE 0XF81F //紫色
#define PINK 0XFE19   //粉色

#define TFT_PENCOLOR RED
#define TFT_BGCOLOR WHITE

// 库相关函数声明
void lcd_init(void);
void lcd_clear(int color);
void lcd_drawpoint(uint16 x, uint16 y, uint16 color);
void lcd_showchar(uint16 x, uint16 y, const int8 dat);
void lcd_showstr(uint16 x, uint16 y, const int8 dat[]);
void lcd_showint8(uint16 x, uint16 y, int8 dat);
void lcd_showuint8(uint16 x, uint16 y, uint8 dat);
void lcd_showint16(uint16 x, uint16 y, int16 dat);
void lcd_showuint16(uint16 x, uint16 y, uint16 dat);
void lcd_showint32(uint16 x, uint16 y, int32 dat, uint8 num);
void lcd_showfloat(uint16 x, uint16 y, double dat, uint8 num, uint8 pointnum);
void lcd_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color);