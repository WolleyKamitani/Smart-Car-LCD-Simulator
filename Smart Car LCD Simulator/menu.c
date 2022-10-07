#include "Menu.h"


struct item items[ITEMMAX];//菜单项数组
struct page pages[PAGEMAX];//菜单页数组
int page_index=0;//菜单页索引
struct item *cur_item =NULL;//当前菜单项
float step = 1;//加减值步长

//================================================================//
//  @brief  :       向前翻页
//  @param  :       无
//  @return :
//  @note   :
//================================================================//
void page_up()
{
	struct page* ftemp=NULL ;
	if (cur_item->findex == NULL)
		return;
	ftemp = cur_item->findex;

	page_index=(int)(ftemp-&pages[0]);
	if (ftemp>&pages[0]&&(strlen((ftemp - 1)->name) > 0))
	{
	    ftemp->cindex->selected=0;
		ftemp = ftemp - 1;
		cur_item = ftemp->cindex;
		cur_item->selected=1;

	}

}
//================================================================//
//  @brief  :       向后翻页
//  @param  :       无
//  @return :
//  @note   :
//================================================================//
void page_down()
{
	struct page* ftemp = NULL;
	if (cur_item->findex == NULL)
		return;
	ftemp = cur_item->findex;

	page_index=(int)(ftemp-&pages[0]);
	if (ftemp< &pages[PAGEMAX]&& (strlen((ftemp + 1)->name)>0))
	{
        ftemp->cindex->selected=0;
		ftemp = ftemp + 1;
		cur_item = ftemp->cindex;
        cur_item->selected=1;
	}

}

//================================================================//
//  @brief  :		上移菜单节点
//  @param  :		无
//  @return :		
//  @note   :		
//================================================================//
void cur_item_up()
{
	if (cur_item->bbindex != NULL)
		cur_item = cur_item->bbindex;
}
//================================================================//
//  @brief  :		下移菜单节点
//  @param  :		无
//  @return :		
//  @note   :		
//================================================================//
void cur_item_down()
{
	if (cur_item->lbindex != NULL)
		cur_item = cur_item->lbindex;
}

//================================================================//
//  @brief  :		当前菜单值加
//  @param  :		无
//  @return :		
//  @note   :		每次执行时，当前节点会加一次step，注意step小于1时，整型值无法加
//================================================================//
void cur_item_add()
{
    switch(cur_item->menu_type)
    {
    case TYPE_F:if (cur_item->fvalue != NULL)
                    *cur_item->fvalue += step;
                break;
    case TYPE_UI:if (cur_item->uivalue != NULL)
                    *cur_item->uivalue += step;
                break;
    case TYPE_I:if (cur_item->ivalue != NULL)
                    *cur_item->ivalue += step;
                break;
    default: return;
    }






}
//================================================================//
//  @brief  :		当前菜单值减
//  @param  :		无
//  @return :		
//  @note   :		每次执行时，当前节点会加一次step，注意step小于1时，整型值无法加
//================================================================//
void cur_item_sub()
{


    switch(cur_item->menu_type)
    {
    case TYPE_I:if (cur_item->ivalue != NULL)
                    *cur_item->ivalue -= step;
                break;
    case TYPE_UI:if (cur_item->uivalue != NULL)
                    *cur_item->uivalue -= step;
                break;
    case TYPE_F:if (cur_item->fvalue != NULL)
                    *cur_item->fvalue -= step;
                break;
    default: return;
    }


}

//================================================================//
//  @brief  :		从菜单节点数组中寻找一个未被使用过的节点返回地址
//  @param  :		无
//  @return :		struct item型 NULL为失败
//  @note   :		
//================================================================//
struct item* Mmalloc()
{
	int index=1;
	for(index=1;index<ITEMMAX;index++)
	{
		if(items[index].name[0]=='\0')
			return &items[index];
	}
	return NULL;
}
//================================================================//
//  @brief  :		获取一个空菜单节点
//  @param  :		无
//  @return :		struct item型 NULL为失败
//  @note   :		
//================================================================//
struct item* get_new_item()
{
	struct item* newitem;
	newitem = (struct item*)Mmalloc();
	if (newitem == NULL)
		return NULL;
	newitem->name[0] = '\0';
	newitem->bbindex = NULL;
	newitem->lbindex = NULL;
	newitem->findex = NULL;
	newitem->action = NULL;
	newitem->ivalue = NULL;
	newitem->uivalue = NULL;
	newitem->fvalue = NULL;

	return newitem;
}
//================================================================//
//  @brief  :		搜索菜单页
//  @param  :		pname为节点名
//  @return :		struct page型 NULL为失败
//  @note   :		
//================================================================//
struct page* search_page(const char *pname)
{
	int i;
	for (i = 0; i < PAGEMAX; i++)
	{
		if (strcmp(pages[i].name, pname) == 0)
			return &pages[i];
	}
	return NULL;
}
//================================================================//
//  @brief  :		搜索菜单节点
//  @param  :		item_name为节点名，start为所在页指针
//  @return :		struct item型 NULL为失败
//  @note   :		
//================================================================//
struct item* search_item(const char *item_name,struct page* start)
{
	struct item* temp=NULL;

	if (strlen(item_name) == 0)
		return NULL;
	if (start == NULL)
		return NULL;

	temp = start->cindex;
	if (NULL == temp)
		return NULL;
	do
	{
		if (strcmp(temp->name, item_name) == 0)
			return temp;
		temp = temp->lbindex;
	} while (temp!=NULL);

	return NULL;
}

//================================================================//
//  @brief  :		创建菜单页
//  @param  :		page为所在页指针，item_name为菜单项名称
//  @return :		int型 0为失败，1为成功
//  @note   :		初始化时调用。
//================================================================//

int creat_page(const char* page_name)
{
	int i = 0;
	for (i = 0; i < PAGEMAX; i++)
		if (strlen(pages[i].name) == 0)
			break;
	if (i >= PAGEMAX)
		return 0;
	strcpy(pages[i].name, page_name);
	pages[i].cindex = NULL;
	creat_item(pages[i].name,PAGE_ITEM_NAME,&page_index,TYPE_I);
	creat_item(pages[i].name,STEP_ITEM_NAME,&step,TYPE_F);
	return 1;
 }
//================================================================//
//  @brief  :		创建子菜单项
//  @param  :		page为所在页指针，item_name为菜单项名称
//  @return :		int型 0为失败，1为成功
//  @note   :		初始化时调用。
//================================================================//
int creat_item(const char* page_name, const char* item_name, void* p, ITEM_TYPE__enum menu_type)
{
	struct item* temp = NULL;
	struct item* new_item = NULL;
	struct page* page;

	page = search_page(page_name);
	if (NULL == page)
		return 0;

	


	if (search_item(item_name, page) != NULL)//查找是否存在同名菜单项，如果存在则停止创建该菜单项
		return 0;
	if (*item_name == '\0')
		return 0;
	
	new_item = get_new_item();//分配空间给新菜单项，如果不存在则停止创建
	if (new_item == NULL)
		return 0;
	
	//新菜单项赋值
	strcpy(new_item->name, item_name);
	new_item->menu_type = menu_type;
	switch (menu_type)
	{
	case TYPE_I:
		new_item->ivalue = (int*)p;
		break;
	case TYPE_UI:
		new_item->uivalue = (unsigned int*)p;
		break;

	case TYPE_F:
		new_item->fvalue = (float*)p;
		break;
	case TYPE_A:
		new_item->action = (void (*)())p;
		break;
	default:
		return 0;
	}
	new_item->findex = page;

	//插入菜单链表
	if (NULL == page->cindex)
		page->cindex = new_item;
	else
	{
		temp = page->cindex;
		while (NULL != temp->lbindex)
		{
			temp = temp->lbindex;
		}
		new_item->bbindex = temp;
		temp->lbindex = new_item;
	}
	new_item->selected=0;
	return 1;
	
		



}
//================================================================//
//  @brief  :       处理向上滚动事件
//  @param  :
//  @return :
//  @note   :       当前菜单项没被选中时，菜单项向上滚动。
//                  当前菜单项被选中时，如果菜单项是[PAGES]，则向前翻页。
//                  如果菜单项是[STEP]，则加减值步长扩大十倍。
//================================================================//
void scroll_up()
{
    if(cur_item->selected==0)
    {
        cur_item_up();
    }
    else
    {
        if(strcmp(cur_item->name,PAGE_ITEM_NAME)==0)
            {
            page_up();
            return;
            }
        if(strcmp(cur_item->name,STEP_ITEM_NAME)==0)
            {
            step=step*10.0;
            return;
            }
        cur_item_add();
    }

}

//================================================================//
//  @brief  :       处理向下滚动事件
//  @param  :
//  @return :
//  @note   :       当前菜单项没被选中时，菜单项向下滚动。
//                  当前菜单项被选中时，如果菜单项是[PAGES]，则向后翻页。
//                  如果菜单项是[STEP]，则加减值步长缩小十倍。
//================================================================//

void scroll_down()
{
    if(cur_item->selected==0)
    {
        cur_item_down();
    }
    else
    {
        if(strcmp(cur_item->name,PAGE_ITEM_NAME)==0)
            {
            page_down();
            return;
            }
        if(strcmp(cur_item->name,STEP_ITEM_NAME)==0)
            {
            step=step/10.0;
            return;
            }
        cur_item_sub();
    }

}
//================================================================//
//  @brief  :       处理按键事件
//  @param  :
//  @return :
//  @note   :       按下确认按钮时，切换菜单项的selected状态。
//================================================================//

void button_push()
{
    if(cur_item->menu_type==TYPE_A)
        if (cur_item->action != NULL)  //菜单项执行
        {
            cur_item->action();
            return;
        }
    cur_item->selected=!cur_item->selected;
}
void Menu()
{
	static struct page* last_page=NULL;
	switch (Key)
	{
	    case KEY_UP: scroll_up(); break;     //菜单项上移
	    case KEY_DOWN:scroll_down(); break;   //菜单项下移
	    case KEY_PUSH:button_push();break;
	}

	Key=0;
	if (last_page != cur_item->findex)
		{
	    clear();
	    show_title(cur_item->findex);
	    show_item(cur_item);
		}
	else
	    show_item(cur_item);

	last_page = cur_item->findex;
}
void Menu_init()
{
	int i;
	for(i=0;i<ITEMMAX;i++)
	{
		items[0].name[0]='\0';
	}
	for (i = 0; i < PAGEMAX; i++)
	{
		pages[i].name[0] = '\0';
		pages[i].cindex = NULL;
	}

	creat_page("information");
	creat_item("information", "age", &age, TYPE_I);
	creat_item("information", "height", &height, TYPE_F);
	creat_item("information", "weight", &weight, TYPE_F);

	creat_page("study");
	creat_item("study", "math", &Math, TYPE_UI);
	creat_item("study", "dothing", (void*)do_something, TYPE_A);
	creat_item("study", "donothing", (void*)do_nothing, TYPE_A);

	if(pages[0].cindex!=NULL)
	cur_item = pages[0].cindex;
	
}