#include "main.h"

int Send_Cmd(char *cmd)//写入管道文件
{
	write(fd_fifo,cmd,strlen(cmd));
	
	return 0;
}


//视频播放
/*
    #输入：视频文件路径
    #输出：视频播放
    #返回：成功返回0，失败返回-1
*/
int VideoPlay_old(char videopath[], int width, int height)
{   
    show_1152000bmp("./picture/video_play.bmp", p_lcd);
    //打开视频文件
    system("killall -9 mplayer");
    char Video[255];
    sprintf(Video, "mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x %d -y %d %s &", width, height, videopath);
    system(Video);
    //操作视频
    int posx, posy;
    int choice;
    int VideoMode = 1;
    do{

        int nTouch = project_touch(&posx, &posy);
        
        if(nTouch == 0) choice = 1; //暂停或继续视频
        else if(nTouch == 4) choice = 2; //退出视频

        switch(choice)
        {
            case 0:
                system("killall -SIGKILL mplayer");
                break;
            case 1:
                if(VideoMode == 1)
                {
                    system("killall -SIGSTOP mplayer");
                    VideoMode = 0;
                }
                else
                {
                    system("killall -SIGCONT mplayer");
                    VideoMode = 1;
                }
                break;
            default:
                printf("Error!\n");
                break;
        }
        
    }while(choice != 0);

    return 0;
}

int VideoPlay(char videopath[])
{   
    //show_1152000bmp("./picture/video_play.bmp", p_lcd);
    //打开视频文件
    system("killall -9 mplayer");
    char Video[255];
    sprintf(Video, "mplayer %s &", videopath);
    system(Video);
    //操作视频
    int posx, posy;
    int choice;
    int VideoMode = 1;
    do{

        int nTouch = project_touch(&posx, &posy);
        
        if(nTouch == 0) choice = 1; //暂停或继续视频
        else if(nTouch == 4) choice = 0; //退出视频

        switch(choice)
        {
            case 0:
                system("killall -SIGKILL mplayer");
                break;
            case 1:
                if(VideoMode == 1)
                {
                    system("killall -SIGSTOP mplayer");
                    VideoMode = 0;
                }
                else
                {
                    system("killall -SIGCONT mplayer");
                    VideoMode = 1;
                }
                break;
            default:
                printf("Error!\n");
                break;
        }
        
    }while(choice != 0);

    return 0;
}