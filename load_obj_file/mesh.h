#pragma once
#ifndef _MESH_
#define _MESH_

#include "dataStructure.h"
#include "wrappedTools.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <cmath>
#include <thread>

#include <gl/glut.h>

using std::cin;
using std::cout;
using std::endl;
using std::ios;
using std::bitset;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

using std::thread;

using std::cos;
using std::sin;

const static double PI = 3.1415926535897;
const int padding = 1;

const unsigned char DRAW_VERTEX = 0x01;			//���ƶ���
const unsigned char DRAW_STRUCTURE = 0x02;		//���ƹǼ�
const unsigned char DRAW_SURF_NOTEXTURE = 0x04;	//���Ʊ��棨������
const unsigned char DRAW_SURF_TEXTURE = 0x08;	//���Ʊ��棨������
const unsigned char DRAW_SURF_LIGHT = 0x10;		//���Ʊ��棨�����գ�
const unsigned char DRAW_OPTION1 = 0x20;		//δ֪ѡ��1
const unsigned char DRAW_OPTION2 = 0x40;		//δ֪ѡ��2
const unsigned char DRAW_OPTION3 = 0x80;		//δ֪ѡ��3

const unsigned char ROTATE_X_CLOCK = 0x01;		//�� X ����ת��˳ʱ�룩
const unsigned char ROTATE_X_ANTICLOCK = 0X02;	//�� X ����ת����ʱ�룩
const unsigned char ROTATE_Y_CLOCK = 0X04;		//�� Y ����ת��˳ʱ�룩
const unsigned char ROTATE_Y_ANTICLOCK = 0X08;	//�� Y ����ת����ʱ�룩
const unsigned char ROTATE_Z_CLOCK = 0x10;		//�� Z ����ת��˳ʱ�룩
const unsigned char ROTATE_Z_ANTICLOCK = 0X20;	//�� Z ����ת����ʱ�룩

class mesh
{
public:
	mesh();
	~mesh();
	bool initData(const char* fileName);

	bool draw(const unsigned char& type, const GLubyte* color);
	//ת�� radian ����
	void rotateRadian(const unsigned char& type, const double& radian);
	//ת�� degree �Ƕ�
	void rotateDegree(const unsigned char& type, const int& degree);
	//ÿ��ת�� speed ���ȣ�һ��ת�� radian �����ȣ���� radian Ϊ������һֱ��ת
	void rotateRadian(const unsigned char& type, const double& radian, const float& speed);
	//ÿ��ת�� speed �Ƕȣ�һ��ת�� degree ���Ƕȣ���� degree Ϊ������һֱ��ת
	void rotateDegree(const unsigned char& type, const int& degree, const int& speed);
	//��ת�����
	void rotateCamera(const unsigned char& type, const int& degree);
	void setView();
private:
	//��� DRAW �����ṩ�ӿ�
	const enum DRAW_TYPE {
		VERTEX, STRUCTURE, SURF_NOTEXTURE, SURF_TEXTURE,
		SURF_LIGHT, OPTION1, OPTION2, OPTION3
	};

	vector<vertex> *vertexList;		//��������
	vector<verNorm> *verNormList;	//���㷨������
	vector<verTexture> *verTextList;//������������
	vector<vector<face> > *faceList;//��ṹ����

	int vertexItems;	//�������
	int verNormItems;	//���㷨�߸���
	int verTextItems;	//�����������
	int faceItems;		//�����

	ifstream inFile;	//�ļ�������

	bool extentSpace();	//�������Ŀռ�
	void dispInfo();	//��ʾ������Ϣ
	void typeDraw(const DRAW_TYPE& type, const GLubyte* color);
	//���߷���
	void drawVertex(const GLubyte* color);
	void drawEdges(const GLubyte* color);

	//��ʾ����ϵ
	void showCoordinate();

	//������ʾ����
	double minX, maxX, minY, maxY, minZ, maxZ;
	double maxR;

	double alpha, beta, gamma;	//���������Ƕȣ���������ϵ��
	double cameraX, cameraY, cameraZ;	//���������λ��
	double cameraAlpha, cameraBeta, cameraGamma;

	unsigned char type;	//��һ�β���������
};

mesh::mesh()
{
	vertexList = new vector<vertex>();
	verNormList = new vector<verNorm>();
	verTextList = new vector<verTexture>();
	faceList = new vector<vector<face> >();

	vertexItems = 0;
	verNormItems = 0;
	verTextItems = 0;
	faceItems = 0;

	minX = maxX = minY = maxY = minZ = maxZ = 0.0;
	maxR = 0.0;

	cameraX = 0;
	cameraY = 0;
	cameraZ = 0;
	cameraAlpha = 0.0;
	cameraBeta = 0.0;
	cameraGamma = 0.0;
}

mesh::~mesh()
{
	delete vertexList;
	delete verNormList;
	delete verTextList;
	delete faceList;

	vertexList = NULL;
	verNormList = NULL;
	verTextList = NULL;
	faceList = NULL;
}

bool mesh::initData(const char* fileName)
{
	inFile.open(fileName, ios::in | ios::out);
	if (!inFile.is_open())
	{
		cout << "���ļ� " << fileName << " ʧ��" << endl;
		return false;
	}
	minX = maxX = minY = maxY = minZ = maxZ = 0.0;
	//ѭ����ȡ�ļ�
	cout << "���ڴ� " << fileName << " ��������" << endl;
	string buffer;
	char str[100], one;
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	while (!inFile.eof())
	{
		extentSpace();	//����ռ�
		inFile.getline(str, 100, ' ');
		buffer = string(str);
		if (buffer.compare("#") == 0)
		{//���ſ�ͷ��ע�ͣ��������в�ʡ��
			getline(inFile, buffer);
			one = inFile.get();	//�������з�
		}
		else if (buffer.compare("vt") == 0)
		{	//������ͼ����
			inFile.getline(str, 100, ' ');
			x = atof(str);
			inFile.getline(str, 100, '\n');
			y = atof(str);

			verTextList->at(verTextItems).x = x;
			verTextList->at(verTextItems).y = y;
			verTextItems++;
		}
		else if (buffer.compare("vn") == 0)
		{	//������
			inFile.getline(str, 100, ' ');
			x = atof(str);
			inFile.getline(str, 100, ' ');
			y = atof(str);
			inFile.getline(str, 100, '\n');
			z = atof(str);

			verNormList->at(verNormItems).nx = x;
			verNormList->at(verNormItems).ny = y;
			verNormList->at(verNormItems).nz = z;
			verNormItems++;
		}
		else if (buffer.compare("v") == 0)
		{	//��������
			inFile.getline(str, 100, ' ');
			x = atof(str);
			inFile.getline(str, 100, ' ');
			y = atof(str);
			inFile.getline(str, 100, '\n');
			z = atof(str);

			vertexList->at(vertexItems).x = x;
			vertexList->at(vertexItems).y = y;
			vertexList->at(vertexItems).z = z;
			vertexItems++;

			if (x < minX)
				minX = x;
			if (x > maxX)
				maxX = x;
			if (y < minY)
				minY = y;
			if (y > maxY)
				maxY = y;
			if (z < minZ)
				minZ = z;
			if (z > maxZ)
				maxZ = z;
			if (maxX > maxR)
				maxR = maxX;
			if (maxY > maxR)
				maxR = maxY;
			if (maxZ > maxR)
				maxR = maxZ;
		}
		else if (buffer.compare("f") == 0)
		{	//f ��������/uv������/��������
			one = inFile.get();
			int value = 0;
			face insertFace;
			while (one != '\n')
			{
				for (int i = 0; i < 3; i++)
				{
					while (one != '/')
					{
						if (one == ' ')
							break;
						if (one == '\n')
							break;
						value = value * 10 + (one - '0');
						one = inFile.get();
					}
					insertFace.getByIndex(i) = value;
					value = 0;
					if (one == '\n')
						break;
					one = inFile.get();
				}
				faceList->at(faceItems).push_back(insertFace);
			}
			faceItems++;
		}
		else if (buffer.compare("g") == 0)
		{
			getline(inFile, buffer);
		}
		else
		{
			getline(inFile, buffer);
		}
	}
	vertexList->resize(vertexItems);
	verNormList->resize(verNormItems);
	verTextList->resize(verTextItems);
	faceList->resize(faceItems);
	cout << "�������" << endl;
	dispInfo();
	inFile.close();
	//glOrtho(minX - padding, maxX + padding, minY - padding, maxY + padding, minZ - padding, maxZ + padding);
}

void mesh::setView()
{
	glMatrixMode(GL_PROJECTION);
	glOrtho(-maxR - padding, maxR + padding, -maxR - padding, maxR + padding, -maxR - padding, maxR * 2 + padding);
	glMatrixMode(GL_MODELVIEW);
	cameraX = maxX;
	cameraY = maxY;
	cameraZ = maxZ;
	gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

//��������ϵ
void mesh::showCoordinate()
{
	//��ɫ X �ᣬ��ɫ Y �ᣬ��ɫ Z ��
	double axis[3][3] = { {maxR + padding, 0, 0}, {0, maxR + padding, 0}, {0, 0, maxR + padding} };
	GLubyte colors[3][3] = { {0,0,255},{0,255,0},{255,0,0} };
	glLineWidth(2);
	glBegin(GL_LINES);
	for (int i = 0; i < 3; i++)
	{
		glColor3ub(colors[i][0], colors[i][1], colors[i][2]);
		glVertex3d(0, 0, 0);
		glVertex3d(axis[i][0], axis[i][1], axis[i][2]);
	}
	glEnd();
	glFlush();
	glLineWidth(1);
	glColor3ub(defColor[0], defColor[1], defColor[2]);
}

bool mesh::draw(const unsigned char& type, const GLubyte* color)
{
	if (type == 0x00)
	{
		cout << "���ʹ��� " << (int)type << endl;
		return false;
	}
	this->type = type;
	bitset<8> bits(type);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	showCoordinate();
	for (int i = 7; i >= 0; i--)
	{
		if (bits.test(i) == 1)
			typeDraw(DRAW_TYPE(i), color);
	}
	glutSwapBuffers();
}

//������ʽ
void mesh::typeDraw(const DRAW_TYPE& type, const GLubyte* color)
{
	switch (type)
	{
	case VERTEX:		drawVertex(color);
		break;
	case STRUCTURE:		drawEdges(color);
		break;
	case SURF_NOTEXTURE:
		break;
	case SURF_TEXTURE:
		break;
	case SURF_LIGHT:
		break;
	case OPTION1:
		break;
	case OPTION2:
		break;
	case OPTION3:
		break;
	}
}

//���ƶ���
void mesh::drawVertex(const GLubyte* color)
{
	cout << "���ƶ���" << endl;
	drawVertexes(vertexList, color);
	cout << "�������" << endl;
}

//���Ʊߺ���
void mesh::drawEdges(const GLubyte* color)
{
	glutSolidCube(1);

	GLubyte r = color[0];
	GLubyte g = color[1];
	GLubyte b = color[2];
	glColor3ub(r, g, b);
	//glLineWidth(2);

	vector<face> *iter = NULL;
	face *innerIter = NULL;
	vertex *vertexIter = NULL;
	for (int i = 0; i < faceItems; i++)
	{
		glBegin(GL_LINE_LOOP);
		//glBegin(GL_POLYGON);
		iter = &(faceList->at(i));
		for (int j = 0; j < iter->size(); j++)
		{
			innerIter = &(iter->at(j));
			vertexIter = &(vertexList->at(innerIter->vertIndex - 1));
			glVertex3d(vertexIter->x, vertexIter->y, vertexIter->z);
		}
		glEnd();
		glFlush();
	}
	//glLineWidth(1);
	glColor3ub(255, 255, 255);
}

//��ʾ������Ϣ
void mesh::dispInfo()
{
	cout << "������Ϣ" << endl;
	cout << "��� " << vertexList->size() << " ��" << endl;
	cout << "��ͼ " << verTextList->size() << " ��" << endl;
	cout << "���� " << verNormList->size() << " ��" << endl;
	cout << "��Ƭ " << faceList->size() << " ��" << endl;
}

//��չ�ڴ�
bool mesh::extentSpace()
{
	try {
		if (vertexList->size() == vertexItems)
			vertexList->resize(2 * vertexItems + 1);
		if (verNormList->size() == verNormItems)
			verNormList->resize(2 * verNormItems + 1);
		if (verTextList->size() == verTextItems)
			verTextList->resize(2 * verTextItems + 1);
		if (faceList->size() == faceItems)
			faceList->resize(2 * faceItems + 1);
	}
	catch (std::bad_alloc)
	{
		cout << "�ڴ治�㣬�����ڴ�ʧ��" << endl;
		return false;
	}
	return true;
}

//��ת
void mesh::rotateDegree(const unsigned char& type, const int& degree)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	showCoordinate();
	double radius = (double)degree * PI / 180.0;
	double cosAngle = 0.0;
	double sinAngle = 0.0;

	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	vector<face> *faceIter = NULL;
	face *innerIter = NULL;
	vertex *vertexIter = NULL;

	switch (type)
	{
	case ROTATE_X_CLOCK:
		alpha += radius;
		cosAngle = cos(alpha);
		sinAngle = sin(alpha);
		break;
	case ROTATE_X_ANTICLOCK:
		alpha -= radius;
		cosAngle = cos(alpha);
		sinAngle = sin(alpha);
		break;
	case ROTATE_Y_CLOCK:
		beta += radius;
		cosAngle = cos(beta);
		sinAngle = sin(beta);
		break;
	case ROTATE_Y_ANTICLOCK:
		beta -= radius;
		cosAngle = cos(beta);
		sinAngle = sin(beta);
		break;
	case ROTATE_Z_CLOCK:
		gamma += radius;
		cosAngle = cos(gamma);
		sinAngle = sin(gamma);
		break;
	case ROTATE_Z_ANTICLOCK:
		gamma -= radius;
		cosAngle = cos(gamma);
		sinAngle = sin(gamma);
		break;
	}

	for (int i = 0; i < faceItems; i++)
	{
		glBegin(GL_LINE_LOOP);
		faceIter = &(faceList->at(i));
		for (int j = 0; j < faceIter->size(); j++)
		{
			innerIter = &(faceIter->at(j));
			vertexIter = &(vertexList->at(innerIter->vertIndex - 1));
			x = vertexIter->x;
			y = vertexIter->y;
			z = vertexIter->z;
			switch (type)
			{
			case ROTATE_X_CLOCK:
			case ROTATE_X_ANTICLOCK:
				glVertex3d(x, y * cosAngle - z * sinAngle, y * sinAngle + z * cosAngle);
				break;
			case ROTATE_Y_CLOCK:
			case ROTATE_Y_ANTICLOCK:
				glVertex3d(x * cosAngle + z * sinAngle, y, -x * sinAngle + z * cosAngle);
				break;
			case ROTATE_Z_CLOCK:
			case ROTATE_Z_ANTICLOCK:
				glVertex3d(x * cosAngle - y * sinAngle, x * sinAngle + y * cosAngle, z);
				break;
			}
		}
		glEnd();
		glFlush();
	}
	glutSwapBuffers();
}

//��ת�����
void mesh::rotateCamera(const unsigned char& type, const int& degree)
{
	double camerax = cameraX;
	double cameray = cameraY;
	double cameraz = cameraZ;
	double radius = degree * PI / 180.0;
	switch (type)
	{
	case ROTATE_X_CLOCK:
		cameraAlpha += radius;
		cameraX = camerax;
		cameraY = cameray * cos(radius) - cameraz * sin(radius);
		cameraZ = cameray * sin(radius) + cameraz * cos(radius);
		break;
	case ROTATE_X_ANTICLOCK:
		cameraAlpha -= radius;
		cameraX = camerax;
		cameraY = cameray * cos(radius) + cameraz * sin(radius);
		cameraZ = -cameray * sin(radius) + cameraz * cos(radius);
		break;
	case ROTATE_Y_CLOCK:
		cameraBeta += radius;
		cameraX = camerax * cos(radius) + cameraz * sin(radius);
		cameraY = cameray;
		cameraZ = -camerax * sin(radius) + cameraz * cos(radius);
		break;
	case ROTATE_Y_ANTICLOCK:
		cameraBeta -= radius;
		cameraX = camerax * cos(radius) - cameraz * sin(radius);
		cameraY = cameray;
		cameraZ = camerax * sin(radius) + cameraz * cos(radius);
		break;
	case ROTATE_Z_CLOCK:
		cameraGamma += radius;
		cameraX = camerax * cos(radius) - cameray * sin(radius);
		cameraY = camerax * sin(radius) + cameray * cos(radius);
		cameraZ = cameraz;
		break;
	case ROTATE_Z_ANTICLOCK:
		cameraGamma -= radius;
		cameraX = camerax * cos(radius) + cameray * sin(radius);
		cameraY = -camerax * sin(radius) + cameray * cos(radius);
		cameraZ = cameraz;
		break;
	}
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glLoadIdentity();

	gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glOrtho(-maxR - padding, maxR + padding, -maxR - padding, maxR + padding, -maxR - padding, maxR * 2 + padding);
	glutPostRedisplay();
	cout << cameraX << ":" << cameraY << ":" << cameraZ << ":" << endl;
	//cout << cameraX * cameraX + cameraY * cameraY + cameraZ * cameraZ << endl;
}
#endif