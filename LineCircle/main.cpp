#include <iostream>
#include <gl/glut.h>
#include <cmath>

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
	//������ӳ��Ϊ����ϵ
	gluOrtho2D(0, window_size, 0, window_size);
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

void lineADD(int x1, int y1, int x2, int y2)
{
	//��ֵ΢�ֻ�ֱ��
	//���°� DDA ����ֱ��
	double y = (double)y1;
	double k = (double)(y2 - y1) / (x2 - x1);
	int left, right, top, bottom;
	for (int x = x1; x <= x2; x++) {
		y += k;

		left = x * grid_size+1;
		right = left + grid_size-1;
		bottom = (int)(y + 0.5)*grid_size + 2;
		top = bottom + grid_size-1;

		glRectf(left, top, right, bottom);
	}
}

void lineMID(int x1, int y1, int x2, int y2)
{
	//���°��е㻭�߷�����ֱ��
	int left, right, top, bottom;
	for (int x = x1, y = y1; x <= x2; x++) {
		//ֱ���·��ĵ�
		int distance1 = (y - y1)*(x2 - x1) - (y2 - y1)*(x - x1);
		//ֱ���Ϸ��ĵ�
		int distance2 = (y - y1 + 1)*(x2 - x1) - (y2 - y1)*(x - x1);
		
		left = x * grid_size + 1 + half_window;
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

void lineBer(int x1, int y1, int x2, int y2) 
{
	//���� Bersenham ����ֱ��
	int left, right, top, bottom;
	double e = -0.5;
	double k = (double)(y2 - y1) / (x2 - x1);
	for (int x = x1, y = y1; x <= x2; x++) {

		left = x * grid_size + 1;
		right = left + grid_size - 1;
		bottom = (int)(y + 0.5)*grid_size + 2 + half_window;
		top = bottom + grid_size - 1;

		glRectf(left, top, right, bottom);
		e += k;
		if (e >= 0) {
			y++;
			e--;
		}
	}
}

void lineBER(int x1, int y1, int x2, int y2)
{
	//���� Bersenham ����ֱ��
	int left, right, top, bottom;
	double e = -0.5;
	double k = (double)(y2 - y1) / (x2 - x1);
	int E = x1 - x2;
	for (int x = x1, y = y1; x <= x2; x++) {

		left = x * grid_size + 1 + half_window;
		right = left + grid_size - 1;
		bottom = (int)(y + 0.5)*grid_size + 2 + half_window;
		top = bottom + grid_size - 1;

		glRectf(left, top, right, bottom);
		E = E + 2 * (y2 - y1);
		if (E >= 0)
		{
			y++;
			E = E - 2 * (x2 - x1);
		}
	}
}

void drawLine(void)
{
	glViewport(0, 0, window_size, window_size);
	glClear(GL_COLOR_BUFFER_BIT);
	initGird(window_size, grid_size);
	//����
	int x1 = 5;
	int y1 = 15;
	int x2 = 45;
	int y2 = 31;
	//����һ������ɫ��С����
	//ע����������궼�Ѿ������������µ���
	//ע�͸ú�����ֹ�����ɫ
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	lineADD(x1, y1, x2, y2);
	lineMID(x1, y1, x2, y2);
	lineBer(x1, y1, x2, y2);
	lineBER(x1, y1, x2, y2);
	glFlush();
}

void circleSQRT(int x0, int y0, int r)
{
	//��ԲΪ�е㷨��Բ
	glViewport(0, 0, window_size, window_size);
	int x = 0;
	int y = r;
	double d0 = 1.25 - r;
	int left, right, top, bottom;
	while (x <= y) {
		left = x * grid_size + 1;
		right = left + grid_size - 1;
		bottom = (int)(y + 0.5)*grid_size + 2;
		top = bottom + grid_size - 1;
		glRectf(left + x0, top + y0, right + x0, bottom + y0);
		glRectf(bottom - 1 + x0, right + 1 + y0, top - 1 + x0, left + 1 + y0);
		glRectf(bottom - 1 + x0, -left + 2 + y0, top - 1 + x0, -right + 2 + y0);
		glRectf(left + x0, -bottom + 3 + y0, right + x0, -top + 3 + y0);
		glRectf(-right + 1 + x0, -bottom + 3 + y0, -left + 1 + x0, -top + 3 + y0);
		glRectf(-top + 2 + x0, -left + 2 + y0, -bottom + 2 + x0, -right + 2 + y0);
		glRectf(-top + 2 + x0, right + 1 + y0, -bottom + 2 + x0, left + 1 + y0);
		glRectf(-right + 1 + x0, top + y0, -left + 1 + x0, bottom + y0);
		if (d0 < 0)
			d0 += 2 * x + 3;
		else {
			d0 += 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
}

void circleBER(int x0, int y0, int r)
{
	//СԲΪ��ʽ��Բ
	glViewport(0, 0, window_size, window_size);
	int x = 0;
	int y = r;
	int left, right, top, bottom;
	while (x <= y) {
		left = x * grid_size + 1;
		right = left + grid_size - 1;
		bottom = (int)(y + 0.5)*grid_size + 2;
		top = bottom + grid_size - 1;
		glRectf(left + x0, top + y0, right + x0, bottom + y0);
		glRectf(bottom - 1 + x0, right + 1 + y0, top - 1 + x0, left + 1 + y0);
		glRectf(bottom - 1 + x0, -left + 2 + y0, top - 1 + x0, -right + 2 + y0);
		glRectf(left + x0, -bottom + 3 + y0, right + x0, -top + 3 + y0);
		glRectf(-right + 1 + x0, -bottom + 3 + y0, -left + 1 + x0, -top + 3 + y0);
		glRectf(-top + 2 + x0, -left + 2 + y0, -bottom + 2 + x0, -right + 2 + y0);
		glRectf(-top + 2 + x0, right + 1 + y0, -bottom + 2 + x0, left + 1 + y0);
		glRectf(-right + 1 + x0, top + y0, -left + 1 + x0, bottom + y0);
		y = (int)(std::sqrt(r*r - x * x) + 0.5);
		x++;
	}
}

void drawCircle(void)
{
	glViewport(0, 0, window_size, window_size);
	glClear(GL_COLOR_BUFFER_BIT);
	initGird(window_size, grid_size);
	//����
	int R1 = ((window_size - 1) / grid_size - 10) / 2;
	int R2 = ((window_size - 1) / grid_size - 30) / 2;
	//����һ������ɫ��С����
	//ע����������궼�Ѿ������������µ���
	//ע�͸ú�����ֹ�����ɫ
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	circleSQRT(half_window,half_window,R2);
	circleBER(half_window, half_window, R1);
	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);   //��ʼ��GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(window_size, window_size);
	glutCreateWindow("����");
	InitEnvironment();   //��ʼ��
	glutDisplayFunc(&drawLine);   //�ص�����

	glutCreateWindow("��Բ");
	InitEnvironment();   //��ʼ��
	glutDisplayFunc(&drawCircle);   //�ص�����

	glutMainLoop();    //������ʾ�������ڸı�����»���ͼ��
	return 0;
}