#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include"bmp.h"
#include"lcd.h"

extern int game[4][4];
/*
	功能：在屏幕上显示一个bmp图片
	参数：
		pathname :您想显示的图片的路径名
		x,y ：您想把这个图片显示在屏幕的哪个位置
*/
void showBmp(const char *pathname,int x,int y)
{
	int fd = open(pathname,O_RDWR);
	if(fd == -1)
	{
		printf("open %s failed\n",pathname);
		return ;
	}
	
	//获取图片的宽度
	lseek(fd,0x12,SEEK_SET);//定位光标到“宽度”位置
	int w;
	read(fd,&w,4);//读取宽度信息，保存在变量w中
	printf("w=%d\n",w);
	
	lseek(fd,0x16,SEEK_SET);
	int h;
	read(fd,&h,4);
	printf("h=%d\n",h);
	
	//读取像素数组
	lseek(fd,54,SEEK_SET);//跳过文件头部分，定位到像素数组位置	
	unsigned char buf[w*h*3];
	read(fd,buf,w*h*3);
	
	//开始显示
	int i,j;
	int color;
	int n = 0;
	unsigned char r,g,b;
	for(i=0;i<h;i++)//图片有h行
	{
		for(j=0;j<w;j++)//每行有w列
		{
			//color = 图片对应位置的颜色
			r = buf[2 + 3*n];
			g = buf[1 + 3*n];
			b = buf[0 + 3*n];
			color = r<<16 | g << 8 | b;
			displayPoint(h-1-i+x,j+y,color);
			n++;
		}
	}
	
	close(fd);
}

/*
	显示游戏界面
*/
void showGameBmp()
{
	int i,j;
	char pathname[100] = "";
	for(i=0;i<4;i++)//游戏界面有四行
	{
		for(j=0;j<4;j++)//有四列
		{
			sprintf(pathname,"2048bmp/%d.bmp",game[i][j]);
			printf("%s\n",pathname);
			showBmp(pathname,30+i*105,190+j*105);
		}
	}
}