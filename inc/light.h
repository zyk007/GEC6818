#ifndef __LIGHT_H__
#define __LIGHT_H__


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>

int Display(int color, int x, int y);
int Lcd_Init();
void Dis_wh();
void Dis_qu();
void Dis_cir();
void Dis_tri();



#endif