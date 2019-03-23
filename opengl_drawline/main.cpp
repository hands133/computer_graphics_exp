#include <iostream>
#include <cmath>
#include <gl/glut.h>

#define ABS(x) ((x)>0?(x):-(x))

using namespace std;
float window_size = 600;
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

void drawLine(void)
{
	int x1 = 40;
	int y1 = 150;
	int x2 = 240;
	int y2 = 220;
	double k = (double)(y2 - y1) / (x2 - x1);
	//����һ������ɫ��С����
	//ע����������궼�Ѿ������������µ���
	glClear(GL_COLOR_BUFFER_BIT);
	
	glViewport(0, half_window, window_size, window_size);
	glBegin(GL_POINTS);
	//���ϰ� DDA ����ֱ��
	double y = (double)y1;
	for (int x = x1; x <= x2; x++) {
		y += k;
		glVertex2f(x, (int)(y + 0.5));
	}
	glEnd();

	//���ϰ��е㻭��
	glViewport(half_window, half_window, window_size, window_size);
	glBegin(GL_POINTS);
	for (int x = x1, Y = y1; x <= x2; x++) {
		//ֱ���·��ĵ�
		int distance1 = (Y - y1)*(x2 - x1) - (y2 - y1)*(x - x1);
		//ֱ���Ϸ��ĵ�
		int distance2 = (Y - y1 + 1)*(x2 - x1) - (y2 - y1)*(x - x1);
		if (ABS(distance1) < ABS(distance2))
			glVertex2f(x, Y);
		else {
			glVertex2f(x, Y + 1);
			Y = Y + 1;
		}
	}
	glEnd();

	//���� Bersenham
	glViewport(0, 0, window_size, window_size);
	glBegin(GL_POINTS);
	double e = -0.5;
	for (int x = x1, Y = y1; x <= x2; x++) {
		glVertex2f(x, Y);
		e += k;
		if (e >= 0) {
			Y++;
			e = e - 1;
		}
	}
	glEnd();

	//���¸Ľ� Bersenham
	glViewport(half_window, 0, window_size, window_size);
	glBegin(GL_POINTS);
	int E = x1 - x2;
	for (int x = x1, Y = y1; x <= x2; x++) {
		glVertex2f(x, Y);
		E = E + 2 * (y2 - y1);
		if (E >= 0) {
			Y++;
			E = E - 2 * (x2 - x1);
		}
	}
	glEnd();

	glViewport(0, 0, window_size, window_size);
	glFlush();
}

void drawCircle(void) {
	//��Բ�㷨
	int R1 = (window_size - 50) / 2;
	int R2 = (window_size - 200) / 2;

	glClear(GL_COLOR_BUFFER_BIT);
	//��������ΪԲ��
	glViewport(0, 0, window_size, window_size);
	glBegin(GL_POINTS);
	int x = 0;
	int yy = R1;
	double d0 = 1.25 - R1;
	while (x <= yy) {
		glVertex2f(x + half_window, yy + half_window);
		glVertex2f(yy + half_window, x + half_window);
		glVertex2f(-x + half_window, yy + half_window);
		glVertex2f(yy + half_window, -x + half_window);
		glVertex2f(x + half_window, -yy + half_window);
		glVertex2f(-yy + half_window, x + half_window);
		glVertex2f(-x + half_window, -yy + half_window);
		glVertex2f(-yy + half_window, -x + half_window);
		if (d0 < 0)
			d0 += 2 * x + 3;		
		else {
			d0 += 2 * (x - yy) + 5;
			yy--;
		}
		x++;
	}
	glEnd();

	glBegin(GL_POINTS);
	int XX = 0;
	int YY = R2;
	while (XX <= YY) {
		glVertex2f(XX + half_window, YY + half_window);
		glVertex2f(YY + half_window, XX + half_window);
		glVertex2f(-XX + half_window, YY + half_window);
		glVertex2f(YY + half_window, -XX + half_window);
		glVertex2f(XX + half_window, -YY + half_window);
		glVertex2f(-YY + half_window, XX + half_window);
		glVertex2f(-XX + half_window, -YY + half_window);
		glVertex2f(-YY + half_window, -XX + half_window);
		YY = (int)(sqrt(R2 * R2 - XX * XX) + 0.5);
		XX++;
	}
	glEnd();

	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);   //��ʼ��GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(window_size, window_size);
	glutCreateWindow("�����㷨");
	InitEnvironment();   //��ʼ��
	glutDisplayFunc(&drawLine);   //�ص�����

	glutCreateWindow("��Բ�㷨");
	InitEnvironment();   //��ʼ��
	glutDisplayFunc(&drawCircle);   //�ص�����

	glutMainLoop();    //������ʾ�������ڸı�����»���ͼ��
	return 0;
}