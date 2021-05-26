#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

extern int game[4][4];//声明这个数组是“外部的”-》是在其他.c文件中定义

int up()
{
	int i,j,k;
	//上下划动是垂直方向的，是以列为单位的
	
	for(i=0;i<4;i++)//i代表的是列坐标，有4列
	{
		for(j=0;j<3;j++)
		{
			//比较game[j][i] 与 下面的元素 game[k][i]
			for(k=j+1;k<4;k++)
			{
				if(game[k][i] != 0)
				{
					if(game[j][i] == 0)
					{
						game[j][i] = game[k][i];
						game[k][i] = 0;
					}
					else
					{
						if(game[j][i] == game[k][i])
						{
							game[j][i] = 2*game[j][i];
							game[k][i] = 0;
							break;
						}
						else
						{
							break;
						}
					}
					
				}
			}
			
		}
	}
	
}

int down()
{
	int i,j,k;
	//上下划动是垂直方向的，是以列为单位的
	
	for(i=0;i<4;i++)//i代表的是列坐标，有4列
	{
		for(j=3;j>0;j--)
		{
			//比较game[j][i] 与 上面的元素 game[k][i]
			for(k=j-1;k>=0;k--)
			{
				if(game[k][i] != 0)
				{
					if(game[j][i] == 0)
					{
						game[j][i] = game[k][i];
						game[k][i] = 0;
					}
					else
					{
						if(game[j][i] == game[k][i])
						{
							game[j][i] = 2*game[j][i];
							game[k][i] = 0;
							break;
						}
						else
						{
							break;
						}
					}
					
				}
			}
			
		}
	}
	
}

int left()
{
	int i,j,k;
	for(i=0;i<4;i++)//i代表行坐标，有四行（左划是以行为单位进行处理的）
	{
		for(j=0;j<3;j++)//依次拿这一行左边的元素game[i][j] 进行处理 
		{
			//拿game[i][j] 与它右边的元素game[i][k]进行比较
			for(k=j+1;k<4;k++)
			{
				//if(game[i][k] == 0)//右边为0，意味着不可能发生有效移动
				if(game[i][k] != 0)//所以只处理不为0的情况
				{
					if(game[i][j] == 0)//左边为0，进行位移
					{
						game[i][j] = game[i][k];
						game[i][k] = 0;
					}
					else
					{
						if(game[i][j] == game[i][k])//相等，进行合并
						{
							game[i][j] = game[i][j] + game[i][k];
							game[i][k] = 0;
							break;//解决连续合并的bug
						}
						else
						{
							break;
						}
					}
					
				}
			}
		}
	}
}

int right()
{
	int i,j,k;
	for(i=0;i<4;i++)//有四行（右划是以行为单位进行处理的）
	{
		for(j=3;j>0;j--)//依次拿这一行右边的元素game[i][j] 进行处理 
		{
			//拿game[i][j] 与它左边的元素game[i][k]进行比较
			for(k=j-1;k>=0;k--)
			{
				//if(game[i][k] == 0)//左边为0，意味着不可能发生有效移动
				if(game[i][k] != 0)//所以只处理不为0的情况
				{
					if(game[i][j] == 0)//右边为0，进行位移
					{
						game[i][j] = game[i][k];
						game[i][k] = 0;
					}
					else
					{
						if(game[i][j] == game[i][k])//相等，进行合并
						{
							game[i][j] = game[i][j] + game[i][k];
							game[i][k] = 0;
							break;//解决连续合并的bug
						}
						else
						{
							break;
						}
					}
					
				}
			}
		}
	}
}

/*
	功能：获取屏幕的划动方向
	返回值：
		上划返回1
		下划返回2
		左划返回3
		右划返回4
		失败返回-1
*/
int getTouch()
{
	int x1=-1,y1=-1;//保存起点坐标
	int x2,y2;//保存终点坐标
	int fd = open("/dev/input/event0",O_RDWR);
	if(fd == -1)
	{
		printf("open event0 failed\n");
		return -1;
	}
	
	struct input_event ev;
	while(1)
	{
		read(fd,&ev,sizeof(ev));
		//printf("ev.type=0x%x,ev.code=0x%x,ev.value=%d\n"
		//,ev.type,ev.code,ev.value);
		
		if(ev.type == 0x03 && ev.code == 0x00)
		{
			if(x1 == -1)
				x1 = ev.value;//获取x轴坐标
			
			x2 = ev.value;
		}
		
		if(ev.type == 0x03 && ev.code == 0x01)
		{
			if(y1 == -1)
				y1 = ev.value;//获取y轴坐标
			
			y2 = ev.value;
		}
		
		if(ev.type==0x1&&ev.code==0x14a&&ev.value==0)
		{
			break;
		}
	}
	
	close(fd);
	if(abs(x2-x1) > abs(y2-y1))//左或者右
	{
		if(x2>x1)//右
		{
			return 4;
		}
		else//左
		{
			return 3;
		}
	}
	else//上或者下
	{
		if(y2>y1)//下
		{
			return 2;
		}
		else//上
		{
			return 1;
		}
	}
}