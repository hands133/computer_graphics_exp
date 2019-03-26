#include <iostream>
#include <gl/glut.h>
#include <cmath>

#define abs(x) ((x)>0?(x):-(x))

float window_size = 601;
float grid_size = 4;
int half_window = (int)(window_size / 2);

const bool ABOVE = true;
const bool BELOW = false;

const double weightMatrix[3][3] =
{	{1.0 / 16, 2.0 / 16, 1.0 / 16},
	{2.0 / 16, 4.0 / 16, 2.0 / 16},
	{1.0 / 16, 2.0 / 16, 1.0 / 16}	};

const GLubyte maxGrayVal = 255;

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
	glFlush();
}

void areaSampling(int x1, int y1, int x2, int y2)
{
	//�������
	int left, right, top, bottom;
}

void drawApixel(GLubyte r, GLubyte g, GLubyte b, const int* judgePoints)
{
	glColor3ub(r, g, b);
	int left, right, top, bottom;

	left = (judgePoints[0] - 1) * grid_size + 1;
	//right = left + grid_size - 1;
	right = left + grid_size;
	bottom = (int)(judgePoints[1] - 0.5)*grid_size + 2;
	//top = bottom + grid_size - 1;
	top = bottom + grid_size;

	glRectf(left, top, right, bottom);
	glFlush();
	glColor3ub(255, 255, 255);
}

bool isAboveLine(const double* coefficient, const double* judgePoint)
{
	//�жϵ� judgePoint ���� deterPoints ȷ����ֱ���Ϸ� true �����·� false
	double A = coefficient[0];
	double B = coefficient[1];
	double C = coefficient[2];
	double res = A * judgePoint[0] + B * judgePoint[1] + C;
	return (res > 0) ? true : false;
}

void countNum(int **restore, const double *coeff, const bool DIRECTION, const int *judgePoint)
{
	const double step = 1.0 / 6.0;
	double x = judgePoint[0];
	double y = judgePoint[1];

	double *microPoint = new double[2];

	int numCount = 0;
	//�жϾŸ����ж�����ֱ���Ϸ�/�·�
	if (DIRECTION)
	{	//�ж����Ϸ�
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				microPoint[0] = x + (2 * i + 1) * step;
				microPoint[1] = y + (2 * j + 1) * step;
				if (isAboveLine(coeff, microPoint))
				{	//����ֱ���Ϸ�
					restore[i][j] = 1;
					numCount++;
				}
				else
					restore[i][j] = 0;
			}
		}
	}
	else
	{	//�ж����·�
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				microPoint[0] = x + (2 * i + 1) * step;
				microPoint[1] = y + (2 * j + 1) * step;
				if (!isAboveLine(coeff, microPoint))
				{	//����ֱ���·�
					restore[i][j] = 1;
					numCount++;
				}
				else
					restore[i][j] = 0;
			}
		}
	}
}

//������õĸ�Ȩ�������ɫ��
void matrixDraw(int** store, int* judgePoint)
{
	double weight = 0.0;
	//ͨ���õ��ľ���Ⱦɫ
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			weight += weightMatrix[i][j] * store[i][j];
	int color = (int)(weight * 255 + 0.5);
	if(color != 0)
		drawApixel(color, color, color, judgePoint);
}

/*
// debug �ã���ʾÿһ��ɫ��������ľ���
void dispMatrix(int** matrix)
{
	for (int i = 2; i >= 0; i--)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << matrix[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
*/

void weightedAreaSampling(int x1, int y1, int x2, int y2, double width)
{
	double coeff[3] = { 0.0 };

	double y = y1;
	double k = (double)(y2 - y1) / (double)(x2 - x1);
	coeff[0] = y1 - y2;
	coeff[1] = x2 - x1;
	coeff[2] = x1 * y2 - y1 * x2;
	double DeltaC = (x1 - x2)*(width / 2)*sqrt(1 + (double)((y1 - y2)*(y1 - y2)) / (double)((x1 - x2)*(x1 - x2)));

	//�ϱ����Ϸ����ء��м����غ��±����·�����
	int **pointAbove = new int*[3];
	int **pointMidUp = new int*[3];
	int **pointMid   = new int*[3];
	int **pointMidDn = new int*[3];
	int **pointBelow = new int*[3];
	for (int i = 0; i < 3; i++)
	{
		pointAbove[i] = new int[3];
		pointMidUp[i] = new int[3];
		pointMid[i]   = new int[3];
		pointMidDn[i] = new int[3];
		pointBelow[i] = new int[3];
	}
	
	int *judgePoint = new int[2];

	for (int x = x1; x <= x2; x++)
	{
		//���������Ÿ�����Щ������ֱ���м�
		//��ʱ����ƽ�����漰�������㣬���߾��������ء��Ϸ����غ��·�����
		//����������ֱ�����жϲ������ɫ
		judgePoint[0] = x;
		//�Ϸ�ֱ���ϵ����ص�Ϊ (x, y + 1)
		judgePoint[1] = (int)(y + 0.5) + 1;
		coeff[2] = x1 * y2 - y1 * x2 + DeltaC;
		countNum(pointAbove, coeff, BELOW, judgePoint);
		matrixDraw(pointAbove, judgePoint);

		//�м�����ص�Ϊ (x, y)
		judgePoint[1] = (int)(y + 0.5);
		coeff[2] = x1 * y2 - y1 * x2 + DeltaC;
		countNum(pointMidUp, coeff, BELOW, judgePoint);
		coeff[2] = x1 * y2 - y1 * x2 - DeltaC;
		countNum(pointMidDn, coeff, ABOVE, judgePoint);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				pointMid[i][j] = (pointMidUp[i][j] == 1 && pointMidDn[i][j] == 1) ? 1 : 0;
		//dispMatrix(pointMidUp);
		//dispMatrix(pointMidDn);
		matrixDraw(pointMid, judgePoint);

		//�·�ֱ���µ����ص�Ϊ (x, y - 1)
		judgePoint[1] = (int)(y + 0.5) - 1;
		coeff[2] = x1 * y2 - y1 * x2 - DeltaC;
		countNum(pointBelow, coeff, ABOVE, judgePoint);
		matrixDraw(pointBelow, judgePoint);
		y += k;
	}
}

void drawLine(void)
{
	glViewport(0, 0, window_size, window_size);
	glClear(GL_COLOR_BUFFER_BIT);
	initGird(window_size, grid_size);
	//����
	int x1 = 5;
	int y1 = 5;
	int x2 = 130;
	int y2 = 60;
	//����һ������ɫ��С����
	//ע����������궼�Ѿ������������µ���
	//ע�͸ú�����ֹ�����ɫ
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	weightedAreaSampling(x1, y1, x2, y2, 1);
	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);   //��ʼ��GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_size, window_size);
	glutCreateWindow("����");
	InitEnvironment();   //��ʼ��
	glutDisplayFunc(&drawLine);   //�ص�����

	glutMainLoop();    //������ʾ�������ڸı�����»���ͼ��
	return 0;
}