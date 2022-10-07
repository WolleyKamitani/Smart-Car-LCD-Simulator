#include <Windows.h>
#include "interface.h"
#include "interactive.h"
#include "headfile.h"
#include "font.h"

extern HWND hwndLCDScreen;

HDC hdc;       // �������
RECT rect;     // ���νṹ��
HBRUSH hBrush; // ��ˢ���

/**
 * @brief ����ת�ַ���
 *
 * @param dat      ����
 * @param p        �ַ���ָ��
 * @param neg_type �������ͣ�0Ϊ����ʾ���ţ�1Ϊ��ʾ����
 * @param radix    ������2Ϊʮ���ƣ�8Ϊ�˽��ƣ�16Ϊʮ������
 * @return uint8   �ַ�������
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
 * @brief �ߵ��ַ���
 *
 * @param d_buff �ַ���ָ��
 * @param len    �ַ�������
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
 * @brief �����ʽ���ַ���
 *
 * @param buff    �ַ���ָ��
 * @param format  ��ʽָ��
 * @param ...     ����
 * @return uint32 �ַ�������
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
            case 'a': // ʮ������p��������������� ��δʵ��
            {
            }
            break;

            case 'c': // һ���ַ�
            {
                int8 ch = (int8)va_arg(arg, uint32);
                *buff = ch;
                buff++;
                buff_len++;
            }
            break;

            case 'd':
            case 'i': // �з���ʮ��������
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

            case 'f': // �����������С�������λ  ����ָ���������
            case 'F': // �����������С�������λ  ����ָ���������
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

            case 'u': // �޷���ʮ��������
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

            case 'o': // �޷��Ű˽�������
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

            case 'x': // �޷���ʮ����������
            case 'X': // �޷���ʮ����������
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

            case 's': // �ַ���
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

            case 'p': // ��16������ʽ���ָ��
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

            case '%': // ����ַ�%
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
 * @brief RGB565ת��ΪRGB888
 *
 * @param color RGB565��ɫֵ
 * @return int  RGB888��ɫֵ
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
 * @brief Һ����ʼ��
 *
 */
void lcd_init()
{
    lcd_clear(WHITE); // ����
}

/**
 * @brief Һ����������
 *
 * @param color ��ɫ����
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
 * @brief Һ������
 *
 * @param x     ����x��������
 * @param y     ����y��������
 * @param color ��Ҫ��ʾ����ɫ
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
 * @brief Һ����ʾ�ַ�
 *
 * @param x   ����x�������� ������Χ 0 -��TFT_X_MAX-1��
 * @param y   ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
 * @param dat ��Ҫ��ʾ���ַ�
 */
void lcd_showchar(uint16 x, uint16 y, const int8 dat)
{
    uint8 temp;

    for (int i = 0; i < 16; i++)
    {
        temp = tft_ascii[dat - 32][i]; //��32��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո���ascii�������32
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
 * @brief Һ����ʾ�ַ���
 *
 * @param x   ����x�������� ������Χ 0 -��TFT_X_MAX-1��
 * @param y   ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
 * @param dat ��Ҫ��ʾ���ַ���
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
 * @brief Һ����ʾ8λ�з���
 *
 * @param x   ����x��������  ������Χ 0 -��TFT_X_MAX-1��
 * @param y   ����y��������  ������Χ 0 -��TFT_Y_MAX/16-1��
 * @param dat ��Ҫ��ʾ�ı�������������int8
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
 * @brief Һ����ʾ8λ�޷���
 *
 * @param x   ����x�������� ������Χ 0 -��TFT_X_MAX-1��
 * @param y   ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
 * @param dat ��Ҫ��ʾ�ı�������������uint8
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
 * @brief Һ����ʾ16λ�з���
 *
 * @param x   ����x�������� ������Χ 0 -��TFT_X_MAX-1��
 * @param y   ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
 * @param dat ��Ҫ��ʾ�ı�������������int16
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
 * @brief Һ����ʾ16λ�޷���
 *
 * @param x   ����x�������� ������Χ 0 -��TFT_X_MAX-1��
 * @param y   ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
 * @param dat ��Ҫ��ʾ�ı�������������uint16
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
 * @brief Һ����ʾ32λ�з���(ȥ������������Ч��0)
 *
 * @param x   ����x�������� ������Χ 0 -��TFT_X_MAX-1��
 * @param y   ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
 * @param dat ��Ҫ��ʾ�ı�������������int32
 * @param num ��Ҫ��ʾ��λ�� ���10λ  ������������
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
        length = (uint8)zf_sprintf(&buff[0], "%d", dat); //����
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

    lcd_showstr(x, y, buff); //��ʾ����
}

/**
 * @brief Һ����ʾ������(ȥ������������Ч��0)
 *
 * @param x        ����x�������� ������Χ 0 -��TFT_X_MAX-1��
 * @param y        ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
 * @param dat      ��Ҫ��ʾ�ı�������������float��double
 * @param num      ����λ��ʾ����   ���10λ
 * @param pointnum С��λ��ʾ����   ���6λ
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
        length = (uint8)zf_sprintf(&buff[0], "%f", dat); //����
    }
    else
    {
        length = (uint8)zf_sprintf(&buff[1], "%f", dat);
        length++;
    }

    point = length - 7;         //����С����λ��
    start = point - num - 1;    //������ʼλ
    end = point + pointnum + 1; //�������λ

    while (0 > start) //����λ����  ĩβӦ�����ո�
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

    lcd_showstr(x, y, buff); //��ʾ����
}

/**
 * @brief ������ʾ
 *
 * @param x      ������ 0-��TFT_X_MAX-1��
 * @param y      ������ 0-��TFT_Y_MAX-1��
 * @param size   ȡģ��ʱ�����õĺ��������С��Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��㣬ȡģ��ʱ����Ҫ������һ���ġ�
 * @param p      ��Ҫ��ʾ�ĺ�������
 * @param number ��Ҫ��ʾ����λ
 * @param color  ��ʾ��ɫ
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
