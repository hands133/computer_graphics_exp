#pragma once

struct vertex
{	//��������
	double x;
	double y;
	double z;
	vertex() :x(0.0), y(0.0), z(0.0) {}
	vertex(double X, double Y, double Z) :x(X), y(Y), z(Z) {}
};

struct verNorm
{	//���㷨��
	double nx;
	double ny;
	double nz;
	verNorm() :nx(0.0), ny(0.0), nz(0.0) {}
	verNorm(double NX, double NY, double NZ) :nx(NX), ny(NY), nz(NZ) {}
};

struct verTexture
{
	double x;
	double y;
	verTexture() :x(0.0), y(0.0) {}
	verTexture(double X, double Y) :x(X), y(Y) {}
};

struct face
{
	int vertIndex;		//��������
	int verTextIndex;	//��ͼ����
	int verNormIndex;	//��������
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
