#include <iostream>

#include "polyline.h"

using namespace std;

float windowSize = 600;
polyline<int> poly((int)windowSize, 1);

bool BezierDrawn = false;	//�ж��Ƿ���Ʊ���������
bool leftButtonHold = false;
bool rightButtonHold = false;	//�����ж����Ҽ��϶�
//�ж��Ƿ��һ�ε��������
//���������������������Ļɾ��
bool first = true;			//��һ������ڵ�
bool storePoint = false;	//��������¼Ҫɾ���ĵ�
int index = -1;				//ɾ��������±�

GLubyte steelBlue[3] = { 70, 130, 180 };	//����

int BSPLINE_ORDER = 5;
int BSPLINE_TYPE = BSPLINE_CLAMP;

void InitEnvironment(double windowSize)	//��ʼ������
{
	//���������ɫ
	glClearColor(0.0, 0.0, 0.0, 0);
	//�Ѵ������Ϊ��ǰ��ɫ
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0, windowSize, 0, windowSize);
}

void mouseInput(int button, int state, int x, int y)
{
	int posx = x;				//������
	int posy = windowSize - y;	//������
	int radius = 10;			//�жϰ뾶

	int distance = -1;	//�㵽ֱ�ߵľ���

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{	//�������
		leftButtonHold = true;
		if (!first)
		{	//�ǵ�һ������
			//�ж��Ƿ����ڶ��㸽��
			//�������궨���㲢�Ƴ��ȴ��Ҽ���Ӧ
			pair<int, int>& nearNode = poly.findNearestPoint(posx, posy, radius, index);
			//cout << "��������index = " << index << endl;
			if (index != -1)
			{	//���㸽��
				storePoint = true;
				//��������
				glClear(GL_COLOR_BUFFER_BIT);
				//if (BezierDrawn)
				//	poly.drawConvexHull(&steelBlue[0]);
				poly.drawEdges();
				if (BezierDrawn)
				{
					poly.drawBezier(BEZIER_NEW);
					poly.drawBSpline(BSPLINE_ORDER, BSPLINE_TYPE);
				}
				poly.drawPoints(DRAW_POINTS);
				glBegin(GL_POINTS);
				glColor3ub(0, 255, 0);
				glVertex2i(nearNode.first, nearNode.second);
				glColor3ub(255, 255, 255);
				glEnd();
				glutSwapBuffers();
				return;
			}
			//�ǵ�����㣬�ж�
			int position = poly.findNearestEdge(posx, posy, radius * 2);
			if (position != -1)
			{	//������ڱߵĸ���
				index = position + 1;
				poly.insertByIndex(posx, posy, position);
				glClear(GL_COLOR_BUFFER_BIT);
				//if (BezierDrawn)
				//	poly.drawConvexHull(&steelBlue[0]);
				poly.drawEdges();
				if (BezierDrawn)
				{
					poly.drawBezier(BEZIER_NEW);
					poly.drawBSpline(BSPLINE_ORDER, BSPLINE_TYPE);
				}
					
				poly.drawPoints(DRAW_POINTS);
				glutSwapBuffers();
			}
			else
			{
				//�ж��Ƿ��Ѿ����ƶ���
				if (BezierDrawn)
				{	//�Ѿ��������ߣ�������ĩβ��ӽڵ㲢���»���
					glClear(GL_COLOR_BUFFER_BIT);
					poly.addNode(posx, posy);
					//poly.drawConvexHull(&steelBlue[0]);
					poly.drawEdges();
					poly.drawBezier(BEZIER_NEW);
					poly.drawBSpline(BSPLINE_ORDER, BSPLINE_TYPE);
					poly.drawPoints(DRAW_POINTS);
					glutSwapBuffers();
					index = poly.size() - 1;
				}
				else
				{	//��δ�������ߣ���ʾ��������
					poly.addNode(posx, posy);
					poly.drawEdges();
					poly.drawPoints(DRAW_POINTS);
					index = poly.size() - 1;

					//��������
					glClear(GL_COLOR_BUFFER_BIT);
					//if (BezierDrawn)
					//	poly.drawConvexHull(&steelBlue[0]);
					
					poly.drawEdges();
					if (BezierDrawn)
					{
						poly.drawBezier(BEZIER_NEW);
						poly.drawBSpline(BSPLINE_ORDER, BSPLINE_TYPE);
					}
						
					poly.drawPoints(DRAW_POINTS);
					glBegin(GL_POINTS);
					glColor3ub(0, 255, 0);
					glVertex2i(poly.getByIndex(index).first, poly.getByIndex(index).second);
					glColor3ub(255, 255, 255);
					glEnd();
					glutSwapBuffers();
				}
			}
		}
		else
		{	//���ǵ�һ�ε��
			first = false;
			poly.addNode(posx, posy);
			poly.drawEdges();
			poly.drawPoints(DRAW_POINTS);
			index = 0;
			glutSwapBuffers();
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{	//�Ҽ�����
		rightButtonHold = true;
		int anotherIndex = -1;
		pair<int, int>& theNearestNode = poly.findNearestPoint(posx, posy, radius, anotherIndex);
		if (index != -1)
		{
			if (!BezierDrawn)
			{	//��δ���� Bezier ����
				BezierDrawn = true;
			}
			else
			{	//����������ɾ���ػ�
				poly.eraseByIndex(index);
			}
			glClear(GL_COLOR_BUFFER_BIT);
			
			//poly.drawConvexHull(&steelBlue[0]);
			poly.drawEdges();
			poly.drawBezier(BEZIER_NEW);
			poly.drawBSpline(BSPLINE_ORDER, BSPLINE_TYPE);
			poly.drawPoints(DRAW_POINTS);
			glutSwapBuffers();
			index = -1;
		}
		else
		{
			poly.drawBezier(BEZIER_NEW);
			poly.drawBSpline(BSPLINE_ORDER, BSPLINE_TYPE);
			poly.drawPoints(DRAW_POINTS);
			BezierDrawn = true;
			glutSwapBuffers();
			if (anotherIndex == -1)
			{
				BezierDrawn = false;
				glClear(GL_COLOR_BUFFER_BIT);
				poly.cleanNodes();
				glutSwapBuffers();
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{	//����ſ�
		leftButtonHold = false;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		rightButtonHold = false;
	}
	else
	return;
}

void keyBoardFunc(unsigned char key, int x, int y)
{
	if (key == '+')
	{
		cout << "���� " << ++BSPLINE_ORDER << endl;
		//�ػ�
		glClear(GL_COLOR_BUFFER_BIT);
		poly.drawEdges();
		if (BezierDrawn)
		{
			poly.drawBezier(BEZIER_NEW);
			poly.drawBSpline(BSPLINE_ORDER, BSPLINE_TYPE);
		}
		poly.drawPoints(DRAW_POINTS);
		glutSwapBuffers();
	}
	else if (key == '-')
	{
		cout << "���� " << --BSPLINE_ORDER << endl;
		if (BSPLINE_ORDER < 2)	//�������С�� 2 �������»���
			BSPLINE_ORDER++;
		else
		{
			//�ػ�
			glClear(GL_COLOR_BUFFER_BIT);
			poly.drawEdges();
			if (BezierDrawn)
			{
				poly.drawBezier(BEZIER_NEW);
				poly.drawBSpline(BSPLINE_ORDER, BSPLINE_TYPE);
			}
			poly.drawPoints(DRAW_POINTS);
			glutSwapBuffers();
		}
	}
}

void dragEntity(int x, int y)
{
	//����ֻ�ϵ㣬ʣ�µ���˵
	int posx = x;
	int posy = windowSize - y;
	int radius = 10;

	if (index == -1)
		return;
	pair<int, int> point = poly.getByIndex(index);

	poly.setPByIndex(posx, posy, index);
	glClear(GL_COLOR_BUFFER_BIT);
	//70 130 180 steelblue
	//poly.drawConvexHull(GLubyte* color);
	GLubyte *steelBlue = new GLubyte[3];
	steelBlue[0] = 70;
	steelBlue[1] = 130;
	steelBlue[2] = 180;
	//if(BezierDrawn)
	//	poly.drawConvexHull(steelBlue);
	poly.drawEdges();
	if (BezierDrawn)
	{
		poly.drawBezier(BEZIER_NEW);
		poly.drawBSpline(BSPLINE_ORDER, BSPLINE_TYPE);
	}
	poly.drawPoints(DRAW_POINTS);

	if (index != -1)
	{
		glBegin(GL_POINTS);
		glColor3ub(0, 255, 0);
		glVertex2i(posx, posy);
		glColor3ub(255, 255, 255);
		glEnd();
		glutSwapBuffers();
	}
}

void drawBezier(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

int main(int argc, char*argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowSize, windowSize);
	glutCreateWindow("Bezier ����");
	InitEnvironment(windowSize);
	glutMouseFunc(mouseInput);	//���������
	glutMotionFunc(dragEntity);	//����϶�����
	glutKeyboardFunc(keyBoardFunc);
	glutDisplayFunc(drawBezier);
	glutMainLoop();
	return 0;
}

