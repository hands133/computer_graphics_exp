#pragma once

#include <gl/glut.h>

struct Color
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	Color() :r(0xff), g(0xff), b(0xff) {}
	Color(GLubyte *color)
	{
		if (color == NULL)
		{
			r = g = b = 0xff;
			return;
		}
		r = color[0];
		g = color[1];
		b = color[2];
	}
	Color(const Color& theColor)
	{
		r = theColor.r;
		g = theColor.g;
		b = theColor.b;
	}
};

struct vertex
{	//顶点坐标
	double x;
	double y;
	double z;
	vertex() :x(0.0), y(0.0), z(0.0) {}
	vertex(double X, double Y, double Z) :x(X), y(Y), z(Z) {}
};

struct verNorm
{	//顶点法线
	double nx;
	double ny;
	double nz;
	verNorm() :nx(0.0), ny(0.0), nz(0.0) {}
	verNorm(double NX, double NY, double NZ) :nx(NX), ny(NY), nz(NZ) {}
};

struct verTexture
{	//贴图
	double x;
	double y;
	verTexture() :x(0.0), y(0.0) {}
	verTexture(double X, double Y) :x(X), y(Y) {}
};

struct face
{	//面
	int vertIndex;		//顶点索引
	int verTextIndex;	//贴图索引
	int verNormIndex;	//法线索引
	face() :vertIndex(0), verTextIndex(0), verNormIndex(0) {}
	face(int vertex, int texture, int norm) :vertIndex(vertex), verTextIndex(texture), verNormIndex(norm) {};
	int& getByIndex(const int& i)
	{
		switch (i)
		{
		case 0:	return vertIndex;
			break;
		case 1:	return verTextIndex;
			break;
		case 2:	return verNormIndex;
			break;
		}
	}
};
