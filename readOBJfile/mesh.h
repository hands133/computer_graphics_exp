#pragma once
#ifndef _MESH_
#define _MESH_

#include "dataStruct.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>

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

enum dType {
	V, VT, VN, VP, DEG, BMAT, STEP, CSTYPE, P, L, F, CURV, CURV2, SURF,
	PARM, TRIM, HOLE, SCRV, SP, END, CON, G, S, MG, O, BEVEL, C_INTERP,
	D_INTERP, LOG, USEMTL, MTLLIB, SHADOW_OBJ, TRACE_OBJ, CTECH, STECH
};

const unsigned char DRAW_VERTEX = 0x01;			//���ƶ���
const unsigned char DRAW_STRUCTURE = 0x02;		//���ƹǼ�
const unsigned char DRAW_SURF_NOTEXTURE = 0x04;	//���Ʊ��棨������
const unsigned char DRAW_SURF_TEXTURE = 0x08;	//���Ʊ��棨������
const unsigned char DRAW_SURF_LIGHT = 0x10;		//���Ʊ��棨�����գ�
const unsigned char DRAW_OPTION1 = 0x20;		//δ֪ѡ��1
const unsigned char DRAW_OPTION2 = 0x40;		//δ֪ѡ��2
const unsigned char DRAW_OPTION3 = 0x80;		//δ֪ѡ��3

class mesh
{
public:
	mesh();
	~mesh();
	bool initData(const char* fileName);
	bool draw(const unsigned char& type, const GLubyte* color);

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

	bool extentSpace();	//�������Ŀռ�
	void dispInfo();	//��ʾ������Ϣ
	void typeDraw(const DRAW_TYPE& type, const GLubyte* color);
	//���߷���
	void drawVertex(const GLubyte* color);
	void drawEdges(const int& option, const GLubyte* color);

	double minX, maxX, minY, maxY, minZ, maxZ;
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
	glOrtho(minX, maxX, minY, maxY, minZ, maxZ);
}

bool mesh::draw(const unsigned char& type, const GLubyte* color)
{
	if (type == 0x00)
	{
		cout << "���ʹ��� " << (int)type << endl;
		return false;
	}
	bitset<8> bits(type);
	for (int i = 7; i >= 0; i--)
	{
		if (bits.test(i) == 1)
			typeDraw(DRAW_TYPE(i), color);
	}
	glutSwapBuffers();
}

void mesh::typeDraw(const DRAW_TYPE& type, const GLubyte* color)
{
	switch (type)
	{
	case VERTEX:		drawVertex(color);
		break;
	case STRUCTURE:		drawEdges(0, color);	//0��ʾ������
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
	cout << "�������" << endl;
}

//���Ʊߺ���
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

void mesh::dispInfo()
{	
	cout << "������Ϣ" << endl;
	cout << "��㹲 " << vertexList->size() << "��:" << vertexItems << endl;
	cout << "��ͼ�� " << verTextList->size() << "��:" << verTextItems << endl;
	cout << "���߹� " << verNormList->size() << "��:" << verNormItems << endl;
	cout << "��Ƭ�� " << faceList->size() << "��:" << faceItems << endl;
}

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
#endif