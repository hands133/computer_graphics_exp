#pragma once
#ifndef _ACTIVEEDGE_
#define _ACTIVEEDGE_

struct activeEdge
{
	float x;		//扫描线与边的交点
	float deltaX;	//横坐标的递归增量
	int ymax;			//每条边的最大 y 值
	bool firstIn;
	activeEdge* next;	//下一个结点指针

	activeEdge(float x, float deltaX, int ymax) :
		x(x), deltaX(deltaX), ymax(ymax) {
		next = NULL;
		firstIn = true;
	}
};

#endif