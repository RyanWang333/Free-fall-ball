#include <GL/glut.h>  // OpenGL��
#include <cmath>      // ��׼��ѧ��
#include <string>     // �ַ�����
using namespace std;

class Box {
public:
    const int WIDTH;              //���ڵĿ��
    const int HEIGHT;             //���ڵĸ߶�
    float Bottom;           //��ĵײ�
    float Top;              //��Ķ���

public:
    Box(int width, int height) : WIDTH(width), HEIGHT(height) {
        Bottom = -HEIGHT / 4;  // ���ߵĵײ��ڴ��ڵ��±߽�߶ȵ�1/4ȡ��
        Top = HEIGHT / 4;      // ���ߵĶ����ڴ��ڵ��ϱ߽�߶ȵ�1/4
    }

    void drawFixedRect() {                     //���ƾ���
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);    //��ƽ�Ʊ任
        glColor4f(0.0f, 0.0f, 0.0f, 0.0f);  // ������ɫΪ��ɫ��͸����Ϊ0��ȫ͸����
        glLineWidth(1.0f);  // �����߿�Ϊ1������
        glBegin(GL_LINE_LOOP);  // ʹ����ѭ�����ƾ���
        glVertex2f(-10, Bottom);  // ͨ���ĸ���������߿����
        glVertex2f(-10, Top);
        glVertex2f(10, Top);
        glVertex2f(10, Bottom);
        glEnd();
        glPopMatrix();
    }
};

class Ball {
public:
    float ballX;             //���ڶ�ά�ռ��е�����λ�� 
    float ballY;
    float ballRadius;        //��İ뾶
    float ballScale;         //�����������������ʱ�Ĵ�С����
    float ballSpeed;         //����ٶ�
    float gravity;           //���������ϵ�����
    float energyLoss;        //����ײʱ��������ʧ

public:
    Ball(float x, float y, float radius, float scale, float speed, float g, float loss)
        : ballX(x), ballY(y), ballRadius(radius), ballScale(scale), ballSpeed(speed), gravity(g), energyLoss(loss) {}

    void drawBall() {
        glPushMatrix();
        glTranslatef(ballX, ballY, 0.0f);  // ƽ��С���λ�õ�(ballX, ballY)��
        glScalef(ballScale, ballScale, 0.0f);  // ����С��Ĵ�С
        glColor3f(1.0f, 0.0f, 0.0f);  // ����С�����ɫΪ��ɫ
        glutSolidSphere(ballRadius, 100, 100);  // �����������壨С��
        glPopMatrix();
    }

    void updateBall(float& Top, float& Bottom) {
        ballSpeed += gravity;  // С��������Ӱ�죬�ٶ�����
        ballY += ballSpeed;  // ����С���������λ��

        float ballTop = ballY + ballRadius;  // С��Ķ�������
        float ballBottom = ballY - ballRadius;  // С��ĵײ�����

        // �ж�С���Ƿ������߽�
        if (ballTop > Top) {  // ��С��Ķ��������ϱ߽�ʱ
            ballY = Top - ballRadius;  // ����С��λ�õ��߽�����
            ballSpeed = -ballSpeed * energyLoss;  // ����Ч�����ٶ�ȡ��������������ʧ����
        }
        else if (ballBottom < Bottom) {  // ��С��ĵײ������±߽�ʱ
            ballY = Bottom + ballRadius;  // ����С��λ�õ��߽�����
            ballSpeed = -ballSpeed * energyLoss;  // ����Ч�����ٶ�ȡ��������������ʧ����
        }
    }
};

// ���ڵĿ�Ⱥ͸߶�
const int WIDTH = 600;
const int HEIGHT = 600;

Ball ball(0.0f, HEIGHT / 2 - 10.0f, 10.0f, 1.0f, 0.0f, -0.1f, 0.9f);  // ����һ��Ball���ʵ��
Box box(WIDTH, HEIGHT);  // ����һ��Box���ʵ��

// ����С���λ�ú��ٶ�
void updateBall(float Top, float Bottom) {
    ball.updateBall(Top, Bottom);// ����Ball���updateBall����������С���������Ϊ��������ײ���ͷ���
}

void update(int value) {
    updateBall(box.Top, box.Bottom); //// ����С���״̬������߽����������ײ����λ�ø���
    glutPostRedisplay();  // �����ػ洰�ڣ��ú�������GLUT�ⴰ�ڵ�������Ҫ����
    glutTimerFunc(16, update, 0);  // ���ö�ʱ����ÿ16�������һ��
}

// ���ƴ���
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ����������Ȼ�����
    glLoadIdentity();  // ���ص�λ����

    // ���ƹ̶��ľ��ο�
    box.drawFixedRect();

    // ����С��
    ball.drawBall();

    glFlush();  // ǿ�ƽ��������е������������ʾ�豸
    glutSwapBuffers();  // ����ǰ�󻺳�������ʾ���Ƶ�ͼ��
}

// ��ʼ������
void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // ���ñ�����ɫΪ��ɫ
    glEnable(GL_DEPTH_TEST);  // ������Ȳ���
}

// ���ڵĴ�С�ı�ʱ�ص�����
void reshape(int width, int height) {
    glViewport(0, 0, width, height);  // �����ӿڴ�С
    glMatrixMode(GL_PROJECTION);  // ����ͶӰģʽ
    glLoadIdentity();  // ���ص�λ����
    gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);  // ��������ͶӰ����
    glMatrixMode(GL_MODELVIEW);  // ����ģ����ͼģʽ
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);  // ��ʼ��GLUT��
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // ������ʾģʽΪ˫���壬RGB��ɫģʽ������ȼ��
    glutInitWindowSize(WIDTH, HEIGHT);  // ���ô��ڵĴ�С
    glutCreateWindow("free fall ball");  // �������ڲ�����
    init();  // ��ʼ������
    glutDisplayFunc(render);  // ������ʾ�ص�����
    glutReshapeFunc(reshape);  // ���ô��ڴ�С�ı�ص�����
    glutTimerFunc(16, update, 0);  // ���ö�ʱ����ÿ16�������һ��
    glutMainLoop();  // ����GLUT�¼�����ѭ��
    return 0;
}