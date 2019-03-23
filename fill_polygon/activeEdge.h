#pragma once
#ifndef _ACTIVEEDGE_
#define _ACTIVEEDGE_

struct activeEdge
{
	float x;		//ɨ������ߵĽ���
	float deltaX;	//������ĵݹ�����
	int ymax;			//ÿ���ߵ���� y ֵ
	bool firstIn;
	activeEdge* next;	//��һ�����ָ��

	activeEdge(float x, float deltaX, int ymax) :
		x(x), deltaX(deltaX), ymax(ymax) {
		next = NULL;
		firstIn = true;
	}
};

#endif