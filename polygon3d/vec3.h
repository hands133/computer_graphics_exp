#pragma once
#ifndef _VEC3_
#define _VEC3_

#include "dependencies.h"

#include "point3.h"

template <typename T = int>
struct vec3
{
public:
	vec3() : startPoint(new node3<T>(0, 0, 0)), vec(new node3<T>(0, 0, 0)) {};
	vec3(const vec3<T>& theVec)
	{
		startPoint.set(theVec.startPoint);
		vec.set(theVec.vec);
	}

	node3<T> endPoint()
	{
		node3<T> ends;
		ends.x = startPoint.x + vec.x;
		ends.y = startPoint.y + vec.y;
		ends.z = startPoint.z + vec.z;
		return ends;
	}

private:
	node3<T> startPoint;	//向量的起始点
	node3<T> vec;			//向量本身（数学意义上，起点默认为0）
};

template <typename T>
T dotProduct(const vec3<T>& vectorOne, const vec3<T>& vectorTwo)
{
	//求两个向量的点积
	return vectorOne.vec.x * vectorTwo.vec.x +
		vectorOne.vec.y + vectorTwo.vec.y +
		vectorOne.vec.z + vectorTwo.vec.z;
}

template <typename T>
vec3<T> dotProduct(const vec3<T>& vectorOne, const vec3<T>& vectorTwo)
{
	//求两个向量的叉积
	vec3<T> result;
	result.startPoint.set(0, 0, 0);
	result.vec.x = vectorOne.vec.y * vectorTwo.vec.z -
		vectorTwo.vec.y * vectorOne.vec.z;
	result.vec.y = vectorOne.vec.z * vectorTwo.vec.x -
		vectorTwo.vec.z * vectorOne.vec.x;
	result.vec.z = vectorOne.vec.x * vectorTwo.vec.y -
		vectorTwo.vec.x * vectorOne.vec.y;
	return result;
}

#endif