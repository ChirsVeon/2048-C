#ifndef _TOUCH_H_
#define _TOUCH_H_

/*
	功能：获取屏幕的划动方向
	返回值：
		上划返回1
		下划返回2
		左划返回3
		右划返回4
		失败返回-1
*/
int getTouch();

void up();

void right();

void left();
#endif