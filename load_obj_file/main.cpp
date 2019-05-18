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

int oldX = 0;
int oldY = 0;

void InitEnvironment()	//初始化操作
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("宝可梦");

	glEnable(GL_DEPTH);
	//设置清除颜色
	glClearColor(0.0, 0.0, 0.0, 0);
	//把窗口清除为当前颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glutSwapBuffers();

	surface->setView();

}

void draw()
{
	GLubyte color[3] = { 255,255,255 };
	surface->draw(DRAW_STRUCTURE, color);
}

void mouseInput(int button, int state, int x, int y)
{
	int posx = x;
	int posy = height - y;
	oldX = x;
	oldY = y;

	//if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	//{	//绕 X 轴旋转 30 度
	//	surface->rotateDegree(ROTATE_X_CLOCK, 30);
	//	GLubyte color[3] = { 255,255,255 };
	//}
}

void keyBoardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	//case ' ':
	//{
	//	if (!rotating)
	//	{	//创建线程循环
	//		rotating = true;
	//		while(true)
	//			surface->rotateDegree(ROTATE_X_CLOCK, 2);
	//	}
	//	else
	//	{
	//		rotating = false;
	//	}
	//	GLubyte color[3] = { 255,255,255 };
	//	surface->draw(DRAW_STRUCTURE, color);
	//}
	//break;
	case 'x':	surface->rotateDegree(ROTATE_X_CLOCK, 2);
		break;
	case 'y':	surface->rotateDegree(ROTATE_Y_CLOCK, 2);
		break;
	case 'z':	surface->rotateDegree(ROTATE_Z_CLOCK, 2);
		break;
	case 'X':	surface->rotateDegree(ROTATE_X_ANTICLOCK, 2);
		break;
	case 'Y':	surface->rotateDegree(ROTATE_Y_ANTICLOCK, 2);
		break;
	case 'Z':	surface->rotateDegree(ROTATE_Z_ANTICLOCK, 2);
		break;
	}
}

void specialKeys(int key, int x, int y)
{
	int rotateDegree = 2;
	switch (key)
	{
	case GLUT_KEY_UP:		surface->rotateCamera(ROTATE_Y_CLOCK, rotateDegree);
		break;
	case GLUT_KEY_DOWN:		surface->rotateCamera(ROTATE_Y_ANTICLOCK, rotateDegree);
		break;
	case GLUT_KEY_LEFT:		surface->rotateCamera(ROTATE_X_ANTICLOCK, rotateDegree);
		break;
	case GLUT_KEY_RIGHT:	surface->rotateCamera(ROTATE_X_CLOCK, rotateDegree);
		break;
	}
}

void dragEntity(int x, int y)
{
	int posx = x;
	int posy = height - y;
	int deltaX = posx - oldX;
	int deltaY = posy - oldY;
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 0.1, 200);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	if (argc < 2 || argc > 2)
	{
		cout << "使用方法：readOBJfile.exe data/XXX.obj";
		exit(1);
	}
	fileName = argv[1];

	string file = string(fileName);
	cout << file << endl;
	surface = new mesh();
	surface->initData(fileName);


	glutInit(&argc, argv);

	InitEnvironment();
	glutMouseFunc(mouseInput);	//鼠标点击函数
	glutMotionFunc(dragEntity);	//鼠标拖动函数
	glutKeyboardFunc(keyBoardFunc);
	glutSpecialFunc(specialKeys);
	glutDisplayFunc(draw);
	//glutReshapeFunc(reshape);
	glutMainLoop();
}
