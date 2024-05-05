#include <GL/glut.h>  // OpenGL库
#include <cmath>      // 标准数学库
#include <string>     // 字符串库
using namespace std;

class Box {
public:
    const int WIDTH;              //窗口的宽度
    const int HEIGHT;             //窗口的高度
    float Bottom;           //框的底部
    float Top;              //框的顶部

public:
    Box(int width, int height) : WIDTH(width), HEIGHT(height) {
        Bottom = -HEIGHT / 4;  // 框线的底部在窗口的下边界高度的1/4取负
        Top = HEIGHT / 4;      // 框线的顶部在窗口的上边界高度的1/4
    }

    void drawFixedRect() {                     //绘制矩形
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);    //无平移变换
        glColor4f(0.0f, 0.0f, 0.0f, 0.0f);  // 设置颜色为黑色，透明度为0（全透明）
        glLineWidth(1.0f);  // 设置线宽为1个像素
        glBegin(GL_LINE_LOOP);  // 使用线循环绘制矩形
        glVertex2f(-10, Bottom);  // 通过四个顶点绘制线框矩形
        glVertex2f(-10, Top);
        glVertex2f(10, Top);
        glVertex2f(10, Bottom);
        glEnd();
        glPopMatrix();
    }
};

class Ball {
public:
    float ballX;             //球在二维空间中的坐标位置 
    float ballY;
    float ballRadius;        //球的半径
    float ballScale;         //可以用来调整球绘制时的大小比例
    float ballSpeed;         //球的速度
    float gravity;           //作用在球上的重力
    float energyLoss;        //在碰撞时能量的损失

public:
    Ball(float x, float y, float radius, float scale, float speed, float g, float loss)
        : ballX(x), ballY(y), ballRadius(radius), ballScale(scale), ballSpeed(speed), gravity(g), energyLoss(loss) {}

    void drawBall() {
        glPushMatrix();
        glTranslatef(ballX, ballY, 0.0f);  // 平移小球的位置到(ballX, ballY)处
        glScalef(ballScale, ballScale, 0.0f);  // 缩放小球的大小
        glColor3f(1.0f, 0.0f, 0.0f);  // 设置小球的颜色为红色
        glutSolidSphere(ballRadius, 100, 100);  // 绘制填充的球体（小球）
        glPopMatrix();
    }

    void updateBall(float& Top, float& Bottom) {
        ballSpeed += gravity;  // 小球受重力影响，速度增加
        ballY += ballSpeed;  // 更新小球的纵坐标位置

        float ballTop = ballY + ballRadius;  // 小球的顶部坐标
        float ballBottom = ballY - ballRadius;  // 小球的底部坐标

        // 判断小球是否碰到边界
        if (ballTop > Top) {  // 当小球的顶部超过上边界时
            ballY = Top - ballRadius;  // 调整小球位置到边界线内
            ballSpeed = -ballSpeed * energyLoss;  // 反弹效果，速度取反并乘以能量损失参数
        }
        else if (ballBottom < Bottom) {  // 当小球的底部超过下边界时
            ballY = Bottom + ballRadius;  // 调整小球位置到边界线内
            ballSpeed = -ballSpeed * energyLoss;  // 反弹效果，速度取反并乘以能量损失参数
        }
    }
};

// 窗口的宽度和高度
const int WIDTH = 600;
const int HEIGHT = 600;

Ball ball(0.0f, HEIGHT / 2 - 10.0f, 10.0f, 1.0f, 0.0f, -0.1f, 0.9f);  // 定义一个Ball类的实例
Box box(WIDTH, HEIGHT);  // 定义一个Box类的实例

// 更新小球的位置和速度
void updateBall(float Top, float Bottom) {
    ball.updateBall(Top, Bottom);// 调用Ball类的updateBall方法，处理小球的物理行为，包括碰撞检测和反弹
}

void update(int value) {
    updateBall(box.Top, box.Bottom); //// 更新小球的状态，传入边界参数进行碰撞检测和位置更新
    glutPostRedisplay();  // 请求重绘窗口，该函数告诉GLUT库窗口的内容需要更新
    glutTimerFunc(16, update, 0);  // 设置定时器，每16毫秒更新一次
}

// 绘制窗口
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲区和深度缓冲区
    glLoadIdentity();  // 加载单位矩阵

    // 绘制固定的矩形框
    box.drawFixedRect();

    // 绘制小球
    ball.drawBall();

    glFlush();  // 强制将缓冲区中的数据输出到显示设备
    glutSwapBuffers();  // 交换前后缓冲区，显示绘制的图像
}

// 初始化窗口
void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // 设置背景颜色为白色
    glEnable(GL_DEPTH_TEST);  // 开启深度测试
}

// 窗口的大小改变时回调函数
void reshape(int width, int height) {
    glViewport(0, 0, width, height);  // 设置视口大小
    glMatrixMode(GL_PROJECTION);  // 设置投影模式
    glLoadIdentity();  // 加载单位矩阵
    gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);  // 设置正交投影矩阵
    glMatrixMode(GL_MODELVIEW);  // 设置模型视图模式
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);  // 初始化GLUT库
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // 设置显示模式为双缓冲，RGB颜色模式，带深度检测
    glutInitWindowSize(WIDTH, HEIGHT);  // 设置窗口的大小
    glutCreateWindow("free fall ball");  // 创建窗口并命名
    init();  // 初始化窗口
    glutDisplayFunc(render);  // 设置显示回调函数
    glutReshapeFunc(reshape);  // 设置窗口大小改变回调函数
    glutTimerFunc(16, update, 0);  // 设置定时器，每16毫秒更新一次
    glutMainLoop();  // 进入GLUT事件处理循环
    return 0;
}