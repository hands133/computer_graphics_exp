#include <iostream>

#include "polyline.h"

using namespace std;

float windowSize = 600;
polyline<int> poly((int)windowSize, 1);

bool BezierDrawn = false;	//判断是否绘制贝塞尔曲线
bool leftButtonHold = false;
bool rightButtonHold = false;	//用于判断左右键拖动
//判断是否第一次点击鼠标左键
//是则跳过，不是则清除屏幕删点
bool first = true;			//第一次输入节点
bool storePoint = false;	//左键点击记录要删除的点
int index = -1;				//删除顶点的下标

GLubyte steelBlue[3] = { 70, 130, 180 };	//锈蓝

int BSPLINE_ORDER = 5;
int BSPLINE_TYPE = BSPLINE_CLAMP;

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
	int posx = x;				//横坐标
	int posy = windowSize - y;	//纵坐标
	int radius = 10;			//判断半径

	int distance = -1;	//点到直线的距离

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{	//左键按下
		leftButtonHold = true;
		if (!first)
		{	//非第一次输入
			//判断是否点击在顶点附近
			//如果是则标定顶点并推出等待右键相应
			pair<int, int>& nearNode = poly.findNearestPoint(posx, posy, radius, index);
			//cout << "点击左键，index = " << index << endl;
			if (index != -1)
			{	//顶点附近
				storePoint = true;
				//高亮顶点
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
			//非点击顶点，判断
			int position = poly.findNearestEdge(posx, posy, radius * 2);
			if (position != -1)
			{	//如果点在边的附近
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
				//判断是否已经绘制顶点
				if (BezierDrawn)
				{	//已经绘制曲线，在曲线末尾添加节点并重新绘制
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
				{	//尚未绘制曲线，表示正在输入
					poly.addNode(posx, posy);
					poly.drawEdges();
					poly.drawPoints(DRAW_POINTS);
					index = poly.size() - 1;

					//高亮顶点
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
		{	//不是第一次点击
			first = false;
			poly.addNode(posx, posy);
			poly.drawEdges();
			poly.drawPoints(DRAW_POINTS);
			index = 0;
			glutSwapBuffers();
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{	//右键按下
		rightButtonHold = true;
		int anotherIndex = -1;
		pair<int, int>& theNearestNode = poly.findNearestPoint(posx, posy, radius, anotherIndex);
		if (index != -1)
		{
			if (!BezierDrawn)
			{	//尚未绘制 Bezier 曲线
				BezierDrawn = true;
			}
			else
			{	//绘制曲线则删点重画
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
	{	//左键放开
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
		cout << "升阶 " << ++BSPLINE_ORDER << endl;
		//重绘
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
		cout << "降阶 " << --BSPLINE_ORDER << endl;
		if (BSPLINE_ORDER < 2)	//如果阶数小于 2 则不再重新绘制
			BSPLINE_ORDER++;
		else
		{
			//重绘
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
	//函数只拖点，剩下的再说
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
	glutCreateWindow("Bezier 曲线");
	InitEnvironment(windowSize);
	glutMouseFunc(mouseInput);	//鼠标点击函数
	glutMotionFunc(dragEntity);	//鼠标拖动函数
	glutKeyboardFunc(keyBoardFunc);
	glutDisplayFunc(drawBezier);
	glutMainLoop();
	return 0;
}

