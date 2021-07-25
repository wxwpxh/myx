/*
 *   g++ -o glewgl2 glewgl2.cpp -lglut -lGL -lGLEW
 *
 * */

#include <GL/glew.h>
#include <GL/glut.h>

void display(void)
{
    // clear all pixels  
    glClear(GL_COLOR_BUFFER_BIT);

    // draw yellow polygon (rectangle) with corners at
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    //绘制开始前必须调用glBegin以通知绘制图形的类型,比如还可以绘制点，线等。

    glVertex3f(0.2f, 0.8f, 0.0f);

    glVertex3f(0.2f, 0.2f, 0.0f);

    glVertex3f(0.8f, 0.2f, 0.0f);
    glEnd();					//结束之后则要调用glEnd函数

    glFlush();
}

void init(void){
    // select clearing color: purple
    glClearColor(0.6, 0.3, 0.6, 0.0);
    
    //initialize viewing values
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc,char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2018计算机图形");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
