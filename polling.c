#include "main.h"

//幻灯片
/*
    #输入：延时时间，单位秒
    #输出：幻灯片播放
    #返回：成功返回0，失败返回-1
*/
int Polling(int delay)
{
    char* bmpname[5] = {"./picture/1.bmp", "./picture/2.bmp", "./picture/3.bmp", "./picture/4.bmp", "./picture/5.bmp"};
    int bmpnum = 0;
    int choice;
    show_1152000bmp(bmpname[0], p_lcd);
    do{
        //换图
        int nTouch = project_touch(&posx, &posy);
        if((nTouch == 0 && posx > 400) || nTouch == 2)//点击到了右边或左划
            choice = 1;
        
        else if((nTouch == 0 && posx < 400) || nTouch == 1)//点击到了左边或右划
            choice = 2;

        else if(nTouch == 4)//上划
            choice = 0;
        
        switch(choice)
        {
            case 0:
                printf("Exit!\n");
                break;
            case 1:
                if(bmpnum == 4){
                    printf("This is the last picture!\n");
                    break;
                }
                bmpnum += 1;
                show_1152000bmp(bmpname[bmpnum], p_lcd);
                break;
            case 2:
                if(bmpnum == 0){
                    printf("This is the first picture!\n");
                    break;
                }
                bmpnum -= 1;
                show_1152000bmp(bmpname[bmpnum], p_lcd);
                break;
            default:
                printf("Error!\n");
                break;
        }
    //end while
    }while(choice != 0);
    
    return 0;
}
