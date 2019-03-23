#include <iostream>
#include <gl/glut.h>

using namespace std;
float window_size = 400;
void InitEnvironment() //�Ի������г�ʼ������
{
	glClearColor(0.0, 0.0, 0.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//������ӳ��Ϊ����ϵ
	gluOrtho2D(0, window_size, 0, window_size);
}

void myDisplay(void)
{

	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, window_size, window_size);
	/*
	void glViewPort(GLInt x; GLInt y; GLSizei Width; GLSizei Height);

	���У�����X��Yָ�����Ӽ���������½��ڴ����е�λ��һ�������Ϊ��0��0��
	Width��Heightָ�����Ӽ�����Ŀ�Ⱥ͸߶�
	ע��OpenGLʹ�õĴ��������WindowsGDIʹ�õĴ��������ǲ�һ����
	*/
	//glcolor3f ���������ֱ�Ϊ red green blue
	//λ�ڻ������Ͻǵĺ�ɫ����
	/*
	glColor3f(1, 0, 0);
	glRectf(0, 200, 200, 400);
	//λ�ڻ������Ͻǵ���ɫ����
	glColor3f(0, 1, 0);
	glRectf(200, 200, 400, 400);
	//λ�ڻ������½ǵ���ɫ����
	glColor3f(0, 0, 1);
	glRectf(0, 0, 200, 200);
	//λ�ڻ������½ǵĺ�ɫ����
	glColor3f(0, 0, 0);
	glRectf(200, 0, 400, 200);
	*/
	glBegin(GL_POINTS);
	glColor3ub(255, 0, 0);
	glVertex2i(0, 0);
	glColor3f(0, 1, 0);
	glVertex2i(100, 0);
	glColor3f(0, 0, 1);
	glVertex2i(100, 100);
	glColor3f(1, 1, 1);
	glVertex2i(0, 100);
	glEnd();

	GLubyte *pixel = new GLubyte[3];
	glReadPixels(0, 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);//��ȡһ��ĺ�ɫͨ��ֵ
	cout << "r:" << (int)pixel[0] << " g:" << (int)pixel[1] << " b:" << (int)pixel[2] << endl;
	glFlush();
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);   //��ʼ��GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(window_size, window_size);
	glutCreateWindow("��ʾ1");
	InitEnvironment();   //��ʼ��
	glutDisplayFunc(&myDisplay);   //�ص�����
	/*
	GLbyte *pixel = (GLbyte*)malloc(30);
	glReadPixels(10, 10, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	cout << "r:" << (int)pixel[0] << " g:" << pixel[1] << " b:" << pixel[2] << endl;
	*/
	glutMainLoop();    //������ʾ�������ڸı�����»���ͼ��
	return 0;
}