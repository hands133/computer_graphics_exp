#include <iostream>
#include <gl/glut.h>
#include <cmath>

#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define abs(x) ((x)>0?(x):-(x))

float window_size = 601;
float grid_size = 6;
int half_window = (int)(window_size / 2);

void InitEnvironment()
{
	glClearColor(0.0, 0.0, 0.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//将画面映射为坐标系
	gluOrtho2D(0, window_size, 0, window_size);
}

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

void initGird(int window_size, int grid_size)
{
	glBegin(GL_LINES);
	glColor3f(0.3f, 0.3f, 0.3f);
	for (int i = 1; i <= window_size; i = i + grid_size) {
		glVertex2f(i, 1);
		glVertex2f(i, window_size);
		glVertex2f(1, i);
		glVertex2f(window_size, i);
	}
	glEnd();
}

void lineMID(int x1, int y1, int x2, int y2)
{
	//右下按中点画线法绘制直线
	int left, right, top, bottom;
	for (int x = x1, y = y1; x <= x2; x++) {
		//直线下方的点
		int distance1 = (y - y1)*(x2 - x1) - (y2 - y1)*(x - x1);
		//直线上方的点
		int distance2 = (y - y1 + 1)*(x2 - x1) - (y2 - y1)*(x - x1);

		left = x * grid_size + 1;
		right = left + grid_size - 1;
		if (abs(distance1) < abs(distance2))
			bottom = (int)(y + 0.5)*grid_size + 2;
		else {
			bottom = (int)(y + 1.5)*grid_size + 2;
			y = y + 1;
		}
		top = bottom + grid_size - 1;

		glRectf(left, top, right, bottom);
	}
}

bool clipTest(float p, float q, float &u1, float &u2)
{
	//判据
	float r;
	if (p < 0)
	{
		r = q / p;
		if (r > u2)
			return false;
		if (r > u1)
			u1 = r;
	}
	else if (p > 0)
	{
		r = q / p;
		if (r < u1)
			return false;
		if (r < u2)
			u2 = r;
	}
	else return (q >= 0);
	return true;
}

void lyd_Barskey_Cut(float x1, float y1, float x2, float y2,
	const float& XL, const float& XR, const float& YB, const float& YT)
{

	//梁友栋-Barskey 线段裁剪算法
	float dx, dy, u1, u2;
	u1 = 0;
	u2 = 1;
	dx = x2 - x1;
	dy = y2 - y1;

	
	glColor3ub(80, 80, 80);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glColor3ub(255, 255, 255);
	

	if (clipTest(-dx, x1 - XL, u1, u2))
		if (clipTest(dx, XR - x1, u1, u2))
			if (clipTest(-dy, y1 - YB, u1, u2))
				if (clipTest(dy, YT - y1, u1, u2))
				{
					//画线
					glBegin(GL_LINES);
					glVertex2f(x1 + u1 * dx, y1 + u1 * dy);
					glVertex2f(x1 + u2 * dx, y1 + u2 * dy);
					glEnd();
					glFlush();
				}
}

int encode(float x, float y)
{
	//添加区域的编码
	return 0;
}

void cohen_Sutherland_cut(float x1, float y1, float x2, float y2,
	const float& XL, const float& XR, const float& YB, const float& YT)
{
	// Cohen-Sutherland 裁剪算法
}

void mid_point_cut(float x1, float y1, float x2, float y2,
	const float& XL, const float& XR, const float& YB, const float& YT)
{
	// 终点分割裁剪算法
}

void cutLine(void)
{
	//glViewport(0, 0, window_size, window_size);
	glClear(GL_COLOR_BUFFER_BIT);
	//initGird(window_size, grid_size);
	//画线
	int numLines = 1000;
	int position[4] = { 0,0,0,0 };
	float XL = 200;
	float XR = 400;
	float YB = 200;
	float YT = 400;

	glColor3f(1.0f, 1.0f, 1.0f);

	srand((unsigned)time(NULL));	//随机种子
	for (int i = 0; i < numLines; i++)
	{
		//生成四个随机数
		for (int j = 0; j < 4; j++)
		{
			//通用公式:a + rand() % n；其中的a是起始值，n是整数的范围。
			position[j] = 0 + rand() % (int)window_size;
		}
		lyd_Barskey_Cut(position[0], position[1],
			position[2], position[3], XL, XR, YB, YT);
		Sleep(10);
	}

	drawCutarea(XL, XR, YB, YT, 2);	//最后画框避免覆盖

	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);   //初始化GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(window_size, window_size);
	glutCreateWindow("画线");
	InitEnvironment();   //初始化
	//glutDisplayFunc(&drawLine);   //回调函数
	glutDisplayFunc(&cutLine);
	glutMainLoop();    //持续显示，当窗口改变会重新绘制图形
	return 0;
}