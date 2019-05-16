#include <string>
#include <Windows.h>

#include "mesh.h"


float width = 600;
float height = 600;
mesh *surface = NULL;
char *fileName = NULL;

const float roateSpeed = 1.0;

bool XPressed = false;
bool YPressed = false;
bool ZPressed = false;
bool motion = false;

bool rotating = false;

void InitEnvironment()	//��ʼ������
{
	glEnable(GL_DEPTH);
	//���������ɫ
	glClearColor(0.0, 0.0, 0.0, 0);
	//�Ѵ������Ϊ��ǰ��ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
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

void mouseInput(int button, int state, int x, int y)
{
	int posx = x;
	int posy = height - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{	//�� X ����ת 30 ��
		surface->rotateDegree(ROTATE_X_CLOCK, 30);
		GLubyte color[3] = { 255,255,255 };
		//surface->draw(DRAW_STRUCTURE, color);
	}
}

void loopRotating(const unsigned char& type, const int& speed, bool&  rotating)
{
	while (rotating)
	{
		surface->rotateDegree(type, speed);
	}
}

void keyBoardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
	{
		if (!rotating)
		{	//�����߳�ѭ��
			rotating = true;
			//std::thread t(loopRotating, ROTATE_X_CLOCK, 2, std::ref(rotating));
			//t.join();
			while(true)
				surface->rotateDegree(ROTATE_X_CLOCK, 2);
			/*loopRotating(ROTATE_X_CLOCK,2,rotating);*/
		}
		else
		{
			rotating = false;
		}
		GLubyte color[3] = { 255,255,255 };
		surface->draw(DRAW_STRUCTURE, color);
	}
	break;
caseX:
	case 'x':
		break;
	caseY:
	case 'y':
		break;
	caseZ:
	case 'z':
		break;
	case 'X': goto caseX;
		break;
	case 'Y': goto caseY;
		break;
	case 'Z': goto caseZ;
		break;
	}
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
	glutMouseFunc(mouseInput);	//���������
	//glutMotionFunc(dragEntity);	//����϶�����
	glutKeyboardFunc(keyBoardFunc);
	glutDisplayFunc(draw);
	glutMainLoop();
}
