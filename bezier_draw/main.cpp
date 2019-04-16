#include <iostream>

#include "polyline.h"

using namespace std;

float windowSize = 600;
polyline<int> poly((int)windowSize, 1);

bool inputting = false;		//���������
bool first = true;			//��һ������
bool modifyPoint = false;	//�޸ĵ�

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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{	//�������״̬
		if (!inputting)
		{	//inputting Ϊ�٣�˵��δ��ʼ���ƻ����Ѿ��������
			inputting = true;
			if (first)
			{	//��δ��ʼ����
				first = false;
			}
			else
			{	//�������
				poly.cleanNodes();
				glClear(GL_COLOR_BUFFER_BIT);

			}
			poly.addNode(x, y);
			poly.drawPoints(DRAW_LAST_POINT);
		}
		else
		{	//inputting Ϊ�棬��������
			poly.addNode(x, y);
			poly.drawPoints(DRAW_LAST_POINT);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{	//�Ҽ�����״̬
		if (inputting)
		{	//inputting Ϊ�棬˵�����ڻ��Ƶ��У�ȡ�����Ʋ���ʾ�߶κ�����
			inputting = false;
			poly.drawEdges();
			poly.drawBezier(BEZIER_NEW);
			poly.drawPoints(DRAW_POINTS);
		}
		else	//inputting Ϊ�٣�˵����δ��ʼ���ƻ��߻�����ϣ�����Ҽ�����
		{
			inputting = false;
		}
	}
	else
		return;		//������������
}

void dragEntity(int x, int y)
{
	if (inputting)
		return;
	if (first)
		return;
	//�����ק����
	int posx = x;
	int posy = windowSize - y;
	int radius = 10;	//�뾶�������֮�ڵĵ�Ϊ�������
	int index = -1;
	pair<int, int>& nearest = poly.findNearestPoint(posx, posy, radius, index);
	if (index == -1)
	{
		inputting = false;
		return;
	}
	else
	{
		poly.setPByIndex(posx, posy, index);
		glClear(GL_COLOR_BUFFER_BIT);	//�����Ļ����������
		//70 130 180 steelblue
		//poly.drawConvexHull(GLubyte* color);
		poly.drawEdges();
		poly.drawBezier(BEZIER_NEW);
		poly.drawPoints(DRAW_POINTS);
	}
}

void drawBezier(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main(int argc, char*argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowSize, windowSize);
	glutCreateWindow("Bezier ����");
	InitEnvironment(windowSize);
	glutMouseFunc(mouseInput);
	glutMotionFunc(dragEntity);
	glutDisplayFunc(drawBezier);
	glutMainLoop();
	return 0;
}

