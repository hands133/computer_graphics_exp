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

	void addNode(const T&x, const T&y, const T&z);	//添加节点
	void delNode(int theSeq);	//删除第 theSeq 个节点

	int getNodes() { return nodes; }

private:
	std::vector<node3<T>>* nodetable;	//顶点序列
	int nodes;
};

template <typename T>
polygon3<T>::polygon3()
{
	//默认的构造函数
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