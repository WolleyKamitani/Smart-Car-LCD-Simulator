#include "screen.h"

char message[20];


void clear()
{
    lcd_clear(0xFFFF);
}


void showstr(unsigned short x, unsigned short y,const char dat[])
{
    lcd_showstr(x,y,dat);
}

struct item* GetPageStart(struct item* cur_item)
{
    int i=0;
    struct item* temp = cur_item;
    struct item* pageitem = cur_item;
    if (cur_item->bbindex == NULL)
        return cur_item;
        while (temp->bbindex != NULL)
            temp = temp->bbindex;
        pageitem = temp;
        while (temp != cur_item)
        {
            i++;
            if (i >= PAGE_ITEM_NUM)
            {
                i = 0;
                pageitem = temp;
            }
            temp = temp->lbindex;
        }

    return pageitem;
}
//================================================================//
//  @brief  :		显示提示信息
//  @param  :		msg为消息字符串
//  @return :		无
//  @note   :		长度超过17个字符则不显示
//================================================================//
void show_message(const char* msg)
{

    if (strlen(msg) == 0)
    {
        message[0] = '\0';
        return;
    }
    if (strlen(msg) >= 18)
    {
        strcpy(message + 1, "message too long");
    }
    else
    {
        strcpy(message + 1, msg);
        message[0] = '!';
        message[strlen(message)] = '!';
        message[strlen(message)+1] = '\0';
    }
    

    message[19] = '\0';

}

//================================================================//
//  @brief  :		显示菜单项
//  @param  :		line为第几行，menuitem为菜单项指针，cur_item为控制焦点菜单项指针
//  @return :		无
//  @note   :		每行字符串长度为WIDTH/8，即行像素数量除以8，当字符串太长时会报错。
//                  第1，2个字符用于显示控制焦点提示，后9个字符显示菜单项名称，余下字
//                  符显示数值。浮点数显示小数点后两位。默认占位9个字符。
//================================================================//
void ShowItem(int line, struct item* show_item, struct item* cur_item)
{
    char tempstr[40]={'\0'};
    switch (show_item->menu_type)
    {
    case TYPE_I:
        sprintf(tempstr,"  %-9s%9d",show_item->name,*(show_item->ivalue));
        break;
    case TYPE_UI:
        sprintf(tempstr,"  %-9s%9d",show_item->name,*(show_item->uivalue));
        break;

    case TYPE_F:
        sprintf(tempstr,"  %-9s%9.2f",show_item->name,*(show_item->fvalue));
        break;
    case TYPE_A:
        sprintf(tempstr,"  %-9s",show_item->name);
        break;
    default:
        return;
    }
    if (show_item == cur_item)
    {
        if(show_item->selected==0)
            tempstr[0] = '>';
        else
            tempstr[0] = '*';

    }
    if (strlen(tempstr) > WIDTH/8)
    {
        sprintf(tempstr + 2, "Error:str too long");
    }
    showstr(0,line, tempstr);
}
void show_title(struct page* item)
{
    int length = 0;
    char buffer[21];
    buffer[20] = '\0';


    length = strlen(item->name);
    if (length > WIDTH / 8)
    {
        showstr(0, 1, "Error:str too long");
        
    }
    else
    {
        showstr((WIDTH / 8-length)*4,0,  item->name);

    }


}
void show_item(struct item* cur_item)
{

    int i = 0;
    struct item* temp=NULL;
    temp=cur_item;
    for(i=0;i<PAGE_ITEM_NUM/2;i++)
    {
        if(temp->bbindex!=NULL)
            temp=temp->bbindex;
        else
            break;

    }
    for (i = 1; i <= PAGE_ITEM_NUM; i++)
    {
        if (temp == NULL)
        {
            showstr(0,i,"                    ");
        }
        else
        {
            ShowItem(i,temp, cur_item);
            temp = temp->lbindex;
        }
    }
    showstr(0, 0, message);
}