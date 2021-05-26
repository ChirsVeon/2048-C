#include<stdio.h>
#include"lcd.h"
#include"bmp.h"
#include"touch.h"

int game[4][4] = {
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
};

void getRand()
{
	srand(time(NULL));
	int i,j;
	int count = 0;//记录空白位置的个数
	int a[16];//用来保存空白位置的行下标
	int b[16];//用来保存空白位置的列下标
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(game[i][j] == 0)
			{
				a[count] = i;
				b[count] = j;
				count ++;
			}
		}
	}
	if(count == 0)
		return ;
	
	int n = rand()%count;
	
	game[a[n]][b[n]] = (rand()%2+1)*2 ;
	
}

int gameOver()
{
	int i,j,k,l,n=0,m=0;
	for(i=0;i<4;i++)
	{
		for(j=0;j<3;j++)
		{
			if(game[i][j] == game[i][j+1] || game[j][i] == game[j+1][i] )
				n++;
			if(game[i][j] == 0)
				m++;
			
		
		}
	}
	for(k=0;k<4;k++)
	{
		for(l=0;l<4;l++)
		{
		
			if(game[k][l] == 0)
				m++;
			
		
		}
	}
		if(n==0&&m==0)
					return 1;
				else 
					return 0;
}

void startGame()
{
	showBmp("....",0,0);//显示背景图片
	int i ,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
			game[i][j] = 0;
	}
	getRand();
	getRand();
	
	showGameBmp();
}

int main()
{
	int lcdfd = initLcd();//初始化屏幕之后才能进行显示操作
	printf("********************initLcd done********************\n");
	startGame();//开始游戏
	/*
	获取划动屏幕的方向
	根据划动方向，调用对应的算法
	如果产生了有效移动，就增加两个随机数
	判断游戏是否结束，如果没有结束，返回第一步继续执行
	如果结束了，显示游戏结束页面，并且执行第六步
	*/
	printf("********************startGame done********************\n");
	while(1)
	{
		int v,n,gg = 0;
		n = getTouch();
		switch(n)
		{
			case 1:
				up();
				v = 1;
				printf("********************up********************\n");
				getRand();
				showGameBmp();
				break;
			case 2:
				down();
				v = 1;
				printf("********************down********************\n");
				getRand();
				showGameBmp();
				break;
			case 3:
				left();
				v = 1;
				printf("********************left********************\n");
				getRand();
				showGameBmp();
				break;
			case 4:
				right();
				v = 1;
				printf("********************right********************\n");
				getRand();
				showGameBmp();
				break;
		}
		gameOver();
		
		if(v = 1)//调用gameOver判断游戏是否结束
		{
			printf("********************moved********************\n");
			
			if(gameOver()==1)
			{
				showBmp("2048bmp/game_over.bmp",0,0);
				break;
			}
		}
	}
	
	uninitLcd(lcdfd);
	return 0;
}