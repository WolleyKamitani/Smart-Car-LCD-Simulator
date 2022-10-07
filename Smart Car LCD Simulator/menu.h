#ifndef MENU_H_
#define MENU_H_



#include <stdio.h>
#include <string.h>
#include "screen.h"
#include "globalvar.h"
#include "action.h"

#define PAGE_ITEM_NUM 7		//每页菜单项数

#define ITEMMAX 20 //菜单项数量
#define PAGEMAX 20 //菜单项数量

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_PUSH 3

#define PAGE_ITEM_NAME "[PAGES]"
#define STEP_ITEM_NAME "[STEP]"

typedef enum //菜单类型
{
	TYPE_I,
	TYPE_UI,
	TYPE_F,
	TYPE_A,
	TYPE_N,

}ITEM_TYPE__enum;

struct page
{
	char name[15];  // 名称
	struct item* cindex;		//子节点id

};

struct item
{
	char name[8];  // 名称
	int selected;
	ITEM_TYPE__enum menu_type;

	struct page* findex;	//父节点id
	struct item *lbindex;	//弟节点id
	struct item *bbindex;	//兄节点id


	void(*action)();     //函数指针
	int *ivalue;     // 整形值指针
	unsigned int* uivalue;
	float *fvalue;		//浮点型值指针

	
};
void Menu(void);
//int CreatMenuItem(const char *fname,const char  *name);
int creat_item(const char* page_name, const char* item_name, void* p, ITEM_TYPE__enum menu_type);
void Menu_init();
struct item* search_item(const char *item_name,struct page* start);

#endif // !MENU_H_