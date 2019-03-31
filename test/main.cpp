
#include <gl/glut.h> 
#include <Windows.h>

GLfloat angle = 0.0f;

void InitEnvironment()
{
	glEnable(GL_DEPTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, 1, 1, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(12, 12, 20, 0, 0, 0, 0, 1, 0);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(angle, 0, 1, 0);

	//���»���һ��������
	glBegin(GL_QUADS);
	//����  
	glColor4f(1, 0, 0, 1);
	glVertex3f(-5, -5, -5);
	glVertex3f(5, -5, -5);
	glColor4f(0, 0, 1, 1);
	glVertex3f(5, 5, -5);
	glVertex3f(-5, 5, -5);
	//����A  
	glColor4f(0, 0, 1, 1);
	glVertex3f(-5, -5, -5);
	glVertex3f(5, -5, -5);
	glColor4f(0, 1, 0, 1);
	glVertex3f(5, -5, 5);
	glVertex3f(-5, -5, 5);
	//����B  
	glColor4f(0, 1, 0, 1);
	glVertex3f(5, -5, -5);
	glVertex3f(5, 5, -5);
	glColor4f(0, 1, 1, 1);
	glVertex3f(5, 5, 5);
	glVertex3f(5, -5, 5);
	//����C  
	glColor4f(1, 1, 0, 1);
	glVertex3f(5, 5, -5);
	glVertex3f(-5, 5, -5);
	glColor4f(1, 0, 1, 1);
	glVertex3f(-5, 5, 5);
	glVertex3f(5, 5, 5);
	//����D  
	glColor4f(1, 0, 1, 1);
	glVertex3f(-5, 5, -5);
	glVertex3f(-5, -5, -5);
	glColor4f(0, 1, 0, 1);
	glVertex3f(-5, -5, 5);
	glVertex3f(-5, 5, 5);
	//����  
	glColor4f(1, 1, 0, 1);
	glVertex3f(-5, -5, 5);
	glVertex3f(5, -5, 5);
	glColor4f(0, 0, 1, 1);
	glVertex3f(5, 5, 5);
	glVertex3f(-5, 5, 5);

	glEnd();
	glutSwapBuffers();
	glPopMatrix();
}

void RotateRect()
{
	angle += 0.5;
	if (angle >= 360)
	{
		angle = 0.0f;
	}
	Sleep(30);
	myDisplay();
}

void OnMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glutIdleFunc(RotateRect);
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glutIdleFunc(NULL);
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);   //��ʼ��GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(500, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("OpenGL");
	InitEnvironment();   //��ʼ����ʾ����
	glutMouseFunc(&OnMouse);  //ע������¼�
	glutDisplayFunc(&myDisplay);   //�ص����� 
	glutMainLoop();    //������ʾ�������ڸı�����»���ͼ��
	return 0;
}