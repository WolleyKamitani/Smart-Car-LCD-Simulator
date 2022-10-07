#ifndef __PINGMU_H
#define __PINGMU_H
#include "Menu.h"
#include "headfile.h"

#define WIDTH 160  //液晶X方宽度
#define HEIGHT 128  //液晶Y方宽度

void show_item(struct item* cur_item);
void show_title(struct page* item);

void show_message(const char* msg);
void clear();
#endif  