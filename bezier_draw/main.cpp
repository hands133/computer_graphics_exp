#include <iostream>

#include "polyline.h"

using namespace std;

float windowSize = 600;
polyline<int> poly((int)windowSize, 1);

bool inputting = false;		//正在输入点
bool first = true;			//第一次输入
bool modifyPoint = false;	//修改点

void InitEnvironment(double windowSize)	//初始化操作
{
	//设置清除颜色
	glClearColor(0.0, 0.0, 0.0, 0);
	//把窗口清除为当前颜色
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0, windowSize, 0, windowSize);
}

void mouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{	//左键按下状态
		if (!inputting)
		{	//inputting 为假，说明未开始绘制或者已经绘制完毕
			inputting = true;
			if (first)
			{	//尚未开始绘制
				first = false;
			}
			else
			{	//绘制完毕
				poly.cleanNodes();
				glClear(GL_COLOR_BUFFER_BIT);

			}
			poly.addNode(x, y);
			poly.drawPoints(DRAW_LAST_POINT);
		}
		else
		{	//inputting 为真，正在输入
			poly.addNode(x, y);
			poly.drawPoints(DRAW_LAST_POINT);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{	//右键按下状态
		if (inputting)
		{	//inputting 为真，说明正在绘制当中，取消绘制并显示线段和曲线
			inputting = false;
			poly.drawEdges();
			poly.drawBezier(BEZIER_NEW);
			poly.drawPoints(DRAW_POINTS);
		}
		else	//inputting 为假，说明尚未开始绘制或者绘制完毕，点击右键无用
		{
			inputting = false;
		}
	}
	else
		return;		//其他按键无用
}

void dragEntity(int x, int y)
{
	if (inputting)
		return;
	if (first)
		return;
	//鼠标拖拽函数
	int posx = x;
	int posy = windowSize - y;
	int radius = 10;	//半径五个像素之内的点为点击区域
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
		glClear(GL_COLOR_BUFFER_BIT);	//清除屏幕上所有内容
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
	glutCreateWindow("Bezier 曲线");
	InitEnvironment(windowSize);
	glutMouseFunc(mouseInput);
	glutMotionFunc(dragEntity);
	glutDisplayFunc(drawBezier);
	glutMainLoop();
	return 0;
}

