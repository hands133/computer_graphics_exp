#pragma once

#ifndef _POINT3_
#define _POINT3_

template <typename T = int>
struct node3
{
	T x, y, z;	//三维点的坐标

	node3() :x(0), y(0), z(0) {}
	node3(T x, T y, T z) :x(x), y(y), z(z) {}
	node3(const node3<T>& node)
	{
		x = node.x;
		y = node.y;
		z = node.z;
	}

	void set(const node3<T>& tar)
	{
		x = tar.x;
		y = tar.y;
		z = tar.z;
	}

	void set(const T& x, const T&y, const T&z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

#endif