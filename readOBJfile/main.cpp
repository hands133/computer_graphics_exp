
#include "mesh.h"

#include <string>

float width = 700;
float height = 700;
mesh *surface = NULL;
char *fileName = NULL;

void InitEnvironment()	//��ʼ������
{
	//���������ɫ
	glClearColor(0.0, 0.0, 0.0, 0);
	//�Ѵ������Ϊ��ǰ��ɫ
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glOrtho(-3, 3, -5, 1, -5, 5);
	//gluOrtho2D(0, 1, 0, 1);
}

void draw()
{
	glutSwapBuffers();
	string file = string(fileName);
	cout << file << endl;
	surface = new mesh();
	surface->initData(fileName);
	GLubyte color[3] = { 255,255,255 };
	surface->draw(DRAW_STRUCTURE, color);
}

int main(int argc, char** argv)
{
	if (argc < 2 || argc > 2)
	{
		cout << "ʹ�÷�����readOBJfile.exe data/XXX.obj";
		exit(1);
	}
	fileName = argv[1];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("������");
	InitEnvironment();
	//glutMouseFunc(mouseInput);	//���������
	//glutMotionFunc(dragEntity);	//����϶�����
	//glutKeyboardFunc(keyBoardFunc);
	glutDisplayFunc(draw);
	glutMainLoop();
}
