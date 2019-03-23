#pragma once
#ifndef _POLYGON_
#define _POLYGON_

#include <iostream>
#include <queue>
#include <stack>
#include <gl/glut.h>

#include "activeEdge.h"

#define ILLEGAL_PARAMATER 1
#define UNDEFINED_METHOD 2

#define abs(x) ((x)>0?(x):-(x))
#define max(a,b) ((a)>(b))?(a):(b)
#define min(a,b) ((a)<(b))?(a):(b)

using namespace std;

template <typename T>
class polygon
{
	typedef pair<T, T> Edge[2];

public:
	polygon(int numOfVertices = 3);
	~polygon() { delete[] nodes; };

	void addNode(const T &x, const T &y);
	void rmNode(int seqNum);	//ɾ�����Ϊ seqNum �Ķ��㣨��1��

	int vertexes() { return endIndex; }
	void disp();

	double area();	//���ض���ε����
	void drawEdge(void);//���ƶ���εı�
	void recurFill(const GLubyte *color, int x, int y);	//�ݹ����
	void scanFill(const GLubyte *color);	//ɨ�������
	void bfsFill(const GLubyte *color, int x, int y);	//��������������

	void polyReccut(int XL, int XR, int YB, int YT);	//���βü��㷨

private:
	int n;		//������
	int endIndex;	//��βָ��
	pair<T, T>* nodes;	//��Ž����������飬���������ֱ�Ϊ x �� y
	int* orderedY;	//�� y ���򣬽���ŷ��������У��±��Ӧ����
	int ymin, ymax;	//�����Сɨ���ߵ�������
	activeEdge** actedgeTable;	//���Ա߱�

	//����㷨
	void RECURFILL(const GLubyte* newColor, GLubyte* oldColor, int posx, int posy);
	void SEEDFILL(const GLubyte* newColor, GLubyte* oldColor, int posx, int posy);
	void SCANFILL();

	//ɨ�����㷨��Ҫ�Ľṹ
	void bubbleSort(activeEdge *head);
	activeEdge* delNode(activeEdge* head, int y);
	void updateNode(activeEdge* head);

	// Sutherland-Hodgman �㷨
	void sutherland_Hodgman_cut(pair<T, T>* inputNodes, vector<pair<T, T>>* outputNodes,
		Edge clipEdge, int inputLength, int &outputLength);
	//��������
	//�ж�һ�������Ƿ��ڿɼ�������ڲ�
	bool isInside(pair<T, T>* testNode, Edge clipEdge);
	//����߶� SP �ʹ��ڱ߽�Ľ���
	void calIntersect(pair<T, T>* S, pair<T, T>* P, Edge clipEdge, pair<T, T>* crossPoint);
	//�������в���Ԫ��
	void insert(pair<T, T>* node, int &position, vector<pair<T, T>>* targetAray);
};

template <typename T>
polygon<T>::polygon(int numOfVertices)
{
	//�����������Ϊ numOfVertices �Ķ����
	if (numOfVertices < 2)
	{
		cout << "��������Ӧ��С�� 3" << endl;
		throw ILLEGAL_PARAMATER;
	}

	orderedY = new int[numOfVertices];
	fill(orderedY, orderedY + n, 0);

	ymin = 0;
	ymax = 0;

	n = numOfVertices;
	endIndex = 0;
	nodes = new pair<T, T>[numOfVertices];
	pair<T, T>* node;
	for (int i = 0; i < n; i++) {
		node = &nodes[i];
		node->first = NULL;
		node->second = NULL;
	}
}

template <typename T>
void polygon<T>::addNode(const T&x, const T&y)
{
	if (endIndex == n)
	{
		//���ٸ���Ŀռ�
		pair<T, T>* ex = new pair<T, T>[n * 2];
		int *orderY = new int[n * 2];
		for (int i = 0; i < n; i++) {
			ex[i].first = nodes[i].first;
			ex[i].second = nodes[i].second;
			orderY[i] = orderedY[i];
		}
		delete nodes;
		delete orderedY;
		nodes = ex;
		orderedY = orderY;
		ex = NULL;
		orderY = NULL;
		n *= 2;
	}
	pair<T, T> insert(x, y);
	nodes[endIndex] = insert;
	if (endIndex == 0) {
		orderedY[0] = endIndex;
		ymin = y;
		ymax = y;
	}
	else {
		int storeY;
		//����
		int i;
		for (i = endIndex; i > 0; i--) {
			//orderedY[i-1] ��ʾǰһ�����ı�ţ�0��
			//nodes[orderedY[i-1]]��ʾǰһ�����
			storeY = nodes[orderedY[i - 1]].second;
			if (storeY > y)
				orderedY[i] = orderedY[i - 1];
			else {
				orderedY[i] = endIndex;
				break;
			}
		}
		if (i == 0)
			orderedY[0] = endIndex;
		//����ɨ����
		if (y > ymax)
			ymax = y;
		if (y < ymin)
			ymin = y;
	}
	endIndex++;
}

template <typename T>
void polygon<T>::rmNode(int seqNum)
{
	//ɾ���� seqNum ����㣨1��
	//�жϽ��
	if (seqNum < 1 || seqNum > endIndex) {
		cout << "�Ƿ�����" << endl;
		throw ILLEGAL_PARAMATER;
	}
	int seq = seqNum - 1;
	for (int i = seq; i < endIndex; i++)
		nodes[i] = nodes[i + 1];
	nodes[endIndex].first = NULL;
	nodes[endIndex].second = NULL;
	//��������
	bool tomove = false;
	for (int i = 0; i < endIndex; i++) {
		if (orderedY[i] == seq)
			tomove = true;
		if (tomove)
			orderedY[i] = orderedY[i + 1];
	}
	endIndex--;
	//��������
	if ((endIndex + 1) < (int)n / 2) {
		//ɾ���ظ��ռ���
		pair<T, T>* ex = new pair<T, T>[n / 2];
		int *orderY = new int[n / 2];
		for (int i = 0; i < endIndex; i++) {
			ex[i].first = nodes[i].first;
			ex[i].second = nodes[i].second;
			orderY[i] = orderedY[i];
		}
		delete nodes;
		delete orderedY;
		nodes = ex;
		orderedY = orderY;
		ex = NULL;
		orderY = NULL;
		n /= 2;
	}
}

template <typename T>
void polygon<T>::disp()
{
	//��ʾ���еĽ��
	cout << "���" << endl;
	for (int i = 0; i < endIndex; i++)
		cout << "x:" << nodes[i].first << "\t y:" << nodes[i].second << endl;
	cout << "����" << endl;
	for (int i = 0; i < endIndex; i++)
		cout << i << ":" << orderedY[i] << endl;
}

template <typename T>
double polygon<T>::area()
{
	if (endIndex == 0)
		throw ILLEGAL_PARAMATER;
	double area = 0.0;
	//���ò��ɻ��ּ�������ཻ����ε����
	for (int i = 0; i < endIndex; i++)
	{
		nodes[endIndex] = nodes[0];
		area += (nodes[i].first * nodes[i + 1].second
			- nodes[i + 1].first * nodes[i].second);
	}
	nodes[endIndex].first = NULL;
	nodes[endIndex].second = NULL;
	return abs(area/2);
}

template <typename T>
void polygon<T>::drawEdge(void)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 600, 600);
	//���ձ߻���
	glBegin(GL_LINE_LOOP);
	nodes[endIndex] = nodes[0];
	for (int i = 0; i < endIndex; i++)
		glVertex2f(nodes[i].first, nodes[i].second);
	glEnd();
	glFlush();
}

template <typename T>
void polygon<T>::recurFill(const GLubyte *color, int x, int y)
{
	throw UNDEFINED_METHOD;
	if (color[0] < 0 || color[1] < 0 || color[2] < 0 || x > 600 || y > 600)
		throw ILLEGAL_PARAMATER;

	glViewport(0, 0, 600, 600);
	glColor3ub(color[0], color[1], color[2]);
	GLubyte oldColor[3] = { 0, 0, 0 };
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, oldColor);
	RECURFILL(color, oldColor, x, y);
	glColor3f(1.0f, 1.0f, 1.0f);
}

template <typename T>
void polygon<T>::RECURFILL(const GLubyte* newColor, GLubyte* oldColor, int posx, int posy)
{
	int x = posx, y = posy;
	glViewport(0, 0, 600, 600);
	GLubyte *pixel = new GLubyte[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

	if (pixel[0] == oldColor[0] && pixel[1] == oldColor[1] && pixel[2] == oldColor[2])
	{
		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
		glFlush();
	}
	else
		return;

	delete pixel;

	RECURFILL(newColor, oldColor, x, y + 1);
	RECURFILL(newColor, oldColor, x - 1, y);
	RECURFILL(newColor, oldColor, x, y - 1);
	RECURFILL(newColor, oldColor, x + 1, y);
}

template <typename T>
void polygon<T>::bfsFill(const GLubyte *color, int x, int y)
{
	if (color[0] < 0 || color[1] < 0 || color[2] < 0 || x > 600 || y > 600)
		throw ILLEGAL_PARAMATER;
	glViewport(0, 0, 600, 600);
	glColor3ub(color[0], color[1], color[2]);

	GLubyte oldColor[3] = { 0,0,0 };
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, oldColor);
	SEEDFILL(color, oldColor, x, y);
	glColor3f(1.0f, 1.0f, 1.0f);
}

template <typename T>
void polygon<T>::SEEDFILL(const GLubyte* newColor, GLubyte* oldColor, int posx, int posy)
{
	int x = posx;
	int y = posy;
	pair<T, T>* top;
	queue<pair<T, T>*> queue;
	pair<T, T> *insert;
	insert = new pair<T, T>(x, y);
	queue.push(insert);

	GLubyte *pixel = new GLubyte[3];

	int offset[8] = { 1, 0, 0, 1, -1, 0, 0, -1 };

	while (!queue.empty()) {
		top = queue.front();
		x = top->first;
		y = top->second;

		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
		glFlush();

		for (int i = 0; i < 4; i++) {
			glReadPixels(x + offset[2 * i], y + offset[2 * i + 1], 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			if (pixel[0] == oldColor[0] && pixel[1] == oldColor[1] && pixel[2] == oldColor[2]) {
				glBegin(GL_POINTS);
				glVertex2i(x + offset[2 * i], y + offset[2 * i + 1]);
				glEnd();
				glFlush();
				insert = new pair<T, T>(x + offset[2 * i], y + offset[2 * i + 1]);
				queue.push(insert);
			}
		}
		queue.pop();
		delete top;
	}
}

template <typename T>
void polygon<T>::scanFill(const GLubyte *color)
{
	glViewport(0, 0, 600, 600);
	glColor3ub(color[0], color[1], color[2]);
	SCANFILL();
	glColor3f(1.0f, 1.0f, 1.0f);
}

template <class T>
void polygon<T>::SCANFILL()
{
	//��ʼ��
	actedgeTable = new activeEdge*[ymax + 1];
	for (int i = 0; i <= ymax; i++)
		actedgeTable[i] = NULL;
	int x1, y1, x2, y2;
	int YMIN;
	float xmin, deltaX;
	activeEdge *insert;	//����߱�Ľ��ָ��
	nodes[endIndex] = nodes[0];

	activeEdge *iter = NULL;	//������������ָʾ���

	for (int i = 0; i < endIndex; i++) {
		y1 = nodes[i].second;
		y2 = nodes[i + 1].second;
		//������ ymin = i �ı߷������
		if (y1 != y2)
		{
			YMIN = min(y1, y2);
			//ע��ˮƽ�߲���Ҫ����¼
			x1 = nodes[i].first;
			x2 = nodes[i + 1].first;
			//����ˮƽ��
			xmin = (y1 < y2) ? x1 : x2;
			deltaX = (float)(x2 - x1) / (float)(y2 - y1);
			if (x1 == x2)
				deltaX = 0;
			insert = new activeEdge(xmin, deltaX, (y1 < y2) ? y2 : y1);
			//���߲���߱���
			iter = actedgeTable[YMIN];
			actedgeTable[YMIN] = insert;
			insert->next = iter;
		}
	}
	//���� AET �ձ����ڴ洢���
	activeEdge *AET = NULL;
	
	for (int i = ymin; i <= ymax; i++)
	{
		//������ [i] �н����뵽 AET �У�˳����룩
		iter = actedgeTable[i];
		if (iter != NULL) {
			while (iter->next != NULL)
				iter = iter->next;
			insert = AET;
			AET = actedgeTable[i];
			iter->next = insert;
			actedgeTable[i] = NULL;
		}
		AET = delNode(AET, i);	//ɾ���������
		bubbleSort(AET);	//ð������
		//���½��
		iter = AET;
		bool draw = true;
		int startX = -1;
		int endX = -1;
		while (iter != NULL)
		{
			if (draw) 
			{
				while (iter->firstIn && iter->next != NULL)
					iter = iter->next;
				startX = iter->x;
				draw = false;
			}
			else
			{
				while (iter->firstIn && iter->next != NULL)
					iter = iter->next;
				endX = (int)(iter->x + 0.5);
				draw = true;

				glBegin(GL_LINES);
				glVertex2i(startX, i);
				glVertex2i(endX, i);
				glEnd();
				glFlush();
			}
			iter = iter->next;
		}
		updateNode(AET);
		//������ز����� AET ��
	}
}

template <typename T>
void polygon<T>::bubbleSort(activeEdge *head)
{
	if (head == NULL || head->next == NULL)
		return;
	activeEdge *previous = NULL;
	activeEdge *iter = NULL;
	int length = 0;
	iter = head;
	while (iter)
	{
		iter = iter->next;
		length++;
	}

	float transX, transDelta, transY;
	bool transFirst;
	for (int i = 0; i < length - 1; i++)
	{	
		iter = head->next;
		previous = head;
		for (int j = 0; j < length - i - 1; j++)
		{
			if (previous->x > iter->x)
			{
				//���� previous ��Ϣ
				transX = previous->x;
				transDelta = previous->deltaX;
				transY = previous->ymax;
				transFirst = previous->firstIn;

				previous->x = iter->x;
				previous->deltaX = iter->deltaX;
				previous->ymax = iter->ymax;
				previous->firstIn = iter->firstIn;

				iter->x = transX;
				iter->deltaX = transDelta;
				iter->ymax = transY;
				iter->firstIn = transFirst;
			}
			iter = iter->next;
			previous = previous->next;
		}
	}
	iter = NULL;
	previous = NULL;
	delete iter;
	delete previous;
}

template <typename T>
activeEdge* polygon<T>::delNode(activeEdge *head, int y)
{
	activeEdge *retPointer = head;
	//ɾȥ y > ymax �Ľ��
	if (head == NULL)
		return head;
	if (head->next == NULL && head->ymax < y)
	{
		delete head;
		head = NULL;
		return head;
	}
	else if (head->next == NULL && head->ymax > y)
		return head;
	else
	{
		activeEdge *leader = new activeEdge(0, 0, y - 1);	//���ڴ������нڵ�
		leader->next = head;

		activeEdge *previous = leader;
		activeEdge *iter = head;

		bool set = true;		//�ж��Ƿ���ͷ�ڵ�
		while (iter != NULL)
		{
			if (previous->ymax >= y && set)
			{
				head = previous;
				set = false;	//ͷ�ڵ��������
				leader->next = NULL;
				delete leader;
				retPointer = head;
				/*
				head = iter;
				previous->next = NULL;
				delete previous;
				previous = iter;
				iter = iter->next;
				if (iter == NULL)
					break;
				*/
			}
			//ɾ���ڵ�
			if (iter->ymax < y)
			{
				previous->next = iter->next;
				delete iter;
				iter = previous->next;
			}
			else
			{
				iter = iter->next;
				previous = previous->next;
			}
			head = head;
		}
		previous = NULL;
		iter = NULL;
	}
	return retPointer;
}

template <typename T>
void polygon<T>::updateNode(activeEdge *head)
{
	if (head == NULL)
		return;
	activeEdge *iter = head;
	while (iter != NULL)
	{
		if (iter->firstIn) {
			iter->firstIn = false;
			continue;
		}
		//����x
		iter->x = iter->x + iter->deltaX;
		//iter->firstIn = false;
		iter = iter->next;
	}
}

template <typename T>
void polygon<T>::calIntersect(pair<T, T>* S, pair<T, T>* P, Edge clipEdge, pair<T, T>* crossPoint)
{
	if (clipEdge[0].second == clipEdge[1].second)
	{
		//ˮƽ�ü���
		crossPoint->second = clipEdge[0].second;
		crossPoint->first = S->first + (clipEdge[0].second - S->second)
			*(P->first - S->first) / (P->second - S->second);
	}
	else
	{
		//��ֱ�ü���
		crossPoint->first = clipEdge[0].first;
		crossPoint->second = S->second + (clipEdge[0].first - S->first)
			*(P->second - S->second) / (P->first - S->first);
	}
}

template <typename T>
bool polygon<T>::isInside(pair<T, T>* testNode, Edge clipEdge)
{
	//�ж�һ�������Ƿ��ڱߵ��ڲ�
	if (clipEdge[1].first > clipEdge[0].first)
	{
		//�ü���Ϊ�����±�
		if (testNode->second > clipEdge[0].second)
			return true;
	}
	else if (clipEdge[1].first < clipEdge[0].first)
	{
		//�ü���Ϊ�����ϱ�
		if (testNode->second <= clipEdge[0].second)
			return true;
	}
	else if (clipEdge[1].second > clipEdge[0].second)
	{
		//�ü���Ϊ�����ұ�
		if (testNode->first <= clipEdge[0].first)
			return true;
	}
	else if (clipEdge[1].second < clipEdge[0].second)
	{
		//�ü���Ϊ�������
		if (testNode->first >= clipEdge[0].first)
			return true;
	}
	return false;
}

template <typename T>
void polygon<T>::insert(pair<T, T>* node, int &position, vector<pair<T, T>> *targetArray)
{
	//��ָ��λ�ò���Ԫ�أ�����Ҫ�ж��Ƿ�Խ��
	//��������Ӧ���ǰ�˳�����β���ģ����ԡ���
	//Ҫ��һ���취�ܼӳ�targetArray
	//�������ҵ�������־ position ��ָ�����һ��λ��
	//��Ҫ����һ��������ʾ���������ռ��С
	pair<T, T> insNode(node->first, node->second);
	targetArray->push_back(insNode);
	position++;
}

template <typename T>
void polygon<T>::polyReccut(int XL, int XR, int YB, int YT)
{
	//����βü�,��������
	Edge cutEdges[4] = {
		{pair<T,T>(XL,YT),pair<T,T>(XL,YB)},
		{pair<T,T>(XR,YT),pair<T,T>(XL,YT)},
		{pair<T,T>(XR,YB),pair<T,T>(XR,YT)},
		{pair<T,T>(XL,YB),pair<T,T>(XR,YB)},
	};
	vector<pair<T, T>> *outputNodes;

	int outputLength = 0;
	for (int i = 0; i < 4; i++)
	{
		outputNodes = new vector<pair<T, T>>();
		sutherland_Hodgman_cut(this->nodes, outputNodes, cutEdges[i], endIndex, outputLength);
		delete[] nodes;
		nodes = new pair<T, T>[outputNodes->size() * sizeof(pair<T, T>)];
		std::copy(outputNodes->begin(), outputNodes->end(), nodes);
		this->endIndex = outputLength;
		outputNodes = NULL;
	}
	int updateY;
	ymax = ymin = 0;
	for (int j = 0; j < endIndex; j++)
	{
		updateY = nodes[j].second;
		//���� ymax �� ymin
		if (updateY > ymax)
			ymax = updateY;
		if (updateY < ymin)
			ymin = updateY;
	}
}

template <typename T>
void polygon<T>::sutherland_Hodgman_cut(pair<T, T>* inputNodes, vector<pair<T, T>> *outputNodes,
	Edge clipEdge, int inputLength, int &outputLength)
{
	//����ε��߲ü��㷨
	pair<T, T>*S = NULL;
	pair<T, T>*P = NULL;
	pair<T, T>*ip = new pair<T, T>();
	int j = 0;
	outputLength = 0;
	S = &inputNodes[inputLength - 1];
	for (j = 0; j < inputLength; j++)
	{
		P = &inputNodes[j];
		if (isInside(P, clipEdge))
		{
			if(isInside(S,clipEdge))
				//SP ͬʱ�ڴ���֮�ڣ����� P �������
				//����Ӧ���ǰѽڵ� P ���� outputNodes ָ����λ�� outLength ��
				insert(P, outputLength, outputNodes);
			else
			{
				//S �ڴ���֮��
				calIntersect(S, P, clipEdge, ip);
				insert(ip, outputLength, outputNodes);
				insert(P, outputLength, outputNodes);
			}
		}
		else if (isInside(S, clipEdge))
		{
			//S �ڴ���֮�ڣ�P�ڴ���֮��
			calIntersect(S, P, clipEdge, ip);
			insert(ip, outputLength, outputNodes);
		}
		S = P;
	}
}

#endif