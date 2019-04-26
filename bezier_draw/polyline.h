#pragma once

#ifndef _POLYLINE_
#define _POLYLINE_

#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <stack>

#include <gl/glut.h>

#define ILLEGAL_PARAMATER 1
#define UNDEFINED_METHOD 2

#define DRAW_POINTS 0
#define DRAW_LAST_POINT 1

#define BEZIER_STUPID 0
#define BEZIER_NEW 1

using namespace std;

template <typename T>
struct queueNode
{
	int index;
	T x;
	T y;
	queueNode(T& theX, T& theY, int& theIndex) : x(theX), y(theY), index(theIndex) {}
	friend bool operator < (const queueNode& firstNode, const queueNode& secondNode)
	{	//默认 (0, 0) 为原点，按照极角从小到大进行排序
		int x1 = firstNode.x;
		int y1 = firstNode.y;
		int x2 = secondNode.x;
		int y2 = secondNode.y;
		if (x1 * x2 > 0)	//两者在 y 轴同一侧
			return y1 * x2 > x1 * y2;
		else if(x1 * x2 < 0)//两者在 x 轴两侧
			return x1 < x2;
		else
		{	//某个顶点在 y 轴上
			if (x1 == 0)
				return x2 > 0;
			//return (x2 == 0) ? false : (x2 < 0);
			else if (x2 == 0)
				return x1 < 0;
				//return (x1 == 0) ? false : (x1 > 0);
			else
				return false;
		}
	}
};

template <typename T>
class polyline
{
public:
	polyline(const int& window_size, int numOfVertices = 1);
	~polyline() { delete[] nodes; };

	void addNode(const T &x, const T &y);	//增加顶点
	void rmNode(int seqNum);				//删除顶点

	int vertexes() { return endIndex; }		//顶点个数

	void drawPoints(const int type);	//绘制顶点
	void drawEdges();					//绘制多边形折线
	void drawBezier(const int &type);	//绘制贝塞尔曲线
	void drawConvexHull(GLubyte *color);//绘制折线的凸包

	//找到距离 (posx, posy) 最近的点（返回顶点）
	pair<T,T>& findNearestPoint(
		const int& posx, const int& posy,
		const int& radius, int& theIndex);
	//找到距离 (posx, posy) 最近的边（返回边起始顶点的下标）
	const int& findNearestEdge(const int& posx, const int& posy, const int& width);

	void setPByIndex(const int& posx, const int& posy, const int& theIndex)
	{
		nodes->at(theIndex).first = posx;
		nodes->at(theIndex).second = posy;

		calConvex();
	}
	void insertByIndex(const int& posx, const int& posy, const int& theIndex)
	{
		nodes->resize(++endIndex);
		nodes->insert(nodes->begin() + theIndex + 1, pair<int, int>(posx, posy));
	}
	void eraseByIndex(const int& theIndex)
	{
		if (theIndex < 0 || theIndex >= endIndex)
			return;
		nodes->erase(nodes->begin() + theIndex);
		nodes->resize(--endIndex);

		if(endIndex > 2)
			calConvex();
		else
		{
			convexNodes->clear();
			convexNodes->resize(0);
			convexIndex = 0;
		}
	}
	const pair<T, T>& getByIndex(const int& theIndex)
	{
		if (theIndex < 0 || theIndex >= endIndex)
			return pair<int, int>(-1, -1);
		return nodes->at(theIndex);
	}
	const int& size() { return endIndex; }
	void cleanNodes()
	{	//清空结点
		endIndex = 0;
		nodes->resize(1);
	}

private:
	int endIndex;	//数组结尾指针
	//存放结点个数的数组，两个分量分别为 x 和 y
	vector<pair<T, T> >* nodes;
	vector<pair<T, T> >* convexNodes;
	int convexIndex;	//convexNodes 的长度
	int windowSize;		//窗口尺寸

	long int combination(const long int& m, const long int& n);	//计算组合数
	long int factorial(const int &n);		//阶乘
	double B_i_n(const int& i,const double& t);		//计算点的权重
	const bool& calConvex();	//计算凸包
	const bool& isLeft(const int& srcIndex, const int& destIndex, const int& judgeIndex);
};

template <typename T>
polyline<T>::polyline(const int& windowSize, int numOfVertices)
{	//建立顶点个数为 numOfVertices 的多边形
	if (windowSize <= 0)
	{
		cout << "窗口大小不应当小于 0" << endl;
		throw ILLEGAL_PARAMATER;
	}
	if (numOfVertices < 0)
	{
		cout << "顶点数不应当小于 0" << endl;
		throw ILLEGAL_PARAMATER;
	}

	this->windowSize = windowSize;
	nodes = new vector<pair<T, T> >();
	nodes->resize(numOfVertices);
	//初始化凸包
	convexNodes = new vector<pair<T, T> >();
	convexNodes->resize(numOfVertices);
	convexIndex = 0;
	//初始化序列
	
	pair<T, T> *node;
	for (int i = 0; i < nodes->size(); i++) {
		node = &(nodes->at(i));
		node->first = 0;
		node->second = 0;

		node = &(convexNodes->at(i));
		node->first = 0;
		node->second = 0;
	}
	endIndex = 0;
}

template <typename T>
void polyline<T>::addNode(const T&x, const T&y)
{
	if (endIndex == nodes->size())	//开辟更多的空间
		nodes->resize(endIndex * 2 + 1);
	nodes->at(endIndex).first = x;
	nodes->at(endIndex).second = y;
	endIndex++;

	if (endIndex > 2)
		calConvex();
	else
	{
		convexNodes->clear();
		convexNodes->resize(0);
		convexIndex = 0;
	}
}

template <typename T>
void polyline<T>::rmNode(int seqNum)
{	//删除第 seqNum 个结点（1起）
	//判断结点
	if (seqNum < 1 || seqNum > endIndex) {
		cout << "非法参数" << endl;
		throw ILLEGAL_PARAMATER;
	}
	int seq = seqNum - 1;

	nodes[endIndex].first = 0;
	nodes[endIndex].second = 0;
	for (int i = seq; i < endIndex; i++)
		nodes[i] = nodes[i + 1];
	endIndex--;
	//数组容量调整
	if ((endIndex + 1) < (int)(nodes->size() / 2))
		nodes->resize(nodes->size() / 2);	//删除重复空间

	calConvex();
}

template <typename T>
void polyline<T>::drawPoints(const int type)
{
	glColor3ub(255, 0, 0);	//红色
	glPointSize(5);			//顶点大小
	glBegin(GL_POINTS);

	if (type == DRAW_POINTS)
	{
		for (int i = 0; i < endIndex; i++)
			glVertex2d((GLdouble)nodes->at(i).first, (GLdouble)nodes->at(i).second);
	}
	else if (type == DRAW_LAST_POINT)
	{
		glVertex2d((GLdouble)nodes->at(endIndex - 1).first, (GLdouble)nodes->at(endIndex - 1).second);
	}
	else
		return;
	glColor3ub(255, 255, 255);
	glPointSize(1);
	glEnd();
	glFlush();
}

template <typename T>
void polyline<T>::drawEdges()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);		//不可以清空缓冲区，否则之前的点会被删除
	glViewport(0, 0, 600, 600);
	glLineWidth(2.0);
	//按照边绘制
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < endIndex; i++)
		glVertex2f(nodes->at(i).first, nodes->at(i).second);
	glEnd();
	glLineWidth(1.0);
	glFlush();
}

template <typename T>
void polyline<T>::drawBezier(const int& type)
{	//绘制贝塞尔曲线
	glColor3ub(0, 255, 0);
	float t = 0.0;
	const float step = 0.02;
	double posx = 0.0;
	double posy = 0.0;
	double weight = 0.0;

	glPointSize(2.0);

	if (type == BEZIER_STUPID)
	{	//传统方法绘制贝塞尔曲线
		while (t < (1.0 + step /2))
		{
			posx = posy = 0.0;
			for (int i = 0; i < endIndex; i++)
			{	//计算单个点
				weight = B_i_n(i, t);
				posx += (double)nodes->at(i).first * weight;
				posy += (double)nodes->at(i).second * weight;
			}
			glBegin(GL_POINTS);
			glVertex2d(posx, posy);
			glEnd();
			glFlush();
			t += step;
		}
	}
	else if (type == BEZIER_NEW)
	{	//将结点放在数组中
		vector<pair<double, double> > *store = new vector<pair<double, double>>();
		store->resize(endIndex);
		//复制，此处为避免精度损失采用 double 类型
		for (int i = 0; i < endIndex; i++)
		{
			store->at(i).first = (double)nodes->at(i).first;
			store->at(i).second = (double)nodes->at(i).second;
		}
		vector<pair<double, double> >::iterator prev;	//指向头部
		vector<pair<double, double> >::iterator next;	//指向次头部
		double posx = 0.0, posy = 0.0;		//迭代点坐标

		glColor3ub(255, 0, 0);	//设定曲线颜色为红色

		glLineWidth(2);
		glBegin(GL_LINE_STRIP);
		while (t < (1.0 + step / 2))
		{
			for (int i = 0; i < endIndex; i++)
			{
				store->at(i).first = (double)nodes->at(i).first;
				store->at(i).second = (double)nodes->at(i).second;
			}
			for (int i = 0; i < endIndex - 1; i++)
			{
				prev = store->begin();	//指向头部
				next = store->begin();	//指向次头部
				next++;
				for (int j = 0; j < endIndex - i - 1; j++)
				{
					posx = (1.0 - t) * (prev->first) + t * (next->first);
					posy = (1.0 - t) * (prev->second) + t * (next->second);
					prev->first = posx;
					prev->second = posy;
					prev++;
					next++;
				}
			}
			//GLubyte r = t * t * 255;
			//GLubyte b = 2 * t * (1 - t) * 255;
			//GLubyte g = (1 - t) * (1 - t) * 255;
			//glColor3ub(r, g, b);
			glVertex2d(posx, posy);
			t += step;
		}
		glEnd();
		glFlush();
		glLineWidth(1.0);
		delete store;
	}
	else			//等待完善
		return;
	
	glColor3ub(255, 255, 255);
}

template <typename T>
void polyline<T>::drawConvexHull(GLubyte *color)
{
	calConvex();
	if (convexNodes->size() < 3)
		return;
	GLubyte r = color[0];
	GLubyte g = color[1];
	GLubyte b = color[2];
	//绘制所在的凸包
	glColor3ub(r, g, b);
	glBegin(GL_POLYGON);
	pair<T, T> *iter = NULL;
	pair<T, T>* drawPoint = NULL;
	for (int i = 0; i < convexNodes->size(); i++)
	{
		glVertex2i(convexNodes->at(i).first, convexNodes->at(i).second);
	}
	glEnd();
	glFlush();
	glColor3ub(255, 255, 255);
}

template <typename T>
long int polyline<T>::factorial(const int& n)
{	//求阶乘函数
	if (n == 0)
		return 1;
	long int res = 1;
	for (int i = 1; i <= n; i++)
		res *= i;
	return res;
}

template <typename T>
long int polyline<T>::combination(const long int& m, const long int& n)
{
	long int res;
	res = factorial(m) / (factorial(n) * factorial(m - n));
	return res;
}

template <typename T>
double polyline<T>::B_i_n(const int& i,const double& t)
{
	long int comb = combination(endIndex - 1, i);
	double res = (double)comb * pow(t, i) * pow(1 - t, endIndex - 1 - i);
	return res;
}

template <typename T>
pair<T, T>& polyline<T>::findNearestPoint(
	const int& posx, const int& posy,
	const int& radius, int& theIndex)
{
	theIndex = -1;
	static pair<T, T> node(-1, -1);
	for (int i = 0; i < endIndex; i++)
	{
		int disx2 = (nodes->at(i).first - posx) * (nodes->at(i).first - posx);
		int disy2 = (nodes->at(i).second - posy) * (nodes->at(i).second - posy);
		if (disx2 + disy2 < radius * radius)
		{
			node.first = nodes->at(i).first;
			node.second = nodes->at(i).second;
			theIndex = i;
			return node;
		}
		else
		{
			node.first = -1;
			node.second = -1;
			theIndex == -1;
		}
	}
	return node;
}

template <typename T>
const int& polyline<T>::findNearestEdge(
	const int& posx, const int& posy, const int& width)
{	//找到距离顶点 (posx, posy) 最近的距离小于 width/2 的顶点，并返回起始下标
	if (nodes->size() <= 1)
		return -1;
	int index = -1;
	int x1, y1, x2, y2;
	int midx, midy;
	int radiusSquare, distanceSquare;
	int alpha = -1;		//比较因子
	int distance = -1;	//点到直线的距离
	int hWidth = width / 2;	//宽度的一半
	for (int i = 0; i < endIndex - 1; i++)
	{
		x1 = nodes->at(i).first;
		y1 = nodes->at(i).second;
		x2 = nodes->at(i + 1).first;
		y2 = nodes->at(i + 1).second;
		midx = (x1 + x2) / 2;	//中点横坐标
		midy = (y1 + y2) / 2;	//中点纵坐标
		radiusSquare = (x1 - midx) * (x1 - midx) + (y1 - midy) * (y1 - midy);
		distanceSquare = (posx - midx) * (posx - midx) + (posy - midy) * (posy - midy);
		
		//判断点是否在圆外
		if (distanceSquare > (radiusSquare - width))
			index = -1;
		else
		{	//判断是否在 width 之内
			alpha = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
			distance = abs((y2 - y1)*(posx - x1) - (posy - y1)*(x2 - x1));
			if (distance > hWidth * alpha)
				index = -1;
			else
				return i;
		}
	}
	return -1;
}

template <typename T>
const bool& polyline<T>::calConvex()
{	//求顶点集的凸包
	if (endIndex < 3)
		return false;
	convexNodes->clear();
	convexNodes->resize(0);
	convexIndex = 0;
	int minYindex = 0;
	stack<int> *nodeStack = new stack<int>();
	//从所有顶点中查找纵坐标最小的
	for (int i = 0; i < endIndex; i++)
	{
		if (nodes->at(i).second < nodes->at(minYindex).second)
			minYindex = i;
	}
	pair<T, T> P0 = nodes->at(minYindex);
	//对剩余顶点按照极角进行排序，创建优先队列
	priority_queue<queueNode<int>, vector<queueNode<int> >, less<queueNode<int> > > *angle_queue
		= new priority_queue<queueNode<int>, vector<queueNode<int> >, less<queueNode<int> > >();
	//按照极角进行排序
	for (int i = 0; i < endIndex; i++)
	{
		if (i == minYindex)
			continue;
		int deltaX = nodes->at(i).first - P0.first;
		int deltaY = nodes->at(i).second - P0.second;
		queueNode<int> insertNode(deltaX, deltaY, i);
		angle_queue->push(insertNode);
	}
	//压入 P0
	nodeStack->push(minYindex);
	//压入 P1
	nodeStack->push(angle_queue->top().index);
	angle_queue->pop();
	//压入 P2
	nodeStack->push(angle_queue->top().index);
	angle_queue->pop();
	//for (int i = 0; i < angle_queue->size(); i++)
	while(!angle_queue->empty())
	{	//Pi
		int Pi = angle_queue->top().index;
		angle_queue->pop();

		int topIndex = nodeStack->top();
		nodeStack->pop();
		int nextIndex = nodeStack->top();
		nodeStack->push(topIndex);

		while (!isLeft(nextIndex, topIndex, Pi))
		{
			nodeStack->pop();

			topIndex = nodeStack->top();
			nodeStack->pop();
			nextIndex = nodeStack->top();
			nodeStack->push(topIndex);
		}
		nodeStack->push(Pi);
	}
	while (!nodeStack->empty())
	{
		convexNodes->push_back(nodes->at(nodeStack->top()));
		nodeStack->pop();
		convexIndex++;
	}
	delete nodeStack;
	delete angle_queue;
}

template <typename T>
const bool& polyline<T>::isLeft(const int& srcIndex, const int& destIndex, const int& judgeIndex)
{	//判断点 judgeIndex 在向量 (srcIndex, destIndex) 的左方还是右方
	pair<T, T> node1 = nodes->at(srcIndex);
	pair<T, T> node2 = nodes->at(destIndex);
	pair<T, T> node = nodes->at(judgeIndex);
	int p1 = (node2.first - node1.first) * (node.second - node1.second);
	int p2 = (node2.second - node1.second) * (node.first - node1.first);
	return p1 > p2;
}

#endif