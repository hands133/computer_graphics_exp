#pragma once
#ifndef _POLYGON3_
#define _POLYGON3_

#include "point3.h"
#include "dependencies.h"

template <typename T = int>
class polygon3
{
public:
	polygon3();
	polygon3(int numOfnodetable);
	~polygon3();

	void addNode(const T&x, const T&y, const T&z);	//��ӽڵ�
	void delNode(int theSeq);	//ɾ���� theSeq ���ڵ�

	int getNodes() { return nodes; }

private:
	std::vector<node3<T>>* nodetable;	//��������
	int nodes;
};

template <typename T>
polygon3<T>::polygon3()
{
	//Ĭ�ϵĹ��캯��
	nodetable = new vector<node3<T>>(1);
	nodes = 0;
}

template <typename T>
polygon3<T>::polygon3(int numOfnodes)
{
	nodetable = new vector<node3<T>>(numOfnodes);
	nodes = numOfnodes;
}

template <typename T>
polygon3<T>::~polygon3()
{
	delete nodetable;
	nodes = 0;
}

template <typename T>
void polygon3<T>::addNode(const T&x, const T&y, const T&z)
{
	node3<T> insert(x, y, z);
	nodetable->push_back(insert);
}

template <typename T>
void polygon3<T>::delNode(int theSeq)
{
	nodetable->erase(nodetable->begin() + theSeq - 1);
	nodes--;
}

#endif