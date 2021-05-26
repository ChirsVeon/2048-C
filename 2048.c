#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
int i, j;
int *p;
int x1,x2,yy1,yy2,bjx,bjy;
struct dian
{
	int h;
	int l;
};
struct fangkuai
{
	struct dian qishi;
	struct dian jieshu;
	int yanse;
};
void getTouch()
{
	int fd = open("/dev/input/event0", O_RDWR);
	if (fd == -1)
	{
		printf("open event0 failed\n");
		return;
	}
	bjx=0;
	bjy=0;
	struct input_event ev;
	while (1)
	{
		
		read(fd, &ev, sizeof(ev));
		printf("ev.type=0x%x,ev.code=0x%x,ev.value=%d\n"
			, ev.type, ev.code, ev.value);
		if (ev.type == 0x03 && ev.code == 0x00)
		{
			if(bjx==1&&bjy==1)
			{
				x2= ev.value;
			}
			else
			{
				x1=ev.value;
				bjx=1;
			}
		}

		if (ev.type == 0x03 && ev.code == 0x01)
		{
			if(bjx==1&&bjy==1)
			{
				yy2= ev.value;
			}
			else
			{
				yy1=ev.value;
				bjy=1;
			}
		}
		if (ev.type == 0x01 && ev.code == 0x14a && ev.value == 0)
		{
			break;
		}
	}
	printf("x1=%d,y1=%d\t x2=%d,y2=%d\n",x1,yy1,x2,yy2);
	close(fd);
}
int initLcd()
{
	int fd = open("/dev/fb0", O_RDWR);
	if (fd == -1)
	{
		printf("open led error\n");
		return ;
	}

	p = mmap(NULL, 480 * 800 * 4, PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);
	if (p == NULL)
	{
		printf("mmap failed\n");
		return -1;
	}
	return fd;
}

void uninitLcd(int fd)
{
	munmap(p, 480 * 800 * 4);
	close(fd);
}

void bianse(struct fangkuai *fk)
{
	int i, j,n=0;
	int fd;
	switch(fk->yanse)
	{
		case 0:fd = open("/cpdd/0.bmp",O_RDWR);break;
		case 1:fd = open("/cpdd/1.bmp",O_RDWR);break;
		case 2:fd = open("/cpdd/2.bmp",O_RDWR);break;
		case 3:fd = open("/cpdd/3.bmp",O_RDWR);break;
		case 4:fd = open("/cpdd/4.bmp",O_RDWR);break;
		case 5:fd = open("/cpdd/5.bmp",O_RDWR);break;
		case 6:fd = open("/cpdd/6.bmp",O_RDWR);break;
		case 7:fd = open("/cpdd/7.bmp",O_RDWR);break;
		case 8:fd = open("/cpdd/8.bmp",O_RDWR);break;
		case 9:fd = open("/cpdd/9.bmp",O_RDWR);break;
		case 10:fd = open("/cpdd/10.bmp",O_RDWR);break;
		default :fd = open("/cpdd/11.bmp",O_RDWR);break;
	}
	if(fd == -1)
	{
		printf("open  failed   \n");
		return ;
	}
	unsigned char buf[200*120*3];
	lseek(fd,0x54,SEEK_SET);
	read(fd,buf,200*120*3);
	unsigned char r,g,b;
	for (i =fk->jieshu.h;i>=fk->qishi.h;i--)
	for (j = fk->qishi.l; j <= fk->jieshu.l;j++)
	{
			b=buf[0+3*n];
			g=buf[1+3*n];
			r=buf[2+3*n];
			n++;
			*(p+i*800+j)=r<<16|g<<8|b;
	}
	close(fd);
}
int cha(int a, int b)
{
	int zhi;
	zhi = a - b;
	if (zhi < 0)
		zhi = -zhi;
	return zhi;
}

void zuohua(struct fangkuai (*tu)[4],int cd)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (tu[i][0].yanse == 0)     //0
		{
			if (tu[i][1].yanse == 0)  //00
			{
				if (tu[i][2].yanse == 0)   //000
				{
					if (tu[i][3].yanse == 0)//0000
						;
					else //0001
					{
						tu[i][0].yanse = tu[i][3].yanse;
						tu[i][3].yanse = 0;
						bianse(&tu[i][0]);
						bianse(&tu[i][3]);
					}
				}
				else//001
				{
					if (tu[i][3].yanse == 0)//0010
					{
						tu[i][0].yanse = tu[i][2].yanse;
						tu[i][2].yanse = 0;
						bianse(&tu[i][0]);
						bianse(&tu[i][2]);
					}
					else//0011
					{
						if (tu[i][3].yanse == tu[i][2].yanse)
						{
							tu[i][2].yanse++;
							tu[i][3].yanse = 0;
							tu[i][0].yanse = tu[i][2].yanse;
							tu[i][2].yanse = 0;
							bianse(&tu[i][0]);
							bianse(&tu[i][2]);
							bianse(&tu[i][3]);
						}
						else
						{
							tu[i][0].yanse = tu[i][2].yanse;
							tu[i][2].yanse = 0;
							tu[i][1].yanse = tu[i][3].yanse;
							tu[i][3].yanse = 0;
							bianse(&tu[i][0]);
							bianse(&tu[i][1]);
							bianse(&tu[i][2]);
							bianse(&tu[i][3]);
						}
					}
				}
			}
			else//01
			{
				if (tu[i][2].yanse == 0)//010
				{
					if(tu[i][2].yanse == 0)//0100
					{
						tu[i][0].yanse =tu[i][1].yanse;
						tu[i][1].yanse = 0;
						bianse(&tu[i][0]);
						bianse(&tu[i][1]);
					}
					else//0101
					{
						if(tu[i][1].yanse == tu[i][3].yanse )
						{
							tu[i][1].yanse++;
							tu[i][0].yanse =tu[i][1].yanse;
							tu[i][1].yanse =0;
							tu[i][3].yanse =0;
							bianse(&tu[i][0]);
							bianse(&tu[i][1]);
							bianse(&tu[i][3]);
						}
						else
						{
							tu[i][0].yanse =tu[i][1].yanse;
							tu[i][1].yanse =tu[i][3].yanse;
							tu[i][3].yanse =0;
							bianse(&tu[i][0]);
							bianse(&tu[i][1]);
							bianse(&tu[i][3]);
						}
					}
				}
				else//011
				{
					if (tu[i][3].yanse == 0)//0110
					{
						if (tu[i][1].yanse == tu[i][2].yanse)
						{
							tu[i][1].yanse++;
							tu[i][0].yanse=tu[i][1].yanse;
							tu[i][1].yanse=0;
							tu[i][2].yanse=0;
							bianse(&tu[i][0]);
							bianse(&tu[i][1]);
							bianse(&tu[i][2]);
						}
						else
						{
							tu[i][0].yanse = tu[i][1].yanse;
							tu[i][1].yanse = tu[i][2].yanse;
							tu[i][2].yanse=0;
							bianse(&tu[i][0]);
							bianse(&tu[i][1]);
							bianse(&tu[i][2]);
						}
					}
					else//0111
					{
						if(tu[i][1].yanse==tu[i][2].yanse)
						{
							tu[i][1].yanse++;
							tu[i][2].yanse=0;
							tu[i][0].yanse = tu[i][1].yanse;
							tu[i][1].yanse=tu[i][3].yanse;
							tu[i][3].yanse=0;
							bianse(&tu[i][0]);
							bianse(&tu[i][1]);
							bianse(&tu[i][2]);
							bianse(&tu[i][3]);
						}
						else
						{
							if(tu[i][2].yanse==tu[i][3].yanse)
							{
								tu[i][0].yanse=tu[i][1].yanse;
								tu[i][2].yanse++;
								tu[i][3].yanse=0;
								tu[i][1].yanse=tu[i][2].yanse;
								tu[i][2].yanse=0;
								bianse(&tu[i][0]);
								bianse(&tu[i][1]);
								bianse(&tu[i][2]);
								bianse(&tu[i][3]);
							}
							else
							{
								tu[i][0].yanse=tu[i][1].yanse;
								tu[i][1].yanse=tu[i][2].yanse;
								tu[i][2].yanse=tu[i][3].yanse;
								tu[i][3].yanse=0;
								bianse(&tu[i][0]);
								bianse(&tu[i][1]);
								bianse(&tu[i][2]);
								bianse(&tu[i][3]);
							}
						}
					}
				}
			}
		}
		else//1
		{
			if (tu[i][1].yanse == 0)//10
			{
				if (tu[i][2].yanse == 0)//100
				{
					if (tu[i][3].yanse == 0)//1000
						;
					else                 //1001
					{
						if (tu[i][0].yanse == tu[i][3].yanse)
						{
							tu[i][0].yanse++;
							tu[i][3].yanse = 0;
							bianse(&tu[i][0]);
							bianse(&tu[i][3]);
						}
						else
						{
							tu[i][1].yanse = tu[i][3].yanse;
							tu[i][3].yanse = 0;
							bianse(&tu[i][1]);
							bianse(&tu[i][3]);
						}
					}
				}
				else//101
				{
					if (tu[i][3].yanse == 0)         //1010
					{
						if (tu[i][0].yanse == tu[i][2].yanse)
						{
							tu[i][0].yanse++;
							tu[i][2].yanse = 0;
							bianse(&tu[i][0]);
							bianse(&tu[i][2]);
						}
						else
						{
							tu[i][1].yanse = tu[i][2].yanse;
							tu[i][2].yanse = 0;
							bianse(&tu[i][1]);
							bianse(&tu[i][2]);
						}
					}
					else     //1011
					{
						if (tu[i][2].yanse == tu[i][0].yanse)
						{
							tu[i][0].yanse++;
							tu[i][2].yanse = 0;
							tu[i][1].yanse = tu[i][3].yanse ;
							tu[i][3].yanse = 0;
							bianse(&tu[i][0]);
							bianse(&tu[i][1]);
							bianse(&tu[i][2]);
							bianse(&tu[i][3]);
						}
						else
						{
							if (tu[i][2].yanse == tu[i][3].yanse)
							{
								tu[i][2].yanse++;
								tu[i][3].yanse = 0;
								tu[i][1].yanse = tu[i][2].yanse;
								tu[i][2].yanse = 0;
								bianse(&tu[i][1]);
								bianse(&tu[i][2]);
								bianse(&tu[i][3]);
							}
							else
							{
								tu[i][1].yanse=tu[i][2].yanse;
								tu[i][2].yanse=tu[i][3].yanse;
								tu[i][3].yanse=0;
								bianse(&tu[i][1]);
								bianse(&tu[i][3]);
								bianse(&tu[i][2]);
							}
						}
					}
				}
			}
			else   //11
			{
				if (tu[i][2].yanse == 0)   //110
				{
					if (tu[i][3].yanse == 0) //1100
					{
						if (tu[i][0].yanse == tu[i][1].yanse)
						{
							tu[i][0].yanse++;
							tu[i][1].yanse = 0;
							bianse(&tu[i][0]);
							bianse(&tu[i][1]);
						}
					}
					else //1101
					{
						if (tu[i][0].yanse == tu[i][1].yanse)
						{
							tu[i][0].yanse++;
							tu[i][1].yanse = tu[i][3].yanse;
							tu[i][3].yanse = 0;
							bianse(&tu[i][0]);
							bianse(&tu[i][1]);
							bianse(&tu[i][3]);
						}
						else
						{
							if (tu[i][1].yanse == tu[i][3].yanse)
							{
								tu[i][1].yanse++;
								tu[i][3].yanse = 0;
								bianse(&tu[i][1]);
								bianse(&tu[i][3]);
							}
							else
							{
								tu[i][2].yanse = tu[i][3].yanse;
								tu[i][3].yanse = 0;
								bianse(&tu[i][2]);
								bianse(&tu[i][3]);
							}
						}
					}
				}
				else   //111
				{
					if (tu[i][3].yanse == 0)  //1110
					{
						if (tu[i][0].yanse == tu[i][1].yanse)
						{
							tu[i][0].yanse++;
							tu[i][1].yanse = tu[i][2].yanse;
							bianse(&tu[i][0]);
							bianse(&tu[i][1]); 
							bianse(&tu[i][2]);
						}
						else
						{
							if (tu[i][1].yanse == tu[i][2].yanse)
							{
								tu[i][1].yanse++;
								tu[i][2].yanse = 0;
								bianse(&tu[i][1]);
								bianse(&tu[i][2]);
							}
						}
					}
					else   //1111
					{
						if (tu[i][0].yanse == tu[i][1].yanse)
						{
							tu[i][0].yanse++;
							tu[i][1].yanse = 0;
							if (tu[i][2].yanse == tu[i][3].yanse)
							{
								tu[i][2].yanse++;
								tu[i][3].yanse = 0;
								tu[i][1].yanse = tu[i][2].yanse;
								tu[i][2].yanse = 0;
								bianse(&tu[i][0]);
								bianse(&tu[i][1]);
								bianse(&tu[i][2]);
								bianse(&tu[i][3]);
							}
							else
							{
								tu[i][1].yanse = tu[i][2].yanse;
								tu[i][2].yanse = tu[i][3].yanse;
								tu[i][3].yanse = 0;
								bianse(&tu[i][0]);
								bianse(&tu[i][1]);
								bianse(&tu[i][2]);
								bianse(&tu[i][3]);
							}
						}
						else
						{
							if (tu[i][1].yanse == tu[i][2].yanse)
							{
								tu[i][1].yanse++;
								tu[i][2].yanse = tu[i][3].yanse;
								tu[i][3].yanse = 0;
								bianse(&tu[i][1]);
								bianse(&tu[i][2]);
								bianse(&tu[i][3]);
							}
							else
							{
								if (tu[i][2].yanse == tu[i][3].yanse)
								{
									tu[i][2].yanse++;
									tu[i][3].yanse = 0;
									bianse(&tu[i][2]);
									bianse(&tu[i][3]);
								}
							}
						}
					}
				}
			}
		}
	}
}

void youhua(struct fangkuai (*tu)[4],int cd)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (tu[i][3].yanse == 0)     //0
		{
			if (tu[i][2].yanse == 0)  //00
			{
				if (tu[i][1].yanse == 0)   //000
				{
					if (tu[i][0].yanse == 0)//0000
						;
					else //0001
					{
						tu[i][3].yanse = tu[i][0].yanse;
						tu[i][0].yanse = 0;
						bianse(&tu[i][3]);
						bianse(&tu[i][0]);
					}
				}
				else//001
				{
					if (tu[i][0].yanse == 0)//0010
					{
						tu[i][3].yanse = tu[i][1].yanse;
						tu[i][1].yanse = 0;
						bianse(&tu[i][3]);
						bianse(&tu[i][1]);
					}
					else//0011
					{
						if (tu[i][0].yanse == tu[i][1].yanse)
						{
							tu[i][1].yanse++;
							tu[i][0].yanse = 0;
							tu[i][3].yanse = tu[i][1].yanse;
							tu[i][1].yanse = 0;
							bianse(&tu[i][3]);
							bianse(&tu[i][1]);
							bianse(&tu[i][0]);
						}
						else
						{
							tu[i][3].yanse = tu[i][1].yanse;
							tu[i][1].yanse = 0;
							tu[i][2].yanse = tu[i][0].yanse;
							tu[i][0].yanse = 0;
							bianse(&tu[i][3]);
							bianse(&tu[i][2]);
							bianse(&tu[i][1]);
							bianse(&tu[i][0]);
						}
					}
				}
			}
			else//01
			{
				if (tu[i][1].yanse == 0)//010
				{
					if(tu[i][1].yanse == 0)//0100
					{
						tu[i][3].yanse =tu[i][2].yanse;
						tu[i][2].yanse = 0;
						bianse(&tu[i][3]);
						bianse(&tu[i][2]);
					}
					else//0101
					{
						if(tu[i][2].yanse == tu[i][0].yanse )
						{
							tu[i][2].yanse++;
							tu[i][3].yanse =tu[i][2].yanse;
							tu[i][2].yanse =0;
							tu[i][0].yanse =0;
							bianse(&tu[i][3]);
							bianse(&tu[i][2]);
							bianse(&tu[i][0]);
						}
						else
						{
							tu[i][3].yanse =tu[i][2].yanse;
							tu[i][2].yanse =tu[i][0].yanse;
							tu[i][0].yanse =0;
							bianse(&tu[i][3]);
							bianse(&tu[i][2]);
							bianse(&tu[i][0]);
						}
					}
				}
				else//011
				{
					if (tu[i][0].yanse == 0)//0110
					{
						if (tu[i][2].yanse == tu[i][1].yanse)
						{
							tu[i][2].yanse++;
							tu[i][3].yanse=tu[i][2].yanse;
							tu[i][2].yanse=0;
							tu[i][1].yanse=0;
							bianse(&tu[i][3]);
							bianse(&tu[i][2]);
							bianse(&tu[i][1]);
						}
						else
						{
							tu[i][3].yanse = tu[i][2].yanse;
							tu[i][2].yanse = tu[i][1].yanse;
							tu[i][1].yanse=0;
							bianse(&tu[i][3]);
							bianse(&tu[i][2]);
							bianse(&tu[i][1]);
						}
					}
					else//0111
					{
						if(tu[i][2].yanse==tu[i][1].yanse)
						{
							tu[i][2].yanse++;
							tu[i][1].yanse=0;
							tu[i][3].yanse = tu[i][2].yanse;
							tu[i][2].yanse=tu[i][0].yanse;
							tu[i][0].yanse=0;
							bianse(&tu[i][3]);
							bianse(&tu[i][2]);
							bianse(&tu[i][1]);
							bianse(&tu[i][0]);
						}
						else
						{
							if(tu[i][1].yanse==tu[i][0].yanse)
							{
								tu[i][3].yanse=tu[i][2].yanse;
								tu[i][1].yanse++;
								tu[i][0].yanse=0;
								tu[i][2].yanse=tu[i][1].yanse;
								tu[i][1].yanse=0;
								bianse(&tu[i][3]);
								bianse(&tu[i][2]);
								bianse(&tu[i][1]);
								bianse(&tu[i][0]);
							}
							else
							{
								tu[i][3].yanse=tu[i][2].yanse;
								tu[i][2].yanse=tu[i][1].yanse;
								tu[i][1].yanse=tu[i][0].yanse;
								tu[i][0].yanse=0;
								bianse(&tu[i][3]);
								bianse(&tu[i][2]);
								bianse(&tu[i][1]);
								bianse(&tu[i][0]);
							}
						}
					}
				}
			}
		}
		else//1
		{
			if (tu[i][2].yanse == 0)//10
			{
				if (tu[i][1].yanse == 0)//100
				{
					if (tu[i][0].yanse == 0)//1000
						;
					else                 //1001
					{
						if (tu[i][3].yanse == tu[i][0].yanse)
						{
							tu[i][3].yanse++;
							tu[i][0].yanse = 0;
							bianse(&tu[i][3]);
							bianse(&tu[i][0]);
						}
						else
						{
							tu[i][2].yanse = tu[i][0].yanse;
							tu[i][0].yanse = 0;
							bianse(&tu[i][2]);
							bianse(&tu[i][0]);
						}
					}
				}
				else//101
				{
					if (tu[i][0].yanse == 0)         //1010
					{
						if (tu[i][3].yanse == tu[i][1].yanse)
						{
							tu[i][3].yanse++;
							tu[i][1].yanse = 0;
							bianse(&tu[i][3]);
							bianse(&tu[i][1]);
						}
						else
						{
							tu[i][2].yanse = tu[i][1].yanse;
							tu[i][1].yanse = 0;
							bianse(&tu[i][2]);
							bianse(&tu[i][1]);
						}
					}
					else     //1011
					{
						if (tu[i][1].yanse == tu[i][3].yanse)
						{
							tu[i][3].yanse++;
							tu[i][1].yanse = 0;
							tu[i][2].yanse = tu[i][0].yanse ;
							tu[i][0].yanse = 0;
							bianse(&tu[i][3]);
							bianse(&tu[i][2]);
							bianse(&tu[i][1]);
							bianse(&tu[i][0]);
						}
						else
						{
							if (tu[i][1].yanse == tu[i][0].yanse)
							{
								tu[i][1].yanse++;
								tu[i][0].yanse = 0;
								tu[i][2].yanse = tu[i][1].yanse;
								tu[i][1].yanse = 0;
								bianse(&tu[i][2]);
								bianse(&tu[i][1]);
								bianse(&tu[i][0]);
							}
							else
							{
								tu[i][2].yanse=tu[i][1].yanse;
								tu[i][1].yanse=tu[i][0].yanse;
								tu[i][0].yanse=0;
								bianse(&tu[i][2]);
								bianse(&tu[i][0]);
								bianse(&tu[i][1]);
							}
						}
					}
				}
			}
			else   //11
			{
				if (tu[i][1].yanse == 0)   //110
				{
					if (tu[i][0].yanse == 0) //1100
					{
						if (tu[i][3].yanse == tu[i][2].yanse)
						{
							tu[i][3].yanse++;
							tu[i][2].yanse = 0;
							bianse(&tu[i][3]);
							bianse(&tu[i][2]);
						}
					}
					else //1101
					{
						if (tu[i][3].yanse == tu[i][2].yanse)
						{
							tu[i][3].yanse++;
							tu[i][2].yanse = tu[i][0].yanse;
							tu[i][0].yanse = 0;
							bianse(&tu[i][3]);
							bianse(&tu[i][2]);
							bianse(&tu[i][0]);
						}
						else
						{
							if (tu[i][2].yanse == tu[i][0].yanse)
							{
								tu[i][2].yanse++;
								tu[i][0].yanse = 0;
								bianse(&tu[i][2]);
								bianse(&tu[i][0]);
							}
							else
							{
								tu[i][1].yanse = tu[i][0].yanse;
								tu[i][0].yanse = 0;
								bianse(&tu[i][1]);
								bianse(&tu[i][0]);
							}
						}
					}
				}
				else   //111
				{
					if (tu[i][0].yanse == 0)  //1110
					{
						if (tu[i][3].yanse == tu[i][2].yanse)
						{
							tu[i][3].yanse++;
							tu[i][2].yanse = tu[i][1].yanse;
							bianse(&tu[i][3]);
							bianse(&tu[i][2]); 
							bianse(&tu[i][1]);
						}
						else
						{
							if (tu[i][2].yanse == tu[i][1].yanse)
							{
								tu[i][2].yanse++;
								tu[i][1].yanse = 0;
								bianse(&tu[i][2]);
								bianse(&tu[i][1]);
							}
						}
					}
					else   //1111
					{
						if (tu[i][3].yanse == tu[i][2].yanse)
						{
							tu[i][3].yanse++;
							tu[i][2].yanse = 0;
							if (tu[i][1].yanse == tu[i][0].yanse)
							{
								tu[i][1].yanse++;
								tu[i][0].yanse = 0;
								tu[i][2].yanse = tu[i][1].yanse;
								tu[i][1].yanse = 0;
								bianse(&tu[i][3]);
								bianse(&tu[i][2]);
								bianse(&tu[i][1]);
								bianse(&tu[i][0]);
							}
							else
							{
								tu[i][2].yanse = tu[i][1].yanse;
								tu[i][1].yanse = tu[i][0].yanse;
								tu[i][0].yanse = 0;
								bianse(&tu[i][3]);
								bianse(&tu[i][2]);
								bianse(&tu[i][1]);
								bianse(&tu[i][0]);
							}
						}
						else
						{
							if (tu[i][2].yanse == tu[i][1].yanse)
							{
								tu[i][2].yanse++;
								tu[i][1].yanse = tu[i][0].yanse;
								tu[i][0].yanse = 0;
								bianse(&tu[i][2]);
								bianse(&tu[i][1]);
								bianse(&tu[i][0]);
							}
							else
							{
								if (tu[i][1].yanse == tu[i][0].yanse)
								{
									tu[i][1].yanse++;
									tu[i][0].yanse = 0;
									bianse(&tu[i][1]);
									bianse(&tu[i][0]);
								}
							}
						}
					}
				}
			}
		}
	}
}


void shanghua(struct fangkuai (*tu)[4],int cd)
{
	int j;
	for (j = 0; j < 4; j++)
	{
		if (tu[0][j].yanse == 0)     //0
		{
			if (tu[1][j].yanse == 0)  //00
			{
				if (tu[2][j].yanse == 0)   //000
				{
					if (tu[3][j].yanse == 0)//0000
						;
					else //0001
					{
						tu[0][j].yanse = tu[3][j].yanse;
						tu[3][j].yanse = 0;
						bianse(&tu[0][j]);
						bianse(&tu[3][j]);
					}
				}
				else//001
				{
					if (tu[3][j].yanse == 0)//0010
					{
						tu[0][j].yanse = tu[2][j].yanse;
						tu[2][j].yanse = 0;
						bianse(&tu[0][j]);
						bianse(&tu[2][j]);
					}
					else//0011
					{
						if (tu[3][j].yanse == tu[2][j].yanse)
						{
							tu[2][j].yanse++;
							tu[3][j].yanse = 0;
							tu[0][j].yanse = tu[2][j].yanse;
							tu[2][j].yanse = 0;
							bianse(&tu[0][j]);
							bianse(&tu[2][j]);
							bianse(&tu[3][j]);
						}
						else
						{
							tu[0][j].yanse = tu[2][j].yanse;
							tu[2][j].yanse = 0;
							tu[1][j].yanse = tu[3][j].yanse;
							tu[3][j].yanse = 0;
							bianse(&tu[0][j]);
							bianse(&tu[1][j]);
							bianse(&tu[2][j]);
							bianse(&tu[3][j]);
						}
					}
				}
			}
			else//01
			{
				if (tu[2][j].yanse == 0)//010
				{
					if(tu[2][j].yanse == 0)//0100
					{
						tu[0][j].yanse =tu[1][j].yanse;
						tu[1][j].yanse = 0;
						bianse(&tu[0][j]);
						bianse(&tu[1][j]);
					}
					else//0101
					{
						if(tu[1][j].yanse == tu[3][j].yanse )
						{
							tu[1][j].yanse++;
							tu[0][j].yanse =tu[1][j].yanse;
							tu[1][j].yanse =0;
							tu[3][j].yanse =0;
							bianse(&tu[0][j]);
							bianse(&tu[1][j]);
							bianse(&tu[3][j]);
						}
						else
						{
							tu[0][j].yanse =tu[1][j].yanse;
							tu[1][j].yanse =tu[3][j].yanse;
							tu[3][j].yanse =0;
							bianse(&tu[0][j]);
							bianse(&tu[1][j]);
							bianse(&tu[3][j]);
						}
					}
				}
				else//011
				{
					if (tu[3][j].yanse == 0)//0110
					{
						if (tu[1][j].yanse == tu[2][j].yanse)
						{
							tu[1][j].yanse++;
							tu[0][j].yanse=tu[1][j].yanse;
							tu[1][j].yanse=0;
							tu[2][j].yanse=0;
							bianse(&tu[0][j]);
							bianse(&tu[1][j]);
							bianse(&tu[2][j]);
						}
						else
						{
							tu[0][j].yanse = tu[1][j].yanse;
							tu[1][j].yanse = tu[2][j].yanse;
							tu[2][j].yanse=0;
							bianse(&tu[0][j]);
							bianse(&tu[1][j]);
							bianse(&tu[2][j]);
						}
					}
					else//0111
					{
						if(tu[1][j].yanse==tu[2][j].yanse)
						{
							tu[1][j].yanse++;
							tu[2][j].yanse=0;
							tu[0][j].yanse = tu[1][j].yanse;
							tu[1][j].yanse=tu[3][j].yanse;
							tu[3][j].yanse=0;
							bianse(&tu[0][j]);
							bianse(&tu[1][j]);
							bianse(&tu[2][j]);
							bianse(&tu[3][j]);
						}
						else
						{
							if(tu[2][j].yanse==tu[3][j].yanse)
							{
								tu[0][j].yanse=tu[1][j].yanse;
								tu[2][j].yanse++;
								tu[3][j].yanse=0;
								tu[1][j].yanse=tu[2][j].yanse;
								tu[2][j].yanse=0;
								bianse(&tu[0][j]);
								bianse(&tu[1][j]);
								bianse(&tu[2][j]);
								bianse(&tu[3][j]);
							}
							else
							{
								tu[0][j].yanse=tu[1][j].yanse;
								tu[1][j].yanse=tu[2][j].yanse;
								tu[2][j].yanse=tu[3][j].yanse;
								tu[3][j].yanse=0;
								bianse(&tu[0][j]);
								bianse(&tu[1][j]);
								bianse(&tu[2][j]);
								bianse(&tu[3][j]);
							}
						}
					}
				}
			}
		}
		else//1
		{
			if (tu[1][j].yanse == 0)//10
			{
				if (tu[2][j].yanse == 0)//100
				{
					if (tu[3][j].yanse == 0)//1000
						;
					else                 //1001
					{
						if (tu[0][j].yanse == tu[3][j].yanse)
						{
							tu[0][j].yanse++;
							tu[3][j].yanse = 0;
							bianse(&tu[0][j]);
							bianse(&tu[3][j]);
						}
						else
						{
							tu[1][j].yanse = tu[3][j].yanse;
							tu[3][j].yanse = 0;
							bianse(&tu[1][j]);
							bianse(&tu[3][j]);
						}
					}
				}
				else//101
				{
					if (tu[3][j].yanse == 0)         //1010
					{
						if (tu[0][j].yanse == tu[2][j].yanse)
						{
							tu[0][j].yanse++;
							tu[2][j].yanse = 0;
							bianse(&tu[0][j]);
							bianse(&tu[2][j]);
						}
						else
						{
							tu[1][j].yanse = tu[2][j].yanse;
							tu[2][j].yanse = 0;
							bianse(&tu[1][j]);
							bianse(&tu[2][j]);
						}
					}
					else     //1011
					{
						if (tu[2][j].yanse == tu[0][j].yanse)
						{
							tu[0][j].yanse++;
							tu[2][j].yanse = 0;
							tu[1][j].yanse = tu[3][j].yanse ;
							tu[3][j].yanse = 0;
							bianse(&tu[0][j]);
							bianse(&tu[1][j]);
							bianse(&tu[2][j]);
							bianse(&tu[3][j]);
						}
						else
						{
							if (tu[2][j].yanse == tu[3][j].yanse)
							{
								tu[2][j].yanse++;
								tu[3][j].yanse = 0;
								tu[1][j].yanse = tu[2][j].yanse;
								tu[2][j].yanse = 0;
								bianse(&tu[1][j]);
								bianse(&tu[2][j]);
								bianse(&tu[3][j]);
							}
							else
							{
								tu[1][j].yanse=tu[2][j].yanse;
								tu[2][j].yanse=tu[3][j].yanse;
								tu[3][j].yanse=0;
								bianse(&tu[1][j]);
								bianse(&tu[3][j]);
								bianse(&tu[2][j]);
							}
						}
					}
				}
			}
			else   //11
			{
				if (tu[2][j].yanse == 0)   //110
				{
					if (tu[3][j].yanse == 0) //1100
					{
						if (tu[0][j].yanse == tu[1][j].yanse)
						{
							tu[0][j].yanse++;
							tu[1][j].yanse = 0;
							bianse(&tu[0][j]);
							bianse(&tu[1][j]);
						}
					}
					else //1101
					{
						if (tu[0][j].yanse == tu[1][j].yanse)
						{
							tu[0][j].yanse++;
							tu[1][j].yanse = tu[3][j].yanse;
							tu[3][j].yanse = 0;
							bianse(&tu[0][j]);
							bianse(&tu[1][j]);
							bianse(&tu[3][j]);
						}
						else
						{
							if (tu[1][j].yanse == tu[3][j].yanse)
							{
								tu[1][j].yanse++;
								tu[3][j].yanse = 0;
								bianse(&tu[1][j]);
								bianse(&tu[3][j]);
							}
							else
							{
								tu[2][j].yanse = tu[3][j].yanse;
								tu[3][j].yanse = 0;
								bianse(&tu[2][j]);
								bianse(&tu[3][j]);
							}
						}
					}
				}
				else   //111
				{
					if (tu[3][j].yanse == 0)  //1110
					{
						if (tu[0][j].yanse == tu[1][j].yanse)
						{
							tu[0][j].yanse++;
							tu[1][j].yanse = tu[2][j].yanse;
							bianse(&tu[0][j]);
							bianse(&tu[1][j]); 
							bianse(&tu[2][j]);
						}
						else
						{
							if (tu[1][j].yanse == tu[2][j].yanse)
							{
								tu[1][j].yanse++;
								tu[2][j].yanse = 0;
								bianse(&tu[1][j]);
								bianse(&tu[2][j]);
							}
						}
					}
					else   //1111
					{
						if (tu[0][j].yanse == tu[1][j].yanse)
						{
							tu[0][j].yanse++;
							tu[1][j].yanse = 0;
							if (tu[2][j].yanse == tu[3][j].yanse)
							{
								tu[2][j].yanse++;
								tu[3][j].yanse = 0;
								tu[1][j].yanse = tu[2][j].yanse;
								tu[2][j].yanse = 0;
								bianse(&tu[0][j]);
								bianse(&tu[1][j]);
								bianse(&tu[2][j]);
								bianse(&tu[3][j]);
							}
							else
							{
								tu[1][j].yanse = tu[2][j].yanse;
								tu[2][j].yanse = tu[3][j].yanse;
								tu[3][j].yanse = 0;
								bianse(&tu[0][j]);
								bianse(&tu[1][j]);
								bianse(&tu[2][j]);
								bianse(&tu[3][j]);
							}
						}
						else
						{
							if (tu[1][j].yanse == tu[2][j].yanse)
							{
								tu[1][j].yanse++;
								tu[2][j].yanse = tu[3][j].yanse;
								tu[3][j].yanse = 0;
								bianse(&tu[1][j]);
								bianse(&tu[2][j]);
								bianse(&tu[3][j]);
							}
							else
							{
								if (tu[2][j].yanse == tu[3][j].yanse)
								{
									tu[2][j].yanse++;
									tu[3][j].yanse = 0;
									bianse(&tu[2][j]);
									bianse(&tu[3][j]);
								}
							}
						}
					}
				}
			}
		}
	}
}

void xiahua(struct fangkuai (*tu)[4],int cd)
{
	int j;
	for (j = 0; j < 4; j++)
	{
		if (tu[3][j].yanse == 0)     //0
		{
			if (tu[2][j].yanse == 0)  //00
			{
				if (tu[1][j].yanse == 0)   //000
				{
					if (tu[0][j].yanse == 0)//0000
						;
					else //0001
					{
						tu[3][j].yanse = tu[0][j].yanse;
						tu[0][j].yanse = 0;
						bianse(&tu[3][j]);
						bianse(&tu[0][j]);
					}
				}
				else//001
				{
					if (tu[0][j].yanse == 0)//0010
					{
						tu[3][j].yanse = tu[1][j].yanse;
						tu[1][j].yanse = 0;
						bianse(&tu[3][j]);
						bianse(&tu[1][j]);
					}
					else//0011
					{
						if (tu[0][j].yanse == tu[1][j].yanse)
						{
							tu[1][j].yanse++;
							tu[0][j].yanse = 0;
							tu[3][j].yanse = tu[1][j].yanse;
							tu[1][j].yanse = 0;
							bianse(&tu[3][j]);
							bianse(&tu[1][j]);
							bianse(&tu[0][j]);
						}
						else
						{
							tu[3][j].yanse = tu[1][j].yanse;
							tu[1][j].yanse = 0;
							tu[2][j].yanse = tu[0][j].yanse;
							tu[0][j].yanse = 0;
							bianse(&tu[3][j]);
							bianse(&tu[2][j]);
							bianse(&tu[1][j]);
							bianse(&tu[0][j]);
						}
					}
				}
			}
			else//01
			{
				if (tu[1][j].yanse == 0)//010
				{
					if(tu[1][j].yanse == 0)//0100
					{
						tu[3][j].yanse =tu[2][j].yanse;
						tu[2][j].yanse = 0;
						bianse(&tu[3][j]);
						bianse(&tu[2][j]);
					}
					else//0101
					{
						if(tu[2][j].yanse == tu[0][j].yanse )
						{
							tu[2][j].yanse++;
							tu[3][j].yanse =tu[2][j].yanse;
							tu[2][j].yanse =0;
							tu[0][j].yanse =0;
							bianse(&tu[3][j]);
							bianse(&tu[2][j]);
							bianse(&tu[0][j]);
						}
						else
						{
							tu[3][j].yanse =tu[2][j].yanse;
							tu[2][j].yanse =tu[0][j].yanse;
							tu[0][j].yanse =0;
							bianse(&tu[3][j]);
							bianse(&tu[2][j]);
							bianse(&tu[0][j]);
						}
					}
				}
				else//011
				{
					if (tu[0][j].yanse == 0)//0110
					{
						if (tu[2][j].yanse == tu[1][j].yanse)
						{
							tu[2][j].yanse++;
							tu[3][j].yanse=tu[2][j].yanse;
							tu[2][j].yanse=0;
							tu[1][j].yanse=0;
							bianse(&tu[3][j]);
							bianse(&tu[2][j]);
							bianse(&tu[1][j]);
						}
						else
						{
							tu[3][j].yanse = tu[2][j].yanse;
							tu[2][j].yanse = tu[1][j].yanse;
							tu[1][j].yanse=0;
							bianse(&tu[3][j]);
							bianse(&tu[2][j]);
							bianse(&tu[1][j]);
						}
					}
					else//0111
					{
						if(tu[2][j].yanse==tu[1][j].yanse)
						{
							tu[2][j].yanse++;
							tu[1][j].yanse=0;
							tu[3][j].yanse = tu[2][j].yanse;
							tu[2][j].yanse=tu[0][j].yanse;
							tu[0][j].yanse=0;
							bianse(&tu[3][j]);
							bianse(&tu[2][j]);
							bianse(&tu[1][j]);
							bianse(&tu[0][j]);
						}
						else
						{
							if(tu[1][j].yanse==tu[0][j].yanse)
							{
								tu[3][j].yanse=tu[2][j].yanse;
								tu[1][j].yanse++;
								tu[0][j].yanse=0;
								tu[2][j].yanse=tu[1][j].yanse;
								tu[1][j].yanse=0;
								bianse(&tu[3][j]);
								bianse(&tu[2][j]);
								bianse(&tu[1][j]);
								bianse(&tu[0][j]);
							}
							else
							{
								tu[3][j].yanse=tu[2][j].yanse;
								tu[2][j].yanse=tu[1][j].yanse;
								tu[1][j].yanse=tu[0][j].yanse;
								tu[0][j].yanse=0;
								bianse(&tu[3][j]);
								bianse(&tu[2][j]);
								bianse(&tu[1][j]);
								bianse(&tu[0][j]);
							}
						}
					}
				}
			}
		}
		else//1
		{
			if (tu[2][j].yanse == 0)//10
			{
				if (tu[1][j].yanse == 0)//100
				{
					if (tu[0][j].yanse == 0)//1000
						;
					else                 //1001
					{
						if (tu[3][j].yanse == tu[0][j].yanse)
						{
							tu[3][j].yanse++;
							tu[0][j].yanse = 0;
							bianse(&tu[3][j]);
							bianse(&tu[0][j]);
						}
						else
						{
							tu[2][j].yanse = tu[0][j].yanse;
							tu[0][j].yanse = 0;
							bianse(&tu[2][j]);
							bianse(&tu[0][j]);
						}
					}
				}
				else//101
				{
					if (tu[0][j].yanse == 0)         //1010
					{
						if (tu[3][j].yanse == tu[1][j].yanse)
						{
							tu[3][j].yanse++;
							tu[1][j].yanse = 0;
							bianse(&tu[3][j]);
							bianse(&tu[1][j]);
						}
						else
						{
							tu[2][j].yanse = tu[1][j].yanse;
							tu[1][j].yanse = 0;
							bianse(&tu[2][j]);
							bianse(&tu[1][j]);
						}
					}
					else     //1011
					{
						if (tu[1][j].yanse == tu[3][j].yanse)
						{
							tu[3][j].yanse++;
							tu[1][j].yanse = 0;
							tu[2][j].yanse = tu[0][j].yanse ;
							tu[0][j].yanse = 0;
							bianse(&tu[3][j]);
							bianse(&tu[2][j]);
							bianse(&tu[1][j]);
							bianse(&tu[0][j]);
						}
						else
						{
							if (tu[1][j].yanse == tu[0][j].yanse)
							{
								tu[1][j].yanse++;
								tu[0][j].yanse = 0;
								tu[2][j].yanse = tu[1][j].yanse;
								tu[1][j].yanse = 0;
								bianse(&tu[2][j]);
								bianse(&tu[1][j]);
								bianse(&tu[0][j]);
							}
							else
							{
								tu[2][j].yanse=tu[1][j].yanse;
								tu[1][j].yanse=tu[0][j].yanse;
								tu[0][j].yanse=0;
								bianse(&tu[2][j]);
								bianse(&tu[0][j]);
								bianse(&tu[1][j]);
							}
						}
					}
				}
			}
			else   //11
			{
				if (tu[1][j].yanse == 0)   //110
				{
					if (tu[0][j].yanse == 0) //1100
					{
						if (tu[3][j].yanse == tu[2][j].yanse)
						{
							tu[3][j].yanse++;
							tu[2][j].yanse = 0;
							bianse(&tu[3][j]);
							bianse(&tu[2][j]);
						}
					}
					else //1101
					{
						if (tu[3][j].yanse == tu[2][j].yanse)
						{
							tu[3][j].yanse++;
							tu[2][j].yanse = tu[0][j].yanse;
							tu[0][j].yanse = 0;
							bianse(&tu[3][j]);
							bianse(&tu[2][j]);
							bianse(&tu[0][j]);
						}
						else
						{
							if (tu[2][j].yanse == tu[0][j].yanse)
							{
								tu[2][j].yanse++;
								tu[0][j].yanse = 0;
								bianse(&tu[2][j]);
								bianse(&tu[0][j]);
							}
							else
							{
								tu[1][j].yanse = tu[0][j].yanse;
								tu[0][j].yanse = 0;
								bianse(&tu[1][j]);
								bianse(&tu[0][j]);
							}
						}
					}
				}
				else   //111
				{
					if (tu[0][j].yanse == 0)  //1110
					{
						if (tu[3][j].yanse == tu[2][j].yanse)
						{
							tu[3][j].yanse++;
							tu[2][j].yanse = tu[1][j].yanse;
							bianse(&tu[3][j]);
							bianse(&tu[2][j]); 
							bianse(&tu[1][j]);
						}
						else
						{
							if (tu[2][j].yanse == tu[1][j].yanse)
							{
								tu[2][j].yanse++;
								tu[1][j].yanse = 0;
								bianse(&tu[2][j]);
								bianse(&tu[1][j]);
							}
						}
					}
					else   //1111
					{
						if (tu[3][j].yanse == tu[2][j].yanse)
						{
							tu[3][j].yanse++;
							tu[2][j].yanse = 0;
							if (tu[1][j].yanse == tu[0][j].yanse)
							{
								tu[1][j].yanse++;
								tu[0][j].yanse = 0;
								tu[2][j].yanse = tu[1][j].yanse;
								tu[1][j].yanse = 0;
								bianse(&tu[3][j]);
								bianse(&tu[2][j]);
								bianse(&tu[1][j]);
								bianse(&tu[0][j]);
							}
							else
							{
								tu[2][j].yanse = tu[1][j].yanse;
								tu[1][j].yanse = tu[0][j].yanse;
								tu[0][j].yanse = 0;
								bianse(&tu[3][j]);
								bianse(&tu[2][j]);
								bianse(&tu[1][j]);
								bianse(&tu[0][j]);
							}
						}
						else
						{
							if (tu[2][j].yanse == tu[1][j].yanse)
							{
								tu[2][j].yanse++;
								tu[1][j].yanse = tu[0][j].yanse;
								tu[0][j].yanse = 0;
								bianse(&tu[2][j]);
								bianse(&tu[1][j]);
								bianse(&tu[0][j]);
							}
							else
							{
								if (tu[1][j].yanse == tu[0][j].yanse)
								{
									tu[1][j].yanse++;
									tu[0][j].yanse = 0;
									bianse(&tu[1][j]);
									bianse(&tu[0][j]);
								}
							}
						}
					}
				}
			}
		}
	}
}



void jiazai(struct fangkuai (*tu)[4],int cd)
{
	int i,j;
	srand((unsigned)time(NULL));
	do
	{
		i=rand()%4;
		j=rand()%4;
		printf("i=%d,j=%d\n",i,j);
	}while(tu[i][j].yanse!=0);
	tu[i][j].yanse=1;
	bianse(&tu[i][j]);
}
void gameover()
{
	int n;
	int fd = open("/cpdd/gg.bmp",O_RDWR);
	if(fd == -1)
	{
		printf("open  failed   \n");
		return ;
	}
	unsigned char buf[800*480*3];
	lseek(fd,0x54,SEEK_SET);
	read(fd,buf,800*480*3);
	unsigned char r,g,b;
	n=0;
	for (i =479;i>=0;i--)
	for (j =0; j <= 799;j++)
	{
			b=buf[0+3*n];
			g=buf[1+3*n];
			r=buf[2+3*n];
			n++;
			*(p+i*800+j)=r<<16|g<<8|b;
	}
	close(fd);
}												

void zlyc()
{
	int n;
	int fd = open("/cpdd/zlyc.bmp",O_RDWR);
	if(fd == -1)
	{
		printf("open  failed   \n");
		return ;
	}
	unsigned char buf[400*120*3];
	lseek(fd,0x54,SEEK_SET);
	read(fd,buf,400*120*3);
	unsigned char r,g,b;
	n=0;
	for (i =369;i>=250;i--)
	for (j =200;j<=599;j++)
	{
			b=buf[0+3*n];
			g=buf[1+3*n];
			r=buf[2+3*n];
			n++;
			*(p+i*800+j)=r<<16|g<<8|b;
	}
	close(fd);
}


int main()
{
	int fd = initLcd();
	struct fangkuai tu[4][4];
	for (i = 0; i < 4; i++)
	for (j = 0; j < 4; j++)
	{
		tu[i][j].qishi.h = i * 120;
		tu[i][j].qishi.l = j * 200;
		tu[i][j].jieshu.h = tu[i][j].qishi.h + 119;
		tu[i][j].jieshu.l = tu[i][j].qishi.l + 199;
		tu[i][j].yanse = 0;
		bianse(&tu[i][j]);
	}
	while (1)
	{
		int man=0,gg;
		getTouch();
		if (cha(x1, x2) > cha(yy1, yy2))
		{
			if (x1 > x2)   
			{
				zuohua(tu,4);
			}
			else
			{
				youhua(tu,4);
			}
		}
		else
		{
			if (yy1 > yy2)
			{
				shanghua(tu,4);
			}
			else
			{
				xiahua(tu,4);
			}
		}
		for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			if(tu[i][j].yanse==11)
				printf("恭喜游戏通关！\n");
			if(tu[i][j].yanse==0)
				man++;
		}
		if(man>0)
		jiazai(tu,4);
		if(man==1||man==0)
		{
			gg=1;
			for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
			{
				if(i-1>=0)
				{
					if(tu[i][j].yanse==tu[i-1][j].yanse)
					{
						gg=0;
					}
				}
				if(i+1<=3)
				{
					if(tu[i][j].yanse==tu[i+1][j].yanse)
					{
						gg=0;
					}
				}
				if(j-1>=0)
				{
					if(tu[i][j].yanse==tu[i][j-1].yanse)
					{
						gg=0;
					}
				}
				if(j+1<=3)
				{
					if(tu[i][j].yanse==tu[i][j+1].yanse)
					{
						gg=0;
					}
				}
			}							
			if(gg)
			{
				gameover();
				zlyc();
				getTouch();
				getTouch();
				for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
				{
					tu[i][j].qishi.h = i * 120;
					tu[i][j].qishi.l = j * 200;
					tu[i][j].jieshu.h = tu[i][j].qishi.h + 119;
					tu[i][j].jieshu.l = tu[i][j].qishi.l + 199;
					tu[i][j].yanse = 0;
					bianse(&tu[i][j]);
				}
			}
		}
	}
	
	
	
	
	
	uninitLcd(fd);
	return 0;
}