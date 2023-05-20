#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#include <linux/input.h>


int Game_Change();
int Dis_zi(int x, int y, int flag);
int Game_Over();
int clear();





#endif