#pragma once
#ifndef _MESH_
#define _MESH_

#include "dataStruct.h"

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

const double PI = 3.1415926535897;
const int padding = 1;
const GLubyte syscolor[3] = { 255,0,0 };

enum dType {
	V, VT, VN, VP, DEG, BMAT, STEP, CSTYPE, P, L, F, CURV, CURV2, SURF,
	PARM, TRIM, HOLE, SCRV, SP, END, CON, G, S, MG, O, BEVEL, C_INTERP,
	D_INTERP, LOG, USEMTL, MTLLIB, SHADOW_OBJ, TRACE_OBJ, CTECH, STECH
};

const unsigned char DRAW_VERTEX = 0x01;			//绘制顶点
const unsigned char DRAW_STRUCTURE = 0x02;		//绘制骨架
const unsigned char DRAW_SURF_NOTEXTURE = 0x04;	//绘制表面（无纹理）
const unsigned char DRAW_SURF_TEXTURE = 0x08;	//绘制表面（带纹理）
const unsigned char DRAW_SURF_LIGHT = 0x10;		//绘制表面（带光照）
const unsigned char DRAW_OPTION1 = 0x20;		//未知选项1
const unsigned char DRAW_OPTION2 = 0x40;		//未知选项2
const unsigned char DRAW_OPTION3 = 0x80;		//未知选项3

const unsigned char ROTATE_X_CLOCK  = 0x01;		//绕 X 轴旋转（顺时针）
const unsigned char ROTATE_X_ANTICLOCK = 0X02;	//绕 X 轴旋转（逆时针）
const unsigned char ROTATE_Y_CLOCK = 0X04;		//绕 Y 轴旋转（顺时针）
const unsigned char ROTATE_Y_ANTICLOCK = 0X08;	//绕 Y 轴旋转（逆时针）
const unsigned char ROTATE_Z_CLOCK = 0x10;		//绕 Z 轴旋转（顺时针）
const unsigned char ROTATE_Z_ANTICLOCK = 0X20;	//绕 Z 轴旋转（逆时针）

const unsigned char LOOP_ROTATING_X = 0x01;		//绕 X 轴循环旋转
const unsigned char LOOP_ROTATING_Y = 0x02;		//绕 Y 轴循环旋转
const unsigned char LOOP_ROTATING_Z = 0x04;		//绕 Z 轴循环旋转

class mesh
{
public:
	mesh();
	~mesh();
	bool initData(const char* fileName);
	
	bool draw(const unsigned char& type, const GLubyte* color);
	//转动 radian 个弧度
	void rotateRadian(const unsigned char& type, const double& radian);
	//每秒转动 speed 个弧度，一共转动 radian 个弧度，如果 radian 为负数则一直旋转
	void rotateRadian(const unsigned char& type, const double& radian, const float& speed);
	//转动 degree 个角度
	void rotateDegree(const unsigned char& type, const int& degree);
	//每秒转动 speed 个角度，一共转动 degree 个角度，如果 degree 为负数则一直旋转
	void rotateDegree(const unsigned char& type, const int& degree, const int& speed);

private:
	const enum DRAW_TYPE {
		VERTEX, STRUCTURE, SURF_NOTEXTURE, SURF_TEXTURE,
		SURF_LIGHT, OPTION1, OPTION2, OPTION3
	};

	vector<vertex> *vertexList;
	vector<verNorm> *verNormList;
	vector<verTexture> *verTextList;
	vector<vector<face> > *faceList;

	int vertexItems;
	int verNormItems;
	int verTextItems;
	int faceItems;

	ifstream inFile;

	bool extentSpace();	//申请更多的空间
	void dispInfo();	//显示数据信息
	void typeDraw(const DRAW_TYPE& type, const GLubyte* color);
	//工具方法
	void drawVertex(const GLubyte* color);
	void drawEdges(const int& option, const GLubyte* color);

	void showCoordinate();

	double minX, maxX, minY, maxY, minZ, maxZ;
	double maxR;

	double alpha, beta, gamma;	//三个角度
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
		cout << "打开文件 " << fileName << " 失败" << endl;
		return false;
	}
	minX = maxX = minY = maxY = minZ = maxZ = 0.0;
	//循环读取文件
	cout << "正在从 " << fileName << " 加载数据" << endl;
	string buffer;
	char str[100], one;
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	while (!inFile.eof())
	{
		extentSpace();	//申请空间
		inFile.getline(str, 100, ' ');
		buffer = string(str);
		if (buffer.compare("#") == 0)
		{//井号开头的注释，读掉该行并省略
			getline(inFile, buffer);
			one = inFile.get();	//读掉换行符
		}
		else if (buffer.compare("vt") == 0)
		{	//法向贴图坐标
			inFile.getline(str, 100, ' ');
			x = atof(str);
			inFile.getline(str, 100, '\n');
			y = atof(str);

			verTextList->at(verTextItems).x = x;
			verTextList->at(verTextItems).y = y;
			verTextItems++;
		}
		else if (buffer.compare("vn") == 0)
		{	//法向量
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
		{	//顶点坐标
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
			if(maxZ > maxR)
				maxR = maxZ;
		}
		else if (buffer.compare("f") == 0)
		{	//f 顶点索引/uv点索引/法线索引
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
	cout << "加载完毕" << endl;
	dispInfo();
	inFile.close();
	//glOrtho(minX - padding, maxX + padding, minY - padding, maxY + padding, minZ - padding, maxZ + padding);
	glOrtho(-maxR - padding, maxR + padding, -maxR - padding, maxR + padding, -maxR - padding, maxR + padding);
	cout << maxR << endl;
}

//绘制坐标系
void mesh::showCoordinate()
{
	double axis[3][3] = { {maxR + padding, 0, 0}, {0, maxR + padding, 0}, {0, 0, maxR + padding} };
	GLubyte colors[3][3] = { {0,0,255},{0,255,0},{255,255,0} };
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
	glColor3ub(syscolor[0], syscolor[1], syscolor[2]);
}

bool mesh::draw(const unsigned char& type, const GLubyte* color)
{
	//cout << minX << " " << maxX << " " << minY << " " << maxY << " " << minZ << " " << maxZ << endl;
	if (type == 0x00)
	{
		cout << "类型错误 " << (int)type << endl;
		return false;
	}
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

//绘制样式
void mesh::typeDraw(const DRAW_TYPE& type, const GLubyte* color)
{
	switch (type)
	{
	case VERTEX:		drawVertex(color);
		break;
	case STRUCTURE:		drawEdges(0, color);	//0表示不画面
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

//绘制顶点
void mesh::drawVertex(const GLubyte* color)
{	
	cout << "绘制顶点" << endl;
	GLubyte r = color[0];
	GLubyte g = color[1];
	GLubyte b = color[2];
	glColor3ub(r, g, b);
	//glPointSize(1);

	glBegin(GL_POINTS);
	vertex *iter = NULL;
	for (int i = 0; i < vertexItems; i++)
	{
		iter = &(vertexList->at(i));
		glVertex3d(iter->x, iter->y, iter->z);
	}
	glEnd();
	glFlush();

	//glPointSize(1);
	glColor3ub(255, 255, 255);
	cout << "绘制完毕" << endl;
}

//绘制边和面
void mesh::drawEdges(const int& option, const GLubyte* color)
{
	GLubyte r = color[0];
	GLubyte g = color[1];
	GLubyte b = color[2];
	glColor3ub(255, 0, 0);
	//glLineWidth(2);

	vector<face> *iter = NULL;
	face *innerIter = NULL;
	vertex *vertexIter = NULL;
	for (int i = 0; i < faceItems; i++)
	{
		glBegin(GL_LINE_LOOP);
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

//显示数据信息
void mesh::dispInfo()
{	
	cout << "数据信息" << endl;
	cout << "结点共 " << vertexList->size() << "个:" << vertexItems << endl;
	cout << "贴图共 " << verTextList->size() << "个:" << verTextItems << endl;
	cout << "法线共 " << verNormList->size() << "个:" << verNormItems << endl;
	cout << "面片共 " << faceList->size() << "个:" << faceItems << endl;
}

//扩展内存
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
		cout << "内存不足，分配内存失败" << endl;
		return false;
	}
	return true;
}

//旋转
void mesh::rotateDegree(const unsigned char& type, const int& degree)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	showCoordinate();
	double radius = (double)degree * PI / 180.0;
	double cosTheta = 0.0;
	double sinTheta = 0.0;

	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	switch (type)
	{
	case ROTATE_X_CLOCK:
	{
		alpha += radius;
		cosTheta = cos(alpha);
		sinTheta = sin(alpha);
		vector<face> *faceiter = NULL;
		face *innerIter = NULL;
		vertex *vertexIter = NULL;
		for (int i = 0; i < faceItems; i++)
		{
			glBegin(GL_LINE_LOOP);
			faceiter = &(faceList->at(i));
			for (int j = 0; j < faceiter->size(); j++)
			{
				innerIter = &(faceiter->at(j));
				vertexIter = &(vertexList->at(innerIter->vertIndex - 1));
				x = vertexIter->x;
				y = vertexIter->y;
				z = vertexIter->z;
				glVertex3d(x, y * cosTheta - z * sinTheta, y * sinTheta + z * cosTheta);
			}
			glEnd();
			glFlush();
		}
	}
		break;
	case ROTATE_X_ANTICLOCK:
		break;
	}
	glutSwapBuffers();
}
#endif