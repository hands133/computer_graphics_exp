#include <iostream>

#include "polygon.h"

using namespace std;



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

polygon<int> poly(5);

//绘制裁剪的矩形区域
void drawCutarea(const float& XL, const float& XR,
	const float& YB, const float& YT, const float& width)
{
	glLineWidth(width);
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(XL, YT);
	glVertex2f(XR, YT);
	glVertex2f(XR, YB);
	glVertex2f(XL, YB);
	glEnd();
	glFlush();
	glColor3ub(255, 255, 255);
	glLineWidth(1);
}

void drawEdge(void)
{
	poly.drawEdge();
	GLubyte color[3] = { 0,255,120 };
	//poly.recurFill(color, 5, 100);
	//poly.bfsFill(color, 150, 150);
	//GLubyte Newcolor[3] = { 120, 150 ,180 };
	//poly.scanFill(Newcolor);
	int XL = 75;
	int XR = 225;
	int YB = 125;
	int YT = 400;
	poly.polyReccut(XL, XR, YB, YT);

	poly.scanFill(color);
	//poly.drawEdge();
	drawCutarea(XL, XR, YB, YT, 3);
}

int main(int argc, char*argv[]) {
	//poly = new polygon<int>(5);
	
	poly.addNode(0, 100);
	poly.addNode(100, 200);
	poly.addNode(150, 500);
	poly.addNode(200, 200);
	poly.addNode(300, 100);
	poly.addNode(250, 50);
	poly.addNode(200, 100);
	poly.addNode(100, 100);
	poly.addNode(50, 50);
	//poly.disp();
	//cout << "试试计算面积" << poly.area() << endl;
	float windowSize = 600;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowSize, windowSize);
	glutCreateWindow("貌似是必须先执行的");
	InitEnvironment(windowSize);
	glutDisplayFunc(drawEdge);
	glutMainLoop();
	return 0;
}

