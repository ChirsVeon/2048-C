#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int *p;//指向映射内存的首地址

/*
	该函数的功能：初始化lcd屏幕（在使用lcd之前调用该函数一次）
	返回值：
		成功返回文件描述符
		失败返回-1
*/
int initLcd()
{
	int fd = open("/dev/fb0",O_RDWR);
	if(fd == -1)
	{
		printf("open led error\n");
		return -1;
	}
	
	p = mmap(NULL,480*800*4,PROT_READ | PROT_WRITE,
		MAP_SHARED,fd,0);
	if(p == NULL)
	{
		printf("mmap failed\n");
		return -1;
	}
	
	return fd;
}

/*
	该函数的功能：解除lcd屏幕映射及关闭lcd文件（在使用完lcd之后调用该函数一次）
	
	参数：
		lcd屏幕文件的文件描述符，就是initLcd的返回值
*/
void uninitLcd(int fd)
{
	munmap(p,480*800*4);
	close(fd);
}

/*
	该函数的功能是：在屏幕上显示一个像素点
	该像素点的位置为 第i行 第j列，由前两个参数决定
	该像素点的颜色为 color
*/
void displayPoint(int i,int j,int color)
{	
	if(i>=0 && i<480 && j>=0 && j<800)
		*(p+800*i+j) = color;
}