#pragma once

#ifndef _POLYLINE_
#define _POLYLINE_

#include <iostream>
#include <vector>
#include <cmath>

#include <gl/glut.h>

#define ILLEGAL_PARAMATER 1
#define UNDEFINED_METHOD 2

#define DRAW_POINTS 0
#define DRAW_LAST_POINT 1

#define BEZIER_STUPID 0
#define BEZIER_NEW 1
/*
#define abs(x) ((x)>0?(x):-(x))
#define max(a,b) ((a)>(b))?(a):(b)
#define min(a,b) ((a)<(b))?(a):(b)
*/
using namespace std;

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

	void cleanNodes()
	{	//清空结点
		endIndex = 0;
		nodes->resize(1);
	}

private:
	int endIndex;	//数组结尾指针
	//存放结点个数的数组，两个分量分别为 x 和 y
	vector<pair<T, T>> *nodes;
	int windowSize;	//窗口尺寸

	long int combination(const long int& m, const long int& n);	//计算组合数公式
	long int factorial(const int &n);		//求n的阶乘
	double B_i_n(const int& i,const double& t);		//计算点的权重
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
	nodes = new vector<pair<T, T>>();
	nodes->resize(numOfVertices);
	pair<T, T> *node;
	for (int i = 0; i < nodes->size(); i++) {
		node = &(nodes->at(i));
		node->first = 0;
		node->second = 0;
	}
	endIndex = 0;
}

template <typename T>
void polyline<T>::addNode(const T&x, const T&y)
{
	if (endIndex == nodes->size())	//开辟更多的空间
		nodes->resize(nodes->size() * 2);
	nodes->at(endIndex).first = x;
	nodes->at(endIndex).second = windowSize - y;
	endIndex++;
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
	//按照边绘制
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < endIndex; i++)
		glVertex2f(nodes->at(i).first, nodes->at(i).second);
	glEnd();
	glFlush();
}

template <typename T>
void polyline<T>::drawBezier(const int& type)
{	//绘制贝塞尔曲线
	glColor3ub(0, 255, 0);
	float t = 0.0;
	const float step = 0.0001;
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
		vector<pair<double, double>> *store = new vector<pair<double, double>>();
		store->resize(endIndex * (endIndex + 1) / 2);
		//复制，此处为避免精度损失采用 double 类型
		for (int i = 0; i < endIndex; i++)
		{
			store->at(i).first = (double)nodes->at(i).first;
			store->at(i).second = (double)nodes->at(i).second;
		}
		//初始化数组迭代器
		typename vector<pair<double, double>>::iterator prev = store->begin();	//指向头部
		typename vector<pair<double, double>>::iterator next = store->begin();	//指向次头部
		next++;

		typename vector<pair<double, double>>::iterator index = store->begin() + endIndex;	//指向修改位置
		int loopTimes = endIndex - 1;	//生成 loopTimes 组点
		int iterTimes = 0;	//生成一组点的迭代次数
		while (t < (1.0 + step / 2))
		{	//循环生成点
			posx = posy = 0;	//绘制点初值
			iterTimes = 0;		//生成一组点的迭代次数
			prev = store->begin();	//头部
			next = prev + 1;		//次头部
			index = store->begin() + endIndex;	//修改位置
			loopTimes = endIndex - 1;

			while (index != store->end()) {
				posx = (1.0 - t) * (double)(prev->first) + t * (double)(next->first);
				posy = (1.0 - t) * (double)(prev->second) + t * (double)(next->second);
				index->first = posx;
				index->second = posy;

				index++;	//修改位置增一
				prev++;		//头部增一
				next++;		//次头部增一
				iterTimes++;//迭代次数增一

				if (iterTimes == loopTimes)
				{	//说明一组点已经生成结束
					prev++;	//头部自增
					next++;	//次头部自增
					loopTimes--;	//生成组数减一
					iterTimes = 0;	//迭代次数归零
				}
			}
			GLubyte r = t * t * 255;
			GLubyte b = 2 * t * (1 - t) * 255;
			GLubyte g = (1 - t) * (1 - t) * 255;
			glColor3ub(r, g, b);
			glBegin(GL_POINTS);
			glVertex2d(posx, posy);
			glEnd();
			glFlush();
			t += step;
		}
		delete store;
	}
	else			//等待完善
		return;
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

#endif