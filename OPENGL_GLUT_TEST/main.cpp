#include <gl/glut.h>

void Show()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glRectf(-0.1f, -0.1f, 0.5f, 0.5f);

	glColor3f(1.0, 1.0, 1.0);
	glPointSize(3.0);
	glBegin(GL_POINTS);
	//画面中心是坐标原点，分为四个象限，最左上右下坐标为1.0f 1.0f -1.0f -1.0f
	//按照相邻像素绘制时可能需要对应除以窗口的宽度和高度
	glVertex2f(0,0);
	glVertex2f(0.1f, -0.1f);
	glVertex2f(-0.2f, 0.2f);
	glVertex2f(0.3f, -0.3f);
	glEnd();

	glFlush();
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("试试中文行不");
	glutDisplayFunc(Show);
	glutMainLoop();
	return 0;
}