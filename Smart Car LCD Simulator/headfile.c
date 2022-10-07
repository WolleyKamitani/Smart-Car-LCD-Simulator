#include <Windows.h>
#include "interface.h"
#include "interactive.h"
#include "headfile.h"
#include "font.h"

extern HWND hwndLCDScreen;

HDC hdc;       // 画布句柄
RECT rect;     // 矩形结构体
HBRUSH hBrush; // 画刷句柄

/**
 * @brief 数字转字符串
 *
 * @param dat      数字
 * @param p        字符串指针
 * @param neg_type 负数类型，0为不显示负号，1为显示负号
 * @param radix    基数，2为十进制，8为八进制，16为十六进制
 * @return uint8   字符串长度
 */
uint8 number_conversion_ascii(uint32 dat, int8 *p, uint8 neg_type, uint8 radix)
{
    int32 neg_dat;
    uint32 pos_dat;
    uint8 temp_data = 0;
    uint8 valid_num = 0;

    if (neg_type)
    {
        neg_dat = (int32)dat;
        if (neg_dat < 0)
            neg_dat = -neg_dat;
        while (1)
        {
            *p = neg_dat % radix + '0';
            neg_dat = neg_dat / radix;
            valid_num++;

            if (!neg_dat)
                break;
            p++;
        }
    }
    else
    {
        pos_dat = dat;
        while (1)
        {
            temp_data = pos_dat % radix;
            if (10 <= temp_data)
                temp_data += 'A' - 10;
            else
                temp_data += '0';

            *p = temp_data;

            pos_dat = pos_dat / radix;
            valid_num++;

            if (!pos_dat)
                break;
            p++;
        }
    }
    return valid_num;
}

/**
 * @brief 颠倒字符串
 *
 * @param d_buff 字符串指针
 * @param len    字符串长度
 */
void printf_reverse_order(int8 *d_buff, uint32 len)
{
    uint32 i;
    int8 temp_data;

    for (i = 0; i < len / 2; i++)
    {
        temp_data = d_buff[len - 1 - i];
        d_buff[len - 1 - i] = d_buff[i];
        d_buff[i] = temp_data;
    }
}

/**
 * @brief 输出格式化字符串
 *
 * @param buff    字符串指针
 * @param format  格式指针
 * @param ...     参数
 * @return uint32 字符串长度
 */
uint32 zf_sprintf(int8 *buff, const int8 *format, ...)
{
    uint32 buff_len = 0;
    va_list arg;
    va_start(arg, format);

    while (*format)
    {
        int8 ret = *format;
        if (ret == '%')
        {
            switch (*++format)
            {
            case 'a': // 十六进制p计数法输出浮点数 暂未实现
            {
            }
            break;

            case 'c': // 一个字符
            {
                int8 ch = (int8)va_arg(arg, uint32);
                *buff = ch;
                buff++;
                buff_len++;
            }
            break;

            case 'd':
            case 'i': // 有符号十进制整数
            {
                int8 vstr[33];
                int32 ival = (int32)va_arg(arg, int32);
                uint8 vlen = number_conversion_ascii((uint32)ival, vstr, 1, 10);

                if (ival < 0)
                {
                    vstr[vlen] = '-';
                    vlen++;
                }

                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff += vlen;
                buff_len += vlen;
            }
            break;

            case 'f': // 浮点数，输出小数点后六位  不能指定输出精度
            case 'F': // 浮点数，输出小数点后六位  不能指定输出精度
            {
                int8 vstr[33];
                double ival = (double)va_arg(arg, double);
                uint8 vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);

                if (ival < 0)
                {
                    vstr[vlen] = '-';
                    vlen++;
                }

                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff += vlen;
                buff_len += vlen;

                ival = ((double)ival - (int32)ival) * 1000000;

                if (ival)
                {
                    vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);
                }
                else
                {
                    vstr[0] = vstr[1] = vstr[2] = vstr[3] = vstr[4] = vstr[5] = '0';
                    vlen = 6;
                }

                while (6 > vlen)
                {
                    vstr[vlen] = '0';
                    vlen++;
                }

                vstr[vlen] = '.';
                vlen++;

                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff_len += vlen;

                break;
            }

            case 'u': // 无符号十进制整数
            {
                int8 vstr[33];
                uint32 ival = (uint32)va_arg(arg, uint32);
                uint8 vlen = number_conversion_ascii(ival, vstr, 0, 10);

                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff += vlen;
                buff_len += vlen;
            }
            break;

            case 'o': // 无符号八进制整数
            {
                int8 vstr[33];
                uint32 ival = (uint32)va_arg(arg, uint32);
                uint8 vlen = number_conversion_ascii(ival, vstr, 0, 8);

                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff += vlen;
                buff_len += vlen;
            }
            break;

            case 'x': // 无符号十六进制整数
            case 'X': // 无符号十六进制整数
            {
                int8 vstr[33];
                uint32 ival = (uint32)va_arg(arg, uint32);
                uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);

                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff += vlen;
                buff_len += vlen;
            }
            break;

            case 's': // 字符串
            {
                int8 *pc = va_arg(arg, int8 *);
                while (*pc)
                {
                    *buff = *pc;
                    buff++;
                    buff_len++;
                    pc++;
                }
            }
            break;

            case 'p': // 以16进制形式输出指针
            {
                int8 vstr[33];
                uint32 ival = (uint32)va_arg(arg, uint32);
                uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);

                printf_reverse_order(vstr, 8);
                memcpy(buff, vstr, 8);
                buff += 8;
                buff_len += 8;
            }
            break;

            case '%': // 输出字符%
            {
                *buff = '%';
                buff++;
                buff_len++;
            }
            break;

            default:
                break;
            }
        }
        else
        {
            *buff = (int8)(*format);
            buff++;
            buff_len++;
        }
        format++;
    }

    va_end(arg);

    return buff_len;
}

/**
 * @brief RGB565转换为RGB888
 *
 * @param color RGB565颜色值
 * @return int  RGB888颜色值
 */
int RGB565toRGB888(uint16 color)
{
    int r, g, b;
    r = (color >> 11) * 255 / 31;
    g = ((color & 0x07E0) >> 5) * 255 / 63;
    b = (color & 0x001F) * 255 / 31;
    return RGB(r, g, b);
}

/**
 * @brief 液晶初始化
 *
 */
void lcd_init()
{
    lcd_clear(WHITE); // 清屏
}

/**
 * @brief 液晶清屏函数
 *
 * @param color 颜色设置
 */
void lcd_clear(int color)
{
    hdc = GetDC(hwndLCDScreen);
    hBrush = CreateSolidBrush(RGB565toRGB888(color));
    SetRect(&rect,
            0, 0,
            (int)(TFT_X_MAX * POINT_SIZE), (int)(TFT_Y_MAX * POINT_SIZE));
    FillRect(hdc, &rect, hBrush);
    ReleaseDC(hwndLCDScreen, hdc);
}

/**
 * @brief 液晶画点
 *
 * @param x     坐标x方向的起点
 * @param y     坐标y方向的起点
 * @param color 需要显示的颜色
 */
void lcd_drawpoint(uint16 x, uint16 y, uint16 color)
{
    hdc = GetDC(hwndLCDScreen);
    hBrush = CreateSolidBrush(RGB565toRGB888(color));
    SetRect(&rect,
            (int)(x * POINT_SIZE), (int)(y * POINT_SIZE),
            (int)((x + 1) * POINT_SIZE), (int)((y + 1) * POINT_SIZE));
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    ReleaseDC(hwndLCDScreen, hdc);
}

/**
 * @brief 液晶显示字符
 *
 * @param x   坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
 * @param y   坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
 * @param dat 需要显示的字符
 */
void lcd_showchar(uint16 x, uint16 y, const int8 dat)
{
    uint8 temp;

    for (int i = 0; i < 16; i++)
    {
        temp = tft_ascii[dat - 32][i]; //减32因为是取模是从空格开始取得 空格在ascii中序号是32
        for (int j = 0; j < 8; j++)
        {
            if (temp & 0x01)
            {
                lcd_drawpoint(x + j, y + i, TFT_PENCOLOR);
            }
            else
            {
                lcd_drawpoint(x + j, y + i, TFT_BGCOLOR);
            }

            temp >>= 1;
        }
    }
}

/**
 * @brief 液晶显示字符串
 *
 * @param x   坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
 * @param y   坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
 * @param dat 需要显示的字符串
 */
void lcd_showstr(uint16 x, uint16 y, const int8 dat[])
{
    uint16 j = 0;

    while (dat[j] != '\0')
    {
        lcd_showchar(x + 8 * j, y * 16, dat[j]);
        j++;
    }
}

/**
 * @brief 液晶显示8位有符号
 *
 * @param x   坐标x方向的起点  参数范围 0 -（TFT_X_MAX-1）
 * @param y   坐标y方向的起点  参数范围 0 -（TFT_Y_MAX/16-1）
 * @param dat 需要显示的变量，数据类型int8
 */
void lcd_showint8(uint16 x, uint16 y, int8 dat)
{
    uint8 a[3];

    if (dat < 0)
    {
        lcd_showchar(x, y * 16, '-');
        dat = -dat;
    }
    else
        lcd_showchar(x, y * 16, ' ');

    a[0] = dat / 100;
    a[1] = dat / 10 % 10;
    a[2] = dat % 10;

    for (int i = 0; i < 3; i++)
    {
        lcd_showchar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
    }
}

/**
 * @brief 液晶显示8位无符号
 *
 * @param x   坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
 * @param y   坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
 * @param dat 需要显示的变量，数据类型uint8
 */
void lcd_showuint8(uint16 x, uint16 y, uint8 dat)
{
    uint8 a[3];

    a[0] = dat / 100;
    a[1] = dat / 10 % 10;
    a[2] = dat % 10;

    for (int i = 0; i < 3; i++)
    {
        lcd_showchar(x + (8 * i), y * 16, '0' + a[i]);
    }
}

/**
 * @brief 液晶显示16位有符号
 *
 * @param x   坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
 * @param y   坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
 * @param dat 需要显示的变量，数据类型int16
 */
void lcd_showint16(uint16 x, uint16 y, int16 dat)
{
    uint8 a[5];

    if (dat < 0)
    {
        lcd_showchar(x, y * 16, '-');
        dat = -dat;
    }
    else
        lcd_showchar(x, y * 16, ' ');

    a[0] = dat / 10000;
    a[1] = dat / 1000 % 10;
    a[2] = dat / 100 % 10;
    a[3] = dat / 10 % 10;
    a[4] = dat % 10;

    for (int i = 0; i < 5; i++)
    {
        lcd_showchar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
    }
}

/**
 * @brief 液晶显示16位无符号
 *
 * @param x   坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
 * @param y   坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
 * @param dat 需要显示的变量，数据类型uint16
 */
void lcd_showuint16(uint16 x, uint16 y, uint16 dat)
{
    uint8 a[5];

    a[0] = dat / 10000;
    a[1] = dat / 1000 % 10;
    a[2] = dat / 100 % 10;
    a[3] = dat / 10 % 10;
    a[4] = dat % 10;

    for (int i = 0; i < 5; i++)
    {
        lcd_showchar(x + (8 * i), y * 16, '0' + a[i]);
    }
}

/**
 * @brief 液晶显示32位有符号(去除整数部分无效的0)
 *
 * @param x   坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
 * @param y   坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
 * @param dat 需要显示的变量，数据类型int32
 * @param num 需要显示的位数 最高10位  不包含正负号
 */
void lcd_showint32(uint16 x, uint16 y, int32 dat, uint8 num)
{
    int8 buff[34];
    uint8 length;

    if (10 < num)
    {
        num = 10;
    }

    num++;

    if (0 > dat)
    {
        length = (uint8)zf_sprintf(&buff[0], "%d", dat); //负数
    }
    else
    {
        buff[0] = ' ';
        length = (uint8)zf_sprintf(&buff[1], "%d", dat);
        length++;
    }

    while (length < num)
    {
        buff[length] = ' ';
        length++;
    }

    buff[num] = '\0';

    lcd_showstr(x, y, buff); //显示数字
}

/**
 * @brief 液晶显示浮点数(去除整数部分无效的0)
 *
 * @param x        坐标x方向的起点 参数范围 0 -（TFT_X_MAX-1）
 * @param y        坐标y方向的起点 参数范围 0 -（TFT_Y_MAX/16-1）
 * @param dat      需要显示的变量，数据类型float或double
 * @param num      整数位显示长度   最高10位
 * @param pointnum 小数位显示长度   最高6位
 */
void lcd_showfloat(uint16 x, uint16 y, double dat, uint8 num, uint8 pointnum)
{
    uint8 length;
    int8 buff[34];
    int8 start, end, point;

    if (6 < pointnum)
    {
        pointnum = 6;
    }

    if (10 < num)
    {
        num = 10;
    }

    if (0 > dat)
    {
        length = (uint8)zf_sprintf(&buff[0], "%f", dat); //负数
    }
    else
    {
        length = (uint8)zf_sprintf(&buff[1], "%f", dat);
        length++;
    }

    point = length - 7;         //计算小数点位置
    start = point - num - 1;    //计算起始位
    end = point + pointnum + 1; //计算结束位

    while (0 > start) //整数位不够  末尾应该填充空格
    {
        buff[end] = ' ';
        end++;
        start++;
    }

    if (0 > dat)
    {
        buff[start] = '-';
    }
    else
    {
        buff[start] = ' ';
    }

    buff[end] = '\0';

    lcd_showstr(x, y, buff); //显示数字
}

/**
 * @brief 汉字显示
 *
 * @param x      横坐标 0-（TFT_X_MAX-1）
 * @param y      纵坐标 0-（TFT_Y_MAX-1）
 * @param size   取模的时候设置的汉字字体大小，也就是一个汉字占用的点阵长宽为多少个点，取模的时候需要长宽是一样的。
 * @param p      需要显示的汉字数组
 * @param number 需要显示多少位
 * @param color  显示颜色
 */
void lcd_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color)
{
    int i, j, k;
    uint8 temp, temp1, temp2;
    const uint8 *p_data;

    temp2 = size / 8;

    for (i = 0; i < size; i++)
    {
        temp1 = number;
        p_data = p + i * temp2;

        while (temp1--)
        {
            for (k = 0; k < temp2; k++)
            {
                for (j = 8; j > 0; j--)
                {
                    temp = (*p_data >> (j - 1)) & 0x01;
                    if (temp)
                    {
                        lcd_drawpoint(x - j + (k + 1) * 8 + (number - temp1 - 1) * size, y + i - 1, color);
                    }
                    else
                    {
                        lcd_drawpoint(x - j + (k + 1) * 8 + (number - temp1 - 1) * size, y + i - 1, TFT_BGCOLOR);
                    }
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2 * size;
        }
    }
}
