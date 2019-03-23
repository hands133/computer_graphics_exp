#include <iostream>
#include <gl/glut.h>

using namespace std;
float window_size = 400;
void InitEnvironment() //对环境进行初始化操作
{
	glClearColor(0.0, 0.0, 0.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//将画面映射为坐标系
	gluOrtho2D(0, window_size, 0, window_size);
}

void myDisplay(void)
{

	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, window_size, window_size);
	/*
	void glViewPort(GLInt x; GLInt y; GLSizei Width; GLSizei Height);

	其中，参数X，Y指定了视见区域的左下角在窗口中的位置一般情况下为（0，0）
	Width和Height指定了视见区域的宽度和高度
	注意OpenGL使用的窗口坐标和WindowsGDI使用的窗口坐标是不一样的
	*/
	//glcolor3f 三个参数分别为 red green blue
	//位于画面左上角的红色矩形
	/*
	glColor3f(1, 0, 0);
	glRectf(0, 200, 200, 400);
	//位于画面右上角的绿色矩形
	glColor3f(0, 1, 0);
	glRectf(200, 200, 400, 400);
	//位于画面左下角的蓝色矩形
	glColor3f(0, 0, 1);
	glRectf(0, 0, 200, 200);
	//位于画面右下角的黑色矩形
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
	glReadPixels(0, 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);//读取一点的红色通道值
	cout << "r:" << (int)pixel[0] << " g:" << (int)pixel[1] << " b:" << (int)pixel[2] << endl;
	glFlush();
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);   //初始化GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(window_size, window_size);
	glutCreateWindow("演示1");
	InitEnvironment();   //初始化
	glutDisplayFunc(&myDisplay);   //回调函数
	/*
	GLbyte *pixel = (GLbyte*)malloc(30);
	glReadPixels(10, 10, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	cout << "r:" << (int)pixel[0] << " g:" << pixel[1] << " b:" << pixel[2] << endl;
	*/
	glutMainLoop();    //持续显示，当窗口改变会重新绘制图形
	return 0;
}