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

	void addNode(const T &x, const T &y);	//���Ӷ���
	void rmNode(int seqNum);				//ɾ������

	int vertexes() { return endIndex; }		//�������

	void drawPoints(const int type);	//���ƶ���
	void drawEdges();					//���ƶ��������
	void drawBezier(const int &type);	//���Ʊ���������
	void drawConvexHull(GLubyte *color);//�������ߵ�͹��

	pair<T,T>& findNearestPoint(
		const int& posx, const int& posy,
		const int& radius, int& theIndex);

	void setPByIndex(const int& posx, const int& posy, const int& theIndex)
	{
		nodes->at(theIndex).first = posx;
		nodes->at(theIndex).second = posy;
	}

	void cleanNodes()
	{	//��ս��
		endIndex = 0;
		nodes->resize(1);
	}

private:
	int endIndex;	//�����βָ��
	//��Ž����������飬���������ֱ�Ϊ x �� y
	vector<pair<T, T> > *nodes;
	int windowSize;	//���ڳߴ�

	long int combination(const long int& m, const long int& n);	//�����������ʽ
	long int factorial(const int &n);		//��n�Ľ׳�
	double B_i_n(const int& i,const double& t);		//������Ȩ��
};

template <typename T>
polyline<T>::polyline(const int& windowSize, int numOfVertices)
{	//�����������Ϊ numOfVertices �Ķ����
	if (windowSize <= 0)
	{
		cout << "���ڴ�С��Ӧ��С�� 0" << endl;
		throw ILLEGAL_PARAMATER;
	}
	if (numOfVertices < 0)
	{
		cout << "��������Ӧ��С�� 0" << endl;
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
	if (endIndex == nodes->size())	//���ٸ���Ŀռ�
		nodes->resize(nodes->size() * 2);
	nodes->at(endIndex).first = x;
	nodes->at(endIndex).second = windowSize - y;
	endIndex++;
}

template <typename T>
void polyline<T>::rmNode(int seqNum)
{	//ɾ���� seqNum ����㣨1��
	//�жϽ��
	if (seqNum < 1 || seqNum > endIndex) {
		cout << "�Ƿ�����" << endl;
		throw ILLEGAL_PARAMATER;
	}
	int seq = seqNum - 1;

	nodes[endIndex].first = 0;
	nodes[endIndex].second = 0;
	for (int i = seq; i < endIndex; i++)
		nodes[i] = nodes[i + 1];
	endIndex--;
	//������������
	if ((endIndex + 1) < (int)(nodes->size() / 2))
		nodes->resize(nodes->size() / 2);	//ɾ���ظ��ռ�
}

template <typename T>
void polyline<T>::drawPoints(const int type)
{
	glColor3ub(255, 0, 0);	//��ɫ
	glPointSize(5);			//�����С
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
	//glClear(GL_COLOR_BUFFER_BIT);		//��������ջ�����������֮ǰ�ĵ�ᱻɾ��
	glViewport(0, 0, 600, 600);
	//���ձ߻���
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < endIndex; i++)
		glVertex2f(nodes->at(i).first, nodes->at(i).second);
	glEnd();
	glFlush();
}

template <typename T>
void polyline<T>::drawBezier(const int& type)
{	//���Ʊ���������
	glColor3ub(0, 255, 0);
	float t = 0.0;
	const float step = 0.02;
	double posx = 0.0;
	double posy = 0.0;
	double weight = 0.0;

	glPointSize(2.0);

	if (type == BEZIER_STUPID)
	{	//��ͳ�������Ʊ���������
		while (t < (1.0 + step /2))
		{
			posx = posy = 0.0;
			for (int i = 0; i < endIndex; i++)
			{	//���㵥����
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
	{	//��������������
		vector<pair<double, double> > *store = new vector<pair<double, double>>();
		store->resize(endIndex);
		//���ƣ��˴�Ϊ���⾫����ʧ���� double ����
		for (int i = 0; i < endIndex; i++)
		{
			store->at(i).first = (double)nodes->at(i).first;
			store->at(i).second = (double)nodes->at(i).second;
		}
		vector<pair<double, double> >::iterator prev;	//ָ��ͷ��
		vector<pair<double, double> >::iterator next;	//ָ���ͷ��
		double posx = 0.0, posy = 0.0;		//����������

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
				prev = store->begin();	//ָ��ͷ��
				next = store->begin();	//ָ���ͷ��
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
			GLubyte r = t * t * 255;
			GLubyte b = 2 * t * (1 - t) * 255;
			GLubyte g = (1 - t) * (1 - t) * 255;
			glColor3ub(r, g, b);
			glVertex2d(posx, posy);
			t += step;
		}
		glEnd();
		glFlush();
		glLineWidth(1.0);
		delete store;
	}
	else			//�ȴ�����
		return;
	
	glColor3ub(255, 255, 255);
}

template <typename T>
void polyline<T>::drawConvexHull(GLubyte *color)
{
	GLubyte r = color[0];
	GLubyte g = color[1];
	GLubyte b = color[2];
	//�������ڵ�͹��
	//����͹��

}

template <typename T>
long int polyline<T>::factorial(const int& n)
{	//��׳˺���
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

#endif