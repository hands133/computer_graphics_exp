#pragma once

#include <iostream>
#include <vector>

#include <gl/glut.h>

#include "dataStructure.h"

using std::cin;
using std::cout;
using std::endl;

using std::vector;

#ifndef _WRAPPEDTOOLS_
#define _WRAPPEDTOOLS_

GLubyte defColor[3] = { 255,255,255 };

const GLubyte WHITE[3] = { 255,255,255 };
const GLubyte RED[3] = { 255,0,0 };
const GLubyte GREEN[3] = { 0,255,0 };
const GLubyte BLUE[3] = { 0,0,255 };

void setDefColor(const GLubyte* color)
{
	if (color == NULL)
		return;
	defColor[0] = color[0];
	defColor[1] = color[1];
	defColor[2] = color[2];
}

void setColor(const GLubyte* color)
{
	if (color == NULL)
		glColor3ub(defColor[0], defColor[1], defColor[2]);
	else
		glColor3ub(color[0], color[1], color[2]);
}

void resetColor()
{
	glColor3ub(defColor[0], defColor[1], defColor[2]);
}

void drawVertex(const int& x, const int& y, const int& z, const GLubyte *color)
{
	setColor(color);
	glBegin(GL_POINTS);
	glVertex3i(x, y, z);
	glEnd();
	resetColor();
}

void drawVertex(const int* coordinate, const GLubyte *color)
{
	if (coordinate == NULL)
		return;
	setColor(color);
	glBegin(GL_POINTS);
	glVertex3i(coordinate[0], coordinate[1], coordinate[2]);
	glEnd();
	resetColor();
}

void drawVertex(const vector<int>* vector, const GLubyte *color)
{
	if (vector == NULL)
		return;
	if (!vector->empty())
		drawVertex(&(vector->at(0)), color);
}

void drawVertexes(const vector<vertex>* vertexes, const GLubyte* color)
{
	if (vertexes == NULL)
		return;
	if (vertexes->empty())
		return;
	vector<vertex>::const_iterator iter = vertexes->begin();
	setColor(color);
	glBegin(GL_POINTS);
	for (int i = 0; i < vertexes->size(); i++)
		glVertex3i((*iter).x, (*iter).y, (*iter).z);
	glEnd();
	resetColor();
}

void drawVertexes(const vector<vertex>* vertexes, const vector<Color>* colors)
{
	if (vertexes == NULL || colors == NULL)
		return;
	if (vertexes->empty())
		return;
	if (vertexes->size() != colors->size())
		return;
	vector<vertex>::const_iterator vertexIter = vertexes->begin();
	vector<Color>::const_iterator colorIter = colors->begin();

	glBegin(GL_POINTS);
	for (int i = 0; i < vertexes->size(); i++)
	{
		glColor3ub((*colorIter).r, (*colorIter).g, (*colorIter).b);
		glVertex3i((*vertexIter).x, (*vertexIter).y, (*vertexIter).z);
	}
	glEnd();
	resetColor();
}

void drawEdges(const vertex* vertexes, const int& length, const GLubyte *color)
{
	if (vertexes == NULL)
		return;
	if (length <= 0)
		return;
	setColor(color);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < length; i++)
		glVertex3i(vertexes[i].x, vertexes[i].y, vertexes[i].z);
	glEnd();
	resetColor();
}

void drawEdges(const vector<vertex>* vertexes, const GLubyte *color)
{
	if (vertexes == NULL)
		return;
	drawEdges(&(vertexes->at(0)), vertexes->size(),color);
}

void drawEdges(const vector<vertex>* vertexes, const vector<Color>* colors)
{
	if (vertexes == NULL || colors == NULL)
		return;
	if (vertexes->empty())
		return;
	if (vertexes->size() != colors->size())
		return;
	vector<vertex>::const_iterator verIter = vertexes->begin();
	vector<Color>::const_iterator colorIter = colors->begin();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < vertexes->size(); i++)
	{
		glColor3ub((*colorIter).r, (*colorIter).g, (*colorIter).b);
		glVertex3i((*verIter).x, (*verIter).y, (*verIter).z);
	}
	glEnd();
	resetColor();
}

#endif